#pragma once
#include <type_traits>
#include <vector>
#include <memory>
#include <string>

class Object;

//namespace Reflection 
//{
//    // 检测是否为容器类型
//    template<typename T, typename = void>
//    struct is_container : std::false_type {};
//
//    template<typename T>
//    struct is_container<T, std::void_t<
//        typename T::value_type,
//        typename T::iterator,
//        typename T::const_iterator,
//        decltype(std::declval<T>().begin()),
//        decltype(std::declval<T>().end())
//        >> : std::true_type{};
//
//    template<typename T>
//    inline constexpr bool is_container_v = is_container<T>::value;
//
//    // 检测是否为智能指针
//    template<typename T>
//    struct is_smart_ptr : std::false_type {};
//
//    template<typename T>
//    struct is_smart_ptr<std::unique_ptr<T>> : std::true_type {};
//
//    template<typename T>
//    struct is_smart_ptr<std::shared_ptr<T>> : std::true_type {};
//
//    template<typename T>
//    inline constexpr bool is_smart_ptr_v = is_smart_ptr<T>::value;
//
//    // 检测是否为Object派生类
//    template<typename T>
//    struct is_object_derived 
//    {
//        static constexpr bool value = std::is_base_of_v<Object, T> && !std::is_same_v<Object, T>;
//    };
//
//    template<typename T>
//    inline constexpr bool is_object_derived_v = is_object_derived<T>::value;
//
//    // 获取基础类型名称（用于序列化）
//    template<typename T>
//    std::string GetTypeName() {
//        if constexpr (std::is_same_v<T, int>) return "int";
//        else if constexpr (std::is_same_v<T, float>) return "float";
//        else if constexpr (std::is_same_v<T, double>) return "double";
//        else if constexpr (std::is_same_v<T, bool>) return "bool";
//        else if constexpr (std::is_same_v<T, std::string>) return "string";
//        else if constexpr (is_object_derived_v<T>) return "object";
//        else if constexpr (is_container_v<T>) return "array";
//        else return "unknown";
//    }
//
//} // namespace Reflection