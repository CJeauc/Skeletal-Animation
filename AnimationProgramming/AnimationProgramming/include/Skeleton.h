#pragma once

#include "Bone.h"
#include "Animation.h"

class Skeleton
{
private:
	std::vector<std::shared_ptr<Bone>> m_bones;
	size_t m_boneCount;

public:
	Skeleton() = default;
	~Skeleton() = default;

	void Init();
	void Update(float p_elapsedTimeRatio);
	
	void LoadAnimationAtFrame(const Animation& p_animation, unsigned int p_frame);

	static void DrawCross(AltMath::Vector4f p_pos, AltMath::Vector3f p_color = AltMath::Vector3f::Zero, float p_size = 4.f);
	void DrawSkeleton(const AltMath::Vector4f& p_translation) const;
	void SetModelPos();

private:
	void InitTPose();
	void LoadBoneAtIndex(unsigned int p_index);
	void SetParentsAndChildren(unsigned int p_index);

};

