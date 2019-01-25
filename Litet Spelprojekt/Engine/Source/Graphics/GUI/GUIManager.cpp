#include <Graphics/GUI/GUIManager.h>
#include <Graphics/GUI/GUIObject.h>
#include <System/Application.h>

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>


GUIManager::GUIManager(float width, float height) : m_OrderIsDirty(false)
{
	m_PerObjectDataGUI.projection = glm::ortho(0.0f, width, 0.0f, height);
	m_PerObjectDataGUI.color = glm::vec3(1.0, 1.0F, 1.0F);

	m_pPerObjectUniform = new UniformBuffer(&m_PerObjectDataGUI, 1, sizeof(PerObjectDataGUI));

	Shader vShader;
	Shader fShader;

	vShader.CompileFromFile("Resources/Shaders/VShaderGUI.glsl", VERTEX_SHADER);
	fShader.CompileFromFile("Resources/Shaders/FShaderGUI.glsl", FRAGMENT_SHADER);

	m_pShaderProgram = new ShaderProgram(vShader, fShader);

	m_VertexQuad[0].texCoords.x = 0.0;
	m_VertexQuad[0].texCoords.y = 0.0;
	m_VertexQuad[1].texCoords.x = 0.0;
	m_VertexQuad[1].texCoords.y = 1.0;
	m_VertexQuad[2].texCoords.x = 1.0;
	m_VertexQuad[2].texCoords.y = 1.0;

	m_VertexQuad[3].texCoords.x = 0.0;
	m_VertexQuad[3].texCoords.y = 0.0;
	m_VertexQuad[4].texCoords.x = 1.0;
	m_VertexQuad[4].texCoords.y = 1.0;
	m_VertexQuad[5].texCoords.x = 1.0;
	m_VertexQuad[5].texCoords.y = 0.0;

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(VertexGUI), m_VertexQuad, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(VertexGUI), (void*)0); //Position
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexGUI), (void*)2); //Tex
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(VertexGUI), (void*)4); //Color
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	AddGUIObject(new GUIObject(200, 50));
}

GUIManager::~GUIManager()
{
	delete m_pPerObjectUniform;
	delete m_pShaderProgram;
}

void GUIManager::AddGUIObject(GUIObject* object)
{
	if (!ContainsGUIObject(m_GUIObjects, object) && !ContainsGUIObject(m_GUIObjectsToAdd, object))
	{
		m_GUIObjectsToAdd.push_back(object);
	}
}

void GUIManager::RemoveGUIObject(GUIObject* object)
{
	if (ContainsGUIObject(m_GUIObjects, object))
	{
		m_GUIObjectsToRemove.push_back(object);
	}
}

void GUIManager::MarkDepthDirty()
{
	m_OrderIsDirty = true;
}

bool GUIManager::ContainsGUIObject(const std::vector<GUIObject*>& list, GUIObject* object)
{
	for (GUIObject* guiObject : list)
	{
		if (guiObject == object)
		{
			return true;
		}
	}
	return false;
}

void GUIManager::RequestRepaint(GUIObject* object)
{
	if (!ContainsGUIObject(m_GUIObjectsDirty, object))
	{
		m_GUIObjectsDirty.push_back(object);
	}
}

void GUIManager::SetVertexQuadData(float x, float y, float width, float height, glm::vec4 color)
{
	m_VertexQuad[0].position.x = x;
	m_VertexQuad[0].position.y = y + height;
	m_VertexQuad[1].position.x = x;
	m_VertexQuad[1].position.y = y;
	m_VertexQuad[2].position.x = x + width;
	m_VertexQuad[2].position.y = y;

	m_VertexQuad[3].position.x = x;
	m_VertexQuad[3].position.y = y + height;
	m_VertexQuad[4].position.x = x + width;
	m_VertexQuad[4].position.y = y;
	m_VertexQuad[5].position.x = x + width;
	m_VertexQuad[5].position.y = y + height;

	for (int i = 0; i < 6; i++)
	{
		m_VertexQuad[i].color = color;
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(m_VertexQuad), m_VertexQuad);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GUIManager::OnUpdate(float dtS)
{
	for (GUIObject* objectToRemove : m_GUIObjectsToRemove)
	{
		int32 counter = 0;
		for (GUIObject* object : m_GUIObjects)
		{
			if (objectToRemove == object)
			{
				m_GUIObjects.erase(m_GUIObjects.begin() + counter);
				objectToRemove->OnRemoved(this);
				std::cout << "GUI Object Removed" << std::endl;
				return;
			}
			counter++;
		}
	}
	for (GUIObject* objectToAdd : m_GUIObjectsToAdd)
	{
		m_GUIObjects.push_back(objectToAdd);
		m_GUIObjectsDirty.push_back(objectToAdd);
		objectToAdd->OnAdded(this);
		m_OrderIsDirty = true;
		std::cout << "GUI Object Added" << std::endl;
	}
	m_GUIObjectsToRemove.clear();
	m_GUIObjectsToAdd.clear();

	if (m_OrderIsDirty)
	{
		m_OrderIsDirty = false;
		std::sort(m_GUIObjects.begin(), m_GUIObjects.end(), [](const GUIObject* a, const GUIObject* b) -> bool
		{
			return a->GetDepth() < b->GetDepth();
		});
	}

	for (GUIObject* object : m_GUIObjects)
	{
		object->OnUpdate(dtS);
	}
}

void GUIManager::OnRender(GLContext* context)
{
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(m_VAO);

	context->SetProgram(m_pShaderProgram);

	context->SetUniformBuffer(m_pPerObjectUniform, 1);

	for (GUIObject* object : m_GUIObjectsDirty)
	{
		m_PerObjectDataGUI.projection = glm::ortho(0.0f, object->GetWidth(), 0.0f, object->GetHeight());
		m_pPerObjectUniform->UpdateData(&m_PerObjectDataGUI);
		SetVertexQuadData(0, 0, object->GetWidth(), object->GetHeight(), glm::vec4(1.0, 0.0, 0.0, 1.0));

		object->InternalOnRender(context);
	}

	m_GUIObjectsDirty.clear();

	m_PerObjectDataGUI.projection = glm::ortho(0.0f, (float)Application::GetInstance().GetWindow().GetWidth(), 0.0f, (float)Application::GetInstance().GetWindow().GetHeight());
	m_pPerObjectUniform->UpdateData(&m_PerObjectDataGUI);

	for (GUIObject* object : m_GUIObjects)
	{
		SetVertexQuadData(object->GetX(), object->GetY(), object->GetWidth(), object->GetHeight(), glm::vec4(1.0, 1.0, 1.0, 1.0));
		context->SetTexture(object->m_pFramebuffer->GetColorAttachment(0), 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
	
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void GUIManager::OnMousePressed(MouseButton mousebutton)
{
	for (GUIObject* object : m_GUIObjects)
	{
		object->OnMousePressed(mousebutton);
	}
}

void GUIManager::OnMouseReleased(MouseButton mousebutton)
{
	for (GUIObject* object : m_GUIObjects)
	{
		object->OnMouseReleased(mousebutton);
	}
}

void GUIManager::OnMouseMove(const glm::vec2& position)
{
	for (GUIObject* object : m_GUIObjects)
	{
		object->OnMouseMove(position);
	}
}

void GUIManager::OnKeyUp(KEY keycode)
{
	for (GUIObject* object : m_GUIObjects)
	{
		object->OnKeyUp(keycode);
	}
}

void GUIManager::OnKeyDown(KEY keycode)
{
	for (GUIObject* object : m_GUIObjects)
	{
		object->OnKeyDown(keycode);
	}
}