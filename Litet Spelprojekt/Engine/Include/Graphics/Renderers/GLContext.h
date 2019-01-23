#pragma once
#include <EnginePch.h>
#include <Defines.h>
#include <Graphics/Geometry/Mesh.h>
#include <Graphics/Shaders/ShaderProgram.h>
#include <Graphics/Textures/Texture.h>
#include <Graphics/Buffers/UniformBuffer.h>
#include <Graphics/Textures/Framebuffer.h>
#include <Graphics/Geometry/FullscreenTri.h>

enum Capability : uint32
{
	DEPTH_TEST = 0x0B71,
	STENCIL_TEST = 0x0B90,
	CULL_FACE = 0x0B44,
	BLEND = 0x0BE2,
	CLIP_DISTANCE0 = 0x3000,
	CLIP_DISTANCE1 = 0x3001,
	CLIP_DISTANCE2 = 0x3002,
	CLIP_DISTANCE3 = 0x3003,
	CLIP_DISTANCE4 = 0x3004,
	CLIP_DISTANCE5 = 0x3005,
	CLIP_DISTANCE6 = 0x3006,
	CLIP_DISTANCE7 = 0x3007,
};

enum PrimitiveTopology : unsigned int
{
	PT_POINTS = 0x0000,
	PT_LINES = 0x0001,
	PT_LINE_LOOP = 0x0002,
	PT_LINE_STRIP = 0x0003,
	PT_TRIANGLES = 0x0004,
	PT_TRIANGLE_STRIP = 0x0005,
	PT_TRIANGLE_FAN = 0x0006,
	PT_LINES_ADJACENCY = 0x000A,
	PT_LINE_STRIP_ADJACENCY = 0x000B,
	PT_TRIANGLES_ADJACENCY = 0x000C,
	PT_TRIANGLE_STRIP_ADJACENCY = 0x000D,
};

enum ClearFlags : uint32
{
	CLEAR_FLAG_UNKNOWN = 0,
	CLEAR_FLAG_COLOR = 0x00004000,
	CLEAR_FLAG_DEPTH = 0x00000100,
	CLEAR_FLAG_STENCIL = 0x00000400
};

enum Func : uint32
{
	FUNC_NEVER = 0x0200,
	FUNC_LESS = 0x0201,
	FUNC_EQUAL = 0x0202,
	FUNC_LESS_EQUAL = 0x0203,
	FUNC_GREATER = 0x0204,
	FUNC_NOT_EQUAL = 0x0205,
	FUNC_GREATER_EQUAL = 0x0206,
	FUNC_ALWAYS = 0x0207,
};

typedef Capability Cap;


class API GLContext
{
public:
	GLContext(GLContext&& other) = delete;
	GLContext(const GLContext& other) = delete;
	GLContext& operator=(GLContext&& other) = delete;
	GLContext& operator=(const GLContext& other) = delete;

	GLContext();
	~GLContext();

	void Enable(Cap cap) const noexcept;
	void Disable(Cap cap) const noexcept;
	
	void SetViewport(uint32 width, uint32 height, uint32 topX, uint32 topY) const noexcept;
	void SetClearColor(float r, float g, float b, float a) const noexcept;
	void SetClearDepth(float depth) const noexcept;
	void SetColorMask(uint8 r, uint8 g, uint8 b, uint8 a) const noexcept;
	void SetDepthMask(bool writeDepth) const noexcept;
	void SetDepthFunc(Func func) const noexcept;
	void SetProgram(const ShaderProgram* pProgram) const noexcept;
	void SetTexture(const Texture* pTexture, uint32 slot) const noexcept;
	void SetUniformBuffer(const UniformBuffer* pBuffer, uint32 slot) const noexcept;
	void SetFramebuffer(const Framebuffer* pFramebuffer) const noexcept;
	
	void Clear(uint32 flags) const noexcept;

	void DrawIndexedMesh(const IndexedMesh& mesh) const noexcept;
	void DrawMesh(const Mesh& mesh, PrimitiveTopology primitiveTopology) const noexcept;
	void DrawFullscreenTriangle(const FullscreenTri& triangle) const noexcept;
};