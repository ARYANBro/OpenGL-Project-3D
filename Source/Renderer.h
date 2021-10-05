#pragma once

#include "Shader.h"
#include "Lights.h"

#include <memory>
#include <vector>

class Camera;
class RenderObject;

class Renderer
{
public:
	static void Init(Camera* camera) noexcept;

	static void SetClearColor(glm::vec4 color) noexcept;
	static void OnRender(const std::vector<std::shared_ptr<PointLight>>& pointLights, const std::vector<std::shared_ptr<SpotLight>>& spotLights, const std::vector<std::shared_ptr<DirectionalLight>>& dirLights) noexcept;
	static void Render(const RenderObject& object, const glm::mat4& transform) noexcept;

private:
	static Camera* s_Camera;
	static std::shared_ptr<Shader> s_Shader;
};