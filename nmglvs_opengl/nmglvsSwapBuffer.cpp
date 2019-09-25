#include "nmgl.h"
#include "nmglvs_nmc0.h"
extern GLFWwindow* window;

void nmglvsSwapBuffer(){
	glfwPollEvents();
	glfwSwapBuffers(window);

}