#pragma once
#include "PanelWidget.h"
#include "../core/Slot.h"

// Canvas面板 - 支持绝对定位和锚点
class CanvasPanel : public PanelWidget
{
public:
    CanvasPanel() = default;

    // 添加子控件并设置Canvas插槽
    template<typename T>
    std::shared_ptr<T> AddChildToCanvas(std::shared_ptr<T> child,
        const ImVec2& position,
        const ImVec2& size = { 100, 50 })
    {
        auto slot = std::make_shared<CanvasSlot>();
        slot->SetPosition(position);
        slot->SetSize(size);
        child->SetSlot(slot);

        return AddChild(child);
    }

    virtual void NativeDraw() override;

protected:
    virtual void ArrangeChildren() override;
};