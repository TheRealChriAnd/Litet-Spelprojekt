#include <EnginePch.h>
#include <IO/DECAL.h>
#include <IO/ResourceHandler.h>

uint32 DECAL::BLOOD = 0;

/*
* Used for preloading resources needed in the loading screen
*/
void DECAL::RegisterResourcesPreLoading()
{

}

void DECAL::RegisterResources()
{
	//B�r tas bort helt och h�llet (Dahlle fixa)
	//BLOOD					= ResourceHandler::RegisterDecal(TEXTURE::BLOOD, TEXTURE::BLOOD_NORMAL);
}