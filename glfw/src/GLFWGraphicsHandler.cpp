/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GLFWGraphicsHandler.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 10:41:31 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/10 11:20:34 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GLFWGraphicsHandler.hpp"

GLFWGraphicsHandler::GLFWGraphicsHandler()
{
}

GLFWGraphicsHandler::~GLFWGraphicsHandler()
{
}

bool GLFWGraphicsHandler::open()
{
	if (!glfwInit())
		throw std::runtime_error("Failed to initialize glfw");
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWmonitor	*monitor = NULL;
	this->_width = 500;
	this->_height = 500;
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

	glClearColor(0.6, 0.8, 1.0, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	return (true);
}

bool GLFWGraphicsHandler::render(GameState state)
{
	(void)state;
	return (false);
}

std::vector<Input> GLFWGraphicsHandler::getInputs()
{
	std::vector<Input>	res;

	return (res);
}

bool GLFWGraphicsHandler::close()
{
	return (true);
}
