#pragma once

class Transform
{
private:
	AltMath::Matrix4f m_initialTransform;
	AltMath::Quaternion m_rotation;
	AltMath::Vector4f m_position;

public:
	Transform() = default;
	Transform(AltMath::Quaternion p_rotation, AltMath::Vector4f p_position);
	~Transform() = default;

	AltMath::Matrix4f CalculateWorldMatrix();
	
	void Translate(const AltMath::Vector4f& p_translation);
	void SetTranslation(const AltMath::Vector4f& p_translation);
	AltMath::Vector4f GetTranslation() const;
	void Rotate(const AltMath::Quaternion& p_rotation);
	void SetRotation(const AltMath::Quaternion& p_rotation);
	AltMath::Quaternion GetRotation() const;
};