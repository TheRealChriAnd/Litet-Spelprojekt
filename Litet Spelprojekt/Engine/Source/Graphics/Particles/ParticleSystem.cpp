#include <EnginePch.h>
#include <System/Random.h>
#include <Graphics/Particles/ParticleSystem.h>
#include <IO/ResourceHandler.h>
#include <GLMHelper.inl>

ParticleSystem::ParticleSystem()
	: m_pTexture(nullptr),
	m_pLivingParticles(nullptr),
	m_pSortedParticles(nullptr),
	m_pParticles(nullptr),
	m_pParticleInstances(nullptr),
	m_TimeToLive(0.0f),
	m_MinSpeed(0.0f),
	m_MaxSpeed(0.0f),
	m_ConeAngle(0.0f),
	m_NumSortedParticles(0),
	m_NumParticles(0),
	m_MaxParticles(0),
	m_Direction(),
	m_BeginColor(),
	m_EndColor()
{
	m_MaxParticles = 100000;

	m_pParticles = new ParticleData[m_MaxParticles];
	m_pLivingParticles = new uint32[m_MaxParticles];
	m_pSortedParticles = new uint32[m_MaxParticles];
	m_pParticleInstances = new ParticleInstance[m_MaxParticles];

	for (uint32 i = 0; i < m_MaxParticles; i++)
	{
		m_pLivingParticles[i] = i;
		m_pSortedParticles[i] = i;

		m_pParticles[i].Position = glm::vec3(0.0f);
		m_pParticles[i].Direction = UP_VECTOR;
		m_pParticles[i].TimeLived = 0.0f;
		m_pParticles[i].Speed = 0.0f;
		m_pParticles[i].DistToCameraSqrd = 0.0f;

		m_pParticleInstances[i].Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		m_pParticleInstances[i].Position = glm::vec3(0.0f);
	}
}

ParticleSystem::~ParticleSystem()
{
	DeleteArrSafe(m_pLivingParticles);
	DeleteArrSafe(m_pSortedParticles);
	DeleteArrSafe(m_pParticles);
	DeleteArrSafe(m_pParticleInstances);
}

void ParticleSystem::Update(const Camera& camera, float deltaTime) noexcept
{
	//Update and sort particles
	m_NumSortedParticles = 0;
	for (uint32 i = 0; i < GetNumParticles(); i++)
	{
		//Update
		ParticleData& particle = GetLivingParticle(i);
		particle.TimeLived += deltaTime;
		if (particle.TimeLived > m_TimeToLive)
		{
			KillParticle(i);
			i--;

			continue;
		}

		//Position
		particle.Position += (particle.Direction * particle.Speed * deltaTime);
		particle.LifePercentage = particle.TimeLived / m_TimeToLive;
		particle.DistToCameraSqrd = LengthSqrd(particle.Position - camera.GetPosition());

		//Color
		glm::vec4 diff =  m_EndColor - m_BeginColor;
		particle.Color = m_BeginColor + (diff * particle.LifePercentage);

		InsertSortedParticle(m_pLivingParticles[i]);
	}

	//Fill instances
	for (uint32 i = 0; i < m_NumSortedParticles; i++)
	{
		ParticleData& data = GetSortedParticle(i);
		m_pParticleInstances[i].Color = data.Color;
		m_pParticleInstances[i].Color.a = data.Color.a * (1.0f - data.LifePercentage);
		m_pParticleInstances[i].Position = data.Position;
	}
}

void ParticleSystem::SetParticleBlendMode(ParticleBlendMode blendMode) noexcept
{
	m_BlendMode = blendMode;
}

ParticleBlendMode ParticleSystem::GetParticleBlendMode() const noexcept
{
	return m_BlendMode;
}

void ParticleSystem::SetConeAngle(float angleRad) noexcept
{
	m_ConeAngle = abs(angleRad);
}

void ParticleSystem::SetSpeed(float min, float max) noexcept
{
	m_MinSpeed = min;
	m_MaxSpeed = max;
}

void ParticleSystem::SetTimeToLive(float timeToLive) noexcept
{
	m_TimeToLive = timeToLive;
}

void ParticleSystem::SetTexture(uint32 textureID) noexcept
{
	m_pTexture = ResourceHandler::GetTexture2D(textureID);
}

void ParticleSystem::SetColor(const glm::vec4& begin, const glm::vec4& end) noexcept
{
	m_BeginColor = begin;
	m_EndColor = end;
}

uint32 ParticleSystem::GetNumParticles() const noexcept
{
	return m_NumParticles;
}

const Texture2D* ParticleSystem::GetTexture() const noexcept
{
	return m_pTexture;
}

const ParticleInstance* ParticleSystem::GetParticleInstances() const noexcept
{
	return m_pParticleInstances;
}

const glm::vec4& ParticleSystem::GetBeginColor() const noexcept
{
	return m_BeginColor;
}

const glm::vec4& ParticleSystem::GetEndColor() const noexcept
{
	return m_EndColor;
}

ParticleData& ParticleSystem::GetLivingParticle(uint32 index) noexcept
{
	return m_pParticles[m_pLivingParticles[index]];
}

ParticleData& ParticleSystem::GetSortedParticle(uint32 index) noexcept
{
	return m_pParticles[m_pSortedParticles[index]];
}

void ParticleSystem::InsertSortedParticle(uint32 id) noexcept
{
	ParticleData& particle = m_pParticles[id];

	int32 index = m_NumSortedParticles;
	for (int32 i = m_NumSortedParticles - 1; i > 0; i--)
	{
		if (particle > GetSortedParticle(i))
		{
			index = i;
			memcpy(&m_pSortedParticles[i + 1], &m_pSortedParticles[i], sizeof(uint32) * (m_NumSortedParticles - i));
			break;
		}
	}

	m_pSortedParticles[index] = id;
	m_NumSortedParticles++;
}

void ParticleSystem::SpawnParticle(const glm::vec3& position) noexcept
{
	if (m_NumParticles < m_MaxParticles)
	{
		ParticleData& particle = GetLivingParticle(m_NumParticles);
		particle.Speed = Random::GenerateFloat(m_MinSpeed, m_MaxSpeed);
		particle.Position = position;

		glm::vec4 direction = glm::rotate(glm::mat4(1.0f), Random::GenerateFloat(0.0f, m_ConeAngle), glm::vec3(0.0f, 0.0f, 1.0f)) * glm::vec4(UP_VECTOR, 0.0f);
		direction = glm::rotate(glm::mat4(1.0f), Random::GenerateFloat(0.0f, glm::two_pi<float>()), glm::vec3(0.0f, 1.0f, 0.0f)) * direction;
		particle.Direction.x = direction.x;
		particle.Direction.y = direction.y;
		particle.Direction.z = direction.z;

		particle.TimeLived = 0.0f;
		particle.DistToCameraSqrd = 0.0f;

		m_NumParticles++;
	}
}

void ParticleSystem::KillParticle(uint32 index)
{
	if (index == m_NumParticles - 1)
	{
		m_NumParticles--;
		return;
	}

	uint32 temp = m_pLivingParticles[index];
	m_pLivingParticles[index] = m_pLivingParticles[m_NumParticles - 1];
	m_pLivingParticles[m_NumParticles - 1] = temp;

	m_NumParticles--;
}