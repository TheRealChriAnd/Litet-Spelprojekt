#include <EnginePch.h>
#include <IO/MESH.h>
#include <IO/ResourceHandler.h>
#include <Graphics/Geometry/IndexedMesh.h>
#include <Graphics/Geometry/MeshParticle.h>
#include <Graphics/Geometry/WaterQuad.h>

uint32 MESH::QUAD = 0;
uint32 MESH::SHIP = 0;
uint32 MESH::CLIFF_3_LOW = 0;
uint32 MESH::CUBE = 0;
uint32 MESH::CUBE_OBJ = 0;
uint32 MESH::CHAIR = 0;
uint32 MESH::SPHERE = 0;
uint32 MESH::BED_BUNK = 0;
uint32 MESH::BED_SINGLE = 0;
uint32 MESH::INSTRUMENT_1 = 0;
uint32 MESH::INSTRUMENT_2 = 0;
uint32 MESH::LAMP = 0;
uint32 MESH::ANIMATED_MODEL_RUN = 0;
uint32 MESH::ANIMATED_MODEL_IDLE = 0;
uint32 MESH::ANIMATED_MODEL_OPENDOOR = 0;
uint32 MESH::ANIMATED_MODEL_SITTING = 0;
uint32 MESH::ANIMATED_MODEL_SLEEP = 0;
uint32 MESH::DOOR = 0;
uint32 MESH::DOOR_FRAME = 0;
uint32 MESH::LADDER = 0;
uint32 MESH::MESH_PARTICLE = 0;
uint32 MESH::WATER_QUAD = 0;
uint32 MESH::CUBOARD = 0;
uint32 MESH::TABLE = 0;
uint32 MESH::TOILET = 0;
uint32 MESH::MISSILE = 0;


/*
* Used for preloading resources needed in the loading screen
*/
void MESH::RegisterResourcesPreLoading()
{
	QUAD					= ResourceHandler::RegisterMesh(IndexedMesh::CreateQuad());
	CUBE					= ResourceHandler::RegisterMesh(IndexedMesh::CreateCube());
	WATER_QUAD				= ResourceHandler::RegisterMesh(WaterQuad::CreateWaterQuad(glm::vec2(21.0f, 6.0f), glm::vec2(40.0f, 10.0f), 2.0, 100));
	SHIP					= ResourceHandler::RegisterMesh("ship1.obj");
}

void MESH::RegisterResources()
{
	CLIFF_3_LOW				= ResourceHandler::RegisterMesh("cliff_3_low.obj");
	CUBE_OBJ				= ResourceHandler::RegisterMesh("cube.obj");
	SPHERE					= ResourceHandler::RegisterMesh("sphere.obj");
	CHAIR					= ResourceHandler::RegisterMesh("Chair.obj");
	BED_BUNK				= ResourceHandler::RegisterMesh("BunkBed.obj");
	BED_SINGLE				= ResourceHandler::RegisterMesh("SingleBed1.obj");
	INSTRUMENT_1			= ResourceHandler::RegisterMesh("Instrument1.obj");
	INSTRUMENT_2			= ResourceHandler::RegisterMesh("Instrument2.obj");
	LAMP					= ResourceHandler::RegisterMesh("Lamp.obj");
	MESH_PARTICLE			= ResourceHandler::RegisterMeshParticle(MeshParticle::CreateCube());
	ANIMATED_MODEL_RUN		= ResourceHandler::RegisterAnimatedMesh("CrewMemberRunning10.fbx");
	ANIMATED_MODEL_IDLE		= ResourceHandler::RegisterAnimatedMesh("CrewMemberIdle3.fbx");
	ANIMATED_MODEL_OPENDOOR = ResourceHandler::RegisterAnimatedMesh("CrewMemberOpenDoor1.fbx");
	ANIMATED_MODEL_SLEEP	= ResourceHandler::RegisterAnimatedMesh("CrewMemberSleeping1.fbx");
	ANIMATED_MODEL_SITTING	= ResourceHandler::RegisterAnimatedMesh("CrewMemberSitting2.fbx");
	DOOR					= ResourceHandler::RegisterMesh("Door.obj");
	DOOR_FRAME				= ResourceHandler::RegisterMesh("DoorFrame.obj");
	LADDER					= ResourceHandler::RegisterMesh("Ladder.obj");
	CUBOARD					= ResourceHandler::RegisterMesh("Cuboard.obj");
	TABLE					= ResourceHandler::RegisterMesh("Table.obj");
	TOILET					= ResourceHandler::RegisterMesh("Toilet.obj");
	MISSILE					= ResourceHandler::RegisterMesh("Missile.obj");
}