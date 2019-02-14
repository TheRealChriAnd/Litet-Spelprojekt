#ifndef CREWMEMBER_H
#define CREWMEMBER_H
#include <Graphics/Lights/PointLight.h>
#include <Graphics/Lights/SpotLight.h>
#include <Graphics/GameObject.h>
#include <System/ThreadHandler.h>
#include <string>
#include "..\Include\Path.h"

#define CHOSEN_LIGHT glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
#define DEFAULT_LIGHT glm::vec4(0.1f, 0.1f, 0.1f, 1.0f)

class Crewmember : public GameObject, public IRunnable
{
	friend class Crew;

public:
	Crewmember(const glm::vec4& lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), const glm::vec3& position = glm::vec3(0.0f, 0.0f, 0.0f), float actionCap = 100, const std::string& name = "");
	Crewmember(Crewmember& other);
	~Crewmember();
	
	///<summary>Moves the objects position in the given direction, use update to apply.</summary>
	void Move(const glm::vec3 & dir);
	///<summary>Finds a path to the goal position.</summary>
	void FindPath(const glm::ivec3& goalPos);
	///<summary>Moves along the path towards the goal position.</summary>
	void FollowPath(float dtS);
	///<summary>Sets the actioncapacity of the crewmember to the specified value.</summary>
	void SetActionCapacity(float actionCap);
	///<summary>Sets a grid for the path finding algorithm.</summary>
	void SetPath(const World* world);
	///<summary>Returns a reference to the PointLight object.</summary>
	PointLight* GetLight() const;
	///<summary>Returns a reference to the SpotLight object.</summary>
	SpotLight* GetTorch() const;
	///<summary>returns the current action capacity of the crewmember.</summary>
	const float GetActionCapacity() const;
	///<summary>Returns true if the object is currently moving. Otherwise returns false.</summary>
	const bool IsMoving() const;
	///<summary>Sets a new position for the object.</summary>
	void SetPosition(const glm::vec3& position) noexcept;
	///<summary>Gets the current direction that the crewmember is facing.</summary>
	const glm::vec3& GetDirection() const noexcept;
	///<summary>Sets the current direction that the crewmember is facing. Use .Update() to apply visual changes.</summary>
	void SetDirection(const glm::vec3& direction) noexcept;
	///<summary>>Switches between point light and spotlight for the crewmember.</summary>
	void SwitchLight() noexcept;
	glm::ivec3 GetTile() const noexcept;
	///<summary>Used to determine path of crewmember to an already selected target. Function made to run on multiple threads.</summary>
	virtual void RunParallel() override;
	///<summary>Updates matrix of the object.</summary>
	virtual void Update(float deltaTime) override;

	virtual void OnPicked();
	virtual void OnHovered();
	virtual void OnNotHovered();
	int32 TestAgainstRay(const glm::vec3 ray, const glm::vec3 origin) noexcept;

	int32 GetShipNumber() const noexcept;

	bool IsHovered() const noexcept;

	int8 GetSkillFire() const noexcept;
	int8 GetSkillMedic() const noexcept;
	int8 GetSkillStrength() const noexcept;
	bool HasInjuryBoneBroken() const noexcept;
	bool HasInjuryBurned() const noexcept;
	bool HasInjurySmoke() const noexcept;

private:
	void SetShipNumber(int32 shipnumber) noexcept;

	float m_ActionCap;
	float m_DeltaTime;
	SpotLight* m_pTorch;
	PointLight* m_pLight;
	Path* m_pPathFinder;
	glm::ivec3 m_PlayerTile;
	glm::ivec3 m_TargetTile;
	glm::ivec3 m_GoalTile;
	glm::ivec3* m_pPath;
	glm::vec3 m_TargetPos;
	glm::vec3 m_Direction;
	int m_NrOfPathTiles;
	bool m_IsHovered;
	int32 m_ShipNumber;

	int8 m_SkillFire;
	int8 m_SkillMedic;
	int8 m_SkillStrength;
	bool m_HasInjuryBoneBroken;
	bool m_HasInjuryBurned;
	bool m_HasInjurySmoke;
};

#endif
