#include <EnginePch.h>
#include "../../Include/Orders/OrderWalk.h"
#include <World/World.h>
#include <Graphics/Scene.h>
#include "../../Include/Crew.h"
#include "../../Include/Path.h"

OrderWalk::OrderWalk(glm::ivec3 goalTile):
	m_pPathFinder(nullptr),
	m_pPath(nullptr),
	m_IsPathReady(false)
{
	m_GoalTile = glm::ivec3(goalTile.x, goalTile.y / 2, goalTile.z);
}

OrderWalk::~OrderWalk()
{
}

void OrderWalk::StartOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept
{
	m_pPathFinder = new Path(pWorld);
	ThreadHandler::RequestExecution(this);
}

bool OrderWalk::UpdateOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers, float dtS) noexcept
{
	if (!m_IsPathReady)
	{
		return false;
	}
	return FollowPath(dtS);
}

void OrderWalk::EndOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept
{
	DeleteSafe(m_pPathFinder);
}

void OrderWalk::AbortOrder(Scene * pScene, World * pWorld, Crew * pCrewMembers) noexcept
{
	DeleteSafe(m_pPathFinder);
}

bool OrderWalk::AllowsMultipleOrders() noexcept
{
	return false;
}

std::string OrderWalk::GetName() noexcept
{
	return "Walk";
}

bool OrderWalk::ReadyToAbort() noexcept
{
	return m_IsPathReady;
}

void OrderWalk::RunParallel()
{
	m_pPath = m_pPathFinder->FindPath(GetCrewMember()->GetTile(), m_GoalTile);
	m_NrOfTilesLeft = m_pPathFinder->GetNrOfPathTiles();
	m_TargetTile = GetCrewMember()->GetTile();
	m_TargetPos = glm::vec3(m_TargetTile.x, m_TargetTile.y * 2 + 0.9, m_TargetTile.z);
	m_IsPathReady = true;
}

bool OrderWalk::FollowPath(float dtS) noexcept
{
	const glm::vec3& position = GetCrewMember()->GetPosition();
	if (m_NrOfTilesLeft > 0)
	{
		if (GetCrewMember()->GetTile() == m_TargetTile)
		{
			m_directionTile = m_pPath[m_NrOfTilesLeft - 1];
			m_TargetTile = m_pPath[--m_NrOfTilesLeft];
			m_TargetPos = glm::vec3(m_TargetTile.x, m_TargetTile.y * 2 + 0.9, m_TargetTile.z);
		}
	}
	else
	{
		return true;
	}

	if ((std::abs(position.x - m_TargetPos.x) > 0.01 || std::abs(position.y - m_TargetPos.y) > 0.01 || std::abs(position.z - m_TargetPos.z) > 0.01))
	{
		glm::vec3 move = m_TargetPos - position;
		move = glm::normalize(move);
		if (std::abs(move.y) > 0.01)
		{
			move.y /= std::abs(move.y);
			GetCrewMember()->SetDirection(glm::vec3(0, 0, 1));
			GetCrewMember()->SetPosition(position + glm::vec3(0, move.y * dtS, 0));
		}
		else
		{
			GetCrewMember()->SetDirection(glm::vec3(move.x, 0, move.z));
			GetCrewMember()->SetPosition(position + GetCrewMember()->GetDirection() * dtS);
		}
	}
	return false;
}