#pragma once

class	AudioDL
{
	public:
		enum class Sound
		{
			MOVE,
			EAT,
		};
	public:
		AudioDL() {}
		virtual ~AudioDL() {}

		virtual void				open() = 0;
		virtual void				play(Sound sound) = 0;
		virtual void				stop() = 0;
};
