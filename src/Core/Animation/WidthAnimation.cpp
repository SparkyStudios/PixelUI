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

#include <SparkyStudios/UI/Pixel/Core/Animation/WidthAnimation.h>

#if PI_ENABLE_ANIMATION

#include <SparkyStudios/UI/Pixel/Widgets/Widget.h>

namespace SparkyStudios::UI::Pixel
{
    WidthAnimation::WidthAnimation(
        PiInt32 startWidth, PiInt32 endWidth, PiTime duration, PiTime delay, Animation::TransitionFunction function)
        : Animation(duration, delay, function)
        , m_startWidth(startWidth)
        , m_endWidth(endWidth)
        , m_delta(endWidth - startWidth)
    {}

    void WidthAnimation::OnStart()
    {
        m_control->SetWidth(m_startWidth);
    }

    void WidthAnimation::Run(PiTime percent)
    {
        const PiInt32 w = m_control->Width();
        if (w < m_endWidth)
            m_control->SetWidth(m_startWidth + m_delta * percent);
    }

    void WidthAnimation::OnFinish()
    {
        m_control->SetWidth(m_endWidth);
    }
} // namespace SparkyStudios::UI::Pixel

#endif // PI_ENABLE_ANIMATION