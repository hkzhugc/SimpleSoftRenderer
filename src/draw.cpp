#include "draw.h"
#include <algorithm>
using namespace std;
Vec3f barycentric(Vec3f * pts, Vec3f p)
{
	Vec3f AB = pts[1] - pts[0];
	Vec3f AC = pts[2] - pts[0];
	Vec3f PA = pts[0] - p;
	Vec3f uv1 = cross(Vec3f(AB.x, AC.x, PA.x), Vec3f(AB.y, AC.y, PA.y));
	if (abs(uv1[2]) < 1e-2) return Vec3f(-1, 1, 1);
	return Vec3f(1.f - (uv1.x + uv1.y) / uv1.z, uv1.x / uv1.z, uv1.y / uv1.z);
}
