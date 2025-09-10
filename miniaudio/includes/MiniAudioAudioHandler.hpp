/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MiniAudioAudioHandler.hpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 10:40:42 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/10 10:45:21 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libs.hpp"
#include "AudioHandler.hpp"
#include "miniaudio.h"

class   MiniAudioAudioHandler : public AudioHandler
{
	public:
		MiniAudioAudioHandler();
		~MiniAudioAudioHandler();
		bool    playSound(const char *path);
};
