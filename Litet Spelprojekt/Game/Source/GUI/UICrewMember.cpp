#include "..\..\Include\GUI\UICrewMember.h"
#include "..\..\Include/Game.h"

UICrewMember::UICrewMember(float x, float y, float width, float height) : Panel(x, y, width, height),
	m_Crewmember(nullptr)
{
	m_TextViewName = new TextView(0, height - 25, width, 25, "Name", true);
	m_TextViewName->SetBackgroundColor(glm::vec4(0.08F, 0.08F, 0.08F, 1.0F));

	Add(m_TextViewName);
	Add(new TextView(5, 85, 100, 40, "Skills"));
	Add(new TextView(5, 20, 100, 40, "Injuries"));

	SetDeleteAllChildrenOnDestruction(true);

	SetVisible(false);
}

UICrewMember::~UICrewMember()
{

}

void UICrewMember::SetCrewMember(const Crewmember* crewmember)
{
	if (m_Crewmember != crewmember)
	{
		m_Crewmember = crewmember;
		if(m_Crewmember)
		{
			m_TextViewName->SetText(crewmember->GetName());
			Game* game = Game::GetGame();
			Camera& camera = game->GetScene()->GetCamera();
			glm::vec4 pos = camera.GetCombinedMatrix() * glm::vec4(crewmember->GetPosition(), 1);
			pos = pos / pos.w;
			float halfWidth = game->GetWindow().GetWidth() / 2;
			float halfHeight = game->GetWindow().GetHeight() / 2;
			SetPosition(pos.x * halfWidth + halfWidth - GetWidth() / 2, pos.y * halfHeight + halfHeight + GetHeight() / 2);
		}
		SetVisible(m_Crewmember);
	}
}

void UICrewMember::OnRender(GUIContext* context)
{
	Panel::OnRender(context);

	if (m_Crewmember)
	{
		static int textureSize = 40;
		std::vector<int32> injuries;
		if (m_Crewmember->HasInjuryBoneBroken())
		{
			injuries.push_back(TEXTURE::ICON_INJURY_BONE);
		}
		if (m_Crewmember->HasInjuryBurned())
		{
			injuries.push_back(TEXTURE::ICON_INJURY_BURN);
		}
		if (m_Crewmember->HasInjurySmoke())
		{
			injuries.push_back(TEXTURE::ICON_INJURY_SMOKE);
		}

		for (int i = 0; i < injuries.size(); i++)
		{
			context->RenderTexture(ResourceHandler::GetTexture2D(injuries[i]), (i + 1) * 75, 20, textureSize, textureSize, GUIContext::COLOR_WHITE);
		}

		context->RenderTexture(ResourceHandler::GetTexture2D(TEXTURE::ICON_SKILL_FIRE), 75, 85, textureSize, textureSize, GUIContext::COLOR_WHITE);
		context->RenderTexture(ResourceHandler::GetTexture2D(TEXTURE::ICON_SKILL_MEDIC), 150, 85, textureSize, textureSize, GUIContext::COLOR_WHITE);
		context->RenderTexture(ResourceHandler::GetTexture2D(TEXTURE::ICON_SKILL_STRENGTH), 225, 85, textureSize, textureSize, GUIContext::COLOR_WHITE);
		context->RenderTexture(ResourceHandler::GetTexture2D(TEXTURE::ICON_CIRCLE), 65, 75, 20, 20, GUIContext::COLOR_WHITE);
		context->RenderTexture(ResourceHandler::GetTexture2D(TEXTURE::ICON_CIRCLE), 140, 75, 20, 20, GUIContext::COLOR_WHITE);
		context->RenderTexture(ResourceHandler::GetTexture2D(TEXTURE::ICON_CIRCLE), 215, 75, 20, 20, GUIContext::COLOR_WHITE);
		context->RenderText(std::to_string(m_Crewmember->GetSkillFire()), 75, 85, GetWidth(), GetHeight(), 0.75F, GUIContext::COLOR_BLACK, CENTER);
		context->RenderText(std::to_string(m_Crewmember->GetSkillMedic()), 150, 85, GetWidth(), GetHeight(), 0.75F, GUIContext::COLOR_BLACK, CENTER);
		context->RenderText(std::to_string(m_Crewmember->GetSkillStrength()), 225, 85, GetWidth(), GetHeight(), 0.75F, GUIContext::COLOR_BLACK, CENTER);
	}
}

void UICrewMember::PrintName() const
{
	std::cout << "UICrewMember" << std::endl;
}