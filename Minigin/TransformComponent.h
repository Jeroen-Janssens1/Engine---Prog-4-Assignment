#pragma once
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec3.hpp>
#pragma warning(pop)
#include "BaseComponent.h"

class TransformComponent final : public BaseComponent
{
public:
	TransformComponent(std::shared_ptr<GameObject>& pOwner, float x = 0.f, float y = 0.f, float z = 0.f);
	const glm::vec3& GetPosition() const { return m_Position; }
	void SetPosition(float x, float y, float z);

	void Update() override;
	void Render() const override;

private:
	glm::vec3 m_Position;
};
