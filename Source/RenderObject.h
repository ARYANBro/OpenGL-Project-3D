#pragma once

#include "Model.h"

#include <glm/glm.hpp>

#include <memory>

class RenderObject
{
public:
	RenderObject(std::vector<Model>&& models, const glm::mat4& transform = glm::mat4(1.0f))
		: m_Models(std::move(models)), m_Transform(transform) {}

	RenderObject() noexcept = default;

	void AddModel(Model&& model) noexcept { m_Models.emplace_back(std::move(model)); }
	void SetTransform(const glm::mat4& transform) noexcept { m_Transform = transform; }

	const glm::mat4& GetTransform() const noexcept { return m_Transform; }
	const std::vector<Model>& GetModels() const noexcept { return m_Models; }
	
private:
	std::vector<Model> m_Models;
	glm::mat4 m_Transform = glm::mat4(1.0f);
};
