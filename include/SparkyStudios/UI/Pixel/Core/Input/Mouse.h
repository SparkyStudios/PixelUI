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

#ifndef PIXEL_UI_CURSOR_H
#define PIXEL_UI_CURSOR_H

#include <SparkyStudios/UI/Pixel/Core/Common.h>

#include <SparkyStudios/UI/Pixel/Graphics/Point.h>

namespace SparkyStudios::UI::Pixel
{
    class MainWindow;

    enum class MouseButton : PiUInt32
    {
        Left,
        Right,
        Middle
    };

    enum class CursorStyle
    {
        Arrow = 0,
        Default = Arrow,
        Edit = 1,
        IBeam = Edit,
        ResizeV = 2,
        ResizeH = 3,
        ResizeNS = ResizeV,
        ResizeWE = ResizeH,
        ResizeNW = 4,
        ResizeNE = 5,
        ResizeSE = ResizeNW,
        ResizeSW = ResizeNE,
        ResizeNWSE = ResizeNW,
        ResizeNESW = ResizeNE,
        ResizeMove = 6,
        ResizeAll = ResizeMove,
        No = 7,
        Unavailable = No,
        Busy = 8,
        Wait = Busy,
        Link = 9,
        Hand = Link,
        MAX
    };

    class Cursor
    {
    public:
        explicit Cursor(const MainWindow* mainWindow);

        void ApplyStyle(CursorStyle style);

        Point GetPosition() const;

        void SetPosition(const Point& position);

    private:
        const MainWindow* _mainWindow;
    };
} // namespace SparkyStudios::UI::Pixel

#endif // PIXEL_UI_CURSOR_H
