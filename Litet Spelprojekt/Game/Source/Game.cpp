#include "..\Include\Game.h"
#include "../Include/Scenes/SceneLoading.h"
#include "../Include/Scenarios/ScenarioManager.h"
#include "../Include/Scenarios/ScenarioFire.h"
#include "../Include/Scenarios/ScenarioWater.h"
#include "../Include/Scenarios/ScenarioMissile.h"
#include "../Include/Scenarios/ScenarioTorpedo.h"
#include <World/Scenarios/Fire/FireAlarm.h>
#include "../Include/GameObjectDoor.h"

#if defined(_DEBUG)
//#define DRAW_DEBUG_BOXES
#endif

float g_Rot = 1.0;

Game::Game() noexcept
	: Application(false, 1920, 1017, "", true),
	m_pSceneNext(nullptr),
	m_pScene(nullptr),
	m_pRenderer(nullptr),
	m_pSkyBoxTex(nullptr),
	m_pSceneLoading(nullptr),
	m_pSceneMenu(nullptr),
	m_pSceneCredits(nullptr),
	m_pSceneOptions(nullptr),
	m_pSceneScenario(nullptr),
	m_pSceneGame(nullptr)
{
	ResourceHandler::SetGameObjectCreator(this);

	m_pSceneLoading = new SceneLoading();

	m_pRenderer = new ForwardRenderer();

	m_pSkyBoxTex = TextureCube::CreateTextureCubeFromPanorama(ResourceHandler::GetTexture2D(TEXTURE::HDR));
	m_pSkyBox = new SkyBox(m_pSkyBoxTex);

	AudioListener::SetPosition(glm::vec3(0.0f));
	m_pAudioSourceMenu = AudioSource::CreateMusicSource(MUSIC::MENU);
	m_pAudioSourceMenu->SetPitch(1.0f);
	m_pAudioSourceMenu->SetLooping(true);
	m_pAudioSourceMenu->Play();

	m_ScenarioMissile	= ScenarioManager::RegisterScenario(new ScenarioMissile());
	m_ScenarioTorpedo	= ScenarioManager::RegisterScenario(new ScenarioTorpedo());
	m_ScenarioFire		= ScenarioManager::RegisterScenario(new ScenarioFire(false));
	m_ScenarioWater		= ScenarioManager::RegisterScenario(new ScenarioWater(true));

	SetScene(m_pSceneLoading);
}

Game::~Game()
{
	DeleteSafe(m_pSceneLoading);
	DeleteSafe(m_pSceneMenu);
	DeleteSafe(m_pSceneCredits);
	DeleteSafe(m_pSceneOptions);
	DeleteSafe(m_pSceneScenario);
	DeleteSafe(m_pSceneGame);

	DeleteSafe(m_pRenderer);
	DeleteSafe(m_pSkyBoxTex);
	DeleteSafe(m_pAudioSourceMenu);

	ScenarioManager::Release();
}

GameObject * Game::CreateGameObject(uint32 gameobject) noexcept
{
	if (gameobject == GAMEOBJECT::FIREALARM)
	{
		return new FireAlarm(SOUND::MONO_FIREALARM);
	}
	else if (gameobject == GAMEOBJECT::DOOR)
	{
		return new GameObjectDoor();
	}
	return new GameObject();
}

void Game::OnResourceLoading(const std::string& file, float percentage)
{
	if (m_pScene)
	{
		m_pScene->OnResourceLoading(file, percentage);
	}
}

void Game::OnResourcesLoaded()
{
#if defined(PRINT_CPU_DEBUG_DATA)
	CPUProfiler::Init();
#endif

	m_pSceneMenu = new SceneMenu();
	m_pSceneCredits = new SceneCredits();
	m_pSceneOptions = new SceneOptions();
	m_pSceneScenario = new SceneScenario();
	m_pSceneGame = new SceneGame();

	m_pScene->OnResourcesLoaded();
}

void Game::OnKeyUp(KEY keycode)
{
	if (m_pScene)
	{
		m_pScene->OnKeyUp(keycode);
	}
}

void Game::OnKeyDown(KEY keycode)
{
	if (m_pScene)
	{
		m_pScene->OnKeyDown(keycode);
	}
}

void Game::OnMouseMove(const glm::vec2& lastPosition, const glm::vec2& position)
{
	if (m_pScene)
	{
		m_pScene->OnMouseMove(lastPosition, position);
	}
}

void Game::OnMouseReleased(MouseButton mousebutton, const glm::vec2& position)
{
	bool clickedOnGUI = false;
	for (GUIObject* pObject : GetGUIManager().GetChildren())
	{
		if (pObject->OwnsPoint(position))
		{
			clickedOnGUI = true;
			break;
		}
	}

	if (!clickedOnGUI && m_pScene)
	{
		m_pScene->OnMouseReleased(mousebutton, position);
	}
}

void Game::OnMouseScroll(const glm::vec2& offset, const glm::vec2& position)
{
	bool clickedOnGUI = false;
	for (GUIObject* pObject : GetGUIManager().GetChildren())
	{
		if (pObject->OwnsPoint(position))
		{
			clickedOnGUI = true;
			break;
		}
	}

	if (!clickedOnGUI && m_pScene)
	{
		m_pScene->OnMouseScroll(offset, position);
	}
}

void Game::OnUpdate(float dtS)
{
	if (m_pSceneNext)
	{
		if (m_pScene)
		{
			m_pScene->SetSkyBox(nullptr);
			m_pScene->OnDeactivated(m_pSceneNext);
		}

		m_pSceneNext->OnActivated(m_pScene, m_pRenderer);
		m_pSceneNext->SetSkyBox(m_pSkyBox);

		m_pScene = m_pSceneNext;
		m_pSceneNext = nullptr;
	}

#if defined(PRINT_CPU_DEBUG_DATA)
	CPUProfiler::StartTimer(CPU_PROFILER_SLOT_1);
#endif
	if (m_pScene)
	{
		m_pScene->OnUpdate(dtS);
	}

#if defined(PRINT_CPU_DEBUG_DATA)
	CPUProfiler::EndTimer("Scene Update took %.3f ms", CPU_PROFILER_SLOT_1);
#endif

#if defined(PRINT_CPU_DEBUG_DATA)
	CPUProfiler::Update(dtS);
	CPUProfiler::PrintTime();
#endif
}

void Game::OnRender(float dtS)
{
	if (m_pSceneGame)
	{
		//m_pSceneGame->OnRender(dtS);
	}
	if (m_pScene)
	{
		m_pScene->OnRender(dtS);
	}
}

void Game::SetScene(SceneInternal* scene) noexcept
{
	m_pSceneNext = scene;

	if (m_pSceneNext == m_pSceneGame)
	{
		m_pAudioSourceMenu->Pause();
	}
	else
	{
		m_pAudioSourceMenu->Play();
	}
}

Game* Game::GetGame()
{
	return (Game*)(&Application::GetInstance());
}