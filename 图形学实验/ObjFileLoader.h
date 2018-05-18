#pragma once

#ifndef OBJ_FILE_LOADER_H
#define OBJ_FILE_LAODER_H

#include <vector>
#include <string>

// �洢��������Ƭ���ݵ����ݽṹ
struct Triangle {
	unsigned int m_vertex_ids[3]; // �������������������
	unsigned int m_texture_ids[3]; // ���������������������������
	unsigned int m_normal_ids[3]; // �������������㴦������������
};

// OBJ�ļ�������
class ObjFileLoader {
public:
	ObjFileLoader(); // ���캯��
	~ObjFileLoader(); // ��������
	ObjFileLoader(const ObjFileLoader& aRhs); // �������캯��
	ObjFileLoader& operator=(const ObjFileLoader& aRhs); //��ֵ����������

public:
	// ��ȡobj�ļ�
	bool read_file(const std::string& filename);
	
	// ��ȡ�������꣺idΪ����������vertexCoords���ڴ洢�ö����X,Y,Z����
	void get_vertex_at(unsigned int id, float* vertexCoords);

	// ��ȡ�������꣺idΪ����������textureCoords���ڴ洢��������� s, t
	void get_texture_at(unsigned int id, float* textureCoords);

	void get_normal_at(unsigned int id, float * normalVector);

	// ��ȡ������Ŀ
	inline int total_vertices() const;
	
	//��ȡ��������Ƭ��Ŀ
	inline int total_faces() const;
	
	// ��ȡָ��id��������
	inline const Triangle& get_triangle_at(unsigned int id) const;
	
	// ��ȡģ�͵�X,Y,Z������Լ���Сֵ
	float get_min_x() const { return m_min_x; }
	float get_min_y() const { return m_min_y; }
	float get_min_z() const { return m_min_z; }
	float get_max_x() const { return m_max_x; }
	float get_max_y() const { return m_max_y; }
	float get_max_z() const { return m_max_z; }

protected:
	
	// ����x,y,z������Լ���Сֵ
	void compute_bbox();

private:
	std::vector<Triangle> m_tris; 	// �洢��������Ƭ������
	std::vector<float> m_verts; 	// �洢���ж��������
	std::vector<float> m_textures; 	// �洢�������������
	std::vector<float> m_normals;	// �洢������������
	float m_min_x, m_max_x;			// �洢X����С�����ֵ
	float m_min_y, m_max_y;			// �洢Y����С�����ֵ
	float m_min_z, m_max_z;			// �洢Z����С�����ֵ
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
