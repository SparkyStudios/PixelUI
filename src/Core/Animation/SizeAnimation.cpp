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

#include <SparkyStudios/UI/Pixel/Core/Animation/SizeAnimation.h>

#if PI_ENABLE_ANIMATION

#include <SparkyStudios/UI/Pixel/Widgets/Widget.h>

namespace SparkyStudios::UI::Pixel
{
    SizeAnimation::SizeAnimation(
        const Size& startSize, const Size& endSize, PiTime duration, PiTime delay, Animation::TransitionFunction function)
        : Animation(duration, delay, function)
        , m_width(new WidthAnimation(startSize.w, endSize.w, duration, delay, function))
        , m_height(new HeightAnimation(startSize.h, endSize.h, duration, delay, function))
    {}

    void SizeAnimation::OnStart()
    {
        m_width->OnStart();
        m_height->OnStart();
    }

    void SizeAnimation::Run(PiTime percent)
    {
        m_width->Run(percent);
        m_height->Run(percent);
    }

    void SizeAnimation::OnFinish()
    {
        m_width->OnFinish();
        m_height->OnFinish();
    }
} // namespace SparkyStudios::UI::Pixel

#endif // PI_ENABLE_ANIMATION