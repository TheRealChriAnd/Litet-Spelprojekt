#pragma once
#include <EnginePch.h>
#include <IO/IResource.h>
#include "MeshBase.h"

class API IndexedMesh : public IResource
{
	friend class GLContext;
	friend class MESH;
	friend class ResourceHandler;

public:
	void SetInstances(const InstanceData* const pInstances, uint32 numInstances) const noexcept;

	uint32 GetIndexCount() const noexcept;
	uint32 GetVertexCount() const noexcept;
	uint32 GetInstanceCount() const noexcept;

protected:
	IndexedMesh(IndexedMesh&& other) = delete;
	IndexedMesh(const IndexedMesh& other) = delete;
	IndexedMesh& operator=(IndexedMesh&& other) = delete;
	IndexedMesh& operator=(const IndexedMesh& other) = delete;

	IndexedMesh(const void* const vertices, const uint32* const indices, uint32 numVertices, uint32 numIndices) noexcept;
	virtual ~IndexedMesh();

	virtual void Construct() override;

protected:
	uint32 m_VAO;
	uint32 m_VBO;
	uint32 m_IBO;
	uint32 m_InstanceBuffer;
	uint32 m_VertexCount;
	uint32 m_IndexCount;
	void* m_Vertices;
	uint32* m_Indices;
	mutable uint32 m_NumInstances;
	mutable uint32 m_NumReservedInstances;

private:
	static IndexedMesh* CreateIndexedMeshFromFile(const char* pFilename);
	static IndexedMesh* CreateCube();
	static IndexedMesh* CreateCubeInvNormals();
	static IndexedMesh* CreateQuad();
};

inline uint32 IndexedMesh::GetIndexCount() const noexcept
{
	return m_IndexCount;
}

inline uint32 IndexedMesh::GetVertexCount() const noexcept
{
	return m_VertexCount;
}

inline uint32 IndexedMesh::GetInstanceCount() const noexcept
{
	return m_NumInstances;
}