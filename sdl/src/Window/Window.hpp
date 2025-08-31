/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Window.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 10:49:36 by mbatty            #+#    #+#             */
/*   Updated: 2025/08/31 12:22:22 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WINDOW_HPP
# define WINDOW_HPP

# include "libs.hpp"

# define WINDOW_NAME "sdl_test"

class	Window
{
	public:
		Window(const std::string &name, int width, int height)
		{
			_width = width;
			_height = height;
			if (SDL_Init(SDL_INIT_VIDEO) < 0)
				throw std::runtime_error(std::string("Failed to initialize SDL! Error: ") + SDL_GetError());

			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

			SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

			_SDLWindowData = SDL_CreateWindow(
				name.c_str(),
				SDL_WINDOWPOS_CENTERED,
				SDL_WINDOWPOS_CENTERED,
				width, height,
				SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
			);

			if (!_SDLWindowData)
			{
				SDL_Quit();
				throw std::runtime_error(std::string("Failed to create window! Error: ") + SDL_GetError());
			}

			_glContext = SDL_GL_CreateContext(_SDLWindowData);
			if (!_glContext)
			{
				SDL_DestroyWindow(_SDLWindowData);
				SDL_Quit();
				throw std::runtime_error(std::string("Failed to create OpenGL context! Error: ") + SDL_GetError());
			}

			if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
			{
				SDL_GL_DeleteContext(_glContext);
				SDL_DestroyWindow(_SDLWindowData);
				SDL_Quit();
				throw std::runtime_error("Failed to initialize GLAD!");
			}

			SDL_GL_SetSwapInterval(1);

			_keyboardState = SDL_GetKeyboardState(NULL);

			glViewport(0, 0, _width, _height);

			glEnable(GL_DEPTH_TEST);
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			glFrontFace(GL_CW);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			_up = true;
		}
		~Window()
		{
			SDL_GL_DeleteContext(_glContext);
			SDL_DestroyWindow(_SDLWindowData);
			SDL_Quit();
		}

		void	pollEvents()
		{
			while (SDL_PollEvent(&_events))
			{
				switch (_events.type)
				{
					case SDL_QUIT:
						close();
						break;
					case SDL_KEYDOWN:
						if (_keyHook)
							_keyHook(this, _events.key.keysym.sym, true);
						break;
					case SDL_KEYUP:
						if (_keyHook)
							_keyHook(this, _events.key.keysym.sym, false);
						break;
					case SDL_MOUSEBUTTONDOWN:
						if (_mouseClickHook)
							_mouseClickHook(this, _events.button.button, true);
						break;
					case SDL_MOUSEBUTTONUP:
						if (_mouseClickHook)
							_mouseClickHook(this, _events.button.button, false);
						break;
					case SDL_MOUSEMOTION:
						if (_mouseMotionHook)
							_mouseMotionHook(this, _events.motion.x, _events.motion.y, _events.motion.xrel, _events.motion.yrel);
						break;
					case SDL_MOUSEWHEEL:
						if (_mouseScrollHook)
							_mouseScrollHook(this, _events.wheel.x, _events.wheel.y);
						break;
					case SDL_WINDOWEVENT:
						if (_events.window.event == SDL_WINDOWEVENT_RESIZED)
						{
							_width = _events.window.data1;
							_height = _events.window.data2;
							glViewport(0, 0, _width, _height);
						}
						break;
					default:
						break;
				}
			}
		}

		void	loopStart()
		{
			pollEvents();
			Uint32 currentFrame = SDL_GetTicks();
    		_deltaTime = (currentFrame - _lastFrame) / 1000.0f;
    		_lastFrame = currentFrame;
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
		}
		void	loopEnd()
		{
			SDL_GL_SwapWindow(_SDLWindowData);
		}

		const Uint8	*getKeyboard()
		{
			return (_keyboardState);
		}

		SDL_Window	*data()
		{
			return (_SDLWindowData);
		}

		bool	up() {return (_up);}
		void	close() {_up = false;}

		float	getDeltaTime() {return (_deltaTime);}
		float	getWidth() {return (_width);}
		float	getHeight() {return (_height);}

		void	setKeyHookHook(std::function<void(Window *, SDL_Keycode, bool)> func) {_keyHook = func;}
		void	setMouseClickHook(std::function<void(Window *, uint8_t, bool)> func) {_mouseClickHook = func;}
		void	setMouseScrollHook(std::function<void(Window *, int, int)> func) {_mouseScrollHook = func;}
		void	setMouseMotionHook(std::function<void(Window *, int, int, int, int)> func) {_mouseMotionHook = func;}
	private:
		std::function<void(Window *, SDL_Keycode, bool)>	_keyHook = NULL;
		std::function<void(Window *, uint8_t, bool)>		_mouseClickHook = NULL;
		std::function<void(Window *, int, int)>				_mouseScrollHook = NULL;
		std::function<void(Window *, int, int, int, int)>	_mouseMotionHook = NULL;
		SDL_Window		*_SDLWindowData;
		SDL_GLContext	_glContext;
		SDL_Event		_events;
		const Uint8		*_keyboardState;
		bool			_up = false;
		
		int				_width;
		int				_height;

		float		_deltaTime;
		float		_lastFrame;
};

#endif
