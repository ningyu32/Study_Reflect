#include "WidgetAnimation.h"

WidgetAnimation::WidgetAnimation(const std::string& name)
    : m_Name(name)
{
    // 默认线性缓动
    m_EasingFunction = [](float t) { return t; };
}

void WidgetAnimation::Play()
{
    m_bPlaying = true;
    m_bPaused = false;
    m_Elapsed = 0.0f;
    m_Progress = 0.0f;
}

void WidgetAnimation::Stop()
{
    m_bPlaying = false;
    m_bPaused = false;
    m_Elapsed = 0.0f;
    m_Progress = 0.0f;
}

void WidgetAnimation::Pause()
{
    m_bPaused = true;
}

void WidgetAnimation::Resume()
{
    m_bPaused = false;
}

void WidgetAnimation::Update(float deltaTime, Widget* widget)
{
    if (!m_bPlaying || m_bPaused) return;

    // 延迟处理
    if (m_Elapsed < m_Delay)
    {
        m_Elapsed += deltaTime;
        return;
    }

    float animTime = m_Elapsed - m_Delay;
    m_Progress = std::min(1.0f, animTime / m_Duration);

    // 应用缓动函数
    float easedProgress = m_EasingFunction(m_Progress);

    // 更新回调
    if (OnUpdate)
    {
        OnUpdate(widget, easedProgress);
    }

    // 完成处理
    if (m_Progress >= 1.0f)
    {
        if (m_bLooping)
        {
            m_Elapsed = m_Delay;
            m_Progress = 0.0f;
        }
        else
        {
            m_bPlaying = false;
            if (OnCompleted)
            {
                OnCompleted(widget);
            }
        }
    }

    m_Elapsed += deltaTime;
}