/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MiniAudioAudioHandler.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 10:41:31 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/10 10:43:43 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MiniAudioAudioHandler.hpp"

MiniAudioAudioHandler::MiniAudioAudioHandler()
{
	std::cout << "created miniaudio" << std::endl;
}

MiniAudioAudioHandler::~MiniAudioAudioHandler()
{
	std::cout << "deleted miniaudio" << std::endl;
}

bool	MiniAudioAudioHandler::playSound(const char *path)
{
	ma_result result;
	ma_engine engine;

	result = ma_engine_init(NULL, &engine);
	if (result != MA_SUCCESS) {
		return -1;
	}

	result = ma_engine_play_sound(&engine, path, NULL);
	
	if (result != MA_SUCCESS) {
		return -1;
	}

	getchar();

	ma_engine_uninit(&engine);

	return 0;
}
