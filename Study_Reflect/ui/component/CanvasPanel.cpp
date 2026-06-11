#include "CanvasPanel.h"
#include "../core/Slot.h"

void CanvasPanel::NativeDraw()
{
    ImVec2 canvasPos = ImGui::GetCursorScreenPos();
    ImVec2 canvasSize = GetSize();

    // 绘制Canvas背景（可选）
    ImGui::SetCursorScreenPos(canvasPos);
    ImGui::InvisibleButton("Canvas", canvasSize);

    // 绘制子控件
    for (auto& child : m_Children)
    {
        if (child && child->IsVisible())
        {
            auto slot = std::dynamic_pointer_cast<CanvasSlot>(child->GetSlot());
            if (slot)
            {
                ImVec2 childPos = ImVec2(
                    canvasPos.x + child->GetPosition().x,
                    canvasPos.y + child->GetPosition().y
                );
                ImGui::SetCursorScreenPos(childPos);
                child->Draw();
            }
        }
    }
}

void CanvasPanel::ArrangeChildren()
{
    // Canvas使用绝对定位，每个子控件独立设置位置
    for (auto& child : m_Children)
    {
        if (auto slot = child->GetSlot())
        {
            slot->Arrange(child);
        }
    }
}