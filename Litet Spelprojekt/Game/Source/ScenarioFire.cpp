#include "..\Include\ScenarioFire.h"
#include <System/Random.h>

ScenarioFire::ScenarioFire(World* world)
{
	m_pWorld = world;
}

void ScenarioFire::OnStart() noexcept
{
	uint32 lvl = Random::GenerateInt(0, m_pWorld->GetNumLevels() - 1);
	uint32 x = Random::GenerateInt(1, m_pWorld->GetLevel(lvl)->GetSizeX() - 2);
	uint32 z = Random::GenerateInt(1, m_pWorld->GetLevel(lvl)->GetSizeZ() - 2);
	m_OnFire.push_back(glm::vec3(x, lvl, z));

	TileData tileData = m_pWorld->GetLevel(lvl)->GetLevelData()[x][z];
	tileData.Temp = 100.0f;
	m_pWorld->SetTileData(glm::ivec3(x, lvl, z), tileData);
}

void ScenarioFire::OnEnd() noexcept
{
	
}

bool ScenarioFire::Update(float dtS, World* world, Scene* scene) noexcept
{
	// current, right, left, forward, back, up, down.
	for (uint32 i = 0; i < m_OnFire.size(); i++)
	{
		glm::vec3 curr = m_OnFire[i];
		CheckFire(dtS, glm::ivec3(0, 0, 0), curr, scene);
		CheckFire(dtS, glm::ivec3(1, 0, 0), curr, scene);
		CheckFire(dtS, glm::ivec3(-1, 0, 0), curr, scene);
		CheckFire(dtS, glm::ivec3(0, 0, 1), curr, scene);
		CheckFire(dtS, glm::ivec3(0, 0, -1), curr, scene);
		
		if (curr.y < m_pWorld->GetNumLevels() - 1)
		{
			CheckFire(dtS, glm::ivec3(0, 1, 0), curr, scene);
		}

		if (curr.y - 1 > 0)
		{
			CheckFire(dtS, glm::ivec3(0, -1, 0), curr, scene);
		}


		TileData tileData;
		for (uint32 i = 0; i < m_OnFire.size(); i++)
		{
			tileData = m_pWorld->GetLevel((int32)m_OnFire[i].y + ((int32)m_OnFire[i].y + 1) % 2)->GetLevelData()[(int32)m_OnFire[i].x][(int32)m_OnFire[i].z];

			tileData.SmokeAmount += m_pWorld->GetLevel((int32)m_OnFire[i].y)->GetLevelData()[(int32)m_OnFire[i].x][(int32)m_OnFire[i].z].Temp;

			m_pWorld->SetTileData(glm::ivec3((int32)m_OnFire[i].x, (int32)m_OnFire[i].y + ((int32)m_OnFire[i].y + 1) % 2, (int32)m_OnFire[i].y), tileData);
			float amount = std::max((tileData.SmokeAmount - tileData.SmokeLimit) * dtS / 4, 0.0f);
			if (amount > 0.0f)
			{
				CheckSmoke(dtS, glm::ivec3(1, 0, 0), glm::ivec3((int32)m_OnFire[i].x, m_OnFire[i].y + ((int32)m_OnFire[i].y + 1) % 2, (int32)m_OnFire[i].z), amount);
				CheckSmoke(dtS, glm::ivec3(-1, 0, 0), glm::ivec3((int32)m_OnFire[i].x, m_OnFire[i].y + ((int32)m_OnFire[i].y + 1) % 2, (int32)m_OnFire[i].z), amount);
				CheckSmoke(dtS, glm::ivec3(0, 0, 1), glm::ivec3((int32)m_OnFire[i].x, m_OnFire[i].y + ((int32)m_OnFire[i].y + 1) % 2, (int32)m_OnFire[i].z), amount);
				CheckSmoke(dtS, glm::ivec3(0, 0, -1), glm::ivec3((int32)m_OnFire[i].x, m_OnFire[i].y + ((int32)m_OnFire[i].y + 1) % 2, (int32)m_OnFire[i].z), amount);
			}
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

void ScenarioFire::CheckFire(float dtS, const glm::ivec3 & offset, const glm::ivec3 & origin, Scene* scene)
{

	TileData tileData;
	TileData originTile;
	const uint32 rateOfSpread = 10;

	const uint32* const** pppMap = new const uint32* const*[m_pWorld->GetNumLevels()];

	for (uint32 i = 0; i < m_pWorld->GetNumLevels(); i++)
	{
		pppMap[i] = m_pWorld->GetLevel(i)->GetLevel();
	}

	if (pppMap[origin.y + offset.y][origin.x + offset.x][origin.z + offset.z] == pppMap[origin.y][origin.x][origin.z] || pppMap[origin.y][origin.x][origin.z] == 0 || pppMap[origin.y + offset.y][origin.x + offset.x][origin.z + offset.z] == 0)
	{
		originTile = m_pWorld->GetLevel(origin.y)->GetLevelData()[origin.x][origin.z];
		tileData = m_pWorld->GetLevel(origin.y + offset.y)->GetLevelData()[origin.x + offset.x][origin.z + offset.z];

		tileData.Temp += std::max((originTile.Temp - tileData.BurnsAt) * dtS, 0.0f);


		if (tileData.Temp >= tileData.BurnsAt && !tileData.Burning)
		{
			m_OnFire.push_back(glm::vec3(origin) + glm::vec3(offset));
			tileData.Burning = true;

			GameObject* pGameObject;
			{
				pGameObject = new GameObject();
				pGameObject->SetMaterial(MATERIAL::RED);
				pGameObject->SetMesh(MESH::CUBE_OBJ);
				pGameObject->SetPosition(glm::vec3(origin) + glm::vec3(offset) + glm::vec3(0.0f, -0.25f, 0.0f));
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
				if (m_OnFire[i] == glm::vec3(origin) + glm::vec3(offset))
				{
					m_OnFire.erase(m_OnFire.begin() + i);
				}
			}
		}

		m_pWorld->SetTileData((glm::ivec3)origin + offset, tileData);
	}
}

void ScenarioFire::CheckSmoke(float dtS, const glm::ivec3 & offset, const glm::ivec3 & origin, float amount)
{
	TileData tileData = m_pWorld->GetLevel(origin.y + offset.y)->GetLevelData()[origin.x + offset.x][origin.z + offset.z];
	const uint32* const** pppAirMap = new const uint32* const*[m_pWorld->GetNumLevels()];
	for (uint32 i = 0; i < m_pWorld->GetNumLevels(); i++)
	{
		pppAirMap[i] = m_pWorld->GetLevel(i)->GetLevel();
	}

	if (pppAirMap[origin.y + offset.y][origin.x + offset.x][origin.z + offset.z] == pppAirMap[origin.y][origin.x][origin.z] || pppAirMap[origin.y][origin.x][origin.z] == 0 || pppAirMap[origin.y + offset.y][origin.x + offset.x][origin.z + offset.z] == 0)
	{
		tileData.SmokeAmount += amount;
		float amount = std::max((tileData.SmokeAmount - tileData.SmokeLimit) * dtS / 4, 0.0f);
		tileData.SmokeAmount -= amount;
		m_pWorld->SetTileData(offset + origin, tileData);

		if (amount > 0.0f)
		{
			// Omtrycket �r h�gre p� en tile b�r den inte f� ta in mer r�k fr�n den l�gre t�ta tilen!
			CheckSmoke(dtS, glm::ivec3(1, 0, 0), offset + origin, amount);
			CheckSmoke(dtS, glm::ivec3(-1, 0, 0), offset + origin, amount);
			CheckSmoke(dtS, glm::ivec3(0, 0, 1), offset + origin, amount);
			CheckSmoke(dtS, glm::ivec3(0, 0, -1), offset + origin, amount);
		}
	}
}