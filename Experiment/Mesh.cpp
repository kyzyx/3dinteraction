#include "Mesh.h"
#include <Windows.h>
#include <iostream>
#include <string>
#include <fstream>
#include <exception>
#include <set>
#include <vector>

using namespace std;

using namespace Eigen;

Mesh::Mesh(void)
{
	vertices = NULL;
	colors = NULL;
	texture = NULL;
	use_color = false;
	isflatshaded = true;

	translation = Vector3f::Zero();
	scale = Vector3f::Zero();
	rotation = Quaternionf::Identity();
}

Mesh::Mesh(const char* filename, Renderer* r, bool reverse, bool flatshaded) {
	render = r;
	isflatshaded = flatshaded;
	ReadOff(filename, reverse);
	translation = Vector3f::Zero();
	scale = Vector3f::Zero();
	rotation = Quaternionf::Identity();
}

bool isblank(string s) {
	for (unsigned int i = 0; i < s.size(); ++i) {
		if (s[i] != ' ' && s[i] != '\t' && s[i] != '\n' && s[i] != '#') return false;
		else if (s[i] == '#') return true;
	}
	return true;
}

inline void getnextline(ifstream& in, string& line) {
	do {
		getline(in, line);
	} while (isblank(line));
	
}

bool Mesh::ReadOff(const char* filename, bool reverse) {
	ifstream in(filename);
	if (in.fail()) throw OFFFileFormatError("Unable to open file");
	// OFF File
	string line;
	getnextline(in, line);
	if (line != "OFF") throw OFFFileFormatError("Invalid OFF Header");
	getnextline(in, line);
	if (sscanf_s(line.c_str(), "%d %d %d", &nvert, &nface, &nedge) != 3) {
		throw OFFFileFormatError("Invalid OFF line counts"); 
	}
	indices = new unsigned int[nface*3];
	vertices = new float[nvert*3];
	colors = new float[nvert*4];
	texture = new float[nvert*2];
	normals = new float[nvert*3];

	vector<float> v;
	vector<float> c;
	vector<float> t;

	// Read in vertex data
	for (int i = 0; i < nvert; ++i) {
		getnextline(in, line);
		int nread = sscanf_s(line.c_str(), "%f %f %f %f %f %f %f %f", &vertices[3*i], &vertices[3*i+1], &vertices[3*i+2], &colors[4*i], &colors[4*i+1], &colors[4*i+2], &colors[4*i+3]);
		if (nread < 3 || nread == 4) {
			throw OFFFileFormatError("Incomplete vertex specification");
		} else if (nread == 5) {
			texture[2*i] = colors[4*i]; texture[2*i+1] = colors[4*i+1];
			colors[4*i] = 0; colors[4*i+1] = 0; colors[4*i+2] = 0; colors[4*i+3] = 0;
		} else {
			use_color = (nread != 3);
			if (nread == 6) colors[4*i+3] = 1;
		}
	}
	if (isflatshaded) nvert = 0;
	for (int i = 0; i < nface; ++i) {
		getnextline(in, line);
		int num;
		// Only handle triangles
		if (!sscanf_s(line.c_str(), "%d", &num)) {
			throw OFFFileFormatError("Invalid face specification");
		}
		if (num != 3) {
			throw OFFFileFormatError("Invalid face size - must be triangle");
		} else {
			int x,y,z;
			if (sscanf_s(line.c_str(), "%d %d %d %d", &num,&x,&y,&z) != 4) {
				throw OFFFileFormatError("Incomplete face specification");
			}
			if (isflatshaded) {
				for (int i = 0; i < 3; ++i) v.push_back(vertices[3*x+i]);
				for (int i = 0; i < 4; ++i) c.push_back(colors[4*x+i]);
				for (int i = 0; i < 2; ++i) t.push_back(texture[2*x+i]);
				x = nvert++;
				for (int i = 0; i < 3; ++i) v.push_back(vertices[3*y+i]);
				for (int i = 0; i < 4; ++i) c.push_back(colors[4*y+i]);
				for (int i = 0; i < 2; ++i) t.push_back(texture[2*y+i]);
				y = nvert++;
				for (int i = 0; i < 3; ++i) v.push_back(vertices[3*z+i]);
				for (int i = 0; i < 4; ++i) c.push_back(colors[4*z+i]);
				for (int i = 0; i < 2; ++i) t.push_back(texture[2*z+i]);
				z = nvert++;
			}
			if (reverse) {
				indices[3*i] = y; indices[3*i+1] = x; indices[3*i+2] = z;
			} else {
				indices[3*i] = x; indices[3*i+1] = y; indices[3*i+2] = z;
			}
		}
	}
	if (isflatshaded) {
		delete [] vertices;
		delete [] colors;
		delete [] texture;
		delete [] normals;

		vertices = new float[nvert*3];
		colors = new float[nvert*4];
		texture = new float[nvert*2];
		normals = new float[nvert*3];
		for (int i = 0; i < nvert; ++i) {
			for (int j = 0; j < 3; ++j) vertices[3*i+j] = v[3*i+j];
			for (int j = 0; j < 4; ++j) colors[4*i+j] = c[4*i+j];
			for (int j = 0; j < 2; ++j) texture[2*i+j] = t[2*i+j];
		}
		calcFaceNormals();
	} else {
		calcNormals();
	}
	return true;
}

void Mesh::calcFaceNormals() {
	for (int i = 0; i < nface; ++i) {
		Vector3f a(vertices[3*indices[3*i+0]], vertices[3*indices[3*i+0]+1], vertices[3*indices[3*i+0]+2]);
		Vector3f b(vertices[3*indices[3*i+1]], vertices[3*indices[3*i+1]+1], vertices[3*indices[3*i+1]+2]);
		Vector3f c(vertices[3*indices[3*i+2]], vertices[3*indices[3*i+2]+1], vertices[3*indices[3*i+2]+2]);
		Vector3f n = (c - a).cross(b - a);
		n.normalize();
		for (int j = 0; j < 3; ++j) {
			for (int k = 0; k < 3; ++k) normals[3*indices[3*i+j]+k] = n[k];
		}
	}
}

void Mesh::calcNormals() {
	// Construct adjacency list
	std::set<int>* adj = new std::set<int>[nvert];
	for (int i = 0; i < nface*3; i += 3) {
		adj[indices[i]].insert(indices[i+1]);
		adj[indices[i]].insert(indices[i+2]);
		adj[indices[i+1]].insert(indices[i]);
		adj[indices[i+1]].insert(indices[i+2]);
		adj[indices[i+2]].insert(indices[i]);
		adj[indices[i+2]].insert(indices[i+1]);
	}

	// Add vectors to each face for normals
	std::set<int>::iterator it;
	for (int i = 0; i < nvert; ++i) {
		normals[3*i] = 0.f; normals[3*i+1] = 0.f; normals[3*i+2] = 0.f;
		for (it = adj[i].begin(); it != adj[i].end(); it++) {
			for (int j = 0; j < 3; ++j) normals[3*i+j] += (vertices[3*i+j] - vertices[3*(*it)+j]);
		}
	}
	// Renormalize
	for (int i = 0; i < nvert; ++i) {
		float tot = sqrt(normals[3*i]*normals[3*i] + normals[3*i+1]*normals[3*i+1] + normals[3*i+2]*normals[3*i+2]);
		normals[3*i] /= tot; normals[3*i+1] /= tot; normals[3*i+2] /= tot;
	}
}

Mesh::~Mesh(void)
{
	if (normals) delete [] normals;
	if (vertices) delete [] vertices;
	if (colors) delete [] colors;
	if (texture) delete [] texture;
	if (indices) delete [] indices;
}
