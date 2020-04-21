#include "FrameBuffer.h"
#include <algorithm>
using namespace std;

FrameBuffer::FrameBuffer()
	: zbuffer(nullptr), ztest(true), default_z(std::numeric_limits<float>::max()), default_color()
{

}


FrameBuffer::~FrameBuffer()
{
	if (zbuffer)
	{
		delete[] zbuffer;
		zbuffer = nullptr;
	}
}

void FrameBuffer::init(int w, int h, int bpp)
{
	if (zbuffer)
	{
		delete[] zbuffer;
	}

	image.init(w, h, bpp);
	zbuffer = new float[w * h];
	for (int i = w*h; i--; zbuffer[i] = default_z);
}

// draw funcions
void FrameBuffer::line(int x0, int y0, int x1, int y1, TGAColor color)
{
	bool steep = false;
	if (abs(x0 - x1) < abs(y0 - y1))
	{
		swap(x0, y0);
		swap(x1, y1);
		steep = true;
	}
	if (x0 > x1)
	{
		swap(x0, x1);
		swap(y0, y1);
	}
	int dy = y1 - y0;
	int dx = x1 - x0;
	int dx2 = 2 * dx;
	int derror = 2 * abs(dy);
	int error = 0;
	int y = y0;
	int iny = y1 > y0 ? 1 : -1;
	for (int x = x0; x <= x1; x++)
	{
		if (steep)
			image.set(y, x, color);
		else
			image.set(x, y, color);
		error += derror;
		if (error > dx)
		{
			y += iny;
			error -= dx2;
		}
	}
}

void FrameBuffer::line(Vec2i v0, Vec2i v1, TGAColor color)
{
	line(v0.x, v0.y, v1.x, v1.y, color);
}
