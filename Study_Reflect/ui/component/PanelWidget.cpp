#include "PanelWidget.h"
#include "../Core/Slot.h"

void PanelWidget::RemoveChild(std::shared_ptr<Widget> child)
{
    auto it = std::find(m_Children.begin(), m_Children.end(), child);
    if (it != m_Children.end())
    {
        (*it)->SetParent(nullptr);
        (*it)->SetSlot(nullptr);
        m_Children.erase(it);
    }
}

void PanelWidget::ClearChildren()
{
    for (auto& child : m_Children)
    {
        child->SetParent(nullptr);
        child->SetSlot(nullptr);
    }
    m_Children.clear();
}