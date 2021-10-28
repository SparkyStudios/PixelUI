// Copyright (c) 2021-present Sparky Studios. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <SparkyStudios/UI/Pixel/Core/Animation/Animation.h>

#if PI_ENABLE_ANIMATION

#include <cmath>

#include <SparkyStudios/UI/Pixel/Core/Utility.h>

namespace SparkyStudios::UI::Pixel
{
    static Animation::WidgetMap gAnimatedWidgets = {};
    static Animation::List gAnimationList = {};

    static Animation::Transition gLinearTransition = { 0.0f, 0.0f, 1.0f, 1.0f };
    static Animation::Transition gEaseTransition = { 0.25f, 1.0f, 0.25f, 1.0f };
    static Animation::Transition gEaseInTransition = { 0.42f, 0.0f, 1.0f, 1.0f };
    static Animation::Transition gEaseOutTransition = { 0.0f, 0.0f, 0.58f, 1.0f };
    static Animation::Transition gEaseInOutTransition = { 0.42f, 0.0f, 0.58f, 1.0f };

    static PiTime CalculateAnimationTime(PiTime time, const Animation::Transition& curve)
    {
        // clang-format off
        return
            curve.p1 * std::pow(1 - time, 3) +
            3 * curve.p2 * std::pow(1 - time, 2) * time +
            3 * curve.p3 * (1 - time) * std::pow(time, 2) +
            curve.p4 * std::pow(time, 3);
        // clang-format on
    }

    void Animation::Add(Widget* widget, Animation* animation)
    {
        animation->m_control = widget;
        gAnimatedWidgets[widget].push_back(animation);
        gAnimationList.push_back(animation);
    }

    void Animation::Cancel(Widget* widget, Animation* animation)
    {
        if (animation != nullptr)
        {
            gAnimatedWidgets[widget].remove(animation);
            gAnimationList.remove(animation);
            delete animation;
        }
        else
        {
            for (auto it = gAnimationList.begin(); it != gAnimationList.end();)
            {
                if ((*it)->m_control != widget)
                {
                    ++it;
                    continue;
                }

                Animation* animation = *it;
                it = gAnimationList.erase(it);
                delete animation;
            }

            gAnimatedWidgets.erase(widget);
        }
    }

    void Animation::Tick(PiTime time)
    {
        for (auto it = gAnimationList.begin(); it != gAnimationList.end();)
        {
            Animation* animation = *it;
            animation->Update(time);

            if (animation->Finished())
            {
                it = gAnimationList.erase(it);
                delete animation;
            }
            else
            {
                ++it;
            }
        }
    }

    Animation::Animation(PiTime duration, PiTime delay, Animation::TransitionFunction function)
        : m_started(false)
        , m_finished(false)
        , m_start(0.0)
        , m_duration(duration)
        , m_delay(delay)
        , m_ease(function)
        , m_customCurve()
    {}

    void Animation::SetTransition(const Transition& curve)
    {
        m_ease = TransitionFunction::Custom;
        m_customCurve = curve;
    }

    void Animation::SetTransition(TransitionFunction function)
    {
        m_ease = function;
    }

    const Animation::Transition& Animation::GetTransition() const
    {
        switch (m_ease)
        {
        case TransitionFunction::Custom:
            return m_customCurve;
        default:
        case TransitionFunction::Linear:
            return gLinearTransition;
        case TransitionFunction::Ease:
            return gEaseTransition;
        case TransitionFunction::EaseIn:
            return gEaseInTransition;
        case TransitionFunction::EaseOut:
            return gEaseOutTransition;
        case TransitionFunction::EaseInOut:
            return gEaseInOutTransition;
        }
    }

    bool Animation::Started()
    {
        return m_started;
    }

    bool Animation::Finished()
    {
        return m_finished;
    }

    void Animation::Start(PiTime time)
    {
        m_start = time + m_delay;
        m_started = true;
        OnStart();
    }

    void Animation::Update(PiTime time)
    {
        if (m_finished)
            return;

        const PiTime elapsed = time - m_start;

        if (elapsed < 0.0)
            return;

        if (!m_started)
            Start(time);

        const PiTime percent = Clamp(elapsed / m_duration, 0.0, 1.0);
        const PiTime eased = CalculateAnimationTime(percent, GetTransition());

        Run(eased);

        if (percent >= 1.0)
            Finish();
    }

    void Animation::Finish()
    {
        m_finished = true;
        OnFinish();
    }
} // namespace SparkyStudios::UI::Pixel

#endif // PI_ENABLE_ANIMATION
