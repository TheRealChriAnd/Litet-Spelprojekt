#pragma once
#include "OrderWalk.h"

class GameObject;

class OrderEat : public OrderWalk
{
public:
	OrderEat(const glm::ivec3& chairTile, GameObject* pChair);
	virtual ~OrderEat();

	virtual void OnStarted(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept override;
	virtual void OnEnded(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept override;
	virtual bool OnUpdate(Scene* pScene, World* pWorld, Crew* pCrewMembers, float dtS) noexcept override;
	virtual bool CanBeStackedWithSameType() noexcept override;
	virtual std::string GetName() noexcept override;
	virtual bool IsIdleOrder() noexcept;

private:
	GameObject* m_pChair;
	glm::vec3 m_Position;
	float m_Timer;
	bool m_IsAtChair;
};