#include <cstdint>
#include <string>

enum class PacketType
{
	CONNECTION,
	UNKNOWN,
};
namespace  std
{
	inline std::string	to_string(PacketType f)
	{
		switch (f)
		{
			case PacketType::CONNECTION:
				return ("connection");
			default:
				return ("unknown");
		}
	}
}

struct	PacketHeader
{
	PacketType	type;
	uint32_t	size;
};

struct Connection
{
	enum class Status
	{
		SUCCESS,
		FAIL,
	};
	Status	status;
};
