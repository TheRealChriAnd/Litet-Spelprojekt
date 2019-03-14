#include <EnginePch.h>
#include <IO/TEXTURE.h>
#include <IO/ResourceHandler.h>
#include <Graphics/Textures/Texture2D.h>

uint32 TEXTURE::BLOOD = 0;
uint32 TEXTURE::BLOOD_NORMAL = 0;
uint32 TEXTURE::SHIP = 0;
uint32 TEXTURE::SHIP_NORMAL = 0;
uint32 TEXTURE::HDR = 0;
uint32 TEXTURE::SINGLE_BED = 0;
uint32 TEXTURE::BUNK_BED = 0;
uint32 TEXTURE::CUPBOARD = 0;
uint32 TEXTURE::TABLE = 0;
uint32 TEXTURE::TOILET = 0;
uint32 TEXTURE::INSTRUMENT_1 = 0;
uint32 TEXTURE::INSTRUMENT_2 = 0;
uint32 TEXTURE::ICON_INJURY_BONE = 0;
uint32 TEXTURE::ICON_INJURY_BURN = 0;
uint32 TEXTURE::ICON_INJURY_SMOKE = 0;
uint32 TEXTURE::ICON_INJURY_BLEED = 0;
uint32 TEXTURE::ICON_SKILL_FIRE = 0;
uint32 TEXTURE::ICON_SKILL_MEDIC = 0;
uint32 TEXTURE::ICON_SKILL_STRENGTH = 0;
uint32 TEXTURE::ICON_CIRCLE = 0;
uint32 TEXTURE::ICON_CHECKED = 0;
uint32 TEXTURE::ICON_CROSSED = 0;
uint32 TEXTURE::ICON_RED_DOOR = 0;
uint32 TEXTURE::ICON_YELLOW_DOOR = 0;
uint32 TEXTURE::ICON_BLUE_DOOR = 0;
uint32 TEXTURE::ICON_GREEN_DOOR = 0;
uint32 TEXTURE::ICON_NOTIFICATION_TRIANGLE = 0;
uint32 TEXTURE::SMOKE = 0;
uint32 TEXTURE::SJOFAN_DIFF = 0;
uint32 TEXTURE::WALL1 = 0;
uint32 TEXTURE::OVEN = 0;
uint32 TEXTURE::FLOOR_NORMAL = 0;
uint32 TEXTURE::FLOOR_SICKBAY1 = 0;
uint32 TEXTURE::FLOOR_TOILET1 = 0;
uint32 TEXTURE::FLOOR_MACHINE1 = 0;
uint32 TEXTURE::FLOOR_MACHINE2 = 0;
uint32 TEXTURE::FLOOR_MACHINE3 = 0;
uint32 TEXTURE::FLOOR_AMMUNITION1 = 0;
uint32 TEXTURE::FLOOR_AMMUNITION2 = 0;
uint32 TEXTURE::FLOOR_AMMUNITION3 = 0;
uint32 TEXTURE::FLOOR_KITCHEN1 = 0;
uint32 TEXTURE::FLOOR_DINING1 = 0;
uint32 TEXTURE::FLOOR_CABOOSE1 = 0;
uint32 TEXTURE::FLOOR_EXT_NORMAL = 0;
uint32 TEXTURE::FLOOR_EXT_SICKBAY1 = 0;
uint32 TEXTURE::FLOOR_EXT_TOILET1 = 0;
uint32 TEXTURE::FLOOR_EXT_MACHINE1 = 0;
uint32 TEXTURE::FLOOR_EXT_MACHINE2 = 0;
uint32 TEXTURE::FLOOR_EXT_MACHINE3 = 0;
uint32 TEXTURE::FLOOR_EXT_AMMUNITION1 = 0;
uint32 TEXTURE::FLOOR_EXT_AMMUNITION2 = 0;
uint32 TEXTURE::FLOOR_EXT_AMMUNITION3 = 0;
uint32 TEXTURE::FLOOR_EXT_KITCHEN1 = 0;
uint32 TEXTURE::FLOOR_EXT_DINING1 = 0;
uint32 TEXTURE::FLOOR_EXT_CABOOSE1 = 0;
uint32 TEXTURE::LOADING_CONTROLS = 0;
uint32 TEXTURE::SHELF_AMMUNITION = 0;
uint32 TEXTURE::SHELF_EMPTY = 0;
uint32 TEXTURE::GENERATOR = 0;
uint32 TEXTURE::FIRE_EXTINGUISHER = 0;
uint32 TEXTURE::FIRESPRINKLER = 0;
uint32 TEXTURE::ICON_LOGBOOK = 0;
uint32 TEXTURE::LOOK_AT = 0;


/*
* Used for preloading resources needed in the loading screen
*/
void TEXTURE::RegisterResourcesPreLoading()
{
	TextureParams params = {};
	params.MinFilter = TEX_PARAM_LINEAR;
	params.MagFilter = TEX_PARAM_LINEAR;
	params.Wrap = TEX_PARAM_EDGECLAMP;
	HDR					= ResourceHandler::RegisterTexture2D("SkyBoxTextures/ocean.hdr", TEX_FORMAT_RGB16F, true, false, params);
}

void TEXTURE::RegisterResources()
{
	TextureParams params = {};
	params.Wrap = TEX_PARAM_REPEAT;
	params.MinFilter = TEX_PARAM_LINEAR;
	params.MagFilter = TEX_PARAM_LINEAR;

	BLOOD							= ResourceHandler::RegisterTexture2D("blood.png", TEX_FORMAT_RGBA, true, false, params);
	BLOOD_NORMAL					= ResourceHandler::RegisterTexture2D("bloodNormalMap.png", TEX_FORMAT_RGBA, true, false, params);
	SHIP							= ResourceHandler::RegisterTexture2D("ship.jpg", TEX_FORMAT_RGBA);
	SHIP_NORMAL						= ResourceHandler::RegisterTexture2D("shipNormalMap.png", TEX_FORMAT_RGBA);
	SINGLE_BED						= ResourceHandler::RegisterTexture2D("uv_Single_Bed_Texture_Map.jpg", TEX_FORMAT_RGBA);
	INSTRUMENT_1					= ResourceHandler::RegisterTexture2D("uv_Instrument1.jpg", TEX_FORMAT_RGBA);
	INSTRUMENT_2					= ResourceHandler::RegisterTexture2D("uv_Instrument2.jpg", TEX_FORMAT_RGBA);
	BUNK_BED						= ResourceHandler::RegisterTexture2D("uv_bunk_Bed_Texture.jpg", TEX_FORMAT_RGBA);
	CUPBOARD						= ResourceHandler::RegisterTexture2D("uv_Cupboard.jpg", TEX_FORMAT_RGBA);
	TABLE							= ResourceHandler::RegisterTexture2D("uv_Table.jpg", TEX_FORMAT_RGBA);
	TOILET							= ResourceHandler::RegisterTexture2D("uv_Toilet.jpg", TEX_FORMAT_RGBA);
	SHELF_AMMUNITION				= ResourceHandler::RegisterTexture2D("uv_AmmunitionShelf.jpg", TEX_FORMAT_RGBA);
	SHELF_EMPTY						= ResourceHandler::RegisterTexture2D("uv_EmptyShelf.jpg", TEX_FORMAT_RGBA);
	FIRE_EXTINGUISHER				= ResourceHandler::RegisterTexture2D("uv_FireExtinguisher.jpg", TEX_FORMAT_RGBA);
	FIRESPRINKLER					= ResourceHandler::RegisterTexture2D("uv_Sprinkler.png", TEX_FORMAT_RGBA);
	GENERATOR						= ResourceHandler::RegisterTexture2D("uv_Generator.jpg", TEX_FORMAT_RGBA);
	SMOKE							= ResourceHandler::RegisterTexture2D("smoke.png", TEX_FORMAT_RGBA, true, false, params);
	SJOFAN_DIFF						= ResourceHandler::RegisterTexture2D("CrewMember_diff.jpg", TEX_FORMAT_RGBA, true, false, params);
	WALL1							= ResourceHandler::RegisterTexture2D("wallTexture.png", TEX_FORMAT_RGBA, true, false, params);
	OVEN							= ResourceHandler::RegisterTexture2D("oven.png", TEX_FORMAT_RGBA, true, false, params);
	FLOOR_NORMAL					= ResourceHandler::RegisterTexture2D("normalFloorTexture.png", TEX_FORMAT_RGBA, true, false, params);
	FLOOR_SICKBAY1					= ResourceHandler::RegisterTexture2D("sickbayFloorTexture.png", TEX_FORMAT_RGBA, true, false, params);
	FLOOR_TOILET1					= ResourceHandler::RegisterTexture2D("toiletFloorTexture.png", TEX_FORMAT_RGBA, true, false, params);
	FLOOR_MACHINE1					= ResourceHandler::RegisterTexture2D("machineFloorTexture1.png", TEX_FORMAT_RGBA, true, false, params);
	FLOOR_MACHINE2					= ResourceHandler::RegisterTexture2D("machineFloorTexture2.png", TEX_FORMAT_RGBA, true, false, params);
	FLOOR_MACHINE3					= ResourceHandler::RegisterTexture2D("machineFloorTexture3.png", TEX_FORMAT_RGBA, true, false, params);
	FLOOR_AMMUNITION1				= ResourceHandler::RegisterTexture2D("ammunitionFloorTexture1.png", TEX_FORMAT_RGBA, true, false, params);
	FLOOR_AMMUNITION2				= ResourceHandler::RegisterTexture2D("ammunitionFloorTexture2.png", TEX_FORMAT_RGBA, true, false, params);
	FLOOR_AMMUNITION3				= ResourceHandler::RegisterTexture2D("ammunitionFloorTexture3.png", TEX_FORMAT_RGBA, true, false, params);
	FLOOR_KITCHEN1					= ResourceHandler::RegisterTexture2D("kitchenFloorTexture.png", TEX_FORMAT_RGBA, true, false, params);
	FLOOR_DINING1					= ResourceHandler::RegisterTexture2D("diningFloorTexture.png", TEX_FORMAT_RGBA, true, false, params);
	FLOOR_CABOOSE1					= ResourceHandler::RegisterTexture2D("cabooseFloorTexture.png", TEX_FORMAT_RGBA, true, false, params);
	FLOOR_EXT_NORMAL				= ResourceHandler::RegisterTexture2D("normalFloorExtTexture.png", TEX_FORMAT_RGBA, true, false, params);
	FLOOR_EXT_SICKBAY1				= ResourceHandler::RegisterTexture2D("sickbayFloorExtTexture.png", TEX_FORMAT_RGBA, true, false, params);
	FLOOR_EXT_TOILET1				= ResourceHandler::RegisterTexture2D("toiletFloorExtTexture.png", TEX_FORMAT_RGBA, true, false, params);
	FLOOR_EXT_MACHINE1				= ResourceHandler::RegisterTexture2D("machineFloorExtTexture1.png", TEX_FORMAT_RGBA, true, false, params);
	FLOOR_EXT_MACHINE2				= ResourceHandler::RegisterTexture2D("machineFloorExtTexture2.png", TEX_FORMAT_RGBA, true, false, params);
	FLOOR_EXT_MACHINE3				= ResourceHandler::RegisterTexture2D("machineFloorExtTexture3.png", TEX_FORMAT_RGBA, true, false, params);
	FLOOR_EXT_AMMUNITION1			= ResourceHandler::RegisterTexture2D("ammunitionFloorExtTexture1.png", TEX_FORMAT_RGBA, true, false, params);
	FLOOR_EXT_AMMUNITION2			= ResourceHandler::RegisterTexture2D("ammunitionFloorExtTexture2.png", TEX_FORMAT_RGBA, true, false, params);
	FLOOR_EXT_AMMUNITION3			= ResourceHandler::RegisterTexture2D("ammunitionFloorExtTexture3.png", TEX_FORMAT_RGBA, true, false, params);
	FLOOR_EXT_KITCHEN1				= ResourceHandler::RegisterTexture2D("kitchenFloorExtTexture.png", TEX_FORMAT_RGBA, true, false, params);
	FLOOR_EXT_DINING1				= ResourceHandler::RegisterTexture2D("diningFloorExtTexture.png", TEX_FORMAT_RGBA, true, false, params);
	FLOOR_EXT_CABOOSE1				= ResourceHandler::RegisterTexture2D("cabooseFloorExtTexture.png", TEX_FORMAT_RGBA, true, false, params);
	LOADING_CONTROLS				= ResourceHandler::RegisterTexture2D("LoadingScreen.png", TEX_FORMAT_RGBA, false, true);

	ICON_INJURY_BLEED				= ResourceHandler::RegisterTexture2D("Icons/InjuryBleed.png", TEX_FORMAT_RGBA, false, true);
	ICON_INJURY_BONE				= ResourceHandler::RegisterTexture2D("Icons/InjuryBone.png", TEX_FORMAT_RGBA, false, true);
	ICON_INJURY_BURN				= ResourceHandler::RegisterTexture2D("Icons/InjuryBurn.png", TEX_FORMAT_RGBA, false, true);
	ICON_INJURY_SMOKE				= ResourceHandler::RegisterTexture2D("Icons/InjurySmoke.png", TEX_FORMAT_RGBA, false, true);
	ICON_SKILL_FIRE					= ResourceHandler::RegisterTexture2D("Icons/SkillFire.png", TEX_FORMAT_RGBA, false, true);
	ICON_SKILL_MEDIC				= ResourceHandler::RegisterTexture2D("Icons/SkillMedic.png", TEX_FORMAT_RGBA, false, true);
	ICON_SKILL_STRENGTH				= ResourceHandler::RegisterTexture2D("Icons/SkillStrength.png", TEX_FORMAT_RGBA, false, true);
	ICON_CIRCLE						= ResourceHandler::RegisterTexture2D("Icons/Circle.png", TEX_FORMAT_RGBA, false, true);
	ICON_CHECKED					= ResourceHandler::RegisterTexture2D("Icons/Checked.png", TEX_FORMAT_RGBA, false, true);
	ICON_CROSSED					= ResourceHandler::RegisterTexture2D("Icons/Crossed.png", TEX_FORMAT_RGBA, false, true);
	ICON_NOTIFICATION_TRIANGLE		= ResourceHandler::RegisterTexture2D("Icons/NotificationTriangle.png", TEX_FORMAT_RGBA, false, true);
	ICON_LOGBOOK					= ResourceHandler::RegisterTexture2D("Icons/NoteBook.png", TEX_FORMAT_RGBA, false, true);
	ICON_GREEN_DOOR		            = ResourceHandler::RegisterTexture2D("Icons/GreenDoor.png", TEX_FORMAT_RGBA, false, true);
	ICON_BLUE_DOOR		            = ResourceHandler::RegisterTexture2D("Icons/BlueDoor.png", TEX_FORMAT_RGBA, false, true);
	ICON_YELLOW_DOOR	            = ResourceHandler::RegisterTexture2D("Icons/YellowDoor.png", TEX_FORMAT_RGBA, false, true);
	ICON_RED_DOOR		            = ResourceHandler::RegisterTexture2D("Icons/RedDoor.png", TEX_FORMAT_RGBA, false, true);

	params.MagFilter = TEX_PARAM_NEAREST;
	params.MinFilter = TEX_PARAM_NEAREST;

	LOOK_AT							= ResourceHandler::RegisterTexture2D("LookAt.png", TEX_FORMAT_RGBA, false, false, params);
}