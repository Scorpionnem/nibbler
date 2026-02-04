/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GLFWDL.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 10:36:54 by mbatty            #+#    #+#             */
/*   Updated: 2025/12/17 15:26:24 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GLFWDL_HPP
# define GLFWDL_HPP

# include <iostream>
# include <GraphicsDL.hpp>
# include <vector>
# include <thread>
# include <atomic>
# include <unistd.h>
# include "GLFW/glfw3.h"

# define SQUARE_SIZE 32

class	GLFWDL : public GraphicsDL
{
	public:
		GLFWDL() {}
		~GLFWDL()
		{
			stop();
		}

		void				open(GameState &gameState);
		void				render(GameState &gameState);
		GraphicsDL::Input	getInput();
		void				stop();
	private:
		static void				_keyHook(GLFWwindow *window, int key, int, int action, int);
		static void				_closeHook(GLFWwindow *window);
		GLFWwindow			*_window;
		std::vector<int>	_inputs;
		bool				_shouldClose = false;
};

#endif
