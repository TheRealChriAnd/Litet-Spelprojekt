#include "../../Include/Orders/OrderToilet.h"
#include "../../Include/Crewmember.h"

OrderToilet::OrderToilet(const glm::ivec3& toiletTile, GameObject* pToilet)
	: OrderWalk(toiletTile)
{
	m_pToilet = pToilet;
	m_Position = glm::vec3(0.0f);
	m_Timer = 15.0f;
	m_IsAtToilet = false;
}

OrderToilet::~OrderToilet()
{
}

void OrderToilet::StartOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept
{
	OrderWalk::StartOrder(pScene, pWorld, pCrewMembers);
}

bool OrderToilet::UpdateOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers, float dtS) noexcept
{
	if (!m_IsAtToilet)
	{
		if (OrderWalk::UpdateOrder(pScene, pWorld, pCrewMembers, dtS))
		{
			m_IsAtToilet = true;
			m_Position = GetCrewMember()->GetPosition();

			GetCrewMember()->UpdateAnimatedMesh(MESH::ANIMATED_MODEL_SITTING);

			float yaw = m_pToilet->GetRotation().w;
			while (yaw > glm::two_pi<float>())
			{
				yaw -= glm::two_pi<float>();
			}
			while (yaw < 0)
			{
				yaw += glm::two_pi<float>();
			}
			yaw = fmod(yaw + glm::quarter_pi<float>(), glm::two_pi<float>());
			int rot = yaw / glm::half_pi<float>();
			
			if (rot == 0)
			{
				GetCrewMember()->SetPosition(m_Position + glm::vec3(-0.3f, 0.0f, 0.0f));
				GetCrewMember()->SetRotation(glm::vec4(0.0f, 1.0f, 0.0f, glm::radians<float>(270.0f)));
			}
			else if (rot == 1)
			{
				GetCrewMember()->SetPosition(m_Position + glm::vec3(0.0f, 0.0f, 0.3f));
				GetCrewMember()->SetRotation(glm::vec4(0.0f, 1.0f, 0.0f, glm::radians<float>(0.0f)));
			}
			else if (rot == 2)
			{
				GetCrewMember()->SetPosition(m_Position + glm::vec3(0.3f, 0.0f, 0.0f));
				GetCrewMember()->SetRotation(glm::vec4(0.0f, 1.0f, 0.0f, glm::radians<float>(90.0f)));
			}
			else if (rot == 3)
			{
				GetCrewMember()->SetPosition(m_Position + glm::vec3(0.0f, 0.0f, -0.3f));
				GetCrewMember()->SetRotation(glm::vec4(0.0f, 1.0f, 0.0f, glm::radians<float>(180.0f)));
			}
		}
	}
	else
	{
		m_Timer -= dtS;
		return m_Timer <= 0.0f;
	}

	return false;
}

void OrderToilet::EndOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept
{
	GetCrewMember()->UpdateAnimatedMesh(MESH::ANIMATED_MODEL_IDLE);
	GetCrewMember()->SetPosition(m_Position);
	GetCrewMember()->SetRotation(glm::vec4(1.0f, 0.0f, 0.0f, glm::radians<float>(0.0f)));

	OrderWalk::EndOrder(pScene, pWorld, pCrewMembers);
}

void OrderToilet::AbortOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept
{
	GetCrewMember()->UpdateAnimatedMesh(MESH::ANIMATED_MODEL_IDLE);
	GetCrewMember()->SetPosition(m_Position);
	GetCrewMember()->SetRotation(glm::vec4(1.0f, 0.0f, 0.0f, glm::radians<float>(0.0f)));

	OrderWalk::AbortOrder(pScene, pWorld, pCrewMembers);
}

bool OrderToilet::AllowsMultipleOrders() noexcept
{
	return false;
}

std::string OrderToilet::GetName() noexcept
{
	return "Order Toilet";
}

bool OrderToilet::ReadyToAbort() noexcept
{
	return OrderWalk::ReadyToAbort();
}

bool OrderToilet::IsIdleOrder() noexcept
{
	return true;
}