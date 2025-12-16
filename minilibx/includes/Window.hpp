/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Window.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/13 10:44:32 by mbatty            #+#    #+#             */
/*   Updated: 2025/12/16 14:07:45 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WINDOW_HPP
# define WINDOW_HPP

extern "C"
{
	# include <mlx.h>
}

#include <iostream>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <exception>
#include <stdexcept>
#include <functional>
#include <map>
#include <ctime>
#include <string>

# define MOUSE_LEFT_CLICK 1
# define MOUSE_RIGHT_CLICK 3
# define MOUSE_SCROLL_UP 4
# define MOUSE_SCROLL_DOWN 5

# define KEY_ESCAPE 65307

class	Window
{
	public:
		class	Texture
		{
			public:
				Texture(Window *win, char *path)
				{
					_data = mlx_xpm_file_to_image(win->getMLX(), path, &_width, &_height);
					_addr = mlx_get_data_addr(_data, &_bits_per_pixel, &_line_length, &_endian);
					_win = win;
				}
				Texture(Window *win, int width, int height)
				{
					_win = win;
					_data = mlx_new_image(_win->getMLX(), width, height);
					if (!_data)
						return ;
					_addr = mlx_get_data_addr(_data, &_bits_per_pixel, &_line_length, &_endian);
					_width = width;
					_height = height;
					for (int x = 0; x < width; x++)
						for (int y = 0; y < width; y++)
							putPixel(x, y, 0x00000000);
				}
				Texture(Window *win, int width, int height, bool special) : Texture(win, width, height)
				{
					_isSpecial = special;
				}
				void	render(int x, int y)
				{
					if (_isSpecial)
					{
						mlx_put_image_to_window(_win->getMLX(), _win->getMLXW(), _data, x, y);
						return ;
					}
					for (int xx = 0; xx < _width; xx++)
						for (int yy = 0; yy < _height; yy++)
						{
							uint	pixel = getPixel(xx, yy);
							if (pixel != 0xFF000000)
								_win->getTexture()->putPixel(xx + x, yy + y, pixel);
						}
				}
				void	putPixel(int x, int y, int color)
				{
					if (x < 0 || x >= _width || y < 0 || y >= _height)
						return ;

					char	*dst;

					dst = _addr + (y * _line_length + x * (_bits_per_pixel / 8));
					*(unsigned int *)dst = color;
				}
				int	getPixel(int x, int y)
				{
					if (x < 0 || x >= _width || y < 0 || y >= _height)
						return 0;
					return (*(unsigned int *)(_addr + (y * _line_length + x * (_bits_per_pixel / 8))));
				}
				void clear(int color = 0x00000000)
				{
					for (int y = 0; y < _height; y++)
						for (int x = 0; x < _width; x++)
							putPixel(x, y, color);
				}
			private:
				bool	_isSpecial = false;
				void	*_data;
				char	*_addr;
				int		_bits_per_pixel;
				int		_line_length;
				int		_endian;
				int		_width;
				int		_height;

				Window	*_win;
		};

		Window() {}
		~Window()
		{
			close();
		}

		void	open(int width, int height, const std::string name)
		{
			_width = width;
			_height = height;
			_name = name;
			_mlx = mlx_init();
			if (!_mlx)
				throw std::runtime_error("Failed to init mlx");
			_mlxWin = mlx_new_window(_mlx, _width, _height, (char *)_name.c_str());
			if (!_mlxWin)
				throw std::runtime_error("Failed to open mlx window");
			mlx_loop_hook(_mlx, (int (*)())_loopHook, this);
			mlx_hook(_mlxWin, 17, 0, (int (*)())_closeHook, this);
			mlx_hook(_mlxWin, 2, 1L << 0, (int (*)())_keyPress, this);
			mlx_hook(_mlxWin, 3, 1L << 1, (int (*)())_keyRelease, this);
			clock_gettime(CLOCK_MONOTONIC, &_lastFrame);
			_texture = new Window::Texture(this, _width, _height, true);
		}
		void	start()
		{
			mlx_loop(_mlx);
		}
		void	close()
		{
			if (!_mlx || !_mlxWin)
				return ;
			delete _texture;
			mlx_destroy_window(_mlx, _mlxWin);
			mlx_destroy_display(_mlx);
			free(_mlx);
			_mlx = NULL;
			_mlxWin = NULL;
		}
		void	loopEnd()
		{
			_shouldClose = true;
			mlx_loop_end(_mlx);
		}
		bool	getKeyInput(int key)
		{
			return (_keyInputs[key]);
		}
		void	setRenderHook(std::function<void(void)> func)
		{
			_renderHook = func;
		}
		void	setUpdateHook(std::function<void(float)> func)
		{
			_updateHook = func;
		}
		void	*getMLX() {return (_mlx);}
		void	*getMLXW() {return (_mlxWin);}
		Texture	*getTexture() {return (_texture);}
		bool	shouldClose() {return (_shouldClose);}
	private:
		static int	_closeHook(Window *win)
		{
			win->loopEnd();
			return (1);
		}
		static int	_keyPress(int keycode, Window *win)
		{
			win->_keyInputs[keycode] = true;
			return (1);
		}
		static int	_keyRelease(int keycode, Window *win)
		{
			win->_keyInputs[keycode] = false;
			return (1);
		}
		static int	_loopHook(Window *win)
		{
			struct timespec	currentFrame;
			double			deltaTime;

			clock_gettime(CLOCK_MONOTONIC, &currentFrame);
			deltaTime = (currentFrame.tv_sec - win->_lastFrame.tv_sec) + (currentFrame.tv_nsec - win->_lastFrame.tv_nsec) * 1e-9;
			win->_texture->clear();
			win->_lastFrame = currentFrame;
			if (win->_updateHook)
				win->_updateHook(deltaTime);
			if (win->_renderHook)
				win->_renderHook();
			win->_texture->render(0, 0);
			return (1);
		}
		Texture						*_texture;
		struct timespec				_lastFrame = {0, 0};
		std::map<int, bool>			_keyInputs;
		std::function<void(void)>	_renderHook = NULL;
		std::function<void(float)>	_updateHook = NULL;
		void						*_mlx = NULL;
		void						*_mlxWin = NULL;
		int							_width = 100;
		int							_height = 100;
		bool						_shouldClose = false;
		std::string					_name;
};

#endif
