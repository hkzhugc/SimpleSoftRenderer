#pragma once
#include "geometry.h"
#include "tgaimage.h"
#include "FrameBuffer.h"

// use extern for uniform val setting
struct IPipeLine // interface pipeline
{
	virtual ~IPipeLine() {}
	// read vertex attr from model(or buffer), compute the attr, and return gl_position to rasterize
	// also do perspective divide for attr(for later interploation)
	virtual Vec4f vertex   (size_t fid, size_t vid) = 0;
	virtual void  rasterize(Vec3f bar, float invz) = 0;  // read vertex attr and barycentric coord, do interplotation
	virtual bool  fragment (TGAColor& color) = 0; // read the , return bool so we can discard some pixel
};

Vec3f barycentric(Vec2f *pts, Vec2f p);
void triangle(Vec4f *pts, IPipeLine &pipeline, FrameBuffer& framebuffer);
void triangle_multi_thread(Vec4f * pts, IPipeLine & pipeline, FrameBuffer& framebuffer);

// some basic gl mat func
Mat4f Map2Screen(int w, int h);
Mat4f toScreen(int w, int h);
Mat4f project(float camera_distance);
Mat4f project(float fovy, float aspect, float near, float far);
Mat4f lookAt(Vec3f eye, Vec3f center, Vec3f up);