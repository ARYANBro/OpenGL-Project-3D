#pragma once

#include "Application.h"
#include "ModelLoader3D.h"
#include "GameCamera.h"
#include "Lights.h"

class GameApplication : public Application
{
public:
	GameApplication(const WindowProps& windowProps)
		: Application(windowProps) {}

	void OnBegin() override;
	void OnUpdate(float deltaTime) override;
	void OnRender() override;
	void OnEnd() override;

private:
	std::vector<std::shared_ptr<PointLight>> m_PointLights;
	std::vector<std::shared_ptr<DirectionalLight>> m_DirLights;
	std::vector<std::shared_ptr<SpotLight>> m_SpotLights;

	std::shared_ptr<PointLight> m_PointLight;
	std::shared_ptr<DirectionalLight> m_DirLight;
	std::shared_ptr<SpotLight> m_SpotLight;

	GameCamera m_Camera;

	RenderObject m_Vase;
	RenderObject m_Barrel;
};
