#pragma once
#include <EnginePch.h>
#include <Graphics/Textures/Framebuffer.h>
#include <Graphics/Shaders/ShaderProgram.h>
#include <Graphics/Buffers/UniformBuffer.h>
#include <Graphics/Renderers/IRenderer.h>

struct OrthoPerObject
{
	glm::mat4 Model;
	glm::vec4 Color;
	float HasTexture;
	float HasNormalMap;
};

struct OrthoPerFrame
{
	glm::mat4 ViewProjection;
	glm::vec3 CameraPosition;
	float vafan;
	glm::vec3 CameraFront;
};

class API OrthographicRenderer final : public IRenderer
{
public:
	OrthographicRenderer(OrthographicRenderer&& other) = delete;
	OrthographicRenderer(const OrthographicRenderer& other) = delete;
	OrthographicRenderer& operator=(OrthographicRenderer&& other) = delete;
	OrthographicRenderer& operator=(const OrthographicRenderer& other) = delete;

	OrthographicRenderer() noexcept;
	~OrthographicRenderer();

	void SetClipDistance(const glm::vec4& plane, uint32 index) override final;
	void DrawScene(const Scene& scene, float dtS) const override final;

private:
	void Create() noexcept;

private:
	UniformBuffer* m_pOrthoPerObject;
	UniformBuffer* m_pOrthoPerFrame;

	ShaderProgram* m_pOrthographicProgram;
};