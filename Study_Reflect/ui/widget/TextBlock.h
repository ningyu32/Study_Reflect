#pragma once
#include "../Core/Widget.h"
#include <string>

class TextBlock : public Widget
{
public:
    TextBlock() = default;

    void SetText(const std::string& text) { m_Text = text; }
    const std::string& GetText() const { return m_Text; }

    void SetColor(const ImVec4& color) { m_Color = color; }
    void SetFontScale(float scale) { m_FontScale = scale; }
    void SetWrapWidth(float width) { m_WrapWidth = width; }

    virtual void NativeDraw() override;

private:
    std::string m_Text = "TextBlock";
    ImVec4 m_Color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    float m_FontScale = 1.0f;
    float m_WrapWidth = -1.0f;
};