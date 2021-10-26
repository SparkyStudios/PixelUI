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

#ifndef PIXEL_UI_RECT_H
#define PIXEL_UI_RECT_H

#include <SparkyStudios/UI/Pixel/Config/Config.h>
#include <SparkyStudios/UI/Pixel/Graphics/Point.h>

namespace SparkyStudios::UI::Pixel
{
    struct PI_EXPORT Rect
    {
        explicit Rect(PiInt32 x_ = 0, PiInt32 y_ = 0, PiInt32 w_ = 0, PiInt32 h_ = 0)
            : x(x_)
            , y(y_)
            , w(w_)
            , h(h_)
        {}

        Rect(const Point& o, const Point& sz)
            : x(o.x)
            , y(o.y)
            , w(sz.x)
            , h(sz.y)
        {}

        PiInt32 Left() const
        {
            return x;
        }

        PiInt32 Right() const
        {
            return x + w;
        }

        PiInt32 Top() const
        {
            return y;
        }

        PiInt32 Bottom() const
        {
            return y + h;
        }

        bool operator==(const Rect& other) const
        {
            return x == other.x && y == other.y && w == other.w && h == other.h;
        }

        Rect operator+(const Rect& rect) const
        {
            const Rect m(x + rect.x, y + rect.y, w + rect.w, h + rect.h);
            return m;
        }

        Rect operator+(const Size& size) const
        {
            const Rect m(x, y, w + size.w, h + size.h);
            return m;
        }

        Size GetSize() const
        {
            return Size(w, h);
        }

        Point GetPosition() const
        {
            return Point(x, y);
        }

        PiInt32 x, y, w, h;
    };
} // namespace SparkyStudios::UI::Pixel

#endif // PIXEL_UI_RECT_H
