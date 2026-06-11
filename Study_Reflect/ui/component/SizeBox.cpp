#include "SizeBox.h"
#include <algorithm>
#include "../core/Slot.h"

void SizeBox::NativeDraw()
{
    ImVec2 boxPos = ImGui::GetCursorScreenPos();
    ImVec2 desiredSize = GetSize();

    // 应用尺寸限制
    if (m_WidthOverride > 0) desiredSize.x = m_WidthOverride;
    if (m_HeightOverride > 0) desiredSize.y = m_HeightOverride;

    desiredSize.x = std::clamp(desiredSize.x, m_MinDesiredWidth, m_MaxDesiredWidth);
    desiredSize.y = std::clamp(desiredSize.y, m_MinDesiredHeight, m_MaxDesiredHeight);

    SetSize(desiredSize);

    ImGui::SetCursorScreenPos(boxPos);
    ImGui::BeginChild(("SizeBox_" + GetName()).c_str(), desiredSize);
    {
        for (auto& child : m_Children)
        {
            if (child && child->IsVisible())
            {
                child->Draw();
            }
        }
    }
    ImGui::EndChild();
}

void SizeBox::ArrangeChildren()
{
    for (auto& child : m_Children)
    {
        if (auto slot = child->GetSlot())
        {
            slot->Arrange(child);
        }
    }
}