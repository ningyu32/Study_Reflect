#pragma once
#include "IEditorUI.h"
#include "../Object.h"
#include "../MyReflect/Registry.h"

namespace Reflection {

    // 属性绘制器（编辑器集成）
    class PropertyDrawer {
    public:
        static PropertyDrawer& Get() {
            static PropertyDrawer instance;
            return instance;
        }

        void SetEditorUI(IEditorUI* ui) { m_EditorUI = ui; }

        bool DrawObject(Object* obj) {
            if (!m_EditorUI || !obj) return false;

            ClassInfo* info = obj->GetClassInfo();
            if (!info) return false;

            bool modified = false;

            // 按分类组织属性
            std::map<std::string, std::vector<std::shared_ptr<IPropertyAccessor>>> categorizedProps;

            for (const auto& [name, prop] : info->GetProperties()) {
                if (!prop->IsVisible()) continue;

                std::string category = prop->GetMetaData().Category;
                if (category.empty()) category = "General";
                categorizedProps[category].push_back(prop);
            }

            // 绘制每个分类
            for (const auto& [category, props] : categorizedProps) {
                m_EditorUI->BeginCategory(category);
                for (auto& prop : props) {
                    if (DrawProperty(obj, prop)) {
                        modified = true;
                    }
                }
                m_EditorUI->EndCategory();
            }

            return modified;
        }

    private:
        bool DrawProperty(Object* obj, std::shared_ptr<IPropertyAccessor> prop) {
            if (!m_EditorUI || !prop) return false;

            PropertyValue value = prop->Get(obj);
            const auto& meta = prop->GetMetaData();
            std::string displayName = meta.DisplayName.empty() ? prop->GetPropertyName() : meta.DisplayName;

            bool modified = false;

            // 根据类型和元数据选择合适的控件
            std::string typeName = prop->GetTypeName();

            if (typeName == "float" && HasFlag(prop->GetFlags(), PropertyFlag::Range)) {
                float val = value.Get<float>();
                if (m_EditorUI->DrawFloatSlider(displayName, val, meta.MinValue, meta.MaxValue)) {
                    value.Set(val);
                    modified = true;
                }
            }
            else if (typeName == "int" && HasFlag(prop->GetFlags(), PropertyFlag::Range)) {
                int val = value.Get<int>();
                if (m_EditorUI->DrawIntSlider(displayName, val, (int)meta.MinValue, (int)meta.MaxValue)) {
                    value.Set(val);
                    modified = true;
                }
            }
            else if (HasFlag(prop->GetFlags(), PropertyFlag::Dropdown) && !meta.DropdownOptions.empty()) {
                // 下拉选择（需要将值映射到选项索引）
            }
            else {
                // 默认绘制
                if (m_EditorUI->DrawPropertyWithMeta(displayName, value, typeName, meta)) {
                    modified = true;
                }
            }

            if (modified && prop->IsEditable()) {
                prop->Set(obj, value);
            }

            return modified;
        }

        IEditorUI* m_EditorUI = nullptr;
    };

} // namespace Reflection