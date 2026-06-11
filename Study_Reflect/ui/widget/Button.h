#pragma once
#include "../Core/Widget.h"
#include <string>
#include <functional>

class Button : public Widget
{
public:
    Button() = default;

    void SetText(const std::string& text) { m_Text = text; }
    const std::string& GetText() const { return m_Text; }

    void SetColor(const ImVec4& color) { m_Color = color; }
    void SetHoverColor(const ImVec4& color) { m_HoverColor = color; }
    void SetActiveColor(const ImVec4& color) { m_ActiveColor = color; }

    virtual void NativeDraw() override;

private:
    std::string m_Text = "Button";
    ImVec4 m_Color = ImVec4(0.26f, 0.59f, 0.98f, 1.0f);
    ImVec4 m_HoverColor = ImVec4(0.36f, 0.69f, 1.0f, 1.0f);
    ImVec4 m_ActiveColor = ImVec4(0.16f, 0.49f, 0.88f, 1.0f);
};