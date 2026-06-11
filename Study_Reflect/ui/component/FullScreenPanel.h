#pragma once
#include "PanelWidget.h"

class FullScreenPanel : public PanelWidget
{
public:
    FullScreenPanel();
    virtual ~FullScreenPanel() = default;

    // 标记为窗口控件
    virtual bool IsWindowWidget() const override { return true; }

    virtual void NativeTick(float deltaTime) override;
    virtual void NativeDraw() override;

    void DrawChildrenInRegion(const ImVec2& regionPos, const ImVec2& regionSize);

    void SetMargin(const ImVec2& margin) { m_Margin = margin; }
    void SetUsePercentMargin(bool usePercent) { m_bUsePercentMargin = usePercent; }
    void SetAnchors(float minX, float minY, float maxX, float maxY)
    {
        m_AnchorMin = ImVec2(minX, minY);
        m_AnchorMax = ImVec2(maxX, maxY);
    }

    // 背景颜色设置
    void SetBackgroundColor(const ImVec4& color) { m_BackgroundColor = color; }
    void SetShowBackground(bool show) { m_bShowBackground = show; }

protected:
    virtual void ArrangeChildren() override;

private:
    ImVec2 m_Margin = ImVec2(0, 0);
    bool m_bUsePercentMargin = false;
    ImVec2 m_AnchorMin = ImVec2(0, 0);
    ImVec2 m_AnchorMax = ImVec2(1, 1);
    ImVec4 m_BackgroundColor = ImVec4(0, 0, 0, 0.5f);
    bool m_bShowBackground = false;
};