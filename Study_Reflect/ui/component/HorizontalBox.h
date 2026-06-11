#pragma once
#include "PanelWidget.h"
#include "../core/Slot.h"

// 水平盒子 - 子控件水平排列
class HorizontalBox : public PanelWidget
{
public:
    HorizontalBox() = default;

    void SetSpacing(float spacing) { m_Spacing = spacing; }

    template<typename T>
    std::shared_ptr<T> AddChildToHorizontalBox(std::shared_ptr<T> child,
        BoxSlot::SizeRule sizeRule = BoxSlot::SizeRule::Auto,
        float fillRatio = 1.0f)
    {
        auto slot = std::make_shared<BoxSlot>();
        slot->SetSizeRule(sizeRule);
        slot->SetFillRatio(fillRatio);
        child->SetSlot(slot);

        return AddChild(child);
    }

    virtual void NativeDraw() override;

protected:
    virtual void ArrangeChildren() override;

    float m_Spacing = 5.0f;
};