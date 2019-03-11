#pragma once
#include "OrderWalk.h"

class OrderWalkMedicBay : public OrderWalk
{
public:
	OrderWalkMedicBay(World* pWorld);
	virtual ~OrderWalkMedicBay() override;

/*	virtual void StartOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept override;*/
	virtual bool OnUpdate(Scene* pScene, World* pWorld, Crew* pCrewMembers, float dtS) noexcept override;
	/*virtual void OnEnded(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept = 0;
	virtual void AbortOrder(Scene* pScene, World* pWorld, Crew* pCrewMembers) noexcept override;
	virtual bool AllowsMultipleOrders() noexcept override;*/
	virtual std::string GetName() noexcept override;
	/*virtual bool ReadyToAbort() noexcept override;
	virtual void RunParallel() override;*/
};