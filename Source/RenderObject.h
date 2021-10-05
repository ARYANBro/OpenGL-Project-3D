#pragma once

#include "Model.h"

#include <glm/glm.hpp>

#include <memory>

class RenderObject
{
public:
	RenderObject(const std::vector<std::shared_ptr<Model>>& models, const glm::mat4& transform = glm::mat4(1.0f))
		: m_Models(models), m_Transform(transform) {}

	RenderObject() noexcept = default;

	void AddModel(const std::shared_ptr<Model>& model) noexcept { m_Models.push_back(model); }
	void SetTransform(const glm::mat4& transform) noexcept { m_Transform = transform; }

	const glm::mat4& GetTransform() const noexcept { return m_Transform; }
	const std::vector<std::shared_ptr<Model>>& GetModels() const noexcept { return m_Models; }
	
private:
	std::vector<std::shared_ptr<Model>> m_Models;
	glm::mat4 m_Transform = glm::mat4(1.0f);
};
