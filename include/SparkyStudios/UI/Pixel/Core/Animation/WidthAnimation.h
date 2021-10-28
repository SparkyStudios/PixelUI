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

#pragma once

#ifndef PIXEL_UI_WIDTHANIMATION_H
#define PIXEL_UI_WIDTHANIMATION_H

#include <SparkyStudios/UI/Pixel/Core/Animation/Animation.h>

#if PI_ENABLE_ANIMATION

namespace SparkyStudios::UI::Pixel
{
    class PI_EXPORT WidthAnimation : public Animation
    {
    public:
        WidthAnimation(
            PiInt32 startWidth,
            PiInt32 endWidth,
            PiTime duration,
            PiTime delay = 0.0,
            TransitionFunction function = TransitionFunction::None);

        void OnStart() override;

        void Run(PiTime percent) override;

        void OnFinish() override;

    protected:
        PiInt32 m_startWidth;
        PiInt32 m_endWidth;
        PiInt32 m_delta;
    };
} // namespace SparkyStudios::UI::Pixel

#endif // PI_ENABLE_ANIMATION

#endif // PIXEL_UI_WIDTHANIMATION_H
