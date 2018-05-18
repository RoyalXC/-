#pragma once

#ifndef OBJ_FILE_LOADER_H
#define OBJ_FILE_LAODER_H

#include <vector>
#include <string>

// 存储三角形面片数据的数据结构
struct Triangle {
	unsigned int m_vertex_ids[3]; // 三角形三个顶点的索引
	unsigned int m_texture_ids[3]; // 三角形三个顶点纹理坐标的索引
	unsigned int m_normal_ids[3]; // 三角形三个顶点处法向量的索引
};

// OBJ文件加载类
class ObjFileLoader {
public:
	ObjFileLoader(); // 构造函数
	~ObjFileLoader(); // 析构函数
	ObjFileLoader(const ObjFileLoader& aRhs); // 拷贝构造函数
	ObjFileLoader& operator=(const ObjFileLoader& aRhs); //赋值操作符重载

public:
	// 读取obj文件
	bool read_file(const std::string& filename);
	
	// 获取顶点坐标：id为顶点索引，vertexCoords用于存储该顶点的X,Y,Z坐标
	void get_vertex_at(unsigned int id, float* vertexCoords);

	// 获取纹理坐标：id为纹理索引，textureCoords用于存储纹理坐标的 s, t
	void get_texture_at(unsigned int id, float* textureCoords);

	void get_normal_at(unsigned int id, float * normalVector);

	// 获取顶点数目
	inline int total_vertices() const;
	
	//获取三角形面片数目
	inline int total_faces() const;
	
	// 获取指定id的三角形
	inline const Triangle& get_triangle_at(unsigned int id) const;
	
	// 获取模型的X,Y,Z的最大以及最小值
	float get_min_x() const { return m_min_x; }
	float get_min_y() const { return m_min_y; }
	float get_min_z() const { return m_min_z; }
	float get_max_x() const { return m_max_x; }
	float get_max_y() const { return m_max_y; }
	float get_max_z() const { return m_max_z; }

protected:
	
	// 计算x,y,z的最大以及最小值
	void compute_bbox();

private:
	std::vector<Triangle> m_tris; 	// 存储三角形面片的数组
	std::vector<float> m_verts; 	// 存储所有顶点的数组
	std::vector<float> m_textures; 	// 存储纹理坐标的数组
	std::vector<float> m_normals;	// 存储法向量的数组
	float m_min_x, m_max_x;			// 存储X的最小、最大值
	float m_min_y, m_max_y;			// 存储Y的最小、最大值
	float m_min_z, m_max_z;			// 存储Z的最小、最大值
};

int ObjFileLoader::total_vertices() const {
	return m_verts.size() / 3;
}

int ObjFileLoader::total_faces() const {
	return m_tris.size();
}

const Triangle& ObjFileLoader::get_triangle_at(unsigned int id) const {
	return m_tris[id];
}

#endif
