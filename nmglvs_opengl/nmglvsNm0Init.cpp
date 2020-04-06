#include "hal_target.h"
#include "hal.h"
#include <time.h>
#include <nmpp.h>

#include "nmgl.h"
#include "nmglvs_nmc0.h"



extern float vertices_DDR[2000 * 12];
extern float normal_DDR[2000 * 9];
extern float vertices_DDR2[2000 * 12];
extern float normal_DDR2[2000 * 9];
GLFWwindow* window;

int nmglvsNm0Init()
{
	glfwInit();
	//��������� GLFW
	//�������� ����������� ��������� ������ OpenGL. 
	//�������� 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
	//��������
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//��������� �������� ��� �������� ��������� ��������
	//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//���������� ����������� ��������� ������� ����
	//	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	int width = 768;
	int height = 768;
	window = glfwCreateWindow(width, height, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		//std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);


	//������������� GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		//std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);	
	return 0;
} 

