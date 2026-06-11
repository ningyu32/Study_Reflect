#include "Widget.h"
#include "Slot.h"
#include "../Animation/WidgetAnimation.h"
#include <algorithm>

Widget::Widget()
{
    m_WorldTickTime = 0.0f;
}

void Widget::SetParent(std::shared_ptr<Widget> parent)
{
    m_Parent = parent;
}

void Widget::Draw()
{
    if (!m_bVisible) return;

    // 更新动画
    UpdateAnimations(ImGui::GetIO().DeltaTime);

    // 更新Tick
    NativeTick(ImGui::GetIO().DeltaTime);
    m_WorldTickTime += ImGui::GetIO().DeltaTime;

    // 应用位置和动画偏移
    ImGui::SetCursorPos(m_Position);

    // 保存当前光标位置
    ImVec2 cursorPos = ImGui::GetCursorPos();

    // 开始绘制
    NativeDraw();

    // 恢复光标位置并绘制子控件
    ImGui::SetCursorPos(cursorPos);
    DrawChildren();

    // 工具提示
    if (!m_TooltipText.empty() && ImGui::IsItemHovered())
    {
        ImGui::SetTooltip("%s", m_TooltipText.c_str());
    }
}

void Widget::DrawChildren()
{
    // 按ZOrder排序子控件
    std::vector<std::shared_ptr<Widget>> sortedChildren = m_Children;
    std::sort(sortedChildren.begin(), sortedChildren.end(),
        [](const std::shared_ptr<Widget>& a, const std::shared_ptr<Widget>& b) {
            return a->GetZOrder() < b->GetZOrder();
        });

    for (auto& child : sortedChildren)
    {
        if (child && child->IsVisible())
        {
            child->Draw();
        }
    }
}

void Widget::RebuildLayout()
{
    if (auto slot = GetSlot())
    {
        slot->Arrange(shared_from_this());
    }

    for (auto& child : m_Children)
    {
        child->RebuildLayout();
    }
}

void Widget::AddAnimation(std::shared_ptr<WidgetAnimation> animation)
{
    m_Animations.push_back(animation);
}

void Widget::RemoveAnimation(const std::string& name)
{
    m_Animations.erase(
        std::remove_if(m_Animations.begin(), m_Animations.end(),
            [&name](const std::shared_ptr<WidgetAnimation>& anim) {
                return anim->GetName() == name;
            }),
        m_Animations.end()
    );
}

void Widget::PlayAnimation(const std::string& name)
{
    for (auto& anim : m_Animations)
    {
        if (anim->GetName() == name)
        {
            anim->Play();
            break;
        }
    }
}

void Widget::StopAnimation(const std::string& name)
{
    for (auto& anim : m_Animations)
    {
        if (anim->GetName() == name)
        {
            anim->Stop();
            break;
        }
    }
}

void Widget::UpdateAnimations(float deltaTime)
{
    for (auto& anim : m_Animations)
    {
        if (anim->IsPlaying())
        {
            anim->Update(deltaTime, this);
        }
    }
}