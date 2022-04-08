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

#ifndef PIXEL_UI_SIZEANIMATION_H
#define PIXEL_UI_SIZEANIMATION_H

#include <SparkyStudios/UI/Pixel/Core/Animation/HeightAnimation.h>
#include <SparkyStudios/UI/Pixel/Core/Animation/WidthAnimation.h>

#if PI_ENABLE_ANIMATION

#include <SparkyStudios/UI/Pixel/Graphics/Point.h>

namespace SparkyStudios::UI::Pixel
{
    /**
     * @brief Animates both the width and height of a widget.
     */
    class PI_EXPORT SizeAnimation : public Animation
    {
    public:
        SizeAnimation(
            const Size& startSize,
            const Size& endSize,
            PiTime duration,
            PiTime delay = 0.0,
            TransitionFunction function = TransitionFunction::Linear);

        void OnStart() override;

        void Run(PiTime percentage) override;

        void OnFinish() override;

    protected:
        WidthAnimation* m_width;
        HeightAnimation* m_height;
    };
} // namespace SparkyStudios::UI::Pixel

#endif // PI_ENABLE_ANIMATION

#endif // PIXEL_UI_SIZEANIMATION_H
