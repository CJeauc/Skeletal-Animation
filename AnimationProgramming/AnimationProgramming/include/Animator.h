#pragma once

#include "Animation.h"
#include "Skeleton.h"

class Animator
{
private:
	std::unordered_map<std::string, Animation> m_animations;
	std::shared_ptr<Animation> m_currentAnimation;
	Skeleton m_skeleton;

	size_t m_currentFrame;
	float m_elapsedTime;

public:
	Animator() = default;
	~Animator() = default;

	void AddAnimation(std::string p_animation, std::string p_name);
	void SetCurrentAnimation(const std::string& p_name);

	void Init(const std::string& p_name);
	void Update(float p_frameTime);
	std::shared_ptr<Animation> GetCurrentAnimation() const;
};
