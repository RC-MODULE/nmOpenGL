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
	//Настройка GLFW
	//Задается минимальная требуемая версия OpenGL. 
	//Мажорная 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
	//Минорная
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//Установка профайла для которого создается контекст
	//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//Выключение возможности изменения размера окна
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


	//инициализация GLEW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		//std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);	
	return 0;
} 

