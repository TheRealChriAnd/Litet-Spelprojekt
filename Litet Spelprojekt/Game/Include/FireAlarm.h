#pragma once
#include <Audio/Sources/AudioSource.h>
#include <Graphics/GameObject.h>

class FireAlarm : public GameObject, public AudioSource
{
public:
	FireAlarm(int32 source);
	void SetPosition(const glm::vec3& position) noexcept;
	void SetDetection(bool fireDetected);
	void Update(float dt);
private:
	bool m_FireDetected;
};