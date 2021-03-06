#pragma once
#include <EnginePch.h>
#include <Graphics/GUI/GUIObject.h>

class API ProgressBar : public GUIObject
{
public:
	ProgressBar(float x, float y, float width, float height);
	virtual ~ProgressBar();

	void SetPercentage(float percentage);
	float GetPercentage() const noexcept;

	const glm::vec4& GetProgressColor() const noexcept;
	void SetProgressColor(const glm::vec4& color);

protected:
	virtual void OnRender(GUIContext* context) override;
	virtual void PrintName() const override;

private:
	float m_Percentage;
	glm::vec4 m_ProgressColor;
};