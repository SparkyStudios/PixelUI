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

#ifndef PIXEL_UI_POINT_H
#define PIXEL_UI_POINT_H

#include <SparkyStudios/UI/Pixel/Core/Common.h>

namespace SparkyStudios::UI::Pixel
{
    struct PI_EXPORT Point
    {
        explicit Point(PiInt32 x_ = 0, PiInt32 y_ = 0)
            : x(x_)
            , y(y_)
        {}

        void Set(PiInt32 x_, PiInt32 y_)
        {
            x = x_, y = y_;
        }

        void operator+=(const Point& p)
        {
            x += p.x;
            y += p.y;
        }

        Point operator+(const Point& p) const
        {
            return Point(x + p.x, p.y + y);
        }

        void operator-=(const Point& p)
        {
            x -= p.x;
            y -= p.y;
        }

        Point operator-(const Point& p) const
        {
            return Point(x - p.x, y - p.y);
        }

        PiInt32 x, y;
    };

    struct PI_EXPORT Size
    {
        explicit Size(PiInt32 w_ = 0, PiInt32 h_ = 0)
            : w(w_)
            , h(h_)
        {}

        void Set(PiInt32 w_, PiInt32 h_)
        {
            w = w_, h = h_;
        }

        void operator+=(const Size& p)
        {
            w += p.w;
            h += p.h;
        }

        Size operator+(const Size& p) const
        {
            return Size(w + p.w, p.h + h);
        }

        void operator-=(const Size& p)
        {
            w -= p.w;
            h -= p.h;
        }

        Size operator-(const Size& p) const
        {
            return Size(w - p.w, h - p.h);
        }

        PiInt32 w, h;
    };
} // namespace SparkyStudios::UI::Pixel

#endif // PIXEL_UI_POINT_H
