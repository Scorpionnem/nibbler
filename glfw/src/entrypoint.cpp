/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entrypoint.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 10:40:25 by mbatty            #+#    #+#             */
/*   Updated: 2025/09/10 10:47:20 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "GLFWGraphicsHandler.hpp"

extern "C" GraphicsHandler *getHandler()
{
    return (new GLFWGraphicsHandler());
}
