#pragma once

#include <glm/glm.hpp>

struct LightColor
{
	glm::vec3 Color;
	glm::vec3 SpecularColor;
	glm::vec3 AmbientColor;	
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
