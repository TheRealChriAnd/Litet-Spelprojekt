﻿#include "..\..\Include\Scenes\SceneCredits.h"
#include "..\..\Include\Scenes\SceneMenu.h"
#include "../../Include/Game.h"
#include <Audio/Sources/AudioSource.h>

SceneCredits::SceneCredits()
{
	m_pAudioSourceSelect = AudioSource::CreateSoundSource(SOUND::UI_SELECT);
	m_pAudioSourceSelect->SetRollOffFactor(10.0f);
	m_pAudioSourceSelect->SetReferenceDistance(0.0f);
	m_pAudioSourceSelect->SetMaxDistance(500.0f);
	m_pAudioSourceSelect->SetLooping(false);

	m_pAudioSourceHover = AudioSource::CreateSoundSource(SOUND::UI_HOVER);
	m_pAudioSourceHover->SetRollOffFactor(10.0f);
	m_pAudioSourceHover->SetReferenceDistance(0.0f);
	m_pAudioSourceHover->SetMaxDistance(500.0f);
	m_pAudioSourceHover->SetLooping(false);
}

SceneCredits::~SceneCredits()
{
	DeleteSafe(m_pAudioSourceSelect);
	DeleteSafe(m_pAudioSourceHover);
}

void SceneCredits::OnActivated(SceneInternal* lastScene, IRenderer* pRenderer) noexcept
{
	SceneInternal::OnActivated(lastScene, pRenderer);

	Game* game = Game::GetGame();
	Window* window = &game->GetWindow();

	m_pPanel = new Panel((window->GetWidth() - 600.0f) / 2.0f, (window->GetHeight() - 600.0f) / 2.0f, 600.0f, 600.0f);
	m_pButtonBack = new Button(0.0f, m_pPanel->GetHeight() - 50.0f, 100.0f, 50.0f, "Tillbaka");
	m_pTextViewTitle = new TextView(0.0f, m_pPanel->GetHeight() - 50.0f, m_pPanel->GetWidth(), 50.0f, "Utvecklare", true);

	m_pTextViewChristoffer = new TextView(0.0f, m_pTextViewTitle->GetY() - 150.0f, m_pPanel->GetWidth(), m_pButtonBack->GetHeight(), "Christoffer Andersson", true);
	m_pTextViewHerman = new TextView(0.0f, m_pTextViewChristoffer->GetY() - 50.0f, m_pPanel->GetWidth(), m_pButtonBack->GetHeight(), "Herman Hansson Söderlund", true);
	m_pTextViewAlexander = new TextView(0.0f, m_pTextViewHerman->GetY() - 50.0f, m_pPanel->GetWidth(), m_pButtonBack->GetHeight(), "Alexander Dahlin", true);
	m_pTextViewTim = new TextView(0.0f, m_pTextViewAlexander->GetY() - 50.0f, m_pPanel->GetWidth(), m_pButtonBack->GetHeight(), "Tim Mellander", true);
	m_pTextViewTimKort = new TextView(0.0f, m_pTextViewTim->GetY() - 50.0f, m_pPanel->GetWidth(), m_pButtonBack->GetHeight(), "Tim Johansson", true);
	m_pTextViewLeo = new TextView(0.0f, m_pTextViewTimKort->GetY() - 50.0f, m_pPanel->GetWidth(), m_pButtonBack->GetHeight(), "Leo Wikström", true);

	m_pButtonBack->SetBackgroundColor(GUIContext::COLOR_TRANSPARENT);
	m_pButtonBack->AddButtonListener(this);

	m_pPanel->SetBorderColor(GUIContext::COLOR_BLACK);
	m_pPanel->SetBoderThickness(3);

	m_pPanel->Add(m_pButtonBack);
	m_pPanel->Add(m_pTextViewTitle);
	m_pPanel->Add(m_pTextViewChristoffer);
	m_pPanel->Add(m_pTextViewHerman);
	m_pPanel->Add(m_pTextViewAlexander);
	m_pPanel->Add(m_pTextViewTim);
	m_pPanel->Add(m_pTextViewTimKort);
	m_pPanel->Add(m_pTextViewLeo);

	game->GetGUIManager().Add(m_pPanel);
}

void SceneCredits::OnUpdate(float dtS) noexcept
{
	SceneInternal::OnUpdate(dtS);
}

void SceneCredits::OnRender(float dtS) noexcept
{
	SceneInternal::OnRender(dtS);
}

void SceneCredits::OnButtonPressed(Button* button)
{
	m_pAudioSourceSelect->Play();
}

void SceneCredits::OnButtonReleased(Button* button)
{
	Game::GetGame()->SetScene(Game::GetGame()->m_pSceneMenu);
}

void SceneCredits::OnButtonHovered(Button* button)
{
	m_pAudioSourceHover->Play();
}

void SceneCredits::OnButtonNotHovered(Button* button)
{
}