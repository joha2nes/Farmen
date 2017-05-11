// SpriteAnimation.hpp

#pragma once

#include "Sprite.hpp"

class SpriteAnimation : public Sprite
{
public:
	struct Frame
	{
		Sprite sprite;
		float time;
	};

public:
	SpriteAnimation(unsigned int numFrames, Frame* frames);
	~SpriteAnimation();

	void update(float dt);

private:
	unsigned int m_numFrames;
	Frame* m_frames;
	float m_time;
};