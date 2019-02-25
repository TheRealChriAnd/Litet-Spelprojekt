#pragma once
#include <Graphics/Textures/PlanarReflector.h>
#include <Graphics/Buffers/UniformBuffer.h>
#include "Material.h"

_declspec(align(16)) struct WaterOutdoorBuffer
{
	float WaveFactor;
};

class API WaterOutdoorMaterial : public Material
{
	friend class ResourceHandler;

public:
	void Bind(const Framebuffer* pGBuffer) const noexcept override;
	void Unbind() const noexcept override;

	void SetDistortionTexture(Texture2D* pDistortion);
	void SetDisplacementMap(Texture2D* pDisplacementMap);
	void SetPlanarReflector(PlanarReflector* pReflector) const;
	void SetWaveFactor(float waveFactor) const;

private:
	WaterOutdoorMaterial();
	~WaterOutdoorMaterial();

	mutable bool m_LastDepthMask;
	mutable Func m_LastDepthFunc;
	Texture2D* m_pDistortion;
	Texture2D* m_pDepthMap;
	Texture2D* m_pDisplacementMap;
	mutable UniformBuffer* m_pWaterBuffer;
	mutable PlanarReflector* m_pReflector;
	mutable WaterOutdoorBuffer m_Buffer;
};