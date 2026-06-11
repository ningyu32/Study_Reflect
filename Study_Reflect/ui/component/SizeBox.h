#pragma once
#include "PanelWidget.h"

// 尺寸盒子 - 强制子控件使用指定尺寸
class SizeBox : public PanelWidget
{
public:
    SizeBox() = default;

    void SetWidthOverride(float width) { m_WidthOverride = width; }
    void SetHeightOverride(float height) { m_HeightOverride = height; }
    void SetMinDesiredWidth(float width) { m_MinDesiredWidth = width; }
    void SetMinDesiredHeight(float height) { m_MinDesiredHeight = height; }
    void SetMaxDesiredWidth(float width) { m_MaxDesiredWidth = width; }
    void SetMaxDesiredHeight(float height) { m_MaxDesiredHeight = height; }

    virtual void NativeDraw() override;

protected:
    virtual void ArrangeChildren() override;

    float m_WidthOverride = 0.0f;
    float m_HeightOverride = 0.0f;
    float m_MinDesiredWidth = 0.0f;
    float m_MinDesiredHeight = 0.0f;
    float m_MaxDesiredWidth = FLT_MAX;
    float m_MaxDesiredHeight = FLT_MAX;
};