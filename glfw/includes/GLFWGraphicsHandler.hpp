/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GLFWGraphicsHandler.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 10:40:42 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/10 10:48:35 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libs.hpp"
#include "GraphicsHandler.hpp"

class	GLFWGraphicsHandler : public GraphicsHandler
{
	public:
		GLFWGraphicsHandler();
		~GLFWGraphicsHandler();

		bool open();
		bool render(GameState state);
		std::vector<Input> getInputs();
		bool close();
	private:
};
