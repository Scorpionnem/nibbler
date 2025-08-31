/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ShaderManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 10:23:08 by mbatty            #+#    #+#             */
/*   Updated: 2025/08/31 11:33:49 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ShaderManager.hpp"

ShaderManager::ShaderManager()
{
}

ShaderManager::~ShaderManager()
{
	for (auto it = shaders.begin(); it != shaders.end(); it++)
		delete it->second;
}

bool	ShaderManager::erase(const std::string &name)
{
	std::unordered_map<std::string, Shader *>::iterator	finder;
	finder = shaders.find(name);
	if (finder == shaders.end())
		return (0);
	shaders.erase(finder);
	return (1);
}

Shader	*ShaderManager::load(ShaderInfos infos)
{
	if (shaders.find(infos.name) != shaders.end())
		return (this->get(infos.name));
	return (shaders.insert(std::make_pair(infos.name, new Shader(infos.vertex, infos.fragment))).first->second);
}

Shader	*ShaderManager::get(const std::string &name)
{
	std::unordered_map<std::string, Shader *>::iterator	finder = shaders.find(name);
	if (finder == shaders.end())
		return (NULL);
	return (finder->second);
}

Shader	*ShaderManager::operator[](const std::string &name)
{
	return (this->get(name));
}
