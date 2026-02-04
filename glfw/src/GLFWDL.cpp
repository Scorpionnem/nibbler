/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GLFWDL.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 14:52:12 by mbatty            #+#    #+#             */
/*   Updated: 2025/12/18 15:15:53 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GLFWDL.hpp"

void	GLFWDL::stop()
{
	glfwDestroyWindow(_window);
	glfwTerminate();
}

void	GLFWDL::_keyHook(GLFWwindow *window, int key, int, int action, int)
{
	GLFWDL	*glfwdl = static_cast<GLFWDL*>(glfwGetWindowUserPointer(window));

	if (action == GLFW_PRESS)
		glfwdl->_inputs.push_back(key);
}

void	GLFWDL::_closeHook(GLFWwindow *window)
{
	GLFWDL	*glfwdl = static_cast<GLFWDL*>(glfwGetWindowUserPointer(window));

	glfwdl->_shouldClose = true;
}

void	GLFWDL::open(GameState &gameState)
{
	int	width = gameState.getWidth() * SQUARE_SIZE;
	int	height = gameState.getHeight() * SQUARE_SIZE;

	glfwInit();
	_window = glfwCreateWindow(width, height, "Nibbler GLFW", NULL, NULL);
	glfwMakeContextCurrent(_window);
	glfwSetKeyCallback(_window, GLFWDL::_keyHook);
	glfwSetWindowCloseCallback(_window, GLFWDL::_closeHook);
	glfwSetWindowUserPointer(_window, this);
}

void	GLFWDL::render(int width, int height, const std::string &map)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glfwSwapBuffers(_window);
	(void)width;(void)height;(void)map;
}

GraphicsDL::Input	GLFWDL::getInput()
{
	glfwPollEvents();

	if (_shouldClose)
	{
		_shouldClose = false;
		return (GraphicsDL::Input::CLOSE);
	}
	if (_inputs.size())
	{
		int	key = _inputs.back();
		_inputs.pop_back();
		switch (key)
		{
			case GLFW_KEY_W: 
				return (GraphicsDL::Input::UP);
			case GLFW_KEY_A:
				return (GraphicsDL::Input::LEFT);
			case GLFW_KEY_S:
				return (GraphicsDL::Input::DOWN);
			case GLFW_KEY_D:
				return (GraphicsDL::Input::RIGHT);
			case GLFW_KEY_1:
				return (GraphicsDL::Input::SWITCH1);
			case GLFW_KEY_2:
				return (GraphicsDL::Input::SWITCH2);
			case GLFW_KEY_3:
				return (GraphicsDL::Input::SWITCH3);
			case GLFW_KEY_ESCAPE:
				return (GraphicsDL::Input::CLOSE);
		}
	}
	return (GraphicsDL::Input::NONE);
}
