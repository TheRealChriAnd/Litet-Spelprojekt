#pragma once
#include <EnginePch.h>
#include <World/IScenario.h>

class API ScenarioManager
{
public:
	static uint32 RegisterScenario(IScenario* scenario) noexcept;
	static void Release() noexcept;
	static void Update(float dtS) noexcept;

private:
	static void SetAsNonActive(int id);

	static std::vector<IScenario*> m_Scenarios;
	static std::vector<int32> m_ActiveSenarios;
	static std::vector<int32> m_NonActiveSenarios;
};