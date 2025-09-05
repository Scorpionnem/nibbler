/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 10:16:58 by mbatty            #+#    #+#             */
/*   Updated: 2025/08/31 15:13:48 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libs.hpp"
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include "AudioHandler.hpp"

class   Miniaudio : public AudioHandler
{
    public:
        Miniaudio()
        {
            std::cout << "created miniaudio" << std::endl;
        }
        ~Miniaudio()
        {
            std::cout << "deleted miniaudio" << std::endl;
        }
        bool    playSound(const char *path)
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
};

extern "C" AudioHandler *getHandler()
{
    return (new Miniaudio());
}
