// SpriteAnimation.cpp

#include "SpriteAnimation.hpp"

SpriteAnimation::SpriteAnimation(unsigned int numFrames, Frame* frames)
	: Sprite(frames[0].sprite)
	, m_frames(nullptr)// new Frame[numFrames])
	, m_numFrames(m_numFrames)
	, m_time(0.0f)
{}

SpriteAnimation::~SpriteAnimation()
{}

void update(float dt)
{

}
