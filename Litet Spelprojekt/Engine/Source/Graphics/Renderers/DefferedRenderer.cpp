#include <EnginePch.h>
#include <Graphics/Renderers/DefferedRenderer.h>
#include <System/Window.h>
#include <System/Application.h>

DefferedRenderer::DefferedRenderer()
	: m_pGBuffer(nullptr),
	m_pDepthPrePassProgram(nullptr),
	m_pGeometryPassProgram(nullptr),
	m_pLightPassProgram(nullptr),
	m_pGPassVSPerFrame(nullptr),
	m_pGPassVSPerObject(nullptr),
	m_pGPassFSPerObject(nullptr),
	m_pLightPassBuffer(nullptr),
	m_pTriangle(nullptr)
{
	Create();
}

DefferedRenderer::~DefferedRenderer()
{
	if (m_pGBuffer != nullptr)
	{
		delete m_pGBuffer;
		m_pGBuffer = nullptr;
	}

	if (m_pGeometryPassProgram != nullptr)
	{
		delete m_pGeometryPassProgram;
		m_pGeometryPassProgram = nullptr;
	}

	if (m_pLightPassProgram != nullptr)
	{
		delete m_pLightPassProgram;
		m_pLightPassProgram = nullptr;
	}

	if (m_pGPassVSPerFrame != nullptr)
	{
		delete m_pGPassVSPerFrame;
		m_pGPassVSPerFrame = nullptr;
	}

	if (m_pGPassVSPerObject != nullptr)
	{
		delete m_pGPassVSPerObject;
		m_pGPassVSPerObject = nullptr;
	}

	if (m_pLightPassBuffer != nullptr)
	{
		delete m_pLightPassBuffer;
		m_pLightPassBuffer = nullptr;
	}

	if (m_pTriangle != nullptr)
	{
		delete m_pTriangle;
		m_pTriangle = nullptr;
	}

	if (m_pDepthPrePassProgram != nullptr)
	{
		delete m_pDepthPrePassProgram;
		m_pDepthPrePassProgram = nullptr;
	}

	if (m_pGPassFSPerObject != nullptr)
	{
		delete m_pGPassFSPerObject;
		m_pGPassFSPerObject = nullptr;
	}
}

void DefferedRenderer::DrawScene(const Scene& scene) const
{
	GLContext& context = Application::GetInstance().GetContext();

	context.SetClearColor(0.392f, 0.584f, 0.929f, 1.0f);
	context.SetClearDepth(1.0f);

	context.SetViewport(m_pGBuffer->GetWidth(), m_pGBuffer->GetHeight(), 0, 0);
	context.SetFramebuffer(m_pGBuffer);

	context.SetUniformBuffer(m_pGPassVSPerFrame, 0);
	context.SetUniformBuffer(m_pGPassVSPerObject, 1);

	context.Clear(CLEAR_FLAG_COLOR | CLEAR_FLAG_DEPTH);

	//DepthPrePass(scene);

	GeometryPass(scene);

	context.SetFramebuffer(nullptr);
	context.Disable(DEPTH_TEST);

	LightPass(scene);

	context.Enable(DEPTH_TEST);
	context.SetDepthMask(true);
	context.SetDepthFunc(FUNC_LESS);
}

void DefferedRenderer::Create() noexcept
{
	std::cout << "Createing deffered renderer" << std::endl;

	//We can destroy desc when gbuffer is created
	{
		FramebufferDesc desc = {};
		desc.ColorAttchmentFormats[0] = TEX_FORMAT_RGBA;
		desc.ColorAttchmentFormats[1] = TEX_FORMAT_RGBA16F;
		desc.NumColorAttachments = 2;
		desc.DepthStencilFormat = TEX_FORMAT_DEPTH;
		//desc.Width = 1920; 
		desc.Width = Window::GetCurrentWindow().GetWidth();
		//desc.Height = 1080;
		desc.Height = Window::GetCurrentWindow().GetHeight();

		m_pGBuffer = new Framebuffer(desc);
	}

	{
		Shader* pVert = new Shader();
		if (pVert->CompileFromFile("Resources/Shaders/defferedGeometryVert.glsl", VERTEX_SHADER))
		{
			std::cout << "Created Geomtrypass Vertex shader" << std::endl;
		}

		Shader* pFrag = new Shader();
		if(pFrag->CompileFromFile("Resources/Shaders/defferedGeometryFrag.glsl", FRAGMENT_SHADER))
		{
			std::cout << "Created Geomtrypass Fragment shader" << std::endl;
		}

		m_pGeometryPassProgram = new ShaderProgram(*pVert, *pFrag);

		delete pVert;
		delete pFrag;
	}

	{
		Shader* pVert = new Shader();
		if (pVert->CompileFromFile("Resources/Shaders/defferedLightningVert.glsl", VERTEX_SHADER))
		{
			std::cout << "Created Lightpass Vertex shader" << std::endl;
		}

		Shader* pFrag = new Shader();
		if (pFrag->CompileFromFile("Resources/Shaders/defferedLightningFrag.glsl", FRAGMENT_SHADER))
		{
			std::cout << "Created Lightpass Fragment shader" << std::endl;
		}

		m_pLightPassProgram = new ShaderProgram(*pVert, *pFrag);

		delete pVert;
		delete pFrag;
	}

	{
		Shader* pVert = new Shader();
		if (pVert->CompileFromFile("Resources/Shaders/defferedDepthPreVert.glsl", VERTEX_SHADER))
		{
			std::cout << "Created DepthPrePass Vertex shader" << std::endl;
		}

		m_pDepthPrePassProgram = new ShaderProgram(*pVert);

		delete pVert;
	}

	//We can destroy object when uniformbuffer is created
	{
		GPassVSPerFrame object = {};
		object.ViewProjection = glm::mat4(1.0f);
		object.CameraPosition = glm::vec3();

		m_pGPassVSPerFrame = new UniformBuffer(&object, 1, sizeof(GPassVSPerFrame));
	}

	{
		GPassVSPerObject object = {};
		object.Model = glm::mat4(1.0f);

		m_pGPassVSPerObject = new UniformBuffer(&object, 1, sizeof(GPassVSPerObject));
	}

	{
		GPassFSPerObject object = {};
		object.Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

		m_pGPassFSPerObject = new UniformBuffer(&object, 1, sizeof(GPassFSPerObject));
	}

	{
		LightPassBuffer buff = {};
		buff.InverseView = glm::mat4(1.0f);
		buff.InverseProjection = glm::mat4(1.0f);
		buff.CameraPosition = glm::vec3();

		m_pLightPassBuffer = new UniformBuffer(&buff, 1, sizeof(LightPassBuffer));
	}

	{
		m_pTriangle = new FullscreenTri();
	}
}

void DefferedRenderer::DepthPrePass(const Scene& scene) const noexcept
{
	GLContext& context = Application::GetInstance().GetContext();

	context.SetProgram(m_pDepthPrePassProgram);

	context.SetColorMask(0, 0, 0, 0);
	context.SetDepthFunc(FUNC_LESS);

	GPassVSPerFrame perFrame = {};
	perFrame.ViewProjection = scene.GetCamera().GetCombinedMatrix();
	perFrame.CameraPosition = scene.GetCamera().GetPosition();
	m_pGPassVSPerFrame->UpdateData(&perFrame);

	GPassVSPerObject perObject = {};
	for (uint32 i = 0; i < scene.GetGameObjects().size(); i++)
	{
		perObject.Model = scene.GetGameObjects()[i]->GetTransform();
		m_pGPassVSPerObject->UpdateData(&perObject);

		context.DrawIndexedMesh(scene.GetGameObjects()[i]->GetMesh());
	}

	context.SetDepthFunc(FUNC_LESS_EQUAL);
	context.SetColorMask(1, 1, 1, 1);

	context.SetDepthMask(false);
}

void DefferedRenderer::GeometryPass(const Scene& scene) const noexcept
{
	GLContext& context = Application::GetInstance().GetContext();

	context.SetProgram(m_pGeometryPassProgram);
	context.SetUniformBuffer(m_pGPassFSPerObject, 2);

	GPassVSPerFrame perFrame = {};
	perFrame.ViewProjection = scene.GetCamera().GetCombinedMatrix();
	perFrame.CameraPosition = scene.GetCamera().GetPosition();
	m_pGPassVSPerFrame->UpdateData(&perFrame);

	GPassVSPerObject perObjectVS = {};
	GPassFSPerObject perObjectFS = {};

	for (uint32 i = 0; i < scene.GetGameObjects().size(); i++)
	{
		GameObject& gameobject = *scene.GetGameObjects()[i];

		perObjectVS.Model = gameobject.GetTransform();
		m_pGPassVSPerObject->UpdateData(&perObjectVS);

		const Material& material = gameobject.GetMaterial();
		perObjectFS.Color = material.GetColor();
		if (material.HasTexture())
		{
			perObjectFS.HasTexture = 1.0f;
			context.SetTexture(material.GetTexture(), 0);
		}
		else
		{
			perObjectFS.HasTexture = 0.0f;
		}

		m_pGPassFSPerObject->UpdateData(&perObjectFS);

		context.DrawIndexedMesh(scene.GetGameObjects()[i]->GetMesh());
	}
}

void DefferedRenderer::LightPass(const Scene& scene) const noexcept
{
	GLContext& context = Application::GetInstance().GetContext();

	context.SetViewport(Window::GetCurrentWindow().GetWidth(), Window::GetCurrentWindow().GetHeight(), 0, 0);
	context.SetProgram(m_pLightPassProgram);
	context.SetUniformBuffer(m_pLightPassBuffer, 0);

	{
		LightPassBuffer buff = {};
		buff.InverseView = glm::inverse(scene.GetCamera().GetViewMatrix());
		buff.InverseProjection = glm::inverse(scene.GetCamera().GetProjectionMatrix());
		buff.CameraPosition = scene.GetCamera().GetPosition();

		m_pLightPassBuffer->UpdateData(&buff);
	}

	context.SetTexture(m_pGBuffer->GetColorAttachment(0), 0);
	context.SetTexture(m_pGBuffer->GetColorAttachment(1), 1);

	context.DrawFullscreenTriangle(*m_pTriangle);
}
