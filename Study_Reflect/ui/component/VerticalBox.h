#pragma once
#include "PanelWidget.h"
#include "../core/Slot.h"

// 垂直盒子 - 子控件垂直排列
class VerticalBox : public PanelWidget
{
public:
    VerticalBox() = default;

    void SetSpacing(float spacing) { m_Spacing = spacing; }

    template<typename T>
    std::shared_ptr<T> AddChildToVerticalBox(std::shared_ptr<T> child,
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
