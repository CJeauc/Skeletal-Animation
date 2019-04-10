#include "stdafx.h"
#include "Animator.h"

void Animator::AddAnimation(std::string p_animation, std::string p_name)
{
	if (m_currentAnimation)
		m_animations[std::move(p_name)] = Animation(std::move(p_animation));
	else
		m_currentAnimation = std::make_shared<Animation>(m_animations[std::move(p_name)] = Animation(std::move(p_animation)));
}

void Animator::SetCurrentAnimation(const std::string& p_name)
{
	const auto animation = m_animations.find(p_name);
	if (animation != m_animations.end())
		m_currentAnimation = std::make_shared<Animation>(animation->second);
	else
		std::cout << "Animation " << p_name << " not find\n";
}

void Animator::Init(const std::string& p_name)
{
	SetCurrentAnimation(p_name);
	m_skeleton.Init();
}

void Animator::Update(float p_frameTime)
{
	if (m_currentAnimation && m_currentAnimation->animKeyBySecond > 0)
	{
		while (m_elapsedTime >= m_currentAnimation->animKeyBySecond)
		{
			if (++m_currentFrame >= m_currentAnimation->animKeyCount)
				m_currentFrame = 0;

			m_skeleton.LoadAnimationAtFrame(*m_currentAnimation, m_currentFrame);

			m_elapsedTime -= m_currentAnimation->animKeyBySecond;
		}
		m_elapsedTime += p_frameTime * m_currentAnimation->speedScale;

		m_skeleton.Update(m_elapsedTime / m_currentAnimation->animKeyBySecond);
		m_skeleton.DrawSkeleton(AltMath::Vector4f(150, 0, 0, 1));;
		m_skeleton.SetModelPos();
	}
}

std::shared_ptr<Animation> Animator::GetCurrentAnimation() const
{
	return m_currentAnimation;
}
