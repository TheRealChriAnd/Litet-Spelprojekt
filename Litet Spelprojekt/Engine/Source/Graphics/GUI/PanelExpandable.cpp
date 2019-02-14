#include <EnginePch.h>
#include <Graphics\GUI\PanelExpandable.h>

PanelExpandable::PanelExpandable(float x, float y, float width, float height, float clientHeight, const std::string& text) : Button(x, y, width, height, text),
	m_pFrameBufferClientArea(nullptr),
	m_Mode(CLOSED),
	m_Percentage(0)
{
	SetClientSize(clientHeight);
	SetBackgroundColor(glm::vec4(0.118F, 0.118F, 0.118F, 1.0F));
	SetOnPressedColor(glm::vec4(0.6F, 0.6F, 0.6F, 1.0F));
	SetOnHoverColor(glm::vec4(0.408F, 0.408F, 0.408F, 1.0F));
}

PanelExpandable::~PanelExpandable()
{
}

void PanelExpandable::SetExpanded(bool expanded) noexcept
{
	if ((m_Mode == CLOSED || m_Mode == COLLAPSING) && expanded)
	{
		m_Mode = EXPANDING;
		AddRealTimeRenderer(this);
	}
	if ((m_Mode == OPEN || m_Mode == EXPANDING) && !expanded)
	{
		m_Mode = COLLAPSING;
	}
}

bool PanelExpandable::IsExpanded() const noexcept
{
	return m_Mode == OPEN || m_Mode == EXPANDING || m_Mode == COLLAPSING;
}

float PanelExpandable::GetClientWidth() const noexcept
{
	return m_pFrameBufferClientArea->GetWidth();
}

float PanelExpandable::GetClientHeight() const noexcept
{
	return m_pFrameBufferClientArea->GetHeight();
}

void PanelExpandable::SetClientSize(float height) noexcept
{
	if (m_pFrameBufferClientArea)
	{
		delete m_pFrameBufferClientArea;
	}
	
	FramebufferDesc desc;
	desc.DepthStencilFormat = TEX_FORMAT_UNKNOWN;
	desc.ColorAttchmentFormats[0] = TEX_FORMAT_RGBA;
	desc.SamplingParams = TextureParams();
	desc.NumColorAttachments = 1;
	desc.Width = static_cast<uint32>(GetWidth());
	desc.Height = static_cast<uint32>(height < GetHeight() ? GetHeight() : height);

	m_pFrameBufferClientArea = new Framebuffer(desc);

	RequestRepaint();
}

bool PanelExpandable::ContainsPoint(const glm::vec2& position, const GUIObject* caller) const noexcept
{
	if (IsMyChild(caller))
	{
		if (IsExpanded())
		{
			float x = GetXInWorld();
			float y = GetYInWorld();

			if (position.x > x && position.x < x + GetWidth())
			{
				if (position.y > y - GetClientHeight() && position.y < y + GetHeight())
				{
					if (HasParent())
					{
						return GetParent()->ContainsPoint(position);
					}
					return true;
				}
			}
		}
		return false;
	}
	return Button::ContainsPoint(position, caller);
}

void PanelExpandable::RenderChildrensFrameBuffers(GUIContext* context)
{
	context->BeginSelfRendering(m_pFrameBufferClientArea, GetBackgroundColor());
	Button::RenderChildrensFrameBuffers(context);
}

void PanelExpandable::RenderRealTime(GUIContext* context)
{
	glm::vec4 viewPortSize = context->GetGraphicsContext()->GetViewPort();
	glScissor(GetXInWorld(), GetYInWorld() - GetClientHeight() * m_Percentage, GetWidth(), GetClientHeight() * m_Percentage + 1);
	glEnable(GL_SCISSOR_TEST);
	context->RenderTexture((Texture2D*)m_pFrameBufferClientArea->GetColorAttachment(0), GetXInWorld(), GetYInWorld() - GetClientHeight(), GetClientWidth(), GetClientHeight(), GUIContext::COLOR_WHITE);
	glScissor(viewPortSize.z, viewPortSize.w, viewPortSize.x, viewPortSize.y);
	glDisable(GL_SCISSOR_TEST);
}

float PanelExpandable::GetYInWorld(const GUIObject* child) const noexcept
{
	float value = GetY();
	if (child && IsMyChild(child))
	{
		value -= GetClientHeight();
	}
	if (HasParent())
	{
		value += GetParent()->GetYInWorld(this);
	}
	return value;
}

void PanelExpandable::OnReleased(const glm::vec2& position, MouseButton mousebutton) noexcept
{
	Button::OnReleased(position, mousebutton);
	SetExpanded(!IsExpanded());
}

void PanelExpandable::OnUpdate(float dtS)
{
	static const float speed = 6.0F;

	if (m_Mode == EXPANDING)
	{
		m_Percentage += dtS * speed;
		if (m_Percentage > 1.0)
		{
			m_Percentage = 1.0;
			m_Mode = OPEN;
		}
	}
	else if (m_Mode == COLLAPSING)
	{
		m_Percentage -= dtS * speed;
		if (m_Percentage < 0.0)
		{
			m_Percentage = 0.0;
			m_Mode = CLOSED;
			RemoveRealTimeRenderer(this);
		}
	}
}

void PanelExpandable::PrintName() const
{
	std::cout << "PanelExpandable" << std::endl;
}
