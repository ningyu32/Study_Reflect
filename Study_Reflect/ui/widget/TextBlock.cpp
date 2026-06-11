#include "TextBlock.h"

void TextBlock::NativeDraw()
{
    if (!m_bVisible) return;

    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImVec2 size = GetSize();

    // 应用字体缩放
    if (m_FontScale != 1.0f)
    {
        ImGui::SetWindowFontScale(m_FontScale);
    }

    // 设置文本颜色
    ImGui::PushStyleColor(ImGuiCol_Text, m_Color);

    // 绘制文本
    if (m_WrapWidth > 0)
    {
        ImGui::PushTextWrapPos(ImGui::GetCursorPosX() + m_WrapWidth);
        ImGui::TextWrapped("%s", m_Text.c_str());
        ImGui::PopTextWrapPos();
    }
    else
    {
        ImGui::Text("%s", m_Text.c_str());
    }

    ImGui::PopStyleColor();

    // 恢复字体缩放
    if (m_FontScale != 1.0f)
    {
        ImGui::SetWindowFontScale(1.0f);
    }

    // 更新实际占用的尺寸
    ImVec2 textSize = ImGui::GetItemRectSize();
    if (textSize.x > 0 && textSize.y > 0)
    {
        // 可以选择更新Size为实际文本大小
        // SetSize(textSize);
    }

    // 处理工具提示
    if (!m_TooltipText.empty() && ImGui::IsItemHovered())
    {
        ImGui::SetTooltip("%s", m_TooltipText.c_str());
    }
}