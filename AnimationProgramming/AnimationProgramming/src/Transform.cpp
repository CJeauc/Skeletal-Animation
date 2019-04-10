#include "stdafx.h"
#include "Transform.h"


Transform::Transform(AltMath::Quaternion p_rotation, AltMath::Vector4f p_position)
{
	if (!p_rotation.IsNormalized())
		p_rotation.Normalize();

	m_initialTransform = AltMath::Matrix4f::Translation(AltMath::Vector3f(p_position.x, p_position.y, p_position.z)) * p_rotation.ToMatrix4();
}

AltMath::Matrix4f Transform::CalculateWorldMatrix()
{
	if (!m_rotation.IsNormalized())
		m_rotation.Normalize();

	return m_initialTransform * AltMath::Matrix4f::Translation(AltMath::Vector3f(m_position.x, m_position.y, m_position.z)) * m_rotation.ToMatrix4();
}

void Transform::Translate(const AltMath::Vector4f& p_translation)
{
	m_position += p_translation;
}

void Transform::SetTranslation(const AltMath::Vector4f& p_translation)
{
	m_position = p_translation;
}

AltMath::Vector4f Transform::GetTranslation() const
{
	return m_position;
}

void Transform::Rotate(const AltMath::Quaternion& p_rotation)
{
	m_rotation *= p_rotation;
}

void Transform::SetRotation(const AltMath::Quaternion& p_rotation)
{
	m_rotation = p_rotation;
}

AltMath::Quaternion Transform::GetRotation() const
{
	return m_rotation;
}
