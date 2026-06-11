#pragma once
#include "D:\C++Source\Study_Reflect\Study_Reflect/Object.h"
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include "imgui.h"

class WidgetManager;
class Slot;

class Widget : public Object, public std::enable_shared_from_this<Widget>
{
	friend class WidgetManager;
	friend class PanelWidget;
public:
    Widget();
    virtual ~Widget() = default;

    // 核心属性
    void SetVisibility(bool bVisible) { m_bVisible = bVisible; }
    bool IsVisible() const { return m_bVisible; }

    void SetEnabled(bool bEnabled) { m_bEnabled = bEnabled; }
    bool IsEnabled() const { return m_bEnabled; }

    void SetPosition(const ImVec2& pos) { m_Position = pos; }
    ImVec2 GetPosition() const { return m_Position; }

    void SetSize(const ImVec2& size) { m_Size = size; }
    ImVec2 GetSize() const { return m_Size; }

    void SetZOrder(int zOrder) { m_ZOrder = zOrder; }
    int GetZOrder() const { return m_ZOrder; }

    void SetTooltip(const std::string& tooltip) { m_TooltipText = tooltip; }

    // 层级关系
    void SetParent(std::shared_ptr<Widget> parent);
    std::shared_ptr<Widget> GetParent() const { return m_Parent.lock(); }

    void SetSlot(std::shared_ptr<Slot> slot) { m_Slot = slot; }
    std::shared_ptr<Slot> GetSlot() const { return m_Slot; }

    // 名称标识
    void SetName(const std::string& name) { m_Name = name; }
    const std::string& GetName() const { return m_Name; }

    // 动画支持
    void AddAnimation(std::shared_ptr<class WidgetAnimation> animation);
    void RemoveAnimation(const std::string& name);
    void PlayAnimation(const std::string& name);
    void StopAnimation(const std::string& name);
    void UpdateAnimations(float deltaTime);

    // 渲染相关
    virtual void Draw();
    virtual void RebuildLayout();

    // 事件回调
    std::function<void()> OnClicked;
    std::function<void()> OnHovered;
    std::function<void()> OnUnhovered;
public:
    // 是否为窗口控件（需要ImGui::Begin/End）
    virtual bool IsWindowWidget() const { return false; }

    // 获取唯一的ImGui ID
    ImGuiID GetImGuiID() const
    {
        return ImGui::GetID(m_Name.c_str());
    }
protected:
    virtual void NativeDraw() {}
    virtual void NativeTick(float deltaTime) {}
    virtual ImVec2 ComputeDesiredSize() const { return m_Size; }

    void DrawChildren();

    std::string m_Name;
    bool m_bVisible = true;
    bool m_bEnabled = true;
    ImVec2 m_Position = { 0, 0 };
    ImVec2 m_Size = { 100, 50 };
    int m_ZOrder = 0;
    std::string m_TooltipText;

    std::weak_ptr<Widget> m_Parent;
    std::shared_ptr<Slot> m_Slot;
    std::vector<std::shared_ptr<Widget>> m_Children;
    std::vector<std::shared_ptr<class WidgetAnimation>> m_Animations;

    float m_WorldTickTime = 0.0f;
};