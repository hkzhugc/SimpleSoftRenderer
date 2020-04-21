#pragma once
#include "tgaimage.h"
#include "geometry.h"
class FrameBuffer
{
public:
	FrameBuffer();
	~FrameBuffer();
	void init(int w, int h, int bpp);
	TGAImage image;
	float *zbuffer;

	// draw function
	void line(int x0, int y0, int x1, int y1, TGAColor color);
	void line(Vec2i v0, Vec2i v1, TGAColor color);
private:
	bool ztest;
	float default_z;
	TGAColor default_color;
};

