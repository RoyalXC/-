
#include "ObjFileLoader.h"

#include <cassert>
#include <fstream>
#include <sstream>
#include <algorithm>
using namespace std;

ObjFileLoader::ObjFileLoader(): m_min_x(1e6),
								m_min_y(1e6),
								m_min_z(1e6),
								m_max_x(-1e6),
								m_max_y(-1e6),
								m_max_z(-1e6)
{

}

ObjFileLoader::~ObjFileLoader() {

}

ObjFileLoader::ObjFileLoader(const ObjFileLoader& aRhs)
{
	m_tris = aRhs.m_tris;
	m_verts = aRhs.m_verts;
	m_textures = aRhs.m_textures;
	m_normals = aRhs.m_normals;

	m_min_x = aRhs.m_min_x;
	m_min_y = aRhs.m_min_y;
	m_min_z = aRhs.m_min_z;

	m_max_x = aRhs.m_max_x;
	m_max_y = aRhs.m_max_y;
	m_max_z = aRhs.m_max_z;
}

ObjFileLoader& ObjFileLoader::operator=(const ObjFileLoader& aRhs) {
	if (this != &aRhs) {
		m_tris = aRhs.m_tris;
		m_verts = aRhs.m_verts;
		m_textures = aRhs.m_textures;
		m_normals = aRhs.m_normals;

		m_min_x = aRhs.m_min_x;
		m_min_y = aRhs.m_min_y;
		m_min_z = aRhs.m_min_z;

		m_max_x = aRhs.m_max_x;
		m_max_y = aRhs.m_max_y;
		m_max_z = aRhs.m_max_z;
	}
	return *this;
}

bool ObjFileLoader::read_file(const string& filename) {
	ifstream inputs(filename);

	if (inputs) {
		while (!inputs.eof()) {
			string line;
			getline(inputs, line, '\n');

			if (line.empty())
				continue;
			if (line[0] == '#') // 注释行
				continue;

			istringstream iss(line);
			string head;
			iss >> head;
			if (head == "v") { // 处理顶点坐标
				// TO DO: 获取顶点的X, Y, Z坐标并将其存入m_verts数组
				float x,y,z;
				iss >> x >> y >> z;
				m_verts.push_back(x);
				m_verts.push_back(y);
				m_verts.push_back(z);
				
			}
			else if (head == "vt") { // 处理纹理坐标
				// TO DO: 获取纹理的s, t坐标并将其存入m_textures数组
				float s,t;
				iss >> s >> t;
				m_textures.push_back(s);
				m_textures.push_back(t);
				
				
			}
			else if (head == "vn") { // 处理法向量坐标
				// TO DO: 获取法向量的X, Y, Z坐标并将其存入m_normals数组
				float x,y,z;
				iss >> x >> y >> z;
				m_normals.push_back(x);
				m_normals.push_back(y);
				m_normals.push_back(z);
				
			}
			else if (head == "f") { // 处理三角形面片
				string v[3];
				Triangle t;

				// 处理三角形的各个顶点
				for (int i = 0; i < 3; ++i) {
					iss >> v[i];
					replace(v[i].begin(), v[i].end(), '/', ' ');
					istringstream ss(v[i]);
					unsigned int vertex_id, texture_id, normal_id;
					ss >> vertex_id >> texture_id >> normal_id;
					t.m_vertex_ids[i] = vertex_id - 1;
					t.m_texture_ids[i] = texture_id - 1;
					t.m_normal_ids[i] = normal_id - 1;
				}
				m_tris.push_back(t);
			}
		}

		// 计算x,y,z的最大和最小值
		compute_bbox();
		
		return true;
	}
	return false;
}

void ObjFileLoader::get_vertex_at(unsigned int id, float* vertexCoords) {
	assert(id >= 0 && id < m_verts.size());
	
	// TO DO: 请从m_verts获取指定id处的X, Y, Z坐标并将其写入vertexCoords数组中
	float x = m_verts.at(id*3);
	float y = m_verts.at(id*3+1);
	float z = m_verts.at(id*3+2);

	vertexCoords[0] = x;
	vertexCoords[1] = y;
	vertexCoords[2] = z;

}

void ObjFileLoader::get_texture_at(unsigned int id, float* textureCoords) {
	assert(id >= 0 && id < m_textures.size());

	// TO DO: 请从m_textures获取指定id处的s, t坐标并将其写入textureCoords数组中
	float s = m_textures.at(id*2);
	float t = m_textures.at(id*2+1);

	textureCoords[0] = s;
	textureCoords[1] = t;

}

void ObjFileLoader::compute_bbox() {
	for (unsigned int i = 0; i < m_verts.size() / 3; ++i) {
		if (m_verts[i*3] < m_min_x)
			m_min_x = m_verts[i * 3];
		if (m_verts[i*3] > m_max_x)
			m_max_x = m_verts[i * 3];

		if (m_verts[i*3+1] < m_min_y)
			m_min_y = m_verts[i * 3 +1];
		if (m_verts[i*3+1] > m_max_y)
			m_max_y = m_verts[i * 3 +1];

		if (m_verts[i*3+2] < m_min_z)
			m_min_z = m_verts[i * 3 +2];
		if (m_verts[i*3+2] > m_max_z)
			m_max_z = m_verts[i * 3 +2];
	}
}