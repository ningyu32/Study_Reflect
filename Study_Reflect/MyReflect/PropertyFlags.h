#pragma once
#include <cstdint>
#include <string>
#include <vector>

namespace Reflection {

    // 属性标志位（用于编辑器行为和序列化控制）
    enum class PropertyFlag : uint32_t {
        None = 0,
        Visible = 1 << 0,        // 编辑器中可见
        Editable = 1 << 1,        // 编辑器中可编辑
        Transient = 1 << 2,       // 不序列化
        Serialize = 1 << 3,       // 需要序列化
        DisplayName = 1 << 4,     // 有显示名称
        Tooltip = 1 << 5,         // 有提示文本
        Range = 1 << 6,           // 有数值范围
        Dropdown = 1 << 7,        // 下拉选择
        Multiline = 1 << 8,       // 多行文本
        AssetReference = 1 << 9,  // 资源引用
        ReadOnly = 1 << 10,       // 只读
        Category = 1 << 11,       // 有分类

        // 常用组合
        DefaultEditor = Visible | Editable | Serialize,
        ReadOnlyEditor = Visible | Serialize | ReadOnly,
        RuntimeOnly = Visible | Editable,
    };

    inline PropertyFlag operator|(PropertyFlag a, PropertyFlag b) 
    {
        return static_cast<PropertyFlag>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
    }

    inline PropertyFlag operator&(PropertyFlag a, PropertyFlag b) 
    {
        return static_cast<PropertyFlag>(static_cast<uint32_t>(a) & static_cast<uint32_t>(b));
    }

    inline bool HasFlag(PropertyFlag value, PropertyFlag flag) 
    {
        return (static_cast<uint32_t>(value) & static_cast<uint32_t>(flag)) != 0;
    }

    // 属性元数据（编辑器相关信息）
    struct PropertyMetaData {
        std::string DisplayName;      // 显示名称
        std::string TooltipText;      // 提示文本
        std::string Category;         // 分类
        float MinValue = 0.0f;        // 最小值
        float MaxValue = 100.0f;      // 最大值
        std::vector<std::string> DropdownOptions; // 下拉选项

        PropertyMetaData() = default;
    };

} // namespace Reflection