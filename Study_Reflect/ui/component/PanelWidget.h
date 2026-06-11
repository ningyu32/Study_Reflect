#pragma once
#include "../Core/Widget.h"
#include <vector>

// 面板控件基类 - 可以包含子控件
class PanelWidget : public Widget
{
public:
    PanelWidget() = default;
    virtual ~PanelWidget() = default;

    // 添加子控件
    template<typename T>
    std::shared_ptr<T> AddChild(std::shared_ptr<T> child)
    {
        child->SetParent(shared_from_this());
        m_Children.push_back(child);
        return child;
    }

    void RemoveChild(std::shared_ptr<Widget> child);
    void ClearChildren();

    // 布局方向
    enum class Orientation
    {
        Horizontal,
        Vertical
    };

protected:
    virtual void ArrangeChildren() = 0;
};