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

#ifndef PIXEL_UI_IINPUTEVENTLISTENER_H
#define PIXEL_UI_IINPUTEVENTLISTENER_H

#include <SparkyStudios/UI/Pixel/Core/Input/Keyboard.h>
#include <SparkyStudios/UI/Pixel/Core/Input/Mouse.h>

namespace SparkyStudios::UI::Pixel
{
    enum class KeyPressMode
    {
        Pressed,
        Released,
    };

    typedef KeyPressMode MouseButtonPressMode;

    class IInputEventListener
    {
    public:
        virtual ~IInputEventListener()
        {}

        virtual bool OnMouseMove(int x, int y, int deltaX, int deltaY) = 0;

        virtual bool OnMouseButton(MouseButton button, MouseButtonPressMode pressMode) = 0;

        virtual bool OnMouseWheel(int val) = 0;

        virtual bool OnModifierKey(Key key, KeyPressMode pressedMode) = 0;

        virtual bool OnKey(Key key, KeyPressMode pressedMode) = 0;

        virtual bool InputQuit() = 0;
    };
} // namespace SparkyStudios::UI::Pixel

#endif // PIXEL_UI_IINPUTEVENTLISTENER_H
