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

#ifndef PIXEL_UI_COLOR_H
#define PIXEL_UI_COLOR_H

#include <SparkyStudios/UI/Pixel/Config/Config.h>
#include <SparkyStudios/UI/Pixel/Config/Types.h>

namespace SparkyStudios::UI::Pixel
{
    struct PI_EXPORT Color
    {
        Color(PiUInt8 r_ = 255, PiUInt8 g_ = 255, PiUInt8 b_ = 255, PiUInt8 a_ = 255)
            : r(r_)
            , g(g_)
            , b(b_)
            , a(a_)
        {}

        Color(const Color& c)
        {
            rgba = c.rgba;
        }

        Color(Color&& c)
        {
            rgba = c.rgba;
        }

        void operator=(const Color& c)
        {
            rgba = c.rgba;
        }

        void operator=(Color&& c)
        {
            rgba = c.rgba;
        }

        void operator+=(const Color& c)
        {
            r = static_cast<unsigned char>(r + c.r);
            g = static_cast<unsigned char>(g + c.g);
            b = static_cast<unsigned char>(b + c.b);
            a = static_cast<unsigned char>(a + c.a);
        }

        void operator-=(const Color& c)
        {
            r = static_cast<unsigned char>(r - c.r);
            g = static_cast<unsigned char>(g - c.g);
            b = static_cast<unsigned char>(b - c.b);
            a = static_cast<unsigned char>(a - c.a);
        }

        void operator*=(float f)
        {
            *this = *this * f;
        }

        Color operator*(float f) const
        {
            return Color(
                static_cast<unsigned char>(f * r), static_cast<unsigned char>(f * g), static_cast<unsigned char>(f * b),
                static_cast<unsigned char>(f * a));
        }

        Color operator-(const Color& c) const
        {
            return Color(
                static_cast<unsigned char>(r - c.r), static_cast<unsigned char>(g - c.g), static_cast<unsigned char>(b - c.b),
                static_cast<unsigned char>(a - c.a));
        }

        Color operator+(const Color& c) const
        {
            return Color(
                static_cast<PiUInt8>(r + c.r), static_cast<PiUInt8>(g + c.g), static_cast<PiUInt8>(b + c.b), static_cast<PiUInt8>(a + c.a));
        }

        bool operator==(const Color& c) const
        {
            return c.r == r && c.g == g && c.b == b && c.a == a;
        }

        union
        {
            PiUInt32 rgba;
            struct
            {
                PiUInt8 r, g, b, a;
            };
        };
    };

    namespace Colors
    {
        static const Color Black(0, 0, 0, 255);
        static const Color Red(255, 0, 0, 255);
        static const Color Yellow(255, 255, 0, 255);
        static const Color White(255, 255, 255, 255);
        static const Color Blue(0, 0, 255, 255);
        static const Color Green(0, 255, 0, 255);
        static const Color Grey(200, 200, 200, 255);
        static const Color GreyLight(230, 230, 230, 255);
    } // namespace Colors
} // namespace SparkyStudios::UI::Pixel

#endif // PIXEL_UI_COLOR_H
