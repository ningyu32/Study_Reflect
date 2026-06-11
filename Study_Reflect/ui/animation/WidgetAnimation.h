#pragma once
#include <string>
#include <functional>
#include "imgui.h"

class Widget;

class WidgetAnimation
{
public:
    WidgetAnimation(const std::string& name);

    void Play();
    void Stop();
    void Pause();
    void Resume();
    bool IsPlaying() const { return m_bPlaying && !m_bPaused; }
    const std::string& GetName() const { return m_Name; }

    // 动画属性设置
    void SetDuration(float duration) { m_Duration = duration; }
    void SetDelay(float delay) { m_Delay = delay; }
    void SetLooping(bool looping) { m_bLooping = looping; }
    void SetEasingFunction(std::function<float(float)> easing) { m_EasingFunction = easing; }

    // 动画更新回调
    std::function<void(Widget*, float)> OnUpdate;
    std::function<void(Widget*)> OnCompleted;

    void Update(float deltaTime, Widget* widget);
    float GetProgress() const { return m_Progress; }

private:
    std::string m_Name;
    float m_Duration = 1.0f;
    float m_Delay = 0.0f;
    float m_Elapsed = 0.0f;
    float m_Progress = 0.0f;
    bool m_bPlaying = false;
    bool m_bPaused = false;
    bool m_bLooping = false;
    std::function<float(float)> m_EasingFunction;
};