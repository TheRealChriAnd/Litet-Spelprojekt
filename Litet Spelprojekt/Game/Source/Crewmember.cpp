#include "..\Include\Crewmember.h"

Crewmember::Crewmember(const glm::vec4 & lightColor, const glm::vec3 & position, const float & actionCap, const std::string & name) : m_pLight(new PointLight(position, lightColor)), m_pMesh(IndexedMesh::CreateCube())
{
	m_ActionCap = actionCap;
	m_Name = name;
	m_pMaterial = new Material();
	m_pMaterial->SetColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	m_nrOfPathTiles = 0;
	m_playerTile = glm::ivec3(std::round(position.x), std::round((position.y - 0.9) / 2),std::round(position.z));
	m_targetTile = m_playerTile;
	m_targetPos = glm::vec3(m_targetTile.x, m_targetTile.y * 2 + 0.9, m_targetTile.z);
	this->SetMaterial(m_pMaterial);
	this->SetMesh(m_pMesh);
	this->SetPosition(position);
	this->SetScale(glm::vec3(0.2, 1.8, 0.5));
	this->UpdateTransform();
}

Crewmember::Crewmember(Crewmember & other): m_pLight(new PointLight(other.GetPosition(), other.m_pLight->GetColor())), m_pMesh(other.m_pMesh)
{
	m_ActionCap = other.m_ActionCap;
	m_Name = other.m_Name;
	m_pMaterial = new Material(*other.m_pMaterial);
	m_nrOfPathTiles = 0;
	m_playerTile = glm::ivec3(std::round(other.GetPosition().x), std::round((other.GetPosition().y - 0.9) / 2),std::round(other.GetPosition().z));
	m_targetTile = m_playerTile;
	m_targetPos = glm::vec3(m_targetTile.x, m_targetTile.y * 2 + 0.9, m_targetTile.z);
	this->SetMaterial(m_pMaterial);
	this->SetMesh(m_pMesh);
	this->SetPosition(other.GetPosition());
	this->UpdateTransform();
}

Crewmember::~Crewmember()
{
	Delete(m_pMaterial);
	Delete(m_pMesh);
	Delete(m_pPathFinder);

}

void Crewmember::Move(const glm::vec3 & dir)
{
	glm::vec3 res = this->GetPosition() + dir;
	this->SetPosition(res);
}

PointLight * Crewmember::GetLight() const
{
	return m_pLight;
}

const float Crewmember::GetActionCapacity() const
{
	return m_ActionCap;
}

const bool Crewmember::IsMoving() const
{
	return (bool)m_nrOfPathTiles || m_pPathFinder->IsGoalSet();
}

void Crewmember::SetPosition(const glm::vec3 & position) noexcept
{
	m_playerTile = glm::ivec3(std::round(position.x), std::round((position.y - 0.9) / 2),std::round(position.z));
	m_targetTile = m_playerTile;
	m_targetPos = glm::vec3(m_targetTile.x, m_targetTile.y * 2 + 0.9, m_targetTile.z);
	GameObject::SetPosition(position);
}

void Crewmember::FindPath(const glm::ivec3& goalPos)
{
	if (!m_pPathFinder->IsGoalSet() && m_nrOfPathTiles == 0) {
		m_pPath = m_pPathFinder->FindPath(m_playerTile, goalPos);
		m_nrOfPathTiles = m_pPathFinder->GetNrOfPathTiles();
	}
}

void Crewmember::FollowPath(float dtS)
{
	if (m_nrOfPathTiles > 0) {
		if (m_playerTile == m_targetTile) {
			m_targetTile = m_pPath[--m_nrOfPathTiles];
			m_targetPos = glm::vec3(m_targetTile.x, m_targetTile.y * 2 + 0.9, m_targetTile.z);
		}
	}
	if (std::abs(this->GetPosition().x - m_targetPos.x) > 0.01 || std::abs(this->GetPosition().y - m_targetPos.y) > 0.01 || std::abs(this->GetPosition().z - m_targetPos.z) > 0.01) {
		glm::vec3 move(m_targetPos.x - this->GetPosition().x, m_targetPos.y - this->GetPosition().y, m_targetPos.z - this->GetPosition().z);
		move = glm::normalize(move);
		if (std::abs(move.y) > 0.01) {
			move.y /= std::abs(move.y);
			GameObject::SetPosition(this->GetPosition() + glm::vec3(0, move.y * dtS, 0));
		}
		else {
			GameObject::SetPosition(this->GetPosition() + glm::vec3(move.x * dtS, 0, move.z * dtS));
		}
		m_playerTile = glm::ivec3(std::round(this->GetPosition().x), std::round((this->GetPosition().y - 0.9) / 2), std::round(this->GetPosition().z));
	}
}

void Crewmember::SetActionCapacity(const float actionCap)
{
	m_ActionCap = actionCap;
}

void Crewmember::SetPath(const World * world)
{
	m_pPathFinder = new Path(world);
}

void Crewmember::UpdateTransform() noexcept
{
	GameObject::UpdateTransform();
	m_pLight->SetPosition(GetPosition());
}
