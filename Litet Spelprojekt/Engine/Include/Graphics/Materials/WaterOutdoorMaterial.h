#pragma once
#include <Graphics/Textures/PlanarReflector.h>
#include <Graphics/Buffers/UniformBuffer.h>
#include "Material.h"

_declspec(align(16)) struct WaterOutdoorBuffer
{
	float DistortionFactor;
};

class API WaterOutdoorMaterial : public Material
{
	friend class ResourceHandler;

public:
	void Bind(const Framebuffer* pGBuffer) const noexcept override;
	void Unbind() const noexcept override;

	void SetDistortionTexture(Texture2D* pDistortion);
	void SetPlanarReflector(PlanarReflector* pReflector) const;
	void SetDistortionFactor(float distortionFactor) const;

private:
	WaterOutdoorMaterial();
	~WaterOutdoorMaterial();

	Texture2D* m_pDistortion;
	Texture2D* m_pDepthMap;
	mutable UniformBuffer* m_pWaterBuffer;
	mutable PlanarReflector* m_pReflector;
	mutable WaterOutdoorBuffer m_Buffer;
};