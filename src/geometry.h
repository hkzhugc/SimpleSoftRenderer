#pragma once
#include <cmath>
#include <vector>
#include <intrin.h>
template <typename t>
struct Vec2;

template <typename t>
struct Vec3;

template <typename T, size_t Len>
struct Vec
{

};

template <typename t>
struct Vec2
{
	union // union can give more interface
	{
		struct { t x, y; };
		struct { t u, v; };
		t raw[2];
	};
	Vec2() : x(0), y(0) {}
	Vec2(t _x, t _y) : x(_x), y(_y) {}
	Vec2(Vec3<t> v3) : x(v3.x), y(v3.y) {}
	template <typename t2>
	Vec2(Vec2<t2> v) : x(t(v.x)), y(t(v.y)) {}
	Vec2<t> operator-(const Vec2<t>& rhs) const { return Vec2<t>(x - rhs.x, y - rhs.y); }
	Vec2<t> operator+(const Vec2<t>& rhs) const { return Vec2<t>(x + rhs.x, y + rhs.y); }
	Vec2<t> operator*(const Vec2<t>& rhs) const { return Vec2<t>(x * rhs.x, y * rhs.y); }
	Vec2<t> operator*(float scaler) const { return Vec2<t>(x * scaler, y * scaler); }
	Vec2<t> operator/(const Vec2<t>& rhs) const { return Vec2<t>(x / rhs.x, y / rhs.y); }
	Vec2<t> operator/(float scaler) const { return Vec2<t>(x / scaler, y / scaler); }
	Vec2<t>& operator=(const Vec2<t>& rhs) { x = rhs.x; y = rhs.y; return *this; }
	Vec2<t>& operator+=(const Vec2<t>& rhs) { x += rhs.x; y += rhs.y; return *this; }
	Vec2<t>& normalize() { *this = *this / norm(); return *this; }
	t& operator[](size_t idx) { return raw[idx]; }
	t norm2() const { return x * x + y * y; }
	t norm()  const { return sqrt(norm2()); }
};

template <typename t> 
t dot(const Vec2<t>& lhs, const Vec2<t>& rhs)
{
	return lhs.x * rhs.x + lhs.y * rhs.y;
}

template <typename t>
struct Vec3
{
	union // union can give more interface
	{
		struct { t x, y, z; }; // Vec3f
		struct { t ivert, iuv, inorm; }; // Vec3i
		t raw[3];
	};
	Vec3(t val) :x(val), y(val), z(val) {}
	Vec3() : x(0), y(0), z(0) {}
	Vec3(t _x, t _y, t _z) : x(_x), y(_y), z(_z) {}
	Vec3(Vec2<t> vec2, t _z) : x(vec2.x), y(vec2.y), z(_z) {}
	operator Vec2<t>() { return Vec2<t>(x, y); }
	Vec3<t> operator-(const Vec3<t>& rhs) const { return Vec3<t>(x - rhs.x, y - rhs.y, z - rhs.z); }
	Vec3<t> operator+(const Vec3<t>& rhs) const { return Vec3<t>(x + rhs.x, y + rhs.y, z + rhs.z); }
	Vec3<t> operator*(const Vec3<t>& rhs) const { return Vec3<t>(x * rhs.x, y * rhs.y, z * rhs.z); }
	Vec3<t> operator*(float scaler) const { return Vec3<t>(x * scaler, y * scaler, z * scaler); }
	Vec3<t> operator/(const Vec3<t>& rhs) const { return Vec3<t>(x / rhs.x, y / rhs.y, z / rhs.z); }
	Vec3<t> operator/(float scaler) const { return Vec3<t>(x / scaler, y / scaler, z / scaler); }
	Vec3<t>& operator=(const Vec3<t>& rhs) { x = rhs.x; y = rhs.y; z = rhs.z; return *this; }
	Vec3<t>& operator+=(const Vec3<t>& rhs) { x += rhs.x; y += rhs.y; z += rhs.z; return *this; }
	Vec3<t>& normalize() { *this = *this / norm(); return *this; }
	t& operator[](size_t idx) { return raw[idx]; }
	t norm2() const { return x * x + y * y + z * z; }
	t norm()  const { return sqrt(norm2()); }
};

template <typename t>
struct Vec4
{
	union // union can give more interface
	{
		struct { t x, y, z, w; }; // Vec4f
		struct { t ivert, iuv, inorm, iw; }; // Vec4i
		t raw[4];
	};
	Vec4(t val) :x(val), y(val), z(val), w(val) {}
	Vec4() : x(0), y(0), z(0), w(0) {}
	Vec4(t _x, t _y, t _z, t _w) : x(_x), y(_y), z(_z), w(_w) {}
	Vec4(Vec3<t> vec3, t _w) : x(vec3.x), y(vec3.y), z(vec3.z), w(_w) {}
	operator Vec3<t>() { return Vec3<t>(x, y, z); }
	Vec4<t> operator-(const Vec4<t>& rhs) const { return Vec4<t>(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w); }
	Vec4<t> operator+(const Vec4<t>& rhs) const { return Vec4<t>(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w); }
	Vec4<t> operator*(const Vec4<t>& rhs) const { return Vec4<t>(x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w); }
	Vec4<t> operator*(float scaler) const { return Vec4<t>(x * scaler, y * scaler, z * scaler, w * scaler); }
	Vec4<t> operator/(const Vec4<t>& rhs) const { return Vec4<t>(x / rhs.x, y / rhs.y, z / rhs.z, w / rhs.w); }
	Vec4<t> operator/(float scaler) const { return Vec4<t>(x / scaler, y / scaler, z / scaler, w / scaler); }
	Vec4<t>& operator=(const Vec4<t>& rhs) { x = rhs.x; y = rhs.y; z = rhs.z; w = rhs.w; return *this; }
	Vec4<t>& operator+=(const Vec4<t>& rhs) { x += rhs.x; y += rhs.y; z += rhs.z; w += rhs.w; return *this; }
	Vec4<t>& normalize() { *this = *this / norm(); return *this; }
	t& operator[](size_t idx) { return raw[idx]; }
	t norm2() const { return x * x + y * y + z * z; } // ignore w
	t norm()  const { return sqrt(norm2()); } // ignore w
};

template <typename t>
Vec3<t> cross(const Vec3<t>& lhs, const Vec3<t>& rhs)
{
	return Vec3<t>(lhs.y*rhs.z - lhs.z*rhs.y, lhs.z*rhs.x - lhs.x*rhs.z, lhs.x*rhs.y -lhs.y*rhs.x);
}

template <typename t>
t dot(const Vec3<t>& lhs, const Vec3<t>& rhs)
{
	return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

template <typename t, size_t orders>
struct Mat
{
	Mat() : row(orders, std::vector<t>(orders, t())) {}
	Mat(t val) : row(orders, std::vector<t>(orders, t()))
	{
		for (size_t i = 0; i < orders; i++)
			row[i][i] = val;
	}
	Mat(const Mat<t, orders>& m) : row(m.row) {}

	template <size_t unique_orders>
	operator Mat<t, unique_orders>()
	{
		Mat<t, unique_orders> ret;
		for (size_t i = 0; i < unique_orders && i < orders; i++)
			for (size_t j = 0; j < unique_orders && j < orders; j++)
				ret.row[i][j] = row[i][j];
		return ret;
	}

	Mat<t, orders> operator*(const Mat<t, orders>& m) const
	{
		Mat ret;
		for (size_t i = 0; i < orders; i++) // row
		{
			for (size_t j = 0; j < orders; j++) // col
			{
				// row first
				t val = 0;
				for (size_t k = 0; k < orders; k++)
				{
					val += row[k][j] * m.row[i][k];
				}
				ret[i][j] = val;
			}
		}
		return ret;
	}

	template <typename vecType>
	vecType operator*(const vecType& v) const
	{
		vecType ret;
		for (size_t i = 0; i < orders; i++)
		{
			t val = 0;
			for (size_t k = 0; k < orders; k++)
			{
				val += row[k][i] * v.raw[k];
			}
			ret.raw[i] = val;
		}
		return ret;
	}

	std::vector<t>& operator[](size_t i) { return row[i]; }
	operator t() const { return row[0][0]; }

	std::vector<std::vector<t>> row;
};

typedef Vec2<float> Vec2f;
typedef Vec2<int>   Vec2i;
typedef Vec3<float> Vec3f;
typedef Vec3<int>   Vec3i;
typedef Vec4<float> Vec4f;
typedef Vec4<int>   Vec4i;
typedef Mat<float, 2> Mat2f;
typedef Mat<int, 2>   Mat2i;
typedef Mat<float, 3> Mat3f;
typedef Mat<int, 3>   Mat3i;
typedef Mat<float, 4> Mat4f;
typedef Mat<int, 4>   Mat4i;
