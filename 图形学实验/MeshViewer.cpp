// MeshViewer.cpp : Defines the entry point for the console application.
//

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/glext.h>

#pragma comment(lib, "glew32.lib")

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

#include "ObjFileLoader.h"

#include <iostream>
#include <string>
#include <ctime>
using namespace std;

ObjFileLoader OBJ_FILE_LOADER; // OBJ文件加载类的对象
static unsigned int MESH_LIST_ID; // 用于显示三维模型的显示列表ID
static unsigned int MESH_LIST2_ID; // 用于显示三维模型的显示列表ID
static unsigned int BARK_TEXTURE_ID; // 模型纹理ID
static unsigned int BRANCH_TEXTURE_ID; // 模型纹理ID
static float MIN_X, MIN_Y, MIN_Z, MAX_X, MAX_Y, MAX_Z; //存储X,Y,Z的最大以及最小值
static float OFFSET_X, OFFSET_Y, OFFSET_Z; // 模型的中心坐标
static float Z_HALF_LENGTH(1.f); // 用于设置照相机位置
static float ROT_X(0.f), ROT_Y(0.f), ROT_Z(0.f); // 绕X,Y,Z轴的旋转角度
static float SCALE(1.f); // 缩放比例
static int TEX_W, TEX_H; // 纹理的宽和高

// 加载纹理图像
cv::Mat loadImage(const string& fname, int& w, int& h) {
	cv::Mat img_bgr = cv::imread(fname, cv::IMREAD_COLOR);
	cv::flip(img_bgr, img_bgr, 0);
	cv::Mat img_rgb;
	cv::cvtColor(img_bgr, img_rgb, cv::COLOR_BGR2RGB);
	w = img_rgb.cols;
	h = img_rgb.rows;
	return img_rgb;
}

// 加载纹理
void loadTexture() {
	// 纹理图像的位置，请根据你工程的实际路径设置
	cv::Mat texture_img = loadImage("C:\\Users\\XXC\\Desktop\\图形学实验\\图形学实验\\Textures\\bark_08.jpg", TEX_W, TEX_H);
	if (texture_img.empty()) {
		cout << "Failed load the texture file!" << endl;
	}
	else {
		glBindTexture(GL_TEXTURE_2D, BARK_TEXTURE_ID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TEX_W, TEX_H, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_img.data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		cout << "Load the texture image successfully!" << endl;
	}//BRANCH_TEXTURE_ID

	// 地板纹理图像的位置
	texture_img = loadImage("C:\\Users\\XXC\\Desktop\\图形学实验\\图形学实验\\Textures\\Branch.jpg", TEX_W, TEX_H);
	if (texture_img.empty()) {
		cout << "Failed load the texture file!" << endl;
	}
	else {
		glBindTexture(GL_TEXTURE_2D, BRANCH_TEXTURE_ID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TEX_W, TEX_H, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_img.data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		cout << "Load the texture image successfully!" << endl;

	}
}
int face1Num = 1;
// 加载模型
bool load_mesh(const string& filename, int num)
{
	cout << "Start loading the obj file, please wait......" << endl;

	// 读取网格对应的obj文件
	clock_t time_start = clock();
	bool load_success = OBJ_FILE_LOADER.read_file(filename);
	float time_elapsed = float(clock() - time_start) / CLOCKS_PER_SEC;
	if (!load_success) {
		cout << "Failed read the mesh obj file!" << endl;
		return false;
	}
	cout << "Finished load the obj file! Time elapsed: " << time_elapsed << " seconds." << endl;
	cout << "Total Vertices: " << OBJ_FILE_LOADER.total_vertices() << endl;
	cout << "Total Faces: " << OBJ_FILE_LOADER.total_faces() << endl;
	if (num == 1)
		face1Num = OBJ_FILE_LOADER.total_faces();
	MIN_X = OBJ_FILE_LOADER.get_min_x();
	MIN_Y = OBJ_FILE_LOADER.get_min_y();
	MIN_Z = OBJ_FILE_LOADER.get_min_z();
	MAX_X = OBJ_FILE_LOADER.get_max_x();
	MAX_Y = OBJ_FILE_LOADER.get_max_y();
	MAX_Z = OBJ_FILE_LOADER.get_max_z();

	cout << "Min x: " << MIN_X << ", Max X: " << MAX_X << endl;
	cout << "Min y: " << MIN_Y << ", Max Y: " << MAX_Y << endl;
	cout << "Min z: " << MIN_Z << ", Max Z: " << MAX_Z << endl;

	OFFSET_X = (MIN_X + MAX_X) * 0.5f;
	OFFSET_Y = (MIN_Y + MAX_Y) * 0.5f;
	OFFSET_Z = (MIN_Z + MAX_Z) * 0.5f;
	Z_HALF_LENGTH = (MAX_Z - MIN_Z) * 0.5f;;


	if (num == 1) {


		// 创建显示列表
		MESH_LIST_ID = glGenLists(1);

		// 填充显示列表
		glNewList(MESH_LIST_ID, GL_COMPILE);
		glBindTexture(GL_TEXTURE_2D, BARK_TEXTURE_ID);
		glBegin(GL_TRIANGLES);
		for (int i = 0; i < OBJ_FILE_LOADER.total_faces(); ++i) {
			const Triangle& t = OBJ_FILE_LOADER.get_triangle_at(i);// 获取三角形面片 
			//获取三角形的顶点数据
			for (int j = 0; j < 3; ++j) {
				float vertex_coords[3] = { 0 };  // 存储顶点的x,y,z坐标
				float texture_coords[2] = { 0 }; // 存储顶点的纹理坐标
				float vector_coords[2] = { 0 }; // 存储顶点的纹理坐标

				// TO DO : 获取并指定三角形三个顶点处的纹理坐标以及X,Y,Z空间坐标
				OBJ_FILE_LOADER.get_vertex_at(t.m_vertex_ids[j], vertex_coords);
				OBJ_FILE_LOADER.get_texture_at(t.m_texture_ids[j], texture_coords);
				OBJ_FILE_LOADER.get_normal_at(t.m_normal_ids[j], vector_coords);

				glTexCoord2f(texture_coords[0], texture_coords[1]);
				glVertex3f(vertex_coords[0], vertex_coords[1], vertex_coords[2]);
				glNormal3fv(vector_coords);

			}
		}
		glEnd();

		glEndList();

		return true;
	}

	else if (num == 2) {


		// 创建显示列表
		MESH_LIST2_ID = glGenLists(1);

		// 填充显示列表
		glNewList(MESH_LIST2_ID, GL_COMPILE);
		glBindTexture(GL_TEXTURE_2D, BRANCH_TEXTURE_ID);
		glBegin(GL_TRIANGLES);
		for (int i = face1Num-1; i < OBJ_FILE_LOADER.total_faces(); ++i) {
			const Triangle& t = OBJ_FILE_LOADER.get_triangle_at(i);// 获取三角形面片 
																   //获取三角形的顶点数据
			for (int j = 0; j < 3; ++j) {
				float vertex_coords[3] = { 0 };  // 存储顶点的x,y,z坐标
				float texture_coords[2] = { 0 }; // 存储顶点的纹理坐标
				float vector_coords[2] = { 0 }; // 存储顶点的纹理坐标

												// TO DO : 获取并指定三角形三个顶点处的纹理坐标以及X,Y,Z空间坐标
				OBJ_FILE_LOADER.get_vertex_at(t.m_vertex_ids[j], vertex_coords);
				OBJ_FILE_LOADER.get_texture_at(t.m_texture_ids[j], texture_coords);
				OBJ_FILE_LOADER.get_normal_at(t.m_normal_ids[j], vector_coords);

				glTexCoord2f(texture_coords[0], texture_coords[1]);
				glVertex3f(vertex_coords[0], vertex_coords[1], vertex_coords[2]);
				glNormal3fv(vector_coords);

			}
		}
		glEnd();

		glEndList();

		return true;
	}


}

void init()
{
	glClearColor(1.f, 1.f, 1.f, 0.f);
	glEnable(GL_DEPTH_TEST);

	glGenTextures(1, &BARK_TEXTURE_ID);
	glGenTextures(2, &BRANCH_TEXTURE_ID);

	loadTexture();

	//glTexEnvf(GL_TEXTURE_ENV, GL_MODULATE, GL_REPLACE);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glEnable(GL_TEXTURE_2D);

	// 三位模型的路径，请根据你实际工程的路径正确设置
	load_mesh("C:\\Users\\XXC\\Desktop\\图形学实验\\图形学实验\\Meshes\\Tree96-Branch.obj", 1);
	load_mesh("C:\\Users\\XXC\\Desktop\\图形学实验\\图形学实验\\Meshes\\Tree96-Ground.obj", 2);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// TO DO: 设置照相机
	gluLookAt(0.0, 0.0, 260.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	glRotatef(ROT_Z, 0.f, 0.f, 1.f);
	glRotatef(ROT_Y, 0.f, 1.f, 0.f);
	glRotatef(ROT_X, 1.f, 0.f, 0.f);

	//glScalef(SCALE, SCALE, SCALE);

	//glTranslatef(-OFFSET_X, -OFFSET_Y, -OFFSET_Z);

	glColor3f(0.f, 0.f, 1.f);


	//glCallList(MESH_LIST_ID);
	glCallList(MESH_LIST2_ID);

	glutSwapBuffers();
}

void reshape(int w, int h) {
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// TO DO: 设置投影变换矩阵
	gluPerspective(60.0, float(w) / float(h), 10.0, 500.0);

}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'x':
		ROT_X += 15.f;
		if (ROT_X > 360.f) ROT_X -= 360.f;
		glutPostRedisplay();
		break;
	case 'y':
		ROT_Y += 15.f;
		if (ROT_Y > 360.f) ROT_Y -= 360.f;
		glutPostRedisplay();
		break;
	case 'z':
		ROT_Z += 15.f;
		if (ROT_Z > 360.f) ROT_Z -= 360.f;
		glutPostRedisplay();
		break;
	case 's':
		SCALE -= 0.1f;
		if (SCALE < 0.1f) SCALE = 0.1f;
		glutPostRedisplay();
		break;
	case 'S':
		SCALE += 0.1f;
		if (SCALE > 5.f) SCALE = 5.f;
		glutPostRedisplay();
		break;
	case 'r':
		ROT_X = 0.f;
		ROT_Y = 0.f;
		ROT_Z = 0.f;
		SCALE = 1.f;
		glutPostRedisplay();
		break;
	default:
		break;
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(680, 680);
	glutCreateWindow("3DMeshView");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);

	glewExperimental = GL_TRUE;
	glewInit();

	init();

	glutMainLoop();

	return 0;
}

