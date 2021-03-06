// Cube.cpp : Defines the entry point for the console application.
//
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/glext.h>

#pragma comment(lib, "glew32.lib")

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include <string>
#include <iostream>
using namespace std;

// 存储绕各坐标轴的旋转角度
float XROT(0.f), YROT(0.f), ZROT(0.f);

// 存储立方体六个面的纹理ID
GLuint TEXTURE_IDS[6];

// 加载纹理图像
cv::Mat loadImage(const string& fname, int& w, int& h) {

	cv::Mat img_bgr = cv::imread(fname, cv::IMREAD_COLOR);
	cv::flip(img_bgr, img_bgr, 0);
	cv::Mat img_rgb;
	cv::cvtColor(img_bgr, img_rgb, cv::COLOR_BGR2RGB);
	w = img_rgb.size[1];
	h = img_rgb.size[0];
	return img_rgb;
}

void loadTexture() {
	/////////////// TO DO ////////////////////////////////////////////
	///////////加载立方体六个面的纹理并设置纹理属性//////////////////

	for (int i = 0; i < 6; i++)
	{
		int w = 0, h = 0;
		string str = "";
		str = "C:\\Users\\XXC\\Desktop\\图形学实验\\图形学实验\\Textures\\side" + std::to_string((i + 1));
		str += ".jpg";

		cv::Mat mat = loadImage(str, w, h);

		glBindTexture(GL_TEXTURE_2D, TEXTURE_IDS[i]);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, mat.data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
}

void init()
{
	glClearColor(0.f, 0.f, 0.f, 0.f);
	glEnable(GL_DEPTH_TEST);//开启深度测试
							//glShadeModel(GL_SMOOTH); 
							///////////// TO DO /////////////////////////////
							//////申请纹理ID、加载纹理、设置纹理环境/////////
	for (int i = 0; i < 6; i++)
		glGenTextures(i, &TEXTURE_IDS[i]);
	loadTexture();

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	//glEnable(GL_TEXTURE_2D);
}


void drawCube()
{
	///////////// TO DO ////////////////////////////
	////////////// 绘制立方体///////////////////////
	glClear(GL_COLOR_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	GLfloat earth_mat_ambient[] = { 0.5f, 0.5f, 0.5f, 1.0f };  //定义材质的环境光颜色  
	GLfloat earth_mat_diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };  //定义材质的漫反射光颜色 
	GLfloat earth_mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };   //定义材质的镜面反射光颜色  
	//GLfloat earth_mat_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };   //定义材质的辐射光颜色，为0  
	GLfloat earth_mat_shininess = 50.0f;
	glMaterialfv(GL_FRONT, GL_AMBIENT, earth_mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, earth_mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, earth_mat_specular);
	//glMaterialfv(GL_FRONT, GL_EMISSION, earth_mat_emission);
	glMaterialf(GL_FRONT, GL_SHININESS, earth_mat_shininess);

	glBindTexture(GL_TEXTURE_2D, TEXTURE_IDS[0]);
	glBegin(GL_TRIANGLE_STRIP);//前表面 								 
	glTexCoord2f(1.f, 1.f);
	glVertex3f(50.0f, 50.0f, 50.0f);

	glTexCoord2f(1.f, 0.f);
	glVertex3f(50.0f, -50.0f, 50.0f);

	glTexCoord2f(0.f, 0.f);
	glVertex3f(-50.0f, -50.0f, 50.0f);

	glTexCoord2f(0.f, 1.f);
	glVertex3f(-50.0f, 50.0f, 50.0f);

	glTexCoord2f(1.f, 1.f);
	glVertex3f(50.0f, 50.0f, 50.0f);

	glEnd();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindTexture(GL_TEXTURE_2D, TEXTURE_IDS[1]);
	//Rectangle
	glBegin(GL_TRIANGLE_STRIP);//后表面  
	glTexCoord2f(0.f, 1.f);
	glVertex3f(50.0f, 50.0f, -50.0f);

	glTexCoord2f(0.f, 0.f);
	glVertex3f(50.0f, -50.0f, -50.0f);

	glTexCoord2f(1.f, 0.f);
	glVertex3f(-50.0f, -50.0f, -50.0f);

	glTexCoord2f(1.f, 1.f);
	glVertex3f(-50.0f, 50.0f, -50.0f);

	glTexCoord2f(0.f, 1.f);
	glVertex3f(50.0f, 50.0f, -50.0f);
	glEnd();




	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindTexture(GL_TEXTURE_2D, TEXTURE_IDS[2]);
	//Rectangle
	glBegin(GL_TRIANGLE_STRIP); //上表面  
	glTexCoord2f(0.f, 1.f);
	glVertex3f(50.0f, 50.0f, -50.0f);

	glTexCoord2f(0.f, 0.f);
	glVertex3f(50.0f, 50.0f, 50.0f);

	glTexCoord2f(1.f, 0.f);
	glVertex3f(-50.0f, 50.0f, 50.0f);

	glTexCoord2f(1.f, 1.f);
	glVertex3f(-50.0f, 50.0f, -50.0f);

	glTexCoord2f(0.f, 1.f);
	glVertex3f(50.0f, 50.0f, -50.0f);
	glEnd();


	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindTexture(GL_TEXTURE_2D, TEXTURE_IDS[3]);
	//Rectangle
	glBegin(GL_TRIANGLE_STRIP); //下表面  
	glTexCoord2f(0.f, 1.f);
	glVertex3f(50.0f, -50.0f, -50.0f);

	glTexCoord2f(0.f, 0.f);
	glVertex3f(50.0f, -50.0f, 50.0f);

	glTexCoord2f(1.f, 0.f);
	glVertex3f(-50.0f, -50.0f, 50.0f);

	glTexCoord2f(1.f, 1.f);
	glVertex3f(-50.0f, -50.0f, -50.0f);

	glTexCoord2f(0.f, 1.f);
	glVertex3f(50.0f, -50.0f, -50.0f);

	glEnd();



	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindTexture(GL_TEXTURE_2D, TEXTURE_IDS[4]);
	//Rectangle
	glBegin(GL_TRIANGLE_STRIP); //左表面  
	glTexCoord2f(0.f, 1.f);
	glVertex3f(50.0f, 50.0f, 50.0f);

	glTexCoord2f(0.f, 0.f);
	glVertex3f(50.0f, 50.0f, -50.0f);

	glTexCoord2f(1.f, 0.f);
	glVertex3f(50.0f, -50.0f, -50.0f);

	glTexCoord2f(1.f, 1.f);
	glVertex3f(50.0f, -50.0f, 50.0f);

	glTexCoord2f(0.f, 1.f);
	glVertex3f(50.0f, 50.0f, 50.0f);

	glEnd();



	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindTexture(GL_TEXTURE_2D, TEXTURE_IDS[5]);
	//Rectangle
	glBegin(GL_TRIANGLE_STRIP); //右表面  
	glTexCoord2f(0.f, 1.f);
	glVertex3f(-50.0f, 50.0f, 50.0f);

	glTexCoord2f(0.f, 0.f);
	glVertex3f(-50.0f, 50.0f, -50.0f);

	glTexCoord2f(1.f, 0.f);
	glVertex3f(-50.0f, -50.0f, -50.0f);

	glTexCoord2f(1.f, 1.f);
	glVertex3f(-50.0f, -50.0f, 50.0f);

	glTexCoord2f(0.f, 1.f);
	glVertex3f(-50.0f, 50.0f, 50.0f);
	glEnd();

}

bool open0 = true;
bool open1 = true;

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	GLfloat sun0_light_position[] = { -1.0f, -1.0f, -1.0f, -1.0f }; //光源的位置在世界坐标系齐次坐标形式  
	GLfloat sun0_light_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f }; //RGBA模式的环境光  
	GLfloat sun0_light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; //RGBA模式的漫反射光  
	GLfloat sun0_light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };  //RGBA模式下的镜面光  
	glLightfv(GL_LIGHT0, GL_POSITION, sun0_light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, sun0_light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, sun0_light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, sun0_light_specular);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	if (open0)
		//开启灯光  
	{
		glEnable(GL_LIGHT0);

	}
	else
	{
		glDisable(GL_LIGHT0);

	}

	GLfloat sun1_light_position[] = { 1.0f, 1.0f, 1.0f, 1.0f }; //光源的位置在世界坐标系齐次坐标形式  
	GLfloat sun1_light_ambient[] = { 1.0f, 0.0f, 0.0f, 1.0f }; //RGBA模式的环境光  
	GLfloat sun1_light_diffuse[] = { 0.0f, 1.0f, 0.0f, 1.0f }; //RGBA模式的漫反射光  
	GLfloat sun1_light_specular[] = { 0.0f, 0.0f, 1.0f, 1.0f };  //RGBA模式下的镜面光  
	glLightfv(GL_LIGHT1, GL_POSITION, sun1_light_position);
	glLightfv(GL_LIGHT1, GL_AMBIENT, sun1_light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, sun1_light_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, sun1_light_specular);

	if (open1)
		//开启灯光  
	{
		glEnable(GL_LIGHT1);

	}
	else
	{
		glDisable(GL_LIGHT1);
	}
	//////////////// TO DO /////////////////////////
	///// 设置模型、视点变换以及立方体显示 /////////

	glPushMatrix();
	gluLookAt(0.0, 0.0, 260.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	glRotatef(ZROT, 0.f, 0.f, 1.f);

	glRotatef(YROT, 0.f, 1.f, 0.f);

	glRotatef(XROT, 1.f, 0.f, 0.f);

	//glScalef(SCALE,SCALE,SCALE);*/

	drawCube();
	glPopMatrix();
	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	////////////// TO DO /////////////////////////////
	//////// 设置三维透视投影/////////////////////////
	gluPerspective(60.0, float(w) / float(h), 10.0, 1000.0);
}

// 键盘按键回掉函数
void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case '1':
		open0 = !open0;
		glutPostRedisplay(); //更新显示内容
		break;
	case '2':
		open1 = !open1;
		glutPostRedisplay(); //更新显示内容
		break;
	case 'x':
		///////////////// TO DO /////////////////////
		//////////////设置绕x轴的旋转角度////////////
		XROT -= 5.0f;

		glutPostRedisplay(); //更新显示内容
		break;
	case 'y':
		///////////////// TO DO /////////////////////
		//////////////设置绕y轴的旋转角度////////////

		YROT -= 5.0f;
		glutPostRedisplay();
		break;
	case 'z':
		///////////////// TO DO /////////////////////
		//////////////设置绕z轴的旋转角度////////////	
		ZROT -= 5.0f;
		glutPostRedisplay();
		break;
	case 'r':
		///////////////// TO DO /////////////////////
		//////////////复原所有旋转角度///////////////
		XROT = 0.f;
		YROT = 0.f;
		ZROT = 0.f;

		glutPostRedisplay();
	default:
		break;
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitContextVersion(2, 1);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Cube");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);

	init();

	glutMainLoop();

	return 0;
}

