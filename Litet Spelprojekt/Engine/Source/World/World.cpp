#include <EnginePch.h>
#include <World/World.h>
#include <World/GameObjectDoor.h>

World::World(WorldLevel* worldLevels[], uint32 numLevels, WorldObject* objects, uint32 numObjects) noexcept
	: m_Levels(),
	m_Stairs(),
	m_Objects(),
	m_Doors(),
	m_Rooms(),
	m_RoomLightsTimers(),
	m_ActiveRooms()
{
	for (uint32 i = 0; i < numLevels; i++)
	{
		m_Levels.push_back(*worldLevels[i]);
	}

	for (uint32 i = 0; i < numObjects; i++)
	{
		m_Objects.push_back(objects[i]);
	}
}

World::~World()
{
	//for (uint32 i = 0; i < m_NumLevels; i++)
	//{
	//	DeleteSafe(m_ppLevels[i]);
	//}

	//DeleteArrSafe(m_ppLevels);
	//DeleteArrSafe(m_pStairs);

	//for (size_t i = 0; i < m_Rooms.size(); i++)
	//{
	//	DeleteSafe(m_Rooms[i]);
	//}
}

void World::AddWorldObject(const WorldObject& object) noexcept
{
	m_Objects.push_back(object);
}

const WorldLevel& const World::GetLevel(uint32 level) const noexcept
{
	assert(level < m_Levels.size());
	return m_Levels[level];
}

WorldLevel& const World::GetLevel(uint32 level) noexcept
{
	assert(level < m_Levels.size());
	return m_Levels[level];
}

uint32 World::GetNumLevels() const noexcept
{
	return m_Levels.size();
}

const WorldObject& World::GetWorldObject(uint32 index) const noexcept
{
	assert(index < m_Objects.size());
	return m_Objects[index];
}

uint32 World::GetNumWorldObjects() const noexcept
{
	return static_cast<uint32>(m_Objects.size());
}

void World::GenerateRooms(Scene& scene) noexcept
{
	//Room generation
	std::vector<glm::uvec4> m_RoomBounds;
	std::vector<glm::uvec4> temp;
	std::vector<glm::vec3> center;

	for (int32 level = 0; level < m_Levels.size(); level += 2)
	{
		m_Levels[level].GenerateRooms();
		temp = m_Levels[level].GetRooms();
		for (size_t i = 0; i < temp.size(); i++)
		{
			if (i >= m_RoomBounds.size())
			{
				m_RoomBounds.push_back(temp[i]);
				center.push_back(glm::vec3((float)temp[i].x + (temp[i].y - temp[i].x) / 2.0f, (float)level + 1.9, (float)temp[i].z + (temp[i].w - temp[i].z) / 2.0f));
			}
			else if (temp[i].x != 11)
			{
				center[i] = glm::vec3((float)temp[i].x + (temp[i].y - temp[i].x) / 2.0f, (float)level + 1.9, (float)temp[i].z + (temp[i].w - temp[i].z) / 2.0f);
			}
		}
	}
	
	//Rooms
	m_Rooms.push_back(Room());
	m_Rooms.push_back(Room());
	for (size_t i = 2; i < center.size(); i++)
	{
		m_Rooms.push_back(Room(center[i]));
	}

	//Generarate walls and shadows
	GenerateWalls(scene);
	GenerateRoomShadows(scene);
}

void World::PlaceGameObjects(Scene& scene) noexcept
{
	//Place objects in scene
	for (size_t i = 0; i < m_Objects.size(); i++)
	{
		const WorldObject& worldObject = m_Objects[i];
		int32 width = m_Levels[worldObject.TileId.y].GetSizeX();
		int32 height = m_Levels[worldObject.TileId.y].GetSizeZ();
		int floorLevel = worldObject.TileId.y / 2;

		GameObject* pGameObject = ResourceHandler::CreateGameObject(worldObject.GameObject);
		glm::uvec3 pos = worldObject.TileId;
		pos.x += 1;
		pos.z += 1;
		pGameObject->SetPosition(pos);
		pGameObject->SetRotation(glm::vec4(0, 1, 0, worldObject.Rotation));
		pGameObject->SetRoom(m_Levels[pos.y].GetLevel()[pos.x][pos.z]);
		pGameObject->UpdateTransform();
		
		scene.AddGameObject(pGameObject);
		m_Levels[pos.y].GetLevelData()[pos.x][pos.z].GameObjects.push_back(pGameObject);
	}
}

void World::PlaceDoors(Scene& scene) noexcept
{
	//Generate Door GameObjects
	for (uint32 i = 0; i < m_Doors.size(); i++)
	{
		const glm::vec3& door1 = m_Doors[i];
		WorldLevel& level = m_Levels[door1.y];
		
		float halfWidth = level.GetSizeX() / 2;
		float halfHeight = level.GetSizeZ() / 2;

		for (uint32 j = i + 1; j < m_Doors.size(); j++)
		{
			const glm::vec3& door2 = m_Doors[j];
			glm::vec3 delta = door1 - door2;

			if (glm::length(delta) <= 1.0)
			{
				glm::vec3 position = (door1 + door2) / 2.0F;

				GameObject* pGameObject = new GameObject();
				pGameObject->SetMaterial(MATERIAL::WHITE);
				pGameObject->SetMesh(MESH::DOOR_FRAME);
				pGameObject->SetPosition(position);
				pGameObject->SetRotation(glm::vec4(0, 1, 0, delta.z * glm::half_pi<float>()));
				pGameObject->UpdateTransform();
				scene.AddGameObject(pGameObject);

				pGameObject = new GameObjectDoor();
				pGameObject->SetPosition(position);
				pGameObject->SetRotation(glm::vec4(0, 1, 0, delta.z * glm::half_pi<float>()));
				pGameObject->UpdateTransform();
				scene.AddGameObject(pGameObject);

				level.GetLevelData()[(int32)door1.x][(int32)door1.z].GameObjects[GAMEOBJECT_CONST_INDEX_DOOR] = pGameObject;
				level.GetLevelData()[(int32)door2.x][(int32)door2.z].GameObjects[GAMEOBJECT_CONST_INDEX_DOOR] = pGameObject;

				break;
			}
		}
	}
}

void World::PlaceStairs(Scene& scene) noexcept
{
	//Generate Ladder GameObjects
	for (uint32 i = 0; i < m_Stairs.size(); i++)
	{
		const glm::ivec3& stair = m_Stairs[i];
		WorldLevel& level = m_Levels[stair.y];
		
		float halfWidth = level.GetSizeX() / 2;
		float halfHeight = level.GetSizeZ() / 2;

		glm::vec3 position = ((glm::vec3)stair);

		const uint32* const* grid = level.GetLevel();
		uint32 myId = grid[stair.x][stair.z];
		float rotation = 0;

		if (grid[stair.x + 1][stair.z] != myId)
		{
			rotation = glm::half_pi<float>() * 2.0F;
		}
		else if (grid[stair.x - 1][stair.z] != myId)
		{
			rotation = 0.0F;
		}
		else if (grid[stair.x][stair.z + 1] != myId)
		{
			rotation = glm::half_pi<float>();
		}
		else if (grid[stair.x][stair.z - 1] != myId)
		{
			rotation = glm::half_pi<float>() * 3.0F;
		}

		GameObject* pGameObject = new GameObject();
		pGameObject->SetMaterial(MATERIAL::WHITE);
		pGameObject->SetMesh(MESH::LADDER);
		pGameObject->SetPosition(position);
		pGameObject->SetRotation(glm::vec4(0, 1, 0, rotation));
		pGameObject->UpdateTransform();
		scene.AddGameObject(pGameObject);
	}
}

void World::GenerateRoomShadows(const Scene& scene) noexcept
{
	for (size_t i = 0; i < m_Rooms.size(); i++)
	{
		m_Rooms[i].GenerateShadows(scene);
	}
}

void World::Generate(Scene& scene) noexcept
{
	GenerateRooms(scene);
	PlaceGameObjects(scene);
	PlaceDoors(scene);
	PlaceStairs(scene);
	GenerateWalls(scene);
	GenerateRoomShadows(scene);
}

void World::GenerateWater(Scene& scene) noexcept
{
	for (int32 level = 0; level < m_Levels.size(); level += 2)
	{
		m_Levels[level].GenerateWater(scene, level);
	}
}

void World::SetStairs(const glm::ivec3* stairs, uint32 nrOfStairs)
{
	m_Stairs.resize(nrOfStairs);
	for (int i = 0; i < m_Stairs.size(); i++)
	{
		m_Stairs[i] = stairs[i];
		m_Levels[m_Stairs[i].y].GetLevelData()[m_Stairs[i].x][m_Stairs[i].z].HasStairs = true;
	}
}

void World::SetDoors(const glm::ivec3* doors, uint32 nrOfDoors)
{
	for (int i = 0; i < nrOfDoors; i++)
	{
		m_Doors.push_back(doors[i]);
		//m_ppLevels[doors[i].y]->GetLevelData()[doors[i].x][doors[i].z].GameObjects[GAMEOBJECT_CONST_INDEX_DOOR] = 
	}
}

const std::vector<glm::ivec3>& World::GetStairs() const noexcept
{
	return m_Stairs;
}

const std::vector<glm::ivec3>& World::GetDoors() const noexcept
{
	return m_Doors;
}

Room& World::GetRoom(uint32 room) noexcept
{
	assert(room < m_Rooms.size());
	return m_Rooms[room];
}

const Room& World::GetRoom(uint32 room) const noexcept
{
	assert(room < m_Rooms.size());
	return m_Rooms[room];
}

const std::vector<uint32>& World::GetActiveRooms() const noexcept
{
	return m_ActiveRooms;
}

const std::vector<float>& World::GetRoomLightTimers() const noexcept
{
	return m_RoomLightsTimers;
}

void World::PushRoomLightTimer(float timer)
{
	m_RoomLightsTimers.push_back(timer);
}

void World::RemoveRoomLightTimer(float timer)
{
	m_RoomLightsTimers.pop_back();
}

void World::PopRoomLightTimer()
{
	m_RoomLightsTimers.pop_back();
}

void World::PushActiveRoom(uint32 roomID)
{
	m_ActiveRooms.push_back(roomID);
}

void World::RemoveActiveRoom(uint32 roomID)
{
	m_ActiveRooms.pop_back();
}

void World::PopActiveRoom()
{
	m_ActiveRooms.pop_back();
}

const glm::ivec3& World::GetDoor(uint32 index) const noexcept
{
	assert(index < m_Doors.size());
	return m_Doors[index];
}

bool World::UpdateVisibility(Scene& pScene, float dt)
{
	bool result = false;
	for (uint32 i = 0; i < m_ActiveRooms.size(); i++)
	{
		m_RoomLightsTimers[i] += dt;
		if (m_RoomLightsTimers[i] >= 5.0f)
		{
			m_RoomLights[i]->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
			m_RoomLightsTimers[i] = 0.0f;
			m_Rooms[m_ActiveRooms[i]].SetActive(false);
			m_ActiveRooms[i] = 1;

			result = true;
		}
	}

	return result;
}

void World::GenerateWalls(Scene& scene) noexcept
{
	for (int32 level = 0; level < m_Levels.size(); level += 2)
	{
		glm::vec4 wall;

		for (int i = 0; i < m_Levels[i].GetNrOfWalls(); i++)
		{
			wall = m_Levels[i].GetWall(i);

			GameObject* pGameObject = new GameObject();
			pGameObject->SetMaterial(MATERIAL::WALL_STANDARD);
			pGameObject->SetMesh(MESH::CUBE);
			pGameObject->SetPosition(glm::vec3(wall.x, 1.0f + level, wall.y));
			pGameObject->SetScale(glm::vec3(wall.z + 0.1f, 2.0f, wall.w + 0.1f));
			pGameObject->UpdateTransform();

			scene.AddGameObject(pGameObject);
		}
	}
}
