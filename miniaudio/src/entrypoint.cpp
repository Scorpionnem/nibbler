/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entrypoint.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 10:40:25 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/10 10:43:24 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MiniAudioAudioHandler.hpp"

extern "C" AudioHandler *getHandler()
{
    return (new MiniAudioAudioHandler());
}
