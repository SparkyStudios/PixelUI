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

#include <SparkyStudios/UI/Pixel/Config/Config.h>

namespace SparkyStudios::UI::Pixel
{
    struct PI_EXPORT Point
    {
        Point(int x_ = 0, int y_ = 0)
            : x(x_)
            , y(y_)
        {}

        void set(int x_, int y_)
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

        int x, y;
    };

    typedef Point Size;
} // namespace SparkyStudios::UI::Pixel

#endif // PIXEL_UI_POINT_H
