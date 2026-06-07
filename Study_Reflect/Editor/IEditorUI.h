#pragma once
#include <string>
#include <functional>
#include "../MyReflect/PropertyValue.h"
#include "../MyReflect/PropertyFlags.h"

class Object;

namespace Reflection {
    class IPropertyAccessor;

    // 编辑器UI接口
    class IEditorUI {
    public:
        virtual ~IEditorUI() = default;

        // 绘制属性
        virtual bool DrawProperty(const std::string& label, PropertyValue& value,
            const std::string& typeName) = 0;

        // 带元数据的属性绘制
        virtual bool DrawPropertyWithMeta(const std::string& label, PropertyValue& value,
            const std::string& typeName,
            const PropertyMetaData& meta) = 0;

        // 对象绘制
        virtual bool DrawObject(Object* obj) = 0;

        // 控件绘制
        virtual bool DrawFloatSlider(const std::string& label, float& value,
            float min, float max) = 0;
        virtual bool DrawIntSlider(const std::string& label, int& value,
            int min, int max) = 0;
        virtual bool DrawDropdown(const std::string& label, int& selected,
            const std::vector<std::string>& options) = 0;
        virtual bool DrawColorPicker(const std::string& label, float color[4]) = 0;

        // 开始/结束区域
        virtual void BeginCategory(const std::string& name) = 0;
        virtual void EndCategory() = 0;
    };

} // namespace Reflection