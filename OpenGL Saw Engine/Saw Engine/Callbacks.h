#pragma once
typedef struct GLFWwindow GLFWwindow;
class Callbacks {
public:
	void init(GLFWwindow* window);
	static void window_close_callback(GLFWwindow* window);
	static void window_size_callback(GLFWwindow* window, int width, int height);
	static void window_iconify_callback(GLFWwindow* window, int iconified);
	static void window_focus_callback(GLFWwindow* window, int focused);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void character_callback(GLFWwindow* window, unsigned int codepoint);
	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	static void cursor_enter_callback(GLFWwindow* window, int entered);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void drop_callback(GLFWwindow* window, int count, const char** paths);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};
