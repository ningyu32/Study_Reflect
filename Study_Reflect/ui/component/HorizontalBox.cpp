#include "HorizontalBox.h"

void HorizontalBox::NativeDraw()
{
    ImVec2 boxPos = ImGui::GetCursorScreenPos();
    ImVec2 boxSize = GetSize();

    // ¼ÆËã×ÜÌî³ä±ÈÀý
    float totalFillRatio = 0.0f;
    float totalAutoSize = 0.0f;

    for (auto& child : m_Children)
    {
        if (!child || !child->IsVisible()) continue;

        auto slot = std::dynamic_pointer_cast<BoxSlot>(child->GetSlot());
        if (slot && slot->GetSizeRule() == BoxSlot::SizeRule::Fill)
        {
            totalFillRatio += slot->GetFillRatio();
        }
        else
        {
            totalAutoSize += child->GetSize().x;
        }
    }

    float availableFillSpace = std::max(0.0f, boxSize.x - totalAutoSize - m_Spacing * (m_Children.size() - 1));
    float currentX = boxPos.x;

    for (auto& child : m_Children)
    {
        if (!child || !child->IsVisible()) continue;

        auto slot = std::dynamic_pointer_cast<BoxSlot>(child->GetSlot());
        float childWidth = child->GetSize().x;

        if (slot && slot->GetSizeRule() == BoxSlot::SizeRule::Fill)
        {
            childWidth = (availableFillSpace * slot->GetFillRatio()) / totalFillRatio;
        }

        ImVec2 childSize = ImVec2(childWidth, boxSize.y);
        child->SetSize(childSize);

        ImGui::SetCursorScreenPos(ImVec2(currentX, boxPos.y));
        child->Draw();

        currentX += childWidth + m_Spacing;
    }
}

void HorizontalBox::ArrangeChildren()
{
    for (auto& child : m_Children)
    {
        if (auto slot = child->GetSlot())
        {
            slot->Arrange(child);
        }
    }
}