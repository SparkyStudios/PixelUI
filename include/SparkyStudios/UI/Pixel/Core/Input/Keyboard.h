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

#ifndef PIXEL_UI_KEYBOARD_H
#define PIXEL_UI_KEYBOARD_H

#include <SparkyStudios/UI/Pixel/Core/Common.h>

namespace SparkyStudios::UI::Pixel
{
    enum class Key : PiUInt32
    {
        Invalid = 0,
        Return,
        Backspace,
        Delete,
        Left,
        Right,
        Shift,
        Tab,
        Space,
        Home,
        End,
        Control,
        Up,
        Down,
        Escape,
        Alt,
        Command, // Mac command/clover
        MAX //!< number of keys
    };

}

#endif // PIXEL_UI_KEYBOARD_H
