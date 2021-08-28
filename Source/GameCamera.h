#pragma once

#include "Camera.h"

class GameCamera : public Camera
{
public:
	virtual void OnBegin() override;
	virtual void OnUpdate(float deltaTime) override;

	void SetSpeed(float speed) noexcept { m_Speed = speed; }
	void SetMouseSenstivity(float senstivity) noexcept { m_MouseSenstivity = senstivity; }

	float GetSpeed() const noexcept { return m_Speed; }
	float GetMouseSenstivity() const noexcept { return m_MouseSenstivity; }

private:
	float m_Speed;
	float m_MouseSenstivity;
};
