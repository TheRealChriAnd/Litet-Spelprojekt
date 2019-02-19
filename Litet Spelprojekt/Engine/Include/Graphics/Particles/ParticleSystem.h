#pragma once
#include <Graphics/Textures/Texture2D.h>
#include <Graphics/Geometry/Particle.h>
#include <Graphics/Camera.h>
#include "ParticleData.h"

enum ParticleBlendMode : uint32
{
	PARTICLE_NORMAL = 1,
	PARTICLE_ADDITIVE = 2
};

class API ParticleSystem
{
	friend class DefferedRenderer;
	friend class ForwardRenderer;
	friend class ParticleEmitter;

public:
	ParticleSystem(ParticleSystem&& other) = delete;
	ParticleSystem(const ParticleSystem& other) = delete;
	ParticleSystem& operator=(ParticleSystem&& other) = delete;
	ParticleSystem& operator=(const ParticleSystem& other) = delete;

	ParticleSystem();
	~ParticleSystem();

	void Update(const Camera& camera, float deltaTime) noexcept;

	void SetParticleBlendMode(ParticleBlendMode mode) noexcept;
	void SetConeAngle(float angleRad) noexcept;
	void SetSpeed(float min, float max) noexcept;
	void SetTimeToLive(float timeToLive) noexcept;
	void SetTexture(uint32 textureID) noexcept;
	void SetColor(const glm::vec4& begin, const glm::vec4& end) noexcept;
	uint32 GetNumParticles() const noexcept;

private:
	void SpawnParticle(const glm::vec3& position) noexcept;
	const Texture2D* GetTexture() const noexcept;
	const ParticleInstance* GetParticleInstances() const noexcept;
	const glm::vec4& GetBeginColor() const noexcept;
	const glm::vec4& GetEndColor() const noexcept;
	ParticleData& GetLivingParticle(uint32 index) noexcept;
	ParticleData& GetSortedParticle(uint32 index) noexcept;
	void InsertSortedParticle(uint32 id) noexcept;
	void KillParticle(uint32 index);
	ParticleBlendMode GetParticleBlendMode() const noexcept;

private:
	const Texture2D* m_pTexture;
	uint32* m_pLivingParticles;
	uint32* m_pSortedParticles;
	uint32* m_pTempBuff;
	ParticleData* m_pParticles;
	ParticleInstance* m_pParticleInstances;
	
	ParticleBlendMode m_BlendMode;
	glm::vec4 m_Direction;
	glm::vec4 m_BeginColor;
	glm::vec4 m_EndColor;
	float m_TimeToLive;
	float m_MinSpeed;
	float m_MaxSpeed;
	float m_ConeAngle;
	uint32 m_NumSortedParticles;
	uint32 m_NumParticles;
	uint32 m_MaxParticles;
};