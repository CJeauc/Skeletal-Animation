#include "stdafx.h"
#include "Skeleton.h"

void Skeleton::Init()
{
	m_boneCount = GetSkeletonBoneCount();
	InitTPose();
	std::sort(m_bones.begin(), m_bones.end(),
	          [](std::shared_ptr<Bone>& p_first, std::shared_ptr<Bone>& p_second)
	          {
		          return p_first->GetId() < p_second->GetId();
	          });
}

void Skeleton::Update(float p_elapsedTimeRatio)
{

	for (auto& bone : m_bones)
	{
		if (bone)
		{
			const std::pair<Transform, Transform>& lerpTransform = bone->GetLerpTransforms();

			auto rot1 = lerpTransform.first.GetRotation();
			auto rot2 = lerpTransform.second.GetRotation();

			bone->SetTranslation(AltMath::Vector4f::Lerp(lerpTransform.first.GetTranslation(), lerpTransform.second.GetTranslation(), p_elapsedTimeRatio));
			bone->SetRotation(AltMath::Quaternion::Slerp(rot1, rot2, p_elapsedTimeRatio));
		}
	}
}

void Skeleton::LoadAnimationAtFrame(const Animation& p_animation, unsigned int p_frame)
{
	if (p_frame < p_animation.animKeyCount)
	{
		AltMath::Vector4f pos, rotation;

		for (unsigned int i = 0; i < m_bones.size(); ++i)
		{
			if (!m_bones[i])
				continue;

			GetAnimLocalBoneTransform(p_animation.animationFileName.c_str(), i, p_frame,
			                          pos.x, pos.y, pos.z,
			                          rotation.w, rotation.x, rotation.y, rotation.z);

			m_bones[i]->SetNextLerpTranform(pos, AltMath::Quaternion(rotation.x, rotation.y, rotation.z, rotation.w));
		}
	}
}

void Skeleton::DrawCross(const AltMath::Vector4f p_pos, AltMath::Vector3f p_color, float p_size)
{
	p_size *= 0.5f;

	DrawLine(p_pos.x, p_pos.y, p_pos.z - p_size,
	         p_pos.x, p_pos.y, p_pos.z + p_size,
	         p_color.x, p_color.y, p_color.z);

	DrawLine(p_pos.x - p_size, p_pos.y, p_pos.z,
	         p_pos.x + p_size, p_pos.y, p_pos.z,
	         p_color.x, p_color.y, p_color.z);
}

void Skeleton::DrawSkeleton(const AltMath::Vector4f& p_translation) const
{
	AltMath::Vector4f pos1, pos2;
	std::shared_ptr<Bone> parent;

	for (auto& bone : m_bones)
	{
		if (!bone)
			continue;

		pos1 = bone->GetWorldTransform().GetColumn(3);
		DrawCross(pos1 + p_translation, AltMath::Vector3f(0.3f, 0.f, 0.6f));

		parent = bone->GetParent();
		pos2 = parent ? parent->GetWorldTransform().GetColumn(3) : pos1;
		DrawLine(pos1.x + p_translation.x, pos1.y + p_translation.y, pos1.z + p_translation.z,
		         pos2.x + p_translation.x, pos2.y + p_translation.y, pos2.z + p_translation.z,
		         1.f, 0.8f, 0.3f);
	}
}

void Skeleton::SetModelPos()
{
	float mat[1024];
	for (unsigned int i = 0; i < m_bones.size(); ++i)
	{
		if (m_bones[i])
			memcpy(&mat[i * 16], m_bones[i]->GetShaderTransform().elements, sizeof(float) * 16);
	}

	SetSkinningPose(mat, m_bones.size());
}

void Skeleton::InitTPose()
{
	m_bones.resize(m_boneCount, nullptr);

	for (size_t i = 0; i < m_bones.size(); ++i)
	{
		if (m_bones[i])
			continue;
		LoadBoneAtIndex(i);
	}

	for (int i = m_bones.size() - 1; i >= 0; --i)
	{
		if (!m_bones[i])
		{
			m_bones.pop_back();
			continue;
		}

		m_bones[i]->InitInverseWorldTPose();
	}
}

void Skeleton::LoadBoneAtIndex(unsigned p_index)
{
	std::string name = GetSkeletonBoneName(p_index);
	if (name.find("ik") != std::string::npos)
		return;

	AltMath::Vector4f pos, rotation;

	GetSkeletonBoneLocalBindTransform(p_index, pos.x, pos.y, pos.z, rotation.w, rotation.x, rotation.y, rotation.z);
	m_bones[p_index] = std::make_shared<Bone>(AltMath::Quaternion(rotation.x, rotation.y, rotation.z, rotation.w),
	                                          pos, p_index);

	SetParentsAndChildren(p_index);
}

void Skeleton::SetParentsAndChildren(unsigned int p_index)
{
	const int id = GetSkeletonBoneParentIndex(p_index);
	if (id >= 0)
	{
		if (!m_bones[id])
			LoadBoneAtIndex(id);

		m_bones[p_index]->SetParent(m_bones[id]);
		m_bones[id]->AddChild(m_bones[p_index]);
	}
}
