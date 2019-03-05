﻿#include "..\..\Include\Scenarios\ScenarioTorpedo.h"
#include "..\..\Include\Scenarios\ScenarioManager.h"
#include "..\..\Include\Game.h"
#include <System/Random.h>
#include <World/Logger.h>
#include <Audio/Sources/AudioSource.h>

ScenarioTorpedo::ScenarioTorpedo() : pAudioSourceExplosion(nullptr)
{

}

ScenarioTorpedo::~ScenarioTorpedo()
{
	DeleteSafe(pAudioSourceExplosion);
}

void ScenarioTorpedo::Init(World* pWorld) noexcept
{

}

void ScenarioTorpedo::OnStart(SceneGame* scene) noexcept
{
	float rotation = Random::GenerateFloat(0.0, glm::two_pi<float>());
	glm::vec3 pos = glm::vec3(glm::cos(rotation) * 200, 0.0F, glm::sin(rotation) * 200);

	glm::vec3 centre = glm::vec3(6, 0, 21);
	glm::vec3 ray = glm::normalize(pos - centre);
	m_Target = pos + ray * (float)TestAgainstRay(ray, pos);

	m_pGameObjectTorpedo = new GameObjectTorpedo(pos, m_Target);

	scene->AddGameObject(m_pGameObjectTorpedo);
}

void ScenarioTorpedo::OnEnd(SceneGame* scene) noexcept
{
	scene->RemoveGameObject(m_pGameObjectTorpedo);
	DeleteSafe(m_pGameObjectTorpedo);

	WaterOutdoorMaterial* pMaterial = reinterpret_cast<WaterOutdoorMaterial*> (ResourceHandler::GetMaterial(MATERIAL::WATER_OUTDOOR));
	pMaterial->SetTorpedoPosition(glm::vec2(0, 0));
}

void ScenarioTorpedo::OnVisibilityChange(World* pWorld, SceneGame* pScene) noexcept
{

}

void ScenarioTorpedo::Escalate(const glm::ivec3& position) noexcept
{

}

bool ScenarioTorpedo::Update(float dtS, World* world, SceneGame* scene) noexcept
{
	if (m_pGameObjectTorpedo)
	{
		const glm::vec3& position = m_pGameObjectTorpedo->GetPosition();
		WaterOutdoorMaterial* pMaterial = reinterpret_cast<WaterOutdoorMaterial*> (ResourceHandler::GetMaterial(MATERIAL::WATER_OUTDOOR));
		pMaterial->SetTorpedoPosition(glm::vec2(position.x, position.z));

		if (m_pGameObjectTorpedo->HasHitTarget())
		{
			ScenarioManager::Escalate(Game::GetGame()->m_ScenarioWater, m_Target);

			MeshEmitter* pMeshEmitter = new MeshEmitter(1.0F, this);
			pMeshEmitter->SetMesh(MESH::MESH_PARTICLE);
			pMeshEmitter->SetTimeToLive(2.0f);
			pMeshEmitter->SetConeAngle(glm::radians<float>(100.0f));
			pMeshEmitter->SetSpeed(1.0f, 2.0f);
			pMeshEmitter->SetScale(glm::vec2(0.1f), glm::vec2(0.5f));
			pMeshEmitter->SetBeginColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
			pMeshEmitter->SetEndColor(glm::vec4(0.05f, 0.05f, 0.05f, 1.0f));
			pMeshEmitter->SetPosition(m_Target);
			pMeshEmitter->SetParticlesPerSeconds(150);
			pMeshEmitter->UpdateTransform();
			pMeshEmitter->SetIsVisible(true);
			scene->AddGameObject(pMeshEmitter);

			pAudioSourceExplosion = AudioSource::CreateSoundSource(SOUND::MONO_EXPLOSION);
			pAudioSourceExplosion->SetRollOffFactor(10.0f);
			pAudioSourceExplosion->SetReferenceDistance(0.0f);
			pAudioSourceExplosion->SetMaxDistance(500.0f);
			pAudioSourceExplosion->SetLooping(false);
			pAudioSourceExplosion->Play();

			Crew* crew = scene->GetCrew();
			for (int i = 0; i < crew->GetCount(); i++)
			{
				float distance = glm::distance(crew->GetMember(i)->GetPosition(), m_Target);
				if (distance <= 10)
				{
					crew->GetMember(i)->ApplyBurnInjury(10 - distance + 1);
					crew->GetMember(i)->ApplyBoneInjury();
					Logger::LogEvent(crew->GetMember(i)->GetName() + " was hit!", false);
				}
			}

			return true;
		}
		return false;
	}
	return true;
}

void ScenarioTorpedo::OnMeshEmitterKilled(MeshEmitter* emitter)
{
	DeleteSafe(pAudioSourceExplosion);
}

std::string ScenarioTorpedo::GetName() noexcept
{
	return "Torpedträff";
}

int32 ScenarioTorpedo::GetCooldownTime() noexcept
{
	return 100;
}

int32 ScenarioTorpedo::GetMaxTimeBeforeOutbreak() noexcept
{
	return 60 * 5;
}

int32 ScenarioTorpedo::TestAgainstRay(const glm::vec3 ray, const glm::vec3 origin) noexcept
{
	glm::vec3 centre = glm::vec3(6, 0, 21);

	glm::vec3 normals[]{
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
	};

	float size[] = {
		5,
		20
	};

	float d1[] = {
		glm::dot(centre - normals[0] * size[0], normals[0]),
		glm::dot(centre - normals[1] * size[1], normals[1])
	};
	float d2[] = {
		glm::dot(centre + normals[0] * size[0], normals[0]),
		glm::dot(centre + normals[1] * size[1], normals[1])
	};

	float t1[2];
	float t2[2];
	float t_min[2];
	float t_max[2];

	for (int j = 0; j < 2; j++)
	{
		if (std::abs(glm::dot(normals[j], ray)) > 0.01)
		{
			t1[j] = (d1[j] - glm::dot(normals[j], origin)) / glm::dot(normals[j], ray);
			t2[j] = (d2[j] - glm::dot(normals[j], origin)) / glm::dot(normals[j], ray);

			t_min[j] = std::min(t1[j], t2[j]);
			t_max[j] = std::max(t1[j], t2[j]);
		}
	}

	return std::min(t_max[0], t_max[1]);
}