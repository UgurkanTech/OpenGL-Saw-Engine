#include "Callbacks.h"
#include "GLEngine.h"

void Callbacks::init(GLFWwindow* window) {
	glfwSetWindowCloseCallback(window, window_close_callback);
	glfwSetWindowSizeCallback(window, window_size_callback);
	glfwSetWindowIconifyCallback(window, window_iconify_callback);
	glfwSetWindowFocusCallback(window, window_focus_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCharCallback(window, character_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetCursorEnterCallback(window, cursor_enter_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetDropCallback(window, drop_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}

void Callbacks::window_close_callback(GLFWwindow* window) {
	glfwSetWindowShouldClose(window, true);
}
void Callbacks::window_size_callback(GLFWwindow* window, int width, int height) {
	//gl->SCR_WIDTH = width;
	//gl->SCR_HEIGHT = height;
	//std::cout << "Window size changed" << std::endl;
	GLEngine::SCR_Scale_X = (double)width / (double)GLEngine::SCR_WIDTH;
	GLEngine::SCR_Scale_Y = (double)height / (double)GLEngine::SCR_HEIGHT;

}
void Callbacks::window_iconify_callback(GLFWwindow* window, int iconified) {
	if (iconified)
	{
		std::cout << "app is iconified" << std::endl;
	}
	else
	{
		std::cout << "app is not iconified" << std::endl;
	}
}
void Callbacks::window_focus_callback(GLFWwindow* window, int focused) {
	if (focused)
	{
		std::cout << "window focussed" << std::endl;
	}
	else
	{
		std::cout << "window not focussed" << std::endl;
	}
}
void Callbacks::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_E && action == GLFW_PRESS)
		std::cout << "Pressed E key" << std::endl;
}
void Callbacks::character_callback(GLFWwindow* window, unsigned int codepoint) {
	//std::cout << "char: " << (char)codepoint << std::endl;
}
bool firstMouse = true;
double lastX = 800.0 / 2.0;
double lastY = 600.0 / 2.0;
void Callbacks::cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	double xoffset = xpos - lastX;
	double yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	//static_cast<Camera*>(camera);
	//
	if(GLEngine::D3)
		GLEngine::camera.ProcessMouseMovement(xoffset, yoffset);
	GLEngine::gui->positionEvent(xpos / GLEngine::SCR_Scale_X, ypos / GLEngine::SCR_Scale_Y);
}
void Callbacks::cursor_enter_callback(GLFWwindow* window, int entered) {
	if (entered)
	{
		//std::cout << "entered" << std::endl;
	}
	else
	{
		//std::cout << "exited" << std::endl;
	}
}
void Callbacks::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		std::cout << "left click (" << xpos << ", " << ypos << ")" << std::endl;
		GLEngine::gui->clickEvent(xpos / GLEngine::SCR_Scale_X, ypos / GLEngine::SCR_Scale_Y);
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
		std::cout << "right click (" << xpos << ", " << ypos << ")" << std::endl;
		GLEngine::gui->clickEvent(xpos / GLEngine::SCR_Scale_X, ypos / GLEngine::SCR_Scale_Y, true);
	}
	
}
void Callbacks::drop_callback(GLFWwindow* window, int count, const char** paths) {
	for (int i = 0; i < count; i++)
		std::cout << paths[i] << std::endl;
}
void Callbacks::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	//std::cout << "yoffset: " << yoffset << std::endl;
	GLEngine::camera.ProcessMouseScroll((float)yoffset);
}
void Callbacks::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
	//std::cout << "framebuffer size changed" << std::endl;
}