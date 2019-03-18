#pragma once
#include "IScenario.h"
#include "../GameObjectArtillery.h"

#if defined(PRINT_CPU_DEBUG_DATA)
#include <System/CPUProfiler.h>
#endif

class ScenarioArtillery : public IScenario, public IMeshListener
{
public:
	ScenarioArtillery(uint32 numInstances);
	virtual ~ScenarioArtillery();

	virtual void Init(World* pWorld) noexcept override;
	virtual void Release() noexcept override;
	virtual void OnStart(SceneGame* scene) noexcept override;
	virtual void OnEnd(SceneGame* scene)noexcept override;
	virtual void Escalate(const glm::ivec3& position, float severity) noexcept override;
	virtual void OnVisibilityChange(World* pWorld, SceneGame* pScene) noexcept override;
	virtual bool Update(float dtS, World* world, SceneGame* scene) noexcept override;
	virtual void OnMeshEmitterKilled(MeshEmitter* emitter) override;
	virtual std::string GetName() noexcept override;
	virtual int32 GetCooldownTime() noexcept override;
	virtual int32 GetMaxTimeBeforeOutbreak() noexcept override;
	virtual bool IsComplete() noexcept override;

private:
	GameObjectArtillery* m_pGameObjectArtillery;
	uint32 m_InstancesToSpawn;
	uint32 m_InstancesComplete;
	glm::vec3 m_Target;
	AudioSource* m_pAudioSourceExplosion;
};