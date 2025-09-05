/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AudioHandler.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 14:21:50 by mbatty            #+#    #+#             */
/*   Updated: 2025/08/31 15:30:28 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AUDIOHANDLER_HPP
# define AUDIOHANDLER_HPP

# include "libs.hpp"

class   AudioHandler
{
    public:
        AudioHandler()
        {
            std::cout << "created AudioHandler" << std::endl;
        }
        virtual ~AudioHandler()
        {
            std::cout << "deleted AudioHandler" << std::endl;
        }
        virtual bool playSound(const char *path) = 0;
};

#endif
