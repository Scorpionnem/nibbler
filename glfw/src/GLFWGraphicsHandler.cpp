/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GLFWGraphicsHandler.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 10:41:31 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/10 13:38:45 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GLFWGraphicsHandler.hpp"

GLFWGraphicsHandler::GLFWGraphicsHandler()
{
}

GLFWGraphicsHandler::~GLFWGraphicsHandler()
{
}

std::vector<Input>	inputs;

void GLFWGraphicsHandler::open(int mapSizeX, int mapSizeY)
{
	if (!glfwInit())
		throw std::runtime_error("Failed to initialize glfw");
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWmonitor	*monitor = NULL;
	this->_width = 32 * mapSizeX;
	this->_height = 32 * mapSizeY;
	#if FULL_SCREEN
		monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode	*monitorInfos = glfwGetVideoMode(monitor);
		this->_height = monitorInfos->height;
		this->_width = monitorInfos->width;
	#endif
	_data = glfwCreateWindow(this->_width, this->_height, "glfw", monitor, NULL);
	if (!_data)
	{
		glfwTerminate();
		throw std::runtime_error("Failed to create window");
	}
	glfwMakeContextCurrent(_data);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		glfwTerminate();
		throw std::runtime_error("Failed to init GLAD");
	}
	glViewport(0, 0, this->_width, this->_height);

	glfwSetKeyCallback(_data, [](GLFWwindow *, int key, int, int action, int)
	{
		if (key == GLFW_KEY_W && action == GLFW_PRESS)
			inputs.push_back(Input::UP);
		if (key == GLFW_KEY_S && action == GLFW_PRESS)
			inputs.push_back(Input::DOWN);
		if (key == GLFW_KEY_A && action == GLFW_PRESS)
			inputs.push_back(Input::LEFT);
		if (key == GLFW_KEY_D && action == GLFW_PRESS)
			inputs.push_back(Input::RIGHT);
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			inputs.push_back(Input::CLOSE);
	});

	glClearColor(0.6, 0.8, 1.0, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GLFWGraphicsHandler::render(GameState state)
{
	glfwPollEvents();
		
return ;
	for (auto it = state.map.begin(); it != state.map.end(); it++)
	{
		for (auto itt = it->begin(); itt != it->end(); itt++)
			std::cout << (int)*itt << std::endl;
	}
	(void)state;
}

std::vector<Input> GLFWGraphicsHandler::getInputs()
{
	std::vector<Input>	res;

	res = inputs;

	inputs.clear();

	return (res);
}

void GLFWGraphicsHandler::close()
{
	glfwDestroyWindow(this->_data);
	glfwTerminate();
}
