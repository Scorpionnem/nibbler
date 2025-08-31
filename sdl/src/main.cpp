/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 10:16:58 by mbatty            #+#    #+#             */
/*   Updated: 2025/08/31 12:32:31 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libs.hpp"

#include "Window.hpp"
#include "ShaderManager.hpp"

void	keyHook(Window *window, SDL_Keycode key, bool pressed)
{
	if (key == SDLK_ESCAPE && pressed)
	{
		window->close();
	}
}

void	mouseClickHook(Window *, uint8_t button, bool pressed)
{
	if (button == SDL_BUTTON_LEFT && pressed)
		std::cout << "left click" << std::endl;
	if (button == SDL_BUTTON_RIGHT && pressed)
		std::cout << "right click" << std::endl;
	if (button == SDL_BUTTON_MIDDLE && pressed)
		std::cout << "middle click" << std::endl;
}

void	mouseScrollHook(Window *, int , int y)
{
	std::cout << "Scroll: " << y << std::endl;
}

int	caca = 0, cucu = 0;

void	mouseMotionHook(Window *, int x, int y, int relx, int rely)
{
	caca = x;
	cucu = y;
	return ;
	std::cout << x << " " << y << relx << " " << rely << std::endl;
}

unsigned int	UIquadVAO = 0;
unsigned int	UIquadVBO = 0;

void	initButtonQuad()
{
	if (UIquadVAO != 0)
		return;

	float vertices[] =
	{
		0.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f
	};

	glGenVertexArrays(1, &UIquadVAO);
	glGenBuffers(1, &UIquadVBO);

	glBindVertexArray(UIquadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, UIquadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

	glBindVertexArray(0);
}

int main(void)
{
	Window	window(WINDOW_NAME, 800, 600);
	window.setKeyHookHook(keyHook);
	window.setMouseClickHook(mouseClickHook);
	window.setMouseScrollHook(mouseScrollHook);
	window.setMouseMotionHook(mouseMotionHook);

	ShaderManager	shaders;

	shaders.load({"gui", "shaders/gui_shader.vs", "shaders/gui_shader.fs"});
	initButtonQuad();

	glDisable(GL_DEPTH_TEST);

	SDL_Surface	*tex = SDL_LoadBMP("mbatty.bmp");

	GLuint texID;
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, tex->pitch / tex->format->BytesPerPixel);

	GLenum format = (tex->format->BytesPerPixel == 4) ? GL_RGBA : GL_RGB;
	GLenum type   = (tex->format->BytesPerPixel == 4) ? GL_UNSIGNED_BYTE : GL_UNSIGNED_BYTE;
	GLenum glFormat = (tex->format->BytesPerPixel == 4) ? GL_BGRA : GL_BGR;
	
	glTexImage2D(GL_TEXTURE_2D, 0, format, tex->w, tex->h, 0, glFormat, type, tex->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	SDL_SetWindowBordered(window.data(), SDL_bool::SDL_FALSE);
	SDL_SetWindowResizable(window.data(), SDL_bool::SDL_TRUE);

	while (window.up())
	{
		window.loopStart();

		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(caca - 50, cucu - 50, 0.0f));
		
		model = glm::translate(model, glm::vec3(50, 50, 0.0f));
		model = glm::rotate(model, glm::radians((float)SDL_GetTicks() / 2), glm::vec3(0, 0, 1));
		model = glm::translate(model, glm::vec3(-50, -50, 0.0f));

		model = glm::scale(model, glm::vec3(100, 100, 1.0f));
		glm::mat4 projection = glm::ortho(0.0f, window.getWidth(), window.getHeight(), 0.0f);

		shaders["gui"]->use();
		shaders["gui"]->setMat4("model", model);
		shaders["gui"]->setMat4("projection", projection);

		shaders["gui"]->setInt("tex0", 0);
		glActiveTexture(GL_TEXTURE0 + 0);
		glBindTexture(GL_TEXTURE_2D, texID);

		glBindVertexArray(UIquadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		window.loopEnd();
	}
	SDL_FreeSurface(tex);
}
