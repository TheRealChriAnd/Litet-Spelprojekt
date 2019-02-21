#include "..\..\Include\Scenarios\ScenarioFire.h"
#include <System/Random.h>

ScenarioFire::ScenarioFire(World* world)
{
	m_pWorld = world;

	m_pppMap = new const uint32* const*[m_pWorld->GetNumLevels()];

	for (uint32 i = 0; i < m_pWorld->GetNumLevels(); i++)
	{
		m_pppMap[i] = m_pWorld->GetLevel(i)->GetLevel();
	}
}

ScenarioFire::~ScenarioFire()
{
	DeleteArrSafe(m_pppMap);
}

void ScenarioFire::OnStart() noexcept
{
	uint32 lvl = Random::GenerateInt(0, m_pWorld->GetNumLevels() - 1);
	uint32 x = Random::GenerateInt(1, m_pWorld->GetLevel(lvl)->GetSizeX() - 2);
	uint32 z = Random::GenerateInt(1, m_pWorld->GetLevel(lvl)->GetSizeZ() - 2);
	m_OnFire.push_back(glm::vec3(x, lvl, z));

	TileData& tileData = m_pWorld->GetLevel(lvl)->GetLevelData()[x][z];
	tileData.Temp = 100.1f;
	tileData.Burning = true;
}

void ScenarioFire::OnEnd() noexcept
{

}

void ScenarioFire::OnVisibilityChange(World * pWorld, Scene * pScene, const std::vector<uint32>& activeRooms) noexcept
{
}

bool ScenarioFire::Update(float dtS, World* world, Scene* scene, const std::vector<uint32>& activeRooms) noexcept
{
	// current, right, left, forward, back, up, down.
	for (uint32 i = 0; i < m_OnFire.size(); i++)
	{
		glm::ivec3 pos = m_OnFire[i];
		CheckFire(dtS, glm::ivec3(0, 0, 0), pos, scene);
		CheckFire(dtS, glm::ivec3(1, 0, 0), pos, scene);
		CheckFire(dtS, glm::ivec3(-1, 0, 0), pos, scene);
		CheckFire(dtS, glm::ivec3(0, 0, 1), pos, scene);
		CheckFire(dtS, glm::ivec3(0, 0, -1), pos, scene);

		if (pos.y < m_pWorld->GetNumLevels() - 1)
		{
			CheckFire(dtS, glm::ivec3(0, 1, 0), pos, scene);
		}

		if (pos.y - 1 > 0)
		{
			CheckFire(dtS, glm::ivec3(0, -1, 0), pos, scene);
		}

	}

	for (uint32 i = 0; i < m_OnFire.size(); i++)
	{
		glm::ivec3& pos = m_OnFire[i];
		TileData& tileData = m_pWorld->GetLevel((int32)pos.y + ((int32)pos.y + 1) % 2)->GetLevelData()[(int32)pos.x][(int32)pos.z];
		bool alreadySmoke = tileData.SmokeAmount >= tileData.SmokeLimit;

		tileData.SmokeAmount += pow(m_pWorld->GetLevel((int32)pos.y)->GetLevelData()[(int32)pos.x][(int32)pos.z].Temp,2.0f);
		tileData.SmokeAmount = std::min(tileData.SmokeAmount, 400.0f);

		if (!alreadySmoke && tileData.SmokeAmount >= tileData.SmokeLimit)
		{
			m_Smoke.push_back(pos);
			GameObject* tmp = new GameObject();
			tmp->SetMesh(MESH::SPHERE);
			tmp->SetMaterial(MATERIAL::GREEN);
			tmp->SetPosition(pos + glm::ivec3(0.0f, ((int32)pos.y + 1) % 2, 0.0f));
			tmp->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
			scene->AddGameObject(tmp);
		}
	}
	uint32 max = m_Smoke.size();
	for (uint32 j = 0; j < max; j++)
	{
		glm::ivec3& smoke = m_Smoke[j];
		TileData& data = m_pWorld->GetLevel((int32)smoke.y)->GetLevelData()[smoke.x][smoke.z];

		float spread = data.SmokeAmount - data.SmokeLimit;
		spread /= 4;
		spread *= dtS;
		uint32 rest = 0;
		if (spread > 0.0f)
		{
			glm::ivec3 smokeOriginPos = glm::ivec3(smoke) /*+ glm::ivec3(0.0, (smoke.y + 1) % 2, 0.0f)*/;
			rest += CheckSmoke(dtS, glm::ivec3(1, 0, 0), smokeOriginPos, spread, scene);
			rest += CheckSmoke(dtS, glm::ivec3(-1, 0, 0), smokeOriginPos, spread, scene);
			rest += CheckSmoke(dtS, glm::ivec3(0, 0, 1), smokeOriginPos, spread, scene);
			rest += CheckSmoke(dtS, glm::ivec3(0, 0, -1), smokeOriginPos, spread, scene);
			data.SmokeAmount -= spread * rest;
		}
	}

	return m_OnFire.empty();
}

std::string ScenarioFire::GetName() noexcept
{
	return "Fire";
}

int32 ScenarioFire::GetCooldownTime() noexcept
{
	return 20;
}

int32 ScenarioFire::GetMaxTimeBeforeOutbreak() noexcept
{
	return 60;
}

void ScenarioFire::CheckFire(float dtS, const glm::ivec3& offset, const glm::ivec3& origin, Scene* scene)
{
	TileData& originTile = m_pWorld->GetLevel(origin.y)->GetLevelData()[origin.x][origin.z];
	TileData& tileData = m_pWorld->GetLevel(origin.y + offset.y)->GetLevelData()[origin.x + offset.x][origin.z + offset.z];
	float rateOfSpread = 0.1f;
	float rateOfWallSpread = 0.002;
	float rateOfFloorSpread = 0.0001;

	rateOfSpread *= (m_pppMap[origin.y + offset.y][origin.x + offset.x][origin.z + offset.z] == m_pppMap[origin.y][origin.x][origin.z] || (tileData.HasDoor && originTile.HasDoor));
	rateOfSpread += (rateOfWallSpread * (offset.y + 1) + rateOfFloorSpread) * (m_pppMap[origin.y + offset.y][origin.x + offset.x][origin.z + offset.z] != 1);

	tileData.Temp += std::max((originTile.Temp - tileData.BurnsAt) * rateOfSpread * dtS, 0.0f);

	if (tileData.Temp >= tileData.BurnsAt && !tileData.Burning)
	{
		m_OnFire.push_back(glm::vec3(origin) + glm::vec3(offset));
		tileData.Burning = true;

		GameObject* pGameObject;
		{
			pGameObject = new GameObject();
			pGameObject->SetMaterial(MATERIAL::BOAT);
			pGameObject->SetMesh(MESH::CUBE_OBJ);
			pGameObject->SetPosition(glm::vec3(origin) + glm::vec3(offset) + glm::vec3(0.0f, 0.5f, 0.0f));
			pGameObject->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
			pGameObject->UpdateTransform();
			scene->AddGameObject(pGameObject);
		}
	}
	else if (tileData.Temp < tileData.BurnsAt && tileData.Burning)
	{
		tileData.Burning = false;
		for (uint32 i = 0; i < m_OnFire.size(); i++)
		{
			if (m_OnFire[i] == glm::ivec3(origin) + glm::ivec3(offset))
			{
				m_OnFire.erase(m_OnFire.begin() + i);
			}
		}
	}
}

bool ScenarioFire::CheckSmoke(float dtS, const glm::ivec3 & offset, const glm::ivec3 & origin, float amount, Scene* scene)
{
	bool res = false;
	TileData& tileData = m_pWorld->GetLevel(origin.y + offset.y)->GetLevelData()[origin.x + offset.x][origin.z + offset.z];
	bool filled = tileData.SmokeAmount >= tileData.SmokeLimit;
	//HasDoor and hasStairs never set?
	if (m_pppMap[origin.y + offset.y][origin.x + offset.x][origin.z + offset.z] == m_pppMap[origin.y][origin.x][origin.z] || tileData.HasDoor || tileData.HasStairs)
	{
		tileData.SmokeAmount += amount;
		if (!filled && tileData.SmokeAmount >= tileData.SmokeLimit)
		{
			m_Smoke.push_back(origin + offset);

			GameObject* tmp = new GameObject();
			tmp->SetMesh(MESH::SPHERE);
			tmp->SetMaterial(MATERIAL::GREEN);
			tmp->SetPosition(origin + offset + glm::ivec3(0.0f, ((int32)(origin + offset).y + 1) % 2, 0.0f));
			tmp->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
			scene->AddGameObject(tmp);
		}
		res = true;
	}
	return res;
}
