#include "stdafx.h"
#include "Bone.h"

Bone::Bone()
	: m_isUpdate(false), m_id(0) { }

Bone::Bone(const AltMath::Quaternion& p_rotation, AltMath::Vector4f p_position, size_t p_id)
	: m_transform(p_rotation, std::move(p_position)), m_isUpdate(false), m_id(p_id) { }

void Bone::Translate(const AltMath::Vector4f& p_translation)
{
	m_transform.Translate(p_translation);
	NeedToUpdate();
}

void Bone::SetTranslation(const AltMath::Vector4f& p_translation)
{
	m_transform.SetTranslation(p_translation);
	NeedToUpdate();
}

void Bone::Rotate(const AltMath::Quaternion& p_rotation)
{
	m_transform.Rotate(p_rotation);
	NeedToUpdate();
}

void Bone::SetRotation(const AltMath::Quaternion& p_rotation)
{
	m_transform.SetRotation(p_rotation);
	NeedToUpdate();
}

void Bone::SetNextLerpTranform(const Transform& p_next)
{
	m_lerpTransforms.first = m_lerpTransforms.second;
	m_lerpTransforms.second = p_next;
}

void Bone::SetNextLerpTranform(const AltMath::Vector4f& p_nextPos, const AltMath::Quaternion& p_nextRotation)
{
	m_lerpTransforms.first = m_lerpTransforms.second;
	m_lerpTransforms.second.SetTranslation(p_nextPos);
	m_lerpTransforms.second.SetRotation(p_nextRotation);
}

void Bone::SetParent(std::shared_ptr<Bone> p_parent)
{
	if (p_parent)
	{
		m_parent = std::move(p_parent);
	}
}

std::shared_ptr<Bone> Bone::GetParent() const
{
	return m_parent;
}

const std::pair<Transform, Transform>& Bone::GetLerpTransforms() const
{
	return m_lerpTransforms;
}

AltMath::Matrix4f Bone::GetWorldTransform()
{
	if (!m_parent)
		return m_transform.CalculateWorldMatrix();
	if (m_isUpdate)
		return m_worldTransformBuffer;

	return UpdateTransform();
}

AltMath::Matrix4f Bone::GetShaderTransform()
{
	if (!m_parent) 
		return m_transform.CalculateWorldMatrix() * m_inverseWorldTPose;
	if (m_isUpdate)
		return m_worldTransformBuffer * m_inverseWorldTPose;

	return UpdateTransform() * m_inverseWorldTPose;
}

void Bone::AddChild(const std::shared_ptr<Bone>& p_child)
{
	if (p_child)
		m_child.emplace_back(p_child);
}

size_t Bone::GetId() const
{
	return m_id;
}

void Bone::InitInverseWorldTPose()
{
	m_inverseWorldTPose = (m_parent ? m_parent->GetWorldTransform() * m_transform.CalculateWorldMatrix() : m_transform.CalculateWorldMatrix() ).Inverse();
}

AltMath::Matrix4f Bone::UpdateTransform()
{
	if (!m_isUpdate && m_parent)
	{
		m_worldTransformBuffer	= m_parent->GetWorldTransform() * m_transform.CalculateWorldMatrix();
		m_isUpdate = true;
	}

	return m_worldTransformBuffer;
}

void Bone::NeedToUpdate()
{
	m_isUpdate = false;

	for (auto& child : m_child)
	{
		if (child)
			child->NeedToUpdate();
	}
}
