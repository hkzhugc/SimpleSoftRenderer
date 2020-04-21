#include <algorithm>
#include <chrono>
#include <thread>
#include <mutex>
#include <atomic>
#include <future>
#include "my_gl.h"
using namespace std;
Vec3f barycentric(Vec2f * pts, Vec2f p)
{
	Vec2f AB = pts[1] - pts[0];
	Vec2f AC = pts[2] - pts[0];
	Vec2f PA = pts[0] - p;
	Vec3f uv1 = cross(Vec3f(AB.x, AC.x, PA.x), Vec3f(AB.y, AC.y, PA.y));
	if (abs(uv1[2]) < 1e-2) return Vec3f(-1, 1, 1);
	return Vec3f(1.f - (uv1.x + uv1.y) / uv1.z, uv1.x / uv1.z, uv1.y / uv1.z);
}

Mat4f Map2Screen(int w, int h)
{
	Mat4f mat(1.0f);
	// from -1 1 to [0, w]
	mat[0][0] = w / 2.f;
	mat[3][0] = w / 2.f;
	// from -1 1 to [0, h]
	mat[1][1] = h / 2.f;
	mat[3][1] = h / 2.f;
	// from -1 1 to [0, 1]
	mat[2][2] = 1.f / 2.f;
	mat[3][2] = 1.f / 2.f;
	return mat;
}

Mat4f toScreen(int w, int h)
{
	Mat4f mat(1.0f);
	// from -1 1 to [0, w]
	mat[0][0] = w / 2.f;
	mat[3][0] = w / 2.f;
	// from -1 1 to [0, h]
	mat[1][1] = h / 2.f;
	mat[3][1] = h / 2.f;
	// from -1 1 to [0, 1]
	mat[2][2] = 1.f / 2.f;
	mat[3][2] = 1.f / 2.f;
	return mat;
}

Mat4f project(float camera_distance)
{
	Mat4f mat(1.f);
	mat[2][3] = -1.f / camera_distance;
	return mat;
}

Mat4f project(float fovy, float aspect, float near, float far)
{
	Mat4f mat(1.f);
	mat[3][3] = 0;
	mat[2][3] = -1.f; // -z
	mat[0][0] = 2.f / (tan(fovy / 2) * aspect);
	mat[1][1] = 2.f / tan(fovy / 2);
	mat[2][2] = (near + far) / (near - far);
	mat[3][2] = (2.f * near * far) / (near - far);
	return mat;
}

Mat4f lookAt(Vec3f eye, Vec3f center, Vec3f up)
{
	Vec3f z = (eye - center).normalize();
	Vec3f x = cross(up, z);
	Vec3f y = cross(z, x);
	Mat4f rotation(1.0f);
	Mat4f translate(1.0f);
	for (int i = 0; i < 3; i++)
	{
		rotation[i][0] = x[i];
		rotation[i][1] = y[i];
		rotation[i][2] = z[i];
		translate[3][i] = -eye[i];
	}
	return rotation * translate;
}

mutex zbuffer_lock;

void thread_func(IPipeLine & pipeline, FrameBuffer& framebuffer, Vec2f P, Vec2f screen_position[3], float depths[3], float invzs[3])
{
	auto& image = framebuffer.image;
	auto& zbuffer = framebuffer.zbuffer;
	auto bar = barycentric(screen_position, P);
	if (bar.x < 0 || bar.y < 0 || bar.z < 0)
		return;
	float invz = 0;
	float depth = 0;
	for (size_t i = 0; i < 3; i++)
	{
		depth += bar[i] * depths[i];
		invz += bar[i] * invzs[i];
	}
	// early z test
	//zbuffer_lock.lock();
	if (depth >= zbuffer[int(P.x + P.y*image.get_width())])
	{
		//zbuffer_lock.unlock(); // before return unlock the z buffer
		return;
	}
	//zbuffer_lock.unlock(); // before return unlock the z buffer
	pipeline.rasterize(bar, invz);
	TGAColor color;
	bool discard = pipeline.fragment(color);
	//zbuffer_lock.lock();
	if (!discard && depth < zbuffer[int(P.x + P.y*image.get_width())])
	{
		zbuffer[int(P.x + P.y*image.get_width())] = depth;
		image.set(P.x, P.y, color);
	}
	//zbuffer_lock.unlock(); // before return unlock the z buffer
	return;
}

void triangle(Vec4f * pts, IPipeLine & pipeline, FrameBuffer& framebuffer)
{
	auto& image = framebuffer.image;
	auto& zbuffer = framebuffer.zbuffer;
	Mat4f toScreen = Map2Screen(image.get_width(), image.get_height());
	Vec2i bbox_min(image.get_width() - 1, image.get_height() - 1);
	Vec2i bbox_max(0, 0);
	Vec2i clamp(image.get_width() - 1, image.get_height() - 1);

	Vec2f screen_position[3];
	float depths[3];
	float invzs[3];
	for (int i = 0; i < 3; i++)
	{
		invzs[i] = 1.0f / pts[i].w;
		auto&& clipSpaceCoord = pts[i] * invzs[i];
		Vec3f p = Vec3f(toScreen * clipSpaceCoord);
		screen_position[i] = Vec2f(p);
		depths[i] = p.z;
		for (int j = 0; j < 2; j++)
		{
			bbox_min[j] = max(0, min(bbox_min[j], (int)screen_position[i][j]));
			bbox_max[j] = min(clamp[j], max(bbox_max[j], (int)screen_position[i][j]));
		}
	}

	Vec2f P;
	for (P.x = bbox_min.x; P.x <= bbox_max.x; P.x++)
	{
		for (P.y = bbox_min.y; P.y <= bbox_max.y; P.y++)
		{
			auto bar = barycentric(screen_position, P);
			if (bar.x < 0 || bar.y < 0 || bar.z < 0)
				continue;
			float invz = 0;
			float depth = 0;
			for (size_t i = 0; i < 3; i++)
			{
				depth += bar[i] * depths[i];
				invz += bar[i] * invzs[i];
			}
			// early z test
			if (depth >= zbuffer[int(P.x + P.y*image.get_width())])
				continue;
			pipeline.rasterize(bar, invz);
			TGAColor color;
			bool discard = pipeline.fragment(color);
			if (!discard && depth < zbuffer[int(P.x + P.y*image.get_width())])
			{
				zbuffer[int(P.x + P.y*image.get_width())] = depth;
				image.set(P.x, P.y, color);
			}
		}
	}
}

void triangle_multi_thread(Vec4f * pts, IPipeLine & pipeline, FrameBuffer& framebuffer)
{
	// lanuch 1 + n thread, thread 1 produce queue<P>, n thread rasterize triangle
	auto& image = framebuffer.image;
	auto& zbuffer = framebuffer.zbuffer;
	Mat4f toScreen = Map2Screen(image.get_width(), image.get_height());
	Vec2i bbox_min(image.get_width() - 1, image.get_height() - 1);
	Vec2i bbox_max(0, 0);
	Vec2i clamp(image.get_width() - 1, image.get_height() - 1);

	Vec2f screen_position[3];
	float depths[3];
	float invzs[3];
	for (int i = 0; i < 3; i++)
	{
		invzs[i] = 1.0f / pts[i].w;
		auto&& clipSpaceCoord = pts[i] * invzs[i];
		Vec3f p = Vec3f(toScreen * clipSpaceCoord);
		screen_position[i] = Vec2f(p);
		depths[i] = p.z;
		for (int j = 0; j < 2; j++)
		{
			bbox_min[j] = max(0, min(bbox_min[j], (int)screen_position[i][j]));
			bbox_max[j] = min(clamp[j], max(bbox_max[j], (int)screen_position[i][j]));
		}
	}

	Vec2f P;
	for (P.x = bbox_min.x; P.x <= bbox_max.x; P.x++)
	{
		for (P.y = bbox_min.y; P.y <= bbox_max.y; P.y++)
		{
			thread tid(thread_func, ref(pipeline), ref(framebuffer), P, screen_position, depths, invzs);
			tid.detach();
			//thread_func(pipeline, framebuffer, P, screen_position, depths, invzs);
		}
	}
}
