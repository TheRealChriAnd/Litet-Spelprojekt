#include "..\..\Include\Scenes\SceneGame.h"
#include "../../Include/Game.h"
#include <World/LightManager.h>
#include <Graphics/Textures/StaticShadowCube.h>
#include "../../Include/Scenarios/ScenarioManager.h"
#include <World/GameObjectDoor.h>

SceneGame::SceneGame() : SceneInternal(false),
	m_pWorld(nullptr),
	m_pTestAudioSource(nullptr),
	cartesianCamera(false),
	m_CurrentElevation(2),
	m_pUIPause(nullptr),
	m_IsPaused(false)
{
	Game* game = Game::GetGame();
	Window* window = &game->GetWindow();

	CreateAudio();
	CreateGameObjects();
	CreateWorld();
	CreateCrew();

	LightManager::Init(this, 2);
	ScenarioManager::Init(m_pWorld);

	ResourceHandler::GetMaterial(MATERIAL::BOAT)->SetStencilTest(true, FUNC_ALWAYS, 0xff, 1, 0xff);
	ResourceHandler::GetMaterial(MATERIAL::BOAT)->SetFrontFaceStencilOp(STENCIL_OP_KEEP, STENCIL_OP_REPLACE, STENCIL_OP_ZERO);
	ResourceHandler::GetMaterial(MATERIAL::BOAT)->SetBackFaceStencilOp(STENCIL_OP_KEEP, STENCIL_OP_KEEP, STENCIL_OP_REPLACE);
	ResourceHandler::GetMaterial(MATERIAL::BOAT)->SetCullMode(CULL_MODE_NONE);

	ResourceHandler::GetMaterial(MATERIAL::WALL_STANDARD)->SetCullMode(CULL_MODE_NONE);

	GameObject* pGameObject;
	//BOB
	{
		pGameObject = new GameObject();
		pGameObject->SetMaterial(MATERIAL::ANIMATED_MODEL);
		pGameObject->SetAnimatedMesh(MESH::ANIMATED_MODEL);
		pGameObject->SetPosition(glm::vec3(6.0f, 1.0f, 6.0f));
		//pGameObject->SetRotation(glm::vec4(1.0f, 0.0f, 0.0f, glm::radians<float>(90.0f)));
		pGameObject->SetScale(glm::vec3(1.0f));
		pGameObject->UpdateTransform();
		AddGameObject(pGameObject);
	}

	//Lights
	{
		for (uint32 i = 0; i < MAX_ROOMS_VISIBLE; i++)
		{
			AddRoomLight(new PointLight(m_pWorld->GetRoom(0).GetCenter(), glm::vec4(2.0f, 2.0f, 2.0f, 2.0f)));
			m_pWorld->PushRoomLightTimer(0.0f);
			m_pWorld->PushActiveRoom(0);
		}
	}
}

SceneGame::~SceneGame()
{
	DeleteSafe(m_pWorld);
	DeleteSafe(m_pUICrew);
	DeleteSafe(m_pTestAudioSource);

	Logger::Save();
	LightManager::Release();
}

void SceneGame::OnActivated(SceneInternal* lastScene, IRenderer* m_pRenderer) noexcept
{
	SceneInternal::OnActivated(lastScene, m_pRenderer);

	Game* game = Game::GetGame();
	Window* window = &game->GetWindow();

	GetRenderer()->SetWorldBuffer(*this, m_pWorld);

	m_pUICrewMember = new UICrewMember(330, 170);
	m_pUILog = new UILog(window->GetWidth() - 350, window->GetHeight() - 450, 350, 450);
	
	game->GetGUIManager().Add(m_pUICrewMember);
	game->GetGUIManager().Add(m_pUILog);

	Logger::SetListener(m_pUILog);

	std::vector<Crewmember*> members;
	for (int i = 0; i < m_Crew.GetCount(); i++)
	{
		members.push_back(m_Crew.GetMember(i));
	}
	m_pUICrew = new UICrew(0, window->GetHeight() - 150, 200, 500, members);
}

void SceneGame::OnDeactivated(SceneInternal* newScene) noexcept
{
	SceneInternal::OnDeactivated(newScene);

	Game* game = Game::GetGame();
	game->GetGUIManager().DeleteChildren();

	DeleteSafe(m_pUICrew);
}

void SceneGame::OnUpdate(float dtS) noexcept
{
	if (m_IsPaused && !IsPaused())
	{
		Game* game = Game::GetGame();
		Window* window = &game->GetWindow();
		m_pUIPause = new UIPause((window->GetWidth() - 600) / 2, (window->GetHeight() - 600) / 2, 600, 600);
		game->GetGUIManager().Add(m_pUIPause);
	}
	else if (IsPaused() && !m_IsPaused)
	{
		Game* game = Game::GetGame();
		game->GetGUIManager().Remove(m_pUIPause);
		m_pUIPause = nullptr;
	}

	if (!IsPaused())
	{
		SceneInternal::OnUpdate(dtS);
		ScenarioManager::Update(dtS, m_pWorld, this);
		UpdateCamera(dtS);

		static float dist = 0.0f;
		dist += 0.02f * dtS;
		((WaterIndoorMaterial*)ResourceHandler::GetMaterial(MATERIAL::WATER_INDOOR))->SetDistortionFactor(dist);

#if defined(PRINT_CPU_DEBUG_DATA)
		CPUProfiler::StartTimer(CPU_PROFILER_SLOT_0);
#endif
		if (m_pWorld->UpdateVisibility(*this, dtS))
		{
			ScenarioManager::OnVisibilityChange(m_pWorld, this);
		}

#if defined(PRINT_CPU_DEBUG_DATA)
		CPUProfiler::EndTimer("World Update took %.3f ms", CPU_PROFILER_SLOT_0);
#endif

		GameObject* pCameraLookAt = GetGameObject("cameraLookAt");
		if (pCameraLookAt)
		{
			pCameraLookAt->SetPosition(GetCamera().GetLookAt());
		}

		AudioListener::SetPosition(GetCamera().GetPosition());
		AudioListener::SetOrientation(GetCamera().GetFront(), GetCamera().GetUp());

		if (Input::IsKeyPressed(KEY_NUMPAD_2))
		{
			if (m_CurrentElevation > 0)
			{
				m_CurrentElevation--;
			}
			std::cout << "Elevation: " << m_CurrentElevation << std::endl;
		}

		if (Input::IsKeyPressed(KEY_NUMPAD_8))
		{
			if (m_CurrentElevation < 2)
			{
				m_CurrentElevation++;
			}
			std::cout << "Elevation: " << m_CurrentElevation << std::endl;
		}
	}
}

void SceneGame::OnRender(float dtS) noexcept
{
	GetRenderer()->DrawScene(*this, m_pWorld, dtS);

#if defined(DRAW_DEBUG_BOXES)
	m_pDebugRenderer->DrawScene(*m_Scenes[m_SceneId]);
#endif
}

void SceneGame::OnMouseMove(const glm::vec2& lastPosition, const glm::vec2& position)
{
	if (!IsPaused())
	{
		if (Input::IsKeyDown(KEY_LEFT_ALT))
		{
			if (!cartesianCamera)
			{
				if (Input::IsButtonDown(MouseButton::MOUSE_BUTTON_LEFT))
				{
					const float cameraRotationSensitivity = 0.005f;
					glm::vec2 deltaPosition = cameraRotationSensitivity * (position - lastPosition);

					GetCamera().MoveRelativeLookAt(PosRelativeLookAt::RotateX, deltaPosition.x);
					GetCamera().MoveRelativeLookAt(PosRelativeLookAt::RotateY, -deltaPosition.y);

					m_pUICrewMember->SetCrewMember(nullptr);
				}

				if (Input::IsButtonDown(MouseButton::MOUSE_BUTTON_RIGHT))
				{
					const float cameraMoveSensitivityX = 0.5f;
					const float cameraMoveSensitivityY = 0.025f;
					glm::vec2 deltaPosition = cameraMoveSensitivityY * (position - lastPosition);
					glm::vec3 forward(0.0f);
					forward.x = GetCamera().GetFront().x;
					forward.z = GetCamera().GetFront().z;
					GetCamera().MoveWorldCoords(-forward * deltaPosition.y, true);
					GetCamera().MoveLocalCoords(glm::vec3(cameraMoveSensitivityX * deltaPosition.x, 0.0f, 0.0f), true);

					m_pUICrewMember->SetCrewMember(nullptr);
				}
			}
		}
		else
		{
			PickCrew(true);
		}
	}
}

void SceneGame::OnMouseScroll(const glm::vec2 & offset, const glm::vec2 & position)
{
	if (!IsPaused())
	{
		if (!cartesianCamera)
		{
			if (Input::IsKeyDown(KEY_LEFT_ALT))
			{
				if (offset.y > 0.0f)
				{
					GetCamera().MoveWorldCoords(glm::vec3(0.0f, 1.0f, 0.0f), true);
				}
				else
				{
					GetCamera().MoveWorldCoords(glm::vec3(0.0f, -1.0f, 0.0f), true);
				}
			}
			else
			{
				const float cameraZoomSensitivity = 0.1f;
				GetCamera().MoveRelativeLookAt(PosRelativeLookAt::Zoom, cameraZoomSensitivity * offset.y);
			}
		}
	}
}

void SceneGame::OnMousePressed(MouseButton mousebutton, const glm::vec2 & position)
{
}

void SceneGame::OnMouseReleased(MouseButton mousebutton, const glm::vec2 & position)
{
	if (!IsPaused())
	{
		switch (mousebutton)
		{
			case MOUSE_BUTTON_LEFT:
			{
				if (!Input::IsKeyDown(KEY_LEFT_ALT) && m_pWorld != nullptr)
				{
					PickPosition();
				}
				break;
			}
			case MOUSE_BUTTON_RIGHT:
			{
				if (!Input::IsKeyDown(KEY_LEFT_ALT) && m_pWorld != nullptr)
				{
					PickCrew(false);
				}
				break;
			}
		}
	}
}

void SceneGame::OnKeyUp(KEY keycode)
{
}

void SceneGame::OnKeyDown(KEY keycode)
{
	if (keycode == KEY_ESCAPE)
	{
		SetPaused(!IsPaused());
	}
	else if (!IsPaused())
	{
		switch (keycode)
		{
			case KEY_O:
			{
				cartesianCamera = !cartesianCamera;
				break;
			}
			case KEY_P:
			{
				m_pTestAudioSource->TogglePause();
				break;
			}
			case KEY_SPACE:
			{
				ExtendScene();
				break;
			}
			case KEY_L:
			{
				for (int i = 0; i < m_Crew.GetCount(); i++)
				{
					m_Crew.GetMember(i)->SwitchLight();
				}
				break;
			}
			case KEY_R:
			{
				ShowCrewmember(0);
				ScenarioManager::OnVisibilityChange(m_pWorld, this);
				break;
			}
		}
	}
}

void SceneGame::OnResize(uint32 width, uint32 height)
{

}

void SceneGame::CreateAudio() noexcept
{
	AudioListener::SetPosition(glm::vec3(0.0f));
	m_pTestAudioSource = AudioSource::CreateMusicSource(MUSIC::WAVES_AND_SEAGULLS);
	m_pTestAudioSource->SetPitch(1.0f);
	m_pTestAudioSource->SetLooping(true);
	//m_pTestAudioSource->Play();
}

void SceneGame::CreateGameObjects() noexcept
{
	GameObject* pGameObject = nullptr;
	{
		//Bottom floor
		{
			pGameObject = new GameObject();
			pGameObject->SetMaterial(MATERIAL::RED);
			pGameObject->SetMesh(MESH::CUBE_OBJ);
			pGameObject->SetPosition(glm::vec3(5.5f, 0.0f, 20.5f));
			pGameObject->SetScale(glm::vec3(10.0f, 0.1f, 40.0f));
			pGameObject->UpdateTransform();
			AddGameObject(pGameObject);
		}

		//Middle floor
		{
			pGameObject = new GameObject();
			pGameObject->SetMaterial(MATERIAL::GREEN);
			pGameObject->SetMesh(MESH::CUBE_OBJ);
			pGameObject->SetPosition(glm::vec3(5.5f, 2.0f, 20.5f));
			pGameObject->SetScale(glm::vec3(10.0f, 0.1f, 40.0f));
			pGameObject->UpdateTransform();
			AddGameObject(pGameObject);
		}

		//Top floor
		{
			pGameObject = new GameObject();
			pGameObject->SetMaterial(MATERIAL::BLUE);
			pGameObject->SetMesh(MESH::CUBE_OBJ);
			pGameObject->SetPosition(glm::vec3(5.5f, 4.0f, 20.5f));
			pGameObject->SetScale(glm::vec3(10.0f, 0.1f, 40.0f));
			pGameObject->UpdateTransform();
			AddGameObject(pGameObject);
		}
	}
}

void SceneGame::CreateWorld() noexcept
{
	GameObject* pGameObject = nullptr;

	m_pWorld = WorldSerializer::Read("world.json");

	// Generate rooms
	m_pWorld->Generate(*this);

	//Generate water
	m_pWorld->GenerateWater(*this);
}

void SceneGame::CreateCrew() noexcept
{
	//Crew
	std::string names[] = {
		"Granfeldt",
		"Ola",
		"Sven",
		"Gunnar",
		"Fysik-GW",
		"Robban",
		"Bengan",
		"Ragnar",
		"Klasse",
		"Gustafsson",
		"Nisse",
		"Per-Egon",
		"Knut",
		"Britt-Marie",
		"Bert Karlsson"
	};

	m_Crew.AddMember(m_pWorld, DEFAULT_LIGHT, glm::vec3(10.0f, 0.9f + 4.0f, 10.0f), 100, names[0]);
	//m_Scenes[0]->AddSpotLight(m_Crew.GetMember(i)->GetTorch());
	//m_Scenes[0]->AddPointLight(m_Crew.GetMember(i)->GetLight());
	m_Crew.GetMember(0)->SetPath();
	m_Crew.GetMember(0)->SetRoom(m_pWorld->GetLevel((int)4.0f).GetLevel()[(int)10.0f][(int)10.0f]);
	m_Crew.GetMember(0)->SetHidden(true);
	m_Crew.GetMember(0)->UpdateTransform();
	AddGameObject(m_Crew.GetMember(0));

	float x, y, z;
	for (int i = 1; i < NUM_CREW; i++)
	{
		y = (std::rand() % (m_pWorld->GetNumLevels() / 2)) * 2;
		x = std::rand() % (m_pWorld->GetLevel(y).GetSizeX() - 2) + 1;
		z = std::rand() % (m_pWorld->GetLevel(y).GetSizeZ() - 2) + 1;
		m_Crew.AddMember(m_pWorld, DEFAULT_LIGHT, glm::vec3(x, 0.9f + y, z), 100, names[i % 15]);
		//m_Scenes[0]->AddSpotLight(m_Crew.GetMember(i)->GetTorch());
		//m_Scenes[0]->AddPointLight(m_Crew.GetMember(i)->GetLight());
		m_Crew.GetMember(i)->SetPath();
		m_Crew.GetMember(i)->SetRoom(m_pWorld->GetLevel((int)y).GetLevel()[(int)x][(int)z]);
		m_Crew.GetMember(i)->SetHidden(true);
		m_Crew.GetMember(i)->UpdateTransform();
		AddGameObject(m_Crew.GetMember(i));
	}
}

void SceneGame::PickPosition()
{
	glm::vec3 rayDir = GetRay(Input::GetMousePosition(), Game::GetGame()->GetWindow().GetWidth(), Game::GetGame()->GetWindow().GetHeight());
	glm::vec3 rayOrigin = GetCamera().GetPosition();
	glm::vec3 pointOnSurface = glm::vec3(0.0f, 0.0f, 0.0f);

	glm::vec3 normal(0.0f, 1.0f, 0.0f);

	float t = -1, lastT = -1;
	for (int d = m_pWorld->GetNumLevels() - 2; d >= 0; d -= 2)
	{
		if (glm::dot(normal, rayDir) < -0.01)
		{
			t = (d - glm::dot(normal, rayOrigin)) / glm::dot(normal, rayDir);
		}

		if ((t >= 0 && lastT == -1) || (t > 0 && t < lastT))
		{
			pointOnSurface = rayOrigin + rayDir * t;

			float extension = GetExtension();
			if (pointOnSurface.x > extension * d / 2.0f)
			{
				pointOnSurface.x -= extension * floor(pointOnSurface.y / 2.0f);
				lastT = t;
			}
			else
			{
				pointOnSurface = glm::vec3(0.0f, 0.0f, 0.0f);
			}
		}
	}

	if (pointOnSurface != glm::vec3(0.0f, 0.0f, 0.0f))
	{
		for (int i = 0; i < m_Crew.GetCount(); i++)
		{
			if (m_Crew.GetMember(i)->GetLight()->GetColor() == CHOSEN_LIGHT || m_Crew.GetMember(i)->GetTorch()->GetColor() == CHOSEN_LIGHT)
			{
				m_Crew.GetMember(i)->FindPath(glm::round(pointOnSurface));
			}
		}
	}
}

void SceneGame::PickCrew(bool hover)
{
	Crewmember* crewmember = RayTestCrewmembers();

	if (crewmember)
	{
		if (hover)
		{
			if (!crewmember->IsHovered())
			{
				crewmember->OnHovered();
			}
		}
		else
		{
			crewmember->OnPicked();
		}
	}
	else if (hover)
	{
		for (int i = 0; i < m_Crew.GetCount(); i++)
		{
			if (m_Crew.GetMember(i)->IsHovered())
			{
				m_Crew.GetMember(i)->OnNotHovered();
			}
		}
	}
}

glm::vec3 SceneGame::GetRay(const glm::vec2 & mousepos, uint32 windowWidth, uint32 windowHeight)
{
	glm::vec4 rayDir4((2.0f * mousepos.x) / windowWidth - 1.0f, 1.0f - (2.0f * mousepos.y) / windowHeight, -1.0, 1.0);
	rayDir4 = GetCamera().GetInverseProjectionMatrix() * rayDir4;
	rayDir4 = glm::vec4(glm::vec2(rayDir4), -1.0, 0.0);
	rayDir4 = GetCamera().GetInverseViewMatrix() * rayDir4;

	glm::vec3 rayDir = glm::normalize(glm::vec3(rayDir4));

	return rayDir;
}

void SceneGame::ShowCrewmember(uint32 crewmember)
{
	glm::ivec3 tile = m_Crew.GetMember(crewmember)->GetTile();
	uint32 roomIndex = m_pWorld->GetLevel(tile.y * 2).GetLevel()[tile.x][tile.z];

	if (!m_pWorld->GetRoom(roomIndex).IsActive())
	{
		const glm::vec3& roomCenter = m_pWorld->GetRoom(roomIndex).GetCenter();
		std::vector<PointLight*>& roomLights = GetRoomLights();

		roomLights[m_CurrentLight]->SetPosition(roomCenter);
		m_RoomLightsTimers[m_CurrentLight] = 0.0f;
		m_ActiveRooms[m_CurrentLight] = roomIndex;

		m_pWorld->GetRoom(roomIndex).SetActive(true);
		m_CurrentLight = (m_CurrentLight + 1) % roomLights.size();
	}
}

Crewmember* SceneGame::RayTestCrewmembers()
{
	glm::vec3 rayDir = GetRay(Input::GetMousePosition(), Game::GetGame()->GetWindow().GetWidth(), Game::GetGame()->GetWindow().GetHeight());
	glm::vec3 rayOrigin = GetCamera().GetPosition();

	float lastT = -1;
	uint32 id = -1;

	for (int i = 0; i < m_Crew.GetCount(); i++)
	{
		int32 t = m_Crew.GetMember(i)->TestAgainstRay(rayDir, rayOrigin, GetExtension());

		if (t > 0 && lastT == -1 || t >= 0 && t < lastT)
		{
			lastT = t;
			id = i;
		}
	}

	if (id != -1)
	{
		return m_Crew.GetMember(id);
	}
	return nullptr;
}

Crewmember* SceneGame::GetCrewmember(uint32 shipNumber)
{
	return m_Crew.GetMember(shipNumber);
}

UICrewMember* SceneGame::GetUICrewMember() noexcept
{
	return m_pUICrewMember;
}

void SceneGame::UpdateCamera(float dtS) noexcept
{
	float cartesianCameraSpeed = 5.0F;
	float cartesianCameraAngularSpeed = 1.5F;

	if (cartesianCamera)
	{
		glm::vec3 localMove(0.0f);

		if (Input::IsKeyDown(KEY_W))
		{
			localMove.z = cartesianCameraSpeed * dtS;
			m_pUICrewMember->SetCrewMember(nullptr);
		}
		else if (Input::IsKeyDown(KEY_S))
		{
			localMove.z = -cartesianCameraSpeed * dtS;
			m_pUICrewMember->SetCrewMember(nullptr);
		}

		if (Input::IsKeyDown(KEY_A))
		{
			localMove.x = cartesianCameraSpeed * dtS;
			m_pUICrewMember->SetCrewMember(nullptr);
		}
		else if (Input::IsKeyDown(KEY_D))
		{
			localMove.x = -cartesianCameraSpeed * dtS;
			m_pUICrewMember->SetCrewMember(nullptr);
		}

		if (Input::IsKeyDown(KEY_E))
		{
			localMove.y = cartesianCameraSpeed * dtS;
			m_pUICrewMember->SetCrewMember(nullptr);
		}
		else if (Input::IsKeyDown(KEY_Q))
		{
			localMove.y = -cartesianCameraSpeed * dtS;
			m_pUICrewMember->SetCrewMember(nullptr);
		}

		GetCamera().MoveLocalCoords(localMove);

		if (Input::IsKeyDown(KEY_UP))
		{
			m_pUICrewMember->SetCrewMember(nullptr);
			GetCamera().OffsetPitch(cartesianCameraAngularSpeed * dtS);
		}
		else if (Input::IsKeyDown(KEY_DOWN))
		{
			GetCamera().OffsetPitch(-cartesianCameraAngularSpeed * dtS);
			m_pUICrewMember->SetCrewMember(nullptr);
		}

		if (Input::IsKeyDown(KEY_LEFT))
		{
			GetCamera().OffsetYaw(-cartesianCameraAngularSpeed * dtS);
			m_pUICrewMember->SetCrewMember(nullptr);
		}
		else if (Input::IsKeyDown(KEY_RIGHT))
		{
			GetCamera().OffsetYaw(cartesianCameraAngularSpeed * dtS);
			m_pUICrewMember->SetCrewMember(nullptr);
		}

		GetCamera().UpdateFromPitchYaw();
	}
	else
	{
		GetCamera().UpdateFromLookAt();
	}
}

void SceneGame::SetPaused(bool paused) noexcept
{
	m_IsPaused = paused;
}

bool SceneGame::IsPaused() const noexcept
{
	return m_pUIPause;
}
