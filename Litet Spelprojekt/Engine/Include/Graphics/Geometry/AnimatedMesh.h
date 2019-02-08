#pragma once
#include <EnginePch.h>

struct AnimatedVertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 tangent;
	glm::vec2 texCoords;
	glm::vec4 boneInfo1;
	glm::vec4 boneInfo2;

	inline bool operator==(const AnimatedVertex& rs) const
	{
		return (position == rs.position) && (normal == rs.normal) && (tangent == rs.tangent) && (texCoords == rs.texCoords) && (boneInfo1 == rs.boneInfo1) && (boneInfo2 == rs.boneInfo2);
	}
};

struct AnimatedInstanceData
{
	glm::mat4 Model;
	glm::mat4 InverseModel;
	glm::vec3 Direction;
};

class API AnimatedMesh
{
	friend class GLContext;
	friend class MESH;
	friend class ResourceHandler;

public:
	AnimatedMesh(AnimatedMesh&& other) = delete;
	AnimatedMesh(const AnimatedMesh& other) = delete;
	AnimatedMesh& operator=(AnimatedMesh&& other) = delete;
	AnimatedMesh& operator=(const AnimatedMesh& other) = delete;

	AnimatedMesh(const AnimatedVertex* const vertices, const uint32* const indices, uint32 numVertices, uint32 numIndices) noexcept;
	~AnimatedMesh();

	void SetInstances(const AnimatedInstanceData* const pInstances, uint32 numInstances) const noexcept;

	uint32 GetIndexCount() const noexcept;
	uint32 GetVertexCount() const noexcept;
	uint32 GetInstanceCount() const noexcept;

private:
	uint32 m_VAO;
	uint32 m_VBO;
	uint32 m_IBO;
	uint32 m_InstanceBuffer;
	uint32 m_VertexCount;
	uint32 m_IndexCount;
	mutable uint32 m_NumInstances;
	mutable uint32 m_NumReservedInstances;

private:
	static AnimatedMesh* CreateAnimatedMeshFromFile(const char* pFilename);

};
