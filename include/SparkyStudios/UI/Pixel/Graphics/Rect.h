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
        Rect(int x_ = 0, int y_ = 0, int w_ = 0, int h_ = 0)
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

        int Left() const
        {
            return x;
        }

        int Right() const
        {
            return x + w;
        }

        int Top() const
        {
            return y;
        }

        int Bottom() const
        {
            return y + h;
        }

        bool operator==(const Rect& other) const
        {
            return x == other.x && y == other.y && w == other.w && h == other.h;
        }

        Rect operator+(const Rect& rct) const
        {
            const Rect m(x + rct.x, y + rct.y, w + rct.w, h + rct.h);
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

        int x, y, w, h;
    };
} // namespace SparkyStudios::UI::Pixel

#endif // PIXEL_UI_RECT_H
