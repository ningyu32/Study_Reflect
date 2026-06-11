#pragma once
#include <memory>
#include "imgui.h"

class Widget;

class Slot
{
public:
    Slot();
    virtual ~Slot() = default;

    virtual void Arrange(std::shared_ptr<Widget> widget) = 0;

    void SetPadding(const ImVec2& padding) { m_Padding = padding; }
    ImVec2 GetPadding() const { return m_Padding; }

    void SetHorizontalAlignment(float alignment) { m_HorizontalAlignment = alignment; }
    void SetVerticalAlignment(float alignment) { m_VerticalAlignment = alignment; }

protected:
    ImVec2 m_Padding = { 0, 0 };
    float m_HorizontalAlignment = 0.0f; // 0=left, 0.5=center, 1=right
    float m_VerticalAlignment = 0.0f;   // 0=top, 0.5=center, 1=bottom
};

class CanvasSlot : public Slot
{
public:
    void SetPosition(const ImVec2& pos) { m_Position = pos; }
    void SetSize(const ImVec2& size) { m_Size = size; }
    void SetAnchors(const ImVec2& min, const ImVec2& max) { m_AnchorMin = min; m_AnchorMax = max; }

    virtual void Arrange(std::shared_ptr<Widget> widget) override;

private:
    ImVec2 m_Position = { 0, 0 };
    ImVec2 m_Size = { 100, 50 };
    ImVec2 m_AnchorMin = { 0, 0 };
    ImVec2 m_AnchorMax = { 0, 0 };
};

class BoxSlot : public Slot
{
public:
    enum class SizeRule
    {
        Auto,
        Fill
    };

    void SetSizeRule(SizeRule rule) { m_SizeRule = rule; }
    void SetFillRatio(float ratio) { m_FillRatio = ratio; }

    SizeRule GetSizeRule() { return m_SizeRule; }
    float GetFillRatio() { return m_FillRatio; }

    virtual void Arrange(std::shared_ptr<Widget> widget) override;

private:
    SizeRule m_SizeRule = SizeRule::Auto;
    float m_FillRatio = 1.0f;
};