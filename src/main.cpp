/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/31 14:03:18 by mbatty            #+#    #+#             */
/*   Updated: 2025/12/17 13:28:50 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include <iostream>
# include "GameState.hpp"
# include "Nibbler.hpp"

int	main(int ac, char **av)
{
	srand(std::time(NULL));

	Nibbler	nibbler;

	return (nibbler.start(ac, av));
}
