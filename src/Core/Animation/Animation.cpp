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
    static Animation::Transition gEaseTransition = { 0.25f, 0.1f, 0.25f, 1.0f };
    static Animation::Transition gEaseInTransition = { 0.42f, 0.0f, 1.0f, 1.0f };
    static Animation::Transition gEaseOutTransition = { 0.0f, 0.0f, 0.58f, 1.0f };
    static Animation::Transition gEaseInOutTransition = { 0.42f, 0.0f, 0.58f, 1.0f };

    constexpr PiUInt32 kNewtonIterations = 4;
    constexpr PiReal64 kNewtonMinSlope = 0.001;
    constexpr PiReal64 kSubdivisionPrecision = 0.0000001;
    constexpr PiUInt32 kSubdivisionMaxIterations = 10;

    constexpr PiUInt32 kSplineTableSize = 11;
    constexpr PiReal64 kSampleStepSize = 1.0 / (kSplineTableSize - 1.0);

    static PiReal64 A(PiReal64 a1, PiReal64 a2)
    {
        return 1.0 - 3.0 * a2 + 3.0 * a1;
    }

    static PiReal64 B(PiReal64 a1, PiReal64 a2)
    {
        return 3.0 * a2 - 6.0 * a1;
    }

    static PiReal64 C(PiReal64 a1)
    {
        return 3.0 * a1;
    }

    // Returns x(t) given t, x1, and x2, or y(t) given t, y1, and y2.
    static PiReal64 CalculateBezier(PiReal64 t, PiReal64 a1, PiReal64 a2)
    {
        return ((A(a1, a2) * t + B(a1, a2)) * t + C(a1)) * t;
    }

    // Returns dx/dt given t, x1, and x2, or dy/dt given t, y1, and y2.
    static PiReal64 GetSlope(PiReal64 t, PiReal64 a1, PiReal64 a2)
    {
        return 3.0 * A(a1, a2) * t * t + 2.0 * B(a1, a2) * t + C(a1);
    }

    static PiReal64 BinarySubdivide(PiReal64 x, PiReal64 a, PiReal64 b, PiReal64 mX1, PiReal64 mX2)
    {
        PiReal64 currentX, currentT, i = 0;
        do
        {
            currentT = a + (b - a) / 2.0;
            currentX = CalculateBezier(currentT, mX1, mX2) - x;
            if (currentX > 0.0)
            {
                b = currentT;
            }
            else
            {
                a = currentT;
            }
        } while (std::abs(currentX) > kSubdivisionPrecision && ++i < kSubdivisionMaxIterations);

        return currentT;
    }

    static PiReal64 NewtonRaphsonIterate(PiReal64 x, PiReal64 aGuessT, PiReal64 mX1, PiReal64 mX2)
    {
        for (PiUInt32 i = 0; i < kNewtonIterations; ++i)
        {
            PiReal64 currentSlope = GetSlope(aGuessT, mX1, mX2);
            if (currentSlope == 0.0)
            {
                return aGuessT;
            }

            PiReal64 currentX = CalculateBezier(aGuessT, mX1, mX2) - x;
            aGuessT -= currentX / currentSlope;
        }

        return aGuessT;
    }

    static PiReal64 LinearEasing(PiReal64 x)
    {
        return x;
    }

    Animation::Transition::Transition(PiReal32 x1_, PiReal32 y1_, PiReal32 x2_, PiReal32 y2_)
        : x1(x1_)
        , y1(y1_)
        , x2(x2_)
        , y2(y2_)
    {
        for (PiUInt32 i = 0; i < kSplineTableSize; ++i)
        {
            _samples[i] = CalculateBezier(i * kSampleStepSize, x1, x2);
        }
    }

    PiTime Animation::Transition::GetTFromX(PiReal64 x) const
    {
        PiReal64 intervalStart = 0.0;
        PiUInt32 currentSample = 1;
        PiUInt32 lastSample = kSplineTableSize - 1;

        for (; currentSample != lastSample && _samples[currentSample] <= x; ++currentSample)
        {
            intervalStart += kSampleStepSize;
        }
        --currentSample;

        // Interpolate to provide an initial guess for t
        PiReal64 dist = (x - _samples[currentSample]) / (_samples[currentSample + 1] - _samples[currentSample]);
        PiReal64 guessForT = intervalStart + dist * kSampleStepSize;

        PiReal64 initialSlope = GetSlope(guessForT, x1, x2);
        if (initialSlope >= kNewtonMinSlope)
        {
            return NewtonRaphsonIterate(x, guessForT, x1, x2);
        }
        else if (initialSlope == 0.0)
        {
            return guessForT;
        }
        else
        {
            return BinarySubdivide(x, intervalStart, intervalStart + kSampleStepSize, x1, x2);
        }
    }

    PiTime Animation::Transition::Ease(PiTime t) const
    {
        if (x1 == x2 && y1 == y2)
        {
            return LinearEasing(t);
        }

        // Don't waste time calculating extreme values.
        if (t <= 0.0 || t >= 1.0)
        {
            return t;
        }

        return CalculateBezier(GetTFromX(t), y1, y2);
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

    Animation::Animation(PiTime duration, PiTime delay, Animation::TransitionFunction function, bool loop)
        : m_started(false)
        , m_finished(false)
        , m_start(0.0)
        , m_duration(duration)
        , m_delay(delay)
        , m_ease(function)
        , m_customCurve(gLinearTransition)
        , m_loop(loop)
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

        if (!m_started)
            Start(time);

        const PiTime elapsed = time - m_start;

        if (elapsed < 0.0)
            return;

        const PiTime percent = Clamp(elapsed / m_duration, 0.0, 1.0);
        const PiTime eased = GetTransition().Ease(percent);

        Run(eased);

        if (percent >= 1.0)
            Finish(time);
    }

    void Animation::Finish(PiTime time)
    {
        m_finished = !m_loop;
        OnFinish();

        if (m_loop)
            m_started = false;
    }
} // namespace SparkyStudios::UI::Pixel

#endif // PI_ENABLE_ANIMATION
