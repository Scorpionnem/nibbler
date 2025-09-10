/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AudioHandler.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 14:21:50 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/05 16:24:57 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AUDIOHANDLER_HPP
# define AUDIOHANDLER_HPP

# include "libs.hpp"

using audioHandlerCreateFunc = class AudioHandler *(*)();

class AudioHandler
{
	public:
		AudioHandler() {}
		virtual ~AudioHandler() {}
		virtual bool playSound(const char *path) = 0;
		static class AudioHandler	*getAudioHandler(const char *path)
		{
			void	*handle = dlopen(path, RTLD_LAZY);
			if (!handle)
				return (NULL);
		
			audioHandlerCreateFunc func = (audioHandlerCreateFunc)dlsym(handle, "getHandler");
			if (!func)
			{
				dlclose(handle);
				return (NULL);
			}
		
			return (func());
		}
};

#endif
