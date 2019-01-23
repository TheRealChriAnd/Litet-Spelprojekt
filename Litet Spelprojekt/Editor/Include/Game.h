#pragma once

#include <System\Application.h>
#include <Graphics\GameObject.h>
#include <Graphics\Buffers\UniformBuffer.h>
#include <Graphics\Camera.h>
#include <Graphics\Scene.h>
#include <Graphics/Renderers/FontRenderer.h>
//#include "../../Game/Include/IO/WorldSerializer.h"

#include <GLM\glm.hpp>
#include <GLM\gtc\type_ptr.hpp>

class Game : public Application
{
public:
	Game() noexcept;
	~Game();

	void OnUpdate(float dtS) override;
	void OnRender() override;

private:
	ShaderProgram* m_pShaderProgramDefault;

	Scene* m_pScene;

	std::vector<UniformBuffer*> m_GameObjectUniforms;

	UniformBuffer* m_pPerFrameUniform;
	float m_PerFrameArray[16 + 3];

	Mesh* m_pGridMesh;
	UniformBuffer* m_pGridUniform;

	FontRenderer* fontRenderer;
};