#pragma once

struct Animation
{
	std::string animationFileName;
	size_t animKeyCount;
	float animKeyBySecond;
	float speedScale;

	Animation() : animKeyCount(0), animKeyBySecond(0), speedScale(1) {}

	explicit Animation(std::string p_animationFileName)
		: animationFileName(std::move(p_animationFileName)), animKeyBySecond(0), speedScale(1)
	{
		animKeyCount = GetAnimKeyCount(animationFileName.c_str());
		if (animKeyCount > 0)
			animKeyBySecond = 1.f / animKeyCount;
	}

	void SetSpeed(float p_speed)
	{
		speedScale = abs(p_speed);
	}
};
