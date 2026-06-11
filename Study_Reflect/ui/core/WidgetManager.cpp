#include "WidgetManager.h"
#include <algorithm>

void WidgetManager::RegisterWidget(std::shared_ptr<Widget> widget)
{
    if (widget && !widget->GetName().empty())
    {
        m_WidgetMap[widget->GetName()] = widget;
    }
}

void WidgetManager::UnregisterWidget(const std::string& name)
{
    auto it = m_WidgetMap.find(name);
    if (it != m_WidgetMap.end())
    {
        // 从根控件列表中移除
        auto& roots = m_RootWidgets;
        roots.erase(
            std::remove(roots.begin(), roots.end(), it->second),
            roots.end()
        );

        m_WidgetMap.erase(it);
    }
}

std::shared_ptr<Widget> WidgetManager::FindWidget(const std::string& name)
{
    auto it = m_WidgetMap.find(name);
    if (it != m_WidgetMap.end())
    {
        return it->second;
    }
    return nullptr;
}

void WidgetManager::AddToViewport(std::shared_ptr<Widget> widget, int zOrder)
{
    if (widget)
    {
        widget->SetZOrder(zOrder);
        m_RootWidgets.push_back(widget);

        // 按ZOrder排序
        std::sort(m_RootWidgets.begin(), m_RootWidgets.end(),
            [](const std::shared_ptr<Widget>& a, const std::shared_ptr<Widget>& b) {
                return a->GetZOrder() < b->GetZOrder();
            });
    }
}

void WidgetManager::RemoveFromViewport(std::shared_ptr<Widget> widget)
{
    m_RootWidgets.erase(
        std::remove(m_RootWidgets.begin(), m_RootWidgets.end(), widget),
        m_RootWidgets.end()
    );
}

void WidgetManager::DrawAll()
{
    for (auto& widget : m_RootWidgets)
    {
        if (widget && widget->IsVisible())
        {
            widget->Draw();
        }
    }
}

void WidgetManager::TickAll(float deltaTime)
{
    for (auto& widget : m_RootWidgets)
    {
        if (widget && widget->IsVisible())
        {
            widget->NativeTick(deltaTime);
        }
    }
}