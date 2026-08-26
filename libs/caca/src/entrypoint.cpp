/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   entrypoint.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbirou <mbirou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/26 10:04:57 by mbirou            #+#    #+#             */
/*   Updated: 2026/08/26 10:33:36 by mbirou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CACAGraphicsDL.hpp"

extern "C" GraphicsDL *getHandler()
{
	return (new CACAGraphicsDL());
}
