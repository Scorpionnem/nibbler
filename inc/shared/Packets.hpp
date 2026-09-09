#pragma once

#include <cstdint>
#include <string>

#include "Tile.hpp"
#include "GraphicsDL.hpp"
#include "AudioDL.hpp"

#define MAX_MAP_SIZE MAX_SIZE * MAX_SIZE // 42 * 42

enum class PacketType
{
	CONNECTION,
	MAPPACKET,
	INPUTPACKET,
	DEATHPACKET,
	SOUNDPACKET,
	UNKNOWN
};
namespace  std
{
	inline std::string	to_string(PacketType f)
	{
		switch (f)
		{
			case PacketType::CONNECTION:
				return ("connection");
			case PacketType::MAPPACKET:
				return ("map");
			case PacketType::INPUTPACKET:
				return ("input");
			case PacketType::DEATHPACKET:
				return ("death");
			case PacketType::SOUNDPACKET:
				return ("sound");
			default:
				return ("unknown");
		}
	}
}

struct	PacketHeader
{
	PacketType	type;
	uint32_t	size;
} __attribute__((packed));

struct ConnectionPacket
{
	PacketHeader	hdr;
	enum class Status
	{
		SUCCESS,
		FAIL,
	};
	Status	status;
} __attribute__((packed));

struct MapPacket
{
	PacketHeader	hdr;
	uint32_t		width;
	uint32_t		height;
	Tile			tiles[MAX_MAP_SIZE] = {Tile::EMPTY};
} __attribute__((packed));

struct InputPacket
{
	PacketHeader		hdr;
	GraphicsDL::Input	input = GraphicsDL::Input::NONE;
} __attribute__((packed));

struct DeathPacket
{
	PacketHeader	hdr;
	bool			value;
} __attribute__((packed));

struct PlaySound
{
	PacketHeader	hdr;
	AudioDL::Sound	sound;
} __attribute__((packed));

constexpr size_t MAX_PACKET_SIZE = std::max(sizeof(PacketHeader), std::max(sizeof(ConnectionPacket), std::max(sizeof(MapPacket), std::max(sizeof(InputPacket), std::max(sizeof(DeathPacket), sizeof(PlaySound))))));
