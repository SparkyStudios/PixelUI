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

#ifndef PIXEL_UI_FONT_H
#define PIXEL_UI_FONT_H

#include <SparkyStudios/UI/Pixel/Core/Common.h>

#include <list>

namespace SparkyStudios::UI::Pixel
{
    struct PI_EXPORT Font
    {
        typedef std::list<Font*> List;

        enum class Weight
        {
            Lighter,
            Light,
            SemiLight,
            Regular,
            Normal = Regular,
            SemiBold,
            Bold,
            Bolder,
        };

        enum class Style
        {
            Normal,
            Italic,
        };

        Font()
            : facename("?")
            , size(10)
            , weight(Weight::Regular)
            , style(Style::Normal)
        {}

        Font(const Font&) = default;
        Font& operator=(const Font&) = default;

        inline bool operator==(const Font& rhs) const
        {
            return facename == rhs.facename && size == rhs.size && weight == rhs.weight && style == rhs.style;
        }

        inline bool operator!=(const Font& rhs) const
        {
            return !(*this == rhs);
        }

        PiString facename;
        float size;
        Weight weight;
        Style style;
    };
} // namespace SparkyStudios::UI::Pixel

namespace std
{
    template<>
    struct hash<SparkyStudios::UI::Pixel::Font>
    {
        typedef SparkyStudios::UI::Pixel::Font argument_type;
        typedef std::size_t result_type;

        result_type operator()(argument_type const& f) const noexcept
        {
            result_type res = std::hash<SparkyStudios::UI::Pixel::PiString>{}(f.facename);

            SparkyStudios::UI::Pixel::HashCombine<decltype(f.size)>(res, f.size);
            SparkyStudios::UI::Pixel::HashCombine<decltype(f.weight)>(res, f.weight);
            SparkyStudios::UI::Pixel::HashCombine<decltype(f.style)>(res, f.style);

            return res;
        }
    };
} // namespace std

#endif // PIXEL_UI_FONT_H
