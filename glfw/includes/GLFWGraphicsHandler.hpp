/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GLFWGraphicsHandler.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 10:40:42 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/10 13:18:50 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libs.hpp"
#include "GraphicsHandler.hpp"

# include "glad.h"
# include "glfw3.h"

# include "glm.hpp"
# include "gtc/matrix_transform.hpp"
# include "gtc/type_ptr.hpp"

class	GLFWGraphicsHandler : public GraphicsHandler
{
	public:
		GLFWGraphicsHandler();
		~GLFWGraphicsHandler();

		void open(int mapSizeX, int mapSizeY);
		void render(GameState state);
		std::vector<Input> getInputs();
		void close();
	private:
		GLFWwindow	*_data;
		int			_height;
		int			_width;
};
