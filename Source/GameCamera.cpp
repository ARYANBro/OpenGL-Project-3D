#include "GameCamera.h"

#include "Application.h"

#include <GLFW/glfw3.h>

static glm::dvec2 s_PreviousMousePos;

void GameCamera::OnBegin()
{
	Camera::OnBegin();

	glfwGetCursorPos(Application::GetInstance().GetWindow().GetHandle(), &s_PreviousMousePos.x, &s_PreviousMousePos.y);
}

void GameCamera::OnUpdate(float deltaTime)
{
	Camera::OnUpdate(deltaTime);

	glm::vec3 velocity(0.0f);

	auto getKey = [this](int key) { return glfwGetKey(Application::GetInstance().GetWindow().GetHandle(), key); };

	if (getKey(GLFW_KEY_S))
		velocity = -GetDirectionalVectors().Forward;
	else if (getKey(GLFW_KEY_W))
		velocity = GetDirectionalVectors().Forward;

	if (getKey(GLFW_KEY_A))
		velocity = -GetDirectionalVectors().Right;
	else if (getKey(GLFW_KEY_D))
		velocity = GetDirectionalVectors().Right;

	if (getKey(GLFW_KEY_Q))
		velocity = -GetDirectionalVectors().Up;
	else if (getKey(GLFW_KEY_E))
		velocity = GetDirectionalVectors().Up;

	if (glm::length(velocity) > 1.0f)
		velocity = glm::normalize(velocity);

	velocity *= m_Speed;

	glm::dvec2 currentMousePos;
	glfwGetCursorPos(Application::GetInstance().GetWindow().GetHandle(), &currentMousePos.x, &currentMousePos.y);

	glm::vec2 offset = currentMousePos - s_PreviousMousePos;
	offset.y = -offset.y;
	offset *= m_MouseSenstivity;

	s_PreviousMousePos = currentMousePos;

	Translate(GetPosition() + velocity * deltaTime);
	Rotate(Rotation{ offset.x, offset.y });
}
