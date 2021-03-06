#include <EnginePch.h>
#include <IO/TEXTURE.h>
#include <IO/ResourceHandler.h>

uint32 TEXTURE::BLOOD = 0;
uint32 TEXTURE::BLOOD_NORMAL = 0;
uint32 TEXTURE::SHIP = 0;
uint32 TEXTURE::SHIP_NORMAL = 0;
uint32 TEXTURE::WATER_DISTORTION = 0;
uint32 TEXTURE::WATER_NORMAL = 0;
uint32 TEXTURE::HDR = 0;
uint32 TEXTURE::SINGLE_BED = 0;
uint32 TEXTURE::BUNK_BED = 0;


/*
* Used for preloading resources needed in the loading screen
*/
void TEXTURE::RegisterResourcesPreLoading()
{
	
}

void TEXTURE::RegisterResources()
{
	TextureParams params = {};
	params.Wrap = TEX_PARAM_REPEAT;
	params.MinFilter = TEX_PARAM_LINEAR;
	params.MagFilter = TEX_PARAM_LINEAR;

	BLOOD				= ResourceHandler::RegisterTexture2D("blood.png", TEX_FORMAT_RGBA, true, params);
	BLOOD_NORMAL		= ResourceHandler::RegisterTexture2D("bloodNormalMap.png", TEX_FORMAT_RGBA, true, params);
	SHIP				= ResourceHandler::RegisterTexture2D("ship.jpg", TEX_FORMAT_RGBA);
	SHIP_NORMAL			= ResourceHandler::RegisterTexture2D("shipNormalMap.png", TEX_FORMAT_RGBA);
	SINGLE_BED			= ResourceHandler::RegisterTexture2D("uv_Single_Bed_Texture_Map.jpg", TEX_FORMAT_RGBA);
	BUNK_BED			= ResourceHandler::RegisterTexture2D("uv_bunk_Bed_Texture.jpg", TEX_FORMAT_RGBA);
	WATER_DISTORTION	= ResourceHandler::RegisterTexture2D("waterDUDV.png", TEX_FORMAT_RGBA, true, params);
	WATER_NORMAL		= ResourceHandler::RegisterTexture2D("waterNormalMap.png", TEX_FORMAT_RGBA, true, params);

	params.Wrap = TEX_PARAM_EDGECLAMP;

	HDR					= ResourceHandler::RegisterTexture2D("SkyBoxTextures/ocean.hdr", TEX_FORMAT_RGB16F, true, params);
}