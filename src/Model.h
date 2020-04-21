#pragma once
#include <vector>
#include <string>
#include "geometry.h"
class Model
{
public:
	Model(const std::string& path);
	~Model();
	size_t nverts() { return verts_.size(); }
	size_t nuvs() { return uvs_.size(); }
	size_t nnormals() { return normals_.size(); }
	size_t nfaces() { return faces_.size(); }
	Vec3f vert(int i) { return verts_[i]; }
	Vec3f vert(int fid, int nthvert) { return verts_[faces_[fid][nthvert].ivert]; }
	Vec3f normal(int i) { return normals_[i]; }
	Vec3f normal(int fid, int nthvert) { return normals_[faces_[fid][nthvert].inorm]; }
	Vec2f uv(int i) { return uvs_[i]; }
	Vec2f uv(int fid, int nthvert) { return uvs_[faces_[fid][nthvert].iuv]; }
	std::vector<Vec3i> face(int i) { return faces_[i]; }
private:
	std::vector<Vec3f> verts_;
	std::vector<Vec2f> uvs_;
	std::vector<Vec3f> normals_;
	std::vector<std::vector<Vec3i>> faces_;
};

