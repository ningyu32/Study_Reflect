#include "Button.h"

void Button::NativeDraw()
{
    if (!m_bVisible) return;

    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImVec2 size = GetSize();

    // 保存原始样式颜色
    ImVec4 originalColor = ImGui::GetStyle().Colors[ImGuiCol_Button];
    ImVec4 originalHoverColor = ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered];
    ImVec4 originalActiveColor = ImGui::GetStyle().Colors[ImGuiCol_ButtonActive];

    // 应用自定义颜色
    ImGui::PushStyleColor(ImGuiCol_Button, m_Color);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, m_HoverColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, m_ActiveColor);

    // 如果按钮被禁用，设置禁用样式
    if (!m_bEnabled)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.5f);
    }

    // 绘制按钮
    bool clicked = ImGui::Button(m_Text.c_str(), size);

    // 处理点击事件
    if (clicked && m_bEnabled)
    {
        if (OnClicked)
        {
            OnClicked();
        }
    }

    // 处理悬停事件
    if (ImGui::IsItemHovered())
    {
        if (OnHovered)
        {
            OnHovered();
        }

        // 显示工具提示
        if (!m_TooltipText.empty())
        {
            ImGui::SetTooltip("%s", m_TooltipText.c_str());
        }
    }
    else if (ImGui::IsItemActive())
    {
        // 按钮被按下但未释放
    }

    // 恢复原始样式
    ImGui::PopStyleColor(3);
    if (!m_bEnabled)
    {
        ImGui::PopStyleVar();
    }

    // 更新尺寸信息
    ImVec2 itemSize = ImGui::GetItemRectSize();
    if (itemSize.x > 0 && itemSize.y > 0)
    {
        // 可以选择更新Size为实际渲染大小
        // SetSize(itemSize);
    }
}