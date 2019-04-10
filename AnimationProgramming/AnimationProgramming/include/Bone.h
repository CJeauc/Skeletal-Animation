#pragma once

#include "Transform.h"

class Bone
{
private:
	std::shared_ptr<Bone> m_parent;
	std::vector<std::shared_ptr<Bone>> m_child;
	Transform m_transform;
	std::pair<Transform, Transform> m_lerpTransforms;
	AltMath::Matrix4f m_inverseWorldTPose;
	AltMath::Matrix4f m_worldTransformBuffer;

	bool m_isUpdate;
	size_t m_id;

public:
	Bone();
	Bone(const AltMath::Quaternion& p_rotation, AltMath::Vector4f p_position, size_t p_id);
	~Bone() = default;

	void Translate(const AltMath::Vector4f& p_translation);
	void SetTranslation(const AltMath::Vector4f& p_translation);
	void Rotate(const AltMath::Quaternion& p_rotation);
	void SetRotation(const AltMath::Quaternion& p_rotation);

	void SetNextLerpTranform(const Transform& p_next);
	void SetNextLerpTranform(const AltMath::Vector4f& p_nextPos, const AltMath::Quaternion& p_nextRotation);

	void SetParent(std::shared_ptr<Bone> p_parent);
	std::shared_ptr<Bone> GetParent() const;
	const std::pair<Transform, Transform>& GetLerpTransforms() const;

	AltMath::Matrix4f GetWorldTransform();
	AltMath::Matrix4f GetShaderTransform();
	void AddChild(const std::shared_ptr<Bone>& p_child);
	size_t GetId() const;
	void InitInverseWorldTPose();

private:
	AltMath::Matrix4f UpdateTransform();
	void NeedToUpdate();
};