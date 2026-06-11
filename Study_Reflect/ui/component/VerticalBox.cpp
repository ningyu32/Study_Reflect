#include "VerticalBox.h"

void VerticalBox::NativeDraw()
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
            totalAutoSize += child->GetSize().y;
        }
    }

    float availableFillSpace = std::max(0.0f, boxSize.y - totalAutoSize - m_Spacing * (m_Children.size() - 1));
    float currentY = boxPos.y;

    for (auto& child : m_Children)
    {
        if (!child || !child->IsVisible()) continue;

        auto slot = std::dynamic_pointer_cast<BoxSlot>(child->GetSlot());
        float childHeight = child->GetSize().y;

        if (slot && slot->GetSizeRule() == BoxSlot::SizeRule::Fill)
        {
            childHeight = (availableFillSpace * slot->GetFillRatio()) / totalFillRatio;
        }

        ImVec2 childSize = ImVec2(boxSize.x, childHeight);
        child->SetSize(childSize);

        ImGui::SetCursorScreenPos(ImVec2(boxPos.x, currentY));
        child->Draw();

        currentY += childHeight + m_Spacing;
    }
}

void VerticalBox::ArrangeChildren()
{
    for (auto& child : m_Children)
    {
        if (auto slot = child->GetSlot())
        {
            slot->Arrange(child);
        }
    }
}