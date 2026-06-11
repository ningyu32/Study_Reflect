#pragma once
#include <memory>
#include <vector>
#include <map>
#include <string>
#include "Widget.h"

class WidgetManager
{
public:
    static WidgetManager& Get()
    {
        static WidgetManager instance;
        return instance;
    }

    // 创建和注册控件
    template<typename T, typename... Args>
    std::shared_ptr<T> CreateWidget(const std::string& name, Args&&... args)
    {
        auto widget = std::make_shared<T>(std::forward<Args>(args)...);
        widget->SetName(name);
        RegisterWidget(widget);
        return widget;
    }

    void RegisterWidget(std::shared_ptr<Widget> widget);
    void UnregisterWidget(const std::string& name);

    // 获取控件
    std::shared_ptr<Widget> FindWidget(const std::string& name);

    template<typename T>
    std::shared_ptr<T> FindWidget(const std::string& name)
    {
        return std::dynamic_pointer_cast<T>(FindWidget(name));
    }

    // 根控件管理
    void AddToViewport(std::shared_ptr<Widget> widget, int zOrder = 0);
    void RemoveFromViewport(std::shared_ptr<Widget> widget);

    // 全局渲染
    void DrawAll();
    void TickAll(float deltaTime);

    // 获取所有根控件
    std::vector<std::shared_ptr<Widget>>& GetRootWidgets() { return m_RootWidgets; }

private:
    WidgetManager() = default;
    ~WidgetManager() = default;
    WidgetManager(const WidgetManager&) = delete;
    WidgetManager& operator=(const WidgetManager&) = delete;

    std::map<std::string, std::shared_ptr<Widget>> m_WidgetMap;
    std::vector<std::shared_ptr<Widget>> m_RootWidgets;
};