#pragma once
#include <EnginePch.h>
#include <Graphics/GUI/TextView.h>
#include <Graphics/GUI/IMouseListener.h>

class API Button : public TextView, public IMouseListener
{
public:
	Button(float x, float y, float width, float height, const std::string& text, void(*onPressedCallback)(Button*) = nullptr, void(*onReleasedCallback)(Button*) = nullptr, int textSize = 50);
	virtual ~Button();

	Texture2D* GetOnPressedTexture() const noexcept;
	void SetOnPressedTexture(Texture2D* texture);

	void SetOnButtonPressed(void(*callback)(Button*));
	void SetOnButtonCallback(void(*callback)(Button*));

protected:
	void OnMousePressed(const glm::vec2& position, MouseButton mousebutton) override;
	void OnMouseReleased(const glm::vec2& position, MouseButton mousebutton) override;
	void OnMouseMove(const glm::vec2& lastPosition, const glm::vec2& position) override;

	virtual void OnAdded(GUIObject* parent) override;
	virtual void OnRemoved(GUIObject* parent) override;

	virtual void RenderBackgroundTexture(GUIContext* context) override;

private:
	Texture2D* m_pOnPressedTexture;
	bool m_IsPressed;
	void (*m_OnPressedCallback)(Button*);
	void (*m_OnReleasedCallback)(Button*);
};