#pragma once

#include <glm/glm.hpp>

struct LightColor
{
	glm::vec3 Diffuse;
	glm::vec3 Specular;
	glm::vec3 Ambient;	
};

struct AttenuationTerms
{
	float Constant;
	float Linear;
	float Quadratic;
};

class Light
{
public:
	void SetColor(LightColor color) noexcept { m_Color = color; }

	LightColor GetColor() const noexcept { return m_Color; }

protected:
	Light(LightColor color) noexcept
		: m_Color(color) {}

	Light() noexcept = default;
	Light(const Light&) noexcept = default;
	Light& operator=(const Light&) noexcept = default;

private:
	LightColor m_Color;
};

class PointLight : public Light
{
public:
	PointLight(LightColor color, glm::vec3 position, AttenuationTerms attenuation) noexcept
		: Light(color), m_Position(position), m_Attenuation(attenuation) {}

	PointLight() noexcept = default;

	void SetPosition(glm::vec3 position) noexcept { m_Position = position; }
	void SetAttenuation(AttenuationTerms attenuation) noexcept { m_Attenuation = attenuation; }

	glm::vec3 GetPosition() const noexcept { return m_Position; }
	AttenuationTerms GetAttenuation() const noexcept { return m_Attenuation; }

private:
	glm::vec3 m_Position;
	AttenuationTerms m_Attenuation;
};

class DirectionalLight : public Light
{
public:
	DirectionalLight(LightColor color, glm::vec3 direction) noexcept
		: Light(color), m_Direction(direction) {}

	DirectionalLight() noexcept = default;

	void SetDirection(glm::vec3 direction) noexcept { m_Direction = direction; }
	
	glm::vec3 GetDirection() const noexcept { return m_Direction; }

private:
	glm::vec3 m_Direction;
};

class SpotLight : public Light
{
public:
	SpotLight(LightColor color, glm::vec3 position, glm::vec3 direction, float cutOff, float outerCutOff, AttenuationTerms attenuation) noexcept
		: Light(color), m_Position(position), m_Direction(direction), m_CutOff(cutOff), m_OuterCutOff(outerCutOff), m_Attenuation(attenuation) {}

	SpotLight() noexcept = default;

	void SetPosition(glm::vec3 position) noexcept { m_Position = position; }
	void SetDirection(glm::vec3 direction) noexcept { m_Direction = direction; }
	void SetCutOff(float cutOff) noexcept { m_CutOff = cutOff; }
	void SetOuterCutOff(float outerCutOff) noexcept { m_OuterCutOff = outerCutOff; }
	void SetAttenuation(AttenuationTerms attenuation) noexcept { m_Attenuation = attenuation; }

	glm::vec3 GetPosition() const noexcept { return m_Position; }
	glm::vec3 GetDirection() const noexcept { return m_Direction; }
	float GetCutOff() const noexcept { return m_CutOff; }
	float GetOuterCutOff() const noexcept { return m_OuterCutOff; }
	AttenuationTerms GetAttenuation() const noexcept { return m_Attenuation; }

private:
	glm::vec3 m_Position;
	glm::vec3 m_Direction;
	float m_CutOff;
	float m_OuterCutOff;
	AttenuationTerms m_Attenuation;
};