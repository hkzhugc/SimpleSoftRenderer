#include <iostream>
#include <algorithm>
#include <typeinfo>
#include "tgaimage.h"
#include "geometry.h"
#include "Model.h"
#include "FrameBuffer.h"
#include "my_gl.h"
using namespace std;
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);
Vec3f light_dir = Vec3f(1, -1, 1).normalize();
int width = 1600;
int height = 1600;

//        0. read next course
// TODO : 1. multi thread to boost render
//        2. SSE implement mat and vector
//        3. mipmap and bilnear sample

struct uvMapPipeLine : public IPipeLine
{
	Mat4f sModel;
	Mat4f sView;
	Mat4f sProject;
	TGAImage* tex1;
	Model* model;
	virtual ~uvMapPipeLine() {}
	virtual Vec4f vertex(size_t fid, size_t nthvert)
	{
		Vec4f gl_Postion = Vec4f(model->vert(fid, nthvert), 1.0f);
		uvs[nthvert] = model->uv(fid, nthvert);
		gl_Postion = sProject * sView * sModel * gl_Postion;
		uvs[nthvert] = uvs[nthvert] / gl_Postion.w;
		return gl_Postion;
	}
	virtual void rasterize(Vec3f bar, float invz)
	{
		uv = uvs[0] * bar[0] + uvs[1] * bar[1] + uvs[2] * bar[2];
		uv = uv / invz;
	}
	virtual bool fragment(TGAColor& color)
	{
		color = tex1->sample(uv.x, uv.y, TGAImage::NEAREST);
		return false;
	}
private:
	Vec2f uvs[3];
	Vec2f uv;
} uvshader;

int main(int argc, char** argv)
{
	FrameBuffer target2;
	target2.init(width, height, TGAImage::RGB);
	Model african_face("../models/african_head.obj");
	TGAImage texture;
	texture.read_tga_file("../models/african_head_diffuse.tga");
	texture.flip_vertically();

	// setting shader uniform val
	uvshader.model = &african_face;
	uvshader.tex1  =  &texture;

	// setting mat
	Vec3f eye(0.f, 0.f, 3.f);
	Vec3f center(0.f);

	Mat4f view = lookAt(eye, center, Vec3f(0.f, 1.f, 0.f));
	Mat4f projection = project(90.f / 180.f * 3.14159, 1, 0.1, 100);
	Mat4f view_port = toScreen(width, height);
	uvshader.sView = view;
	uvshader.sProject = projection;
	uvshader.sModel = Mat4f(1.0f);

	for (size_t i = 0; i < african_face.nfaces(); i++)
	{
		Vec4f screen_coords[3];
		for (size_t j = 0; j < 3; j++)
		{
			screen_coords[j] = uvshader.vertex(i, j);
		}
		triangle(screen_coords, uvshader, target2);
		//triangle_multi_thread(screen_coords, uvshader, target2);
	}
	cout << "ok" << endl;
	target2.image.flip_vertically();
	target2.image.write_tga_file("output2.tga");
	system("PAUSE");
	return 0;
}