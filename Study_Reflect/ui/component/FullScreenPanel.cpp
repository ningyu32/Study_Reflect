#include "FullScreenPanel.h"
#include "../Core/Slot.h"

FullScreenPanel::FullScreenPanel()
{
    SetName("FullScreenPanel_" + std::to_string(reinterpret_cast<uintptr_t>(this)));
}

void FullScreenPanel::NativeTick(float deltaTime)
{
    // 获取当前视口大小
    ImVec2 viewportSize = ImGui::GetMainViewport()->Size;
    ImVec2 workSize = ImGui::GetMainViewport()->WorkSize;
    ImVec2 workPos = ImGui::GetMainViewport()->WorkPos;

    // 计算实际可用区域
    ImVec2 anchoredPos, anchoredSize;

    anchoredPos.x = workPos.x + workSize.x * m_AnchorMin.x;
    anchoredPos.y = workPos.y + workSize.y * m_AnchorMin.y;
    anchoredSize.x = workSize.x * (m_AnchorMax.x - m_AnchorMin.x);
    anchoredSize.y = workSize.y * (m_AnchorMax.y - m_AnchorMin.y);

    // 应用边距
    if (m_bUsePercentMargin)
    {
        anchoredPos.x += anchoredSize.x * m_Margin.x;
        anchoredPos.y += anchoredSize.y * m_Margin.y;
        anchoredSize.x -= anchoredSize.x * m_Margin.x * 2;
        anchoredSize.y -= anchoredSize.y * m_Margin.y * 2;
    }
    else
    {
        anchoredPos.x += m_Margin.x;
        anchoredPos.y += m_Margin.y;
        anchoredSize.x -= m_Margin.x * 2;
        anchoredSize.y -= m_Margin.y * 2;
    }

    // 确保最小尺寸
    anchoredSize.x = std::max(anchoredSize.x, 100.0f);
    anchoredSize.y = std::max(anchoredSize.y, 100.0f);

    SetPosition(anchoredPos);
    SetSize(anchoredSize);
}

void FullScreenPanel::NativeDraw()
{
    ImVec2 panelPos = GetPosition();
    ImVec2 panelSize = GetSize();

    // 使用唯一的窗口名称（基于控件名称）
    std::string windowName = "##" + GetName();

    // 设置窗口位置和大小
    ImGui::SetNextWindowPos(panelPos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(panelSize, ImGuiCond_Always);

    // 窗口标志
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoBringToFrontOnFocus |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoScrollWithMouse;

    // 是否显示背景
    if (!m_bShowBackground)
    {
        flags |= ImGuiWindowFlags_NoBackground;
    }

    bool windowOpen = true;
    ImGui::Begin(windowName.c_str(), &windowOpen, flags);
    {
        // 如果显示背景，设置背景颜色
        if (m_bShowBackground)
        {
            ImDrawList* drawList = ImGui::GetWindowDrawList();
            drawList->AddRectFilled(panelPos,
                ImVec2(panelPos.x + panelSize.x, panelPos.y + panelSize.y),
                ImGui::GetColorU32(m_BackgroundColor));
        }

        // 获取窗口内的可用区域
        ImVec2 contentRegionMin = ImGui::GetWindowContentRegionMin();
        ImVec2 contentRegionMax = ImGui::GetWindowContentRegionMax();
        ImVec2 contentPos = ImVec2(panelPos.x + contentRegionMin.x,
            panelPos.y + contentRegionMin.y);
        ImVec2 contentSize = ImVec2(contentRegionMax.x - contentRegionMin.x,
            contentRegionMax.y - contentRegionMin.y);

        // 绘制子控件（在窗口的客户区内）
        DrawChildrenInRegion(contentPos, contentSize);
    }
    ImGui::End();
}

void FullScreenPanel::DrawChildrenInRegion(const ImVec2& regionPos, const ImVec2& regionSize)
{
    for (auto& child : m_Children)
    {
        if (child && child->IsVisible())
        {
            // 设置子控件在窗口客户区内的位置
            ImGui::SetCursorScreenPos(ImVec2(
                regionPos.x + child->GetPosition().x,
                regionPos.y + child->GetPosition().y
            ));

            child->Draw();
        }
    }
}

void FullScreenPanel::ArrangeChildren()
{
    for (auto& child : m_Children)
    {
        if (auto slot = child->GetSlot())
        {
            slot->Arrange(child);
        }
    }
}