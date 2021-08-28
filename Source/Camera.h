#pragma once

#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
	struct CameraProperties
	{
		float Fov;
		float AspectRatio;
		float zNear;
		float zFar;
	};

	struct Rotation
	{
		float Yaw;
		float Pitch;
	};

	struct DirectionalVectors
	{
		glm::vec3 Forward;
		glm::vec3 Right;
		glm::vec3 Up;
		static constexpr glm::vec3 WorldUp = { 0.0f, 1.0f, 0.0f };
	};

public:
	virtual void OnBegin();
	virtual void OnUpdate(float deltaTime);

	void Rotate(Rotation rotation) noexcept;
	void Translate(glm::vec3 position) noexcept { m_Position = position; }
	void LookAt(glm::vec3 direction) noexcept { m_Direction = glm::normalize(direction); }

	void SetCameraProperties(CameraProperties cameraProps) { m_CameraProps = cameraProps; }

	CameraProperties GetCameraProperties() const noexcept { return m_CameraProps; }
	const glm::mat4& GetProjectionTransform() const noexcept { return m_ProjectionTransform; }
	const glm::mat4& GetViewTransform() const noexcept { return m_ViewTransform; }
	Rotation GetRotation() const noexcept { return m_Rotation; }
	glm::vec3 GetPosition() const noexcept { return m_Position; }
	DirectionalVectors GetDirectionalVectors() const noexcept { return m_DirectionalVecs; }

private:
	CameraProperties m_CameraProps;
	glm::mat4 m_ProjectionTransform;
	glm::mat4 m_ViewTransform;

	Rotation m_Rotation;
	glm::vec3 m_Position;
	glm::vec3 m_Direction;

	DirectionalVectors m_DirectionalVecs;

private:
	glm::mat4 ConstructLookAtMatrix(glm::vec3 cameraPos, glm::vec3 viewPos, glm::vec3 worldUp) noexcept;
};