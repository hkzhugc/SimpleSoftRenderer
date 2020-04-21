#include "Model.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;
Model::Model(const std::string& path)
{
	ifstream file(path);
	if (!file.is_open())
	{
		cerr << "cannot open file from " << path << endl;
		return;
	}
	string line;
	while (!file.eof())
	{
		getline(file, line);
		istringstream ss(line);
		char temp;
		string prefix;
		if (line.compare(0, 2, "v ") == 0) // vertex position
		{
			ss >> prefix; // read v
			Vec3f p;
			for (int i = 0; i < 3; i++) ss >> p.raw[i];
			verts_.push_back(p);
		}
		else if (line.compare(0, 3, "vt ") == 0) // vertex position
		{
			ss >> prefix; // read vt
			Vec3f p;
			for (int i = 0; i < 3; i++) ss >> p.raw[i];
			uvs_.push_back(p);
		}
		else if (line.compare(0, 3, "vn ") == 0) // vertex position
		{
			ss >> prefix; // read vt
			Vec3f p;
			for (int i = 0; i < 3; i++) ss >> p.raw[i];
			normals_.push_back(p);
		}
		else if (line.compare(0, 2, "f ") == 0) // face info
		{
			ss >> prefix; // read f
			vector<Vec3i> f;
			Vec3i idx;
			while (ss >> idx.ivert >> temp >> idx.iuv >> temp >> idx.inorm)
			{
				idx.ivert--;
				idx.iuv--;
				idx.inorm--;
				f.push_back(idx);
			}
			faces_.push_back(f);
		}
	}
	cout << "read #v " << nverts() << " #f " << nfaces() << endl;
}


Model::~Model()
{
}
