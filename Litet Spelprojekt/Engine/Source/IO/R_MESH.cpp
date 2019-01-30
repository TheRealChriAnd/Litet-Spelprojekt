#include <EnginePch.h>
#include <IO/MESH.h>
#include <IO/ResourceHandler.h>

uint32 MESH::QUAD = 0;
uint32 MESH::SHIP = 0;
uint32 MESH::CLIFF_3_LOW = 0;
uint32 MESH::CUBE = 0;
uint32 MESH::CUBE_OBJ = 0;

void MESH::RegisterResources()
{
	QUAD					= ResourceHandler::RegisterMesh(IndexedMesh::CreateQuad());
	CUBE					= ResourceHandler::RegisterMesh(IndexedMesh::CreateCube());
	SHIP					= ResourceHandler::RegisterMesh("ship.obj");
	CLIFF_3_LOW				= ResourceHandler::RegisterMesh("cliff_3_low.obj");
	CUBE_OBJ				= ResourceHandler::RegisterMesh("cube.obj");
}