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

#include <SparkyStudios/UI/Pixel/Core/Animation/HeightAnimation.h>

#if PI_ENABLE_ANIMATION

#include <SparkyStudios/UI/Pixel/Widgets/Widget.h>

namespace SparkyStudios::UI::Pixel
{
    HeightAnimation::HeightAnimation(
        PiInt32 startHeight, PiInt32 endHeight, PiTime duration, PiTime delay, Animation::TransitionFunction function)
        : Animation(duration, delay, function)
        , m_startHeight(startHeight)
        , m_endHeight(endHeight)
        , m_delta(endHeight - startHeight)
    {}

    void HeightAnimation::OnStart()
    {
        m_widget->SetHeight(m_startHeight);
    }

    void HeightAnimation::Run(PiTime percent)
    {
        const PiInt32 h = m_widget->Height();
        if (h < m_endHeight)
            m_widget->SetHeight(m_startHeight + m_delta * percent);
    }

    void HeightAnimation::OnFinish()
    {
        m_widget->SetHeight(m_endHeight);
    }
} // namespace SparkyStudios::UI::Pixel

#endif // PI_ENABLE_ANIMATION