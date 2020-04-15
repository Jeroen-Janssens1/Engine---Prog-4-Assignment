#pragma once
#include "RenderComponent.h"
#include <string>
#include "MiniginPCH.h"
#include "Font.h"

class TextRenderComponent :
	public RenderComponent
{
public:
	TextRenderComponent(std::shared_ptr<GameObject>& pOwner, std::shared_ptr<TransformComponent>& pTransform, const std::string& text, const std::shared_ptr<dae::Font>& font);
	~TextRenderComponent() = default;

	void Update() override;
	void Render() const override;

	void SetText(const std::string& text);
	

protected:
	bool m_NeedsUpdate;
	std::string m_Text;
	std::shared_ptr<dae::Font> m_Font;

};