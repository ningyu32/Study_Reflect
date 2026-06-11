#include "Slot.h"
#include "Widget.h"

Slot::Slot()
{
}

void CanvasSlot::Arrange(std::shared_ptr<Widget> widget)
{
    if (!widget) return;

    widget->SetPosition(m_Position);
    widget->SetSize(m_Size);
}

void BoxSlot::Arrange(std::shared_ptr<Widget> widget)
{
    if (!widget) return;
    
}