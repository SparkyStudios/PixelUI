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

    /**
     * @brief Interface for input event listeners.
     */
    class IInputEventListener
    {
    public:
        virtual ~IInputEventListener() = default;

        /**
         * @brief Called when the mouse moves.
         *
         * @param x The new x position of the mouse.
         * @param y The new y position of the mouse.
         * @param deltaX The change in x position of the mouse.
         * @param deltaY The change in y position of the mouse.
         *
         * @return true if the event was consumed, false otherwise.
         */
        virtual bool OnMouseMove(PiInt32 x, PiInt32 y, PiInt32 deltaX, PiInt32 deltaY) = 0;

        /**
         * @brief Called when a mouse button is pressed.
         *
         * @param button The button that was pressed.
         * @param pressMode The press mode of the button.
         *
         * @return true if the event was consumed, false otherwise.
         */
        virtual bool OnMouseButton(MouseButton button, MouseButtonPressMode pressMode) = 0;

        /**
         * @brief Called when the mouse wheel is scrolled.
         *
         * @param x The amount the mouse wheel was scrolled in the x direction.
         * @param y The amount the mouse wheel was scrolled in the y direction.
         *
         * @return true if the event was consumed, false otherwise.
         */
        virtual bool OnMouseWheel(PiReal32 x, PiReal32 y) = 0;

        /**
         * @brief Called when a keyboard is special key is pressed.
         *
         * @param key The special key that was pressed.
         * @param pressedMode The press mode of the key.
         *
         * @return true if the event was consumed, false otherwise.
         */
        virtual bool OnSpecialKey(Key key, KeyPressMode pressedMode) = 0;

        /**
         * @brief Called when a character key is pressed.
         *
         * @param character The character that was pressed.
         *
         * @return true if the event was consumed, false otherwise.
         */
        virtual bool OnCharacter(char character) = 0;
    };
} // namespace SparkyStudios::UI::Pixel

#endif // PIXEL_UI_IINPUTEVENTLISTENER_H
