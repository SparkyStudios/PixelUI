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

#include <SparkyStudios/UI/Pixel/Core/Input/Mouse.h>
#include <SparkyStudios/UI/Pixel/Core/MainWindow.h>

#include <allegro5/allegro5.h>

namespace SparkyStudios::UI::Pixel
{
    static ALLEGRO_SYSTEM_MOUSE_CURSOR TranslateToAllegro(CursorStyle style)
    {
        switch (style)
        {
        case CursorStyle::Arrow:
            return ALLEGRO_SYSTEM_MOUSE_CURSOR_ARROW;
        case CursorStyle::Busy:
            return ALLEGRO_SYSTEM_MOUSE_CURSOR_BUSY;
        case CursorStyle::IBeam:
            return ALLEGRO_SYSTEM_MOUSE_CURSOR_EDIT;
        case CursorStyle::Hand:
            return ALLEGRO_SYSTEM_MOUSE_CURSOR_LINK;
        case CursorStyle::ResizeMove:
            return ALLEGRO_SYSTEM_MOUSE_CURSOR_MOVE;
        case CursorStyle::ResizeH:
            return ALLEGRO_SYSTEM_MOUSE_CURSOR_RESIZE_E;
        case CursorStyle::ResizeNE:
            return ALLEGRO_SYSTEM_MOUSE_CURSOR_RESIZE_NE;
        case CursorStyle::ResizeNS:
            return ALLEGRO_SYSTEM_MOUSE_CURSOR_RESIZE_N;
        case CursorStyle::ResizeNW:
            return ALLEGRO_SYSTEM_MOUSE_CURSOR_RESIZE_NW;
        case CursorStyle::Unavailable:
            return ALLEGRO_SYSTEM_MOUSE_CURSOR_UNAVAILABLE;
        default:
            return ALLEGRO_SYSTEM_MOUSE_CURSOR_NONE;
        }
    }

    Cursor::Cursor(const MainWindow* mainWindow)
        : _mainWindow(mainWindow)
    {}

    const Point& Cursor::GetPosition() const
    {
        ALLEGRO_MOUSE_STATE mouse;
        al_get_mouse_state(&mouse);
        Point po{};
        po.x = mouse.x;
        po.y = mouse.y;
        return po;
    }

    void Cursor::SetPosition(const Point& position)
    {
        al_set_mouse_xy(static_cast<ALLEGRO_DISPLAY*>(_mainWindow->GetNativeHandle()), position.x, position.y);
    }

    void Cursor::ApplyStyle(CursorStyle style)
    {
        al_set_system_mouse_cursor(
            static_cast<ALLEGRO_DISPLAY*>(_mainWindow->GetNativeHandle()),
            static_cast<ALLEGRO_SYSTEM_MOUSE_CURSOR>(TranslateToAllegro(style)));
    }
} // namespace SparkyStudios::UI::Pixel