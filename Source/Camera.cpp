#include "Camera.h"

#include "Application.h"

#include <GLFW/glfw3.h>

void Camera::OnBegin()
{
	LookAt(glm::vec3{ 0.0f, 0.0f, -1.0f });
	Rotate(Rotation{ -90.0f, 0.0f });
}

void Camera::OnUpdate(float deltaTime)
{
	m_ProjectionTransform = glm::perspective(m_CameraProps.Fov, m_CameraProps.AspectRatio, m_CameraProps.zNear, m_CameraProps.zFar);
	m_ViewTransform = ConstructLookAtMatrix(m_Position, m_Direction + m_Position, DirectionalVectors::WorldUp);
}

void Camera::Rotate(Rotation rotation) noexcept
{
	m_Rotation.Yaw += rotation.Yaw;
	m_Rotation.Pitch += rotation.Pitch;

	m_Rotation.Pitch = std::clamp(m_Rotation.Pitch, -89.0f, 89.0f);

	glm::vec3 direction;
	direction.x = std::cos(glm::radians(m_Rotation.Yaw)) * std::cos(glm::radians(m_Rotation.Pitch));
	direction.y = std::sin(glm::radians(m_Rotation.Pitch));
	direction.z = std::sin(glm::radians(m_Rotation.Yaw)) * std::cos(glm::radians(m_Rotation.Pitch));

	m_Direction = glm::normalize(direction);
}

glm::mat4 Camera::ConstructLookAtMatrix(glm::vec3 cameraPos, glm::vec3 targetPos, glm::vec3 worldUp) noexcept
{
	m_DirectionalVecs.Forward = glm::normalize(cameraPos - targetPos);
	m_DirectionalVecs.Right = glm::normalize(glm::cross(worldUp, m_DirectionalVecs.Forward));
	m_DirectionalVecs.Up = glm::cross(m_DirectionalVecs.Forward, m_DirectionalVecs.Right);

	glm::mat4 rotation(1.0f);
	rotation[0][0] = m_DirectionalVecs.Right.x;
	rotation[1][0] = m_DirectionalVecs.Right.y;
	rotation[2][0] = m_DirectionalVecs.Right.z;
	
	rotation[0][1] = m_DirectionalVecs.Up.x;
	rotation[1][1] = m_DirectionalVecs.Up.y;
	rotation[2][1] = m_DirectionalVecs.Up.z;

	rotation[0][2] = m_DirectionalVecs.Forward.x;
	rotation[1][2] = m_DirectionalVecs.Forward.y;
	rotation[2][2] = m_DirectionalVecs.Forward.z;

	glm::mat4 translation = glm::translate(glm::mat4(1.0f), -cameraPos);

	return rotation * translation;
}