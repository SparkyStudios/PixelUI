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

#ifndef PIXEL_UI_BASE_H
#define PIXEL_UI_BASE_H

#include <map>

#include <SparkyStudios/UI/Pixel/Core/Events/EventListener.h>

namespace SparkyStudios::UI::Pixel
{
    class Widget;

    /**
     * @brief Configures margin around a widget.
     */
    struct PI_EXPORT Margin
    {
        Margin(PiInt32 l, PiInt32 t, PiInt32 r, PiInt32 b)
            : top(t)
            , bottom(b)
            , left(l)
            , right(r)
        {}

        Margin(PiInt32 h, PiInt32 v)
            : Margin(h, v, h, v)
        {}

        Margin(PiInt32 all)
            : Margin(all, all)
        {}

        Margin()
            : Margin(0)
        {}

        Margin operator+(const Margin& margin) const
        {
            Margin m;
            m.top = top + margin.top;
            m.bottom = bottom + margin.bottom;
            m.left = left + margin.left;
            m.right = right + margin.right;
            return m;
        }

        PiInt32 top, bottom, left, right;
    };

    /**
     * @brief Configures padding around a widget.
     */
    typedef Margin Padding;

    /**
     * @brief HSV representation of a color.
     */
    struct PI_EXPORT HSV
    {
        PiReal32 h;
        PiReal32 s;
        PiReal32 v;
    };

    /**
     * @brief Holds data about a drag and drop operation.
     */
    struct PI_EXPORT DragAndDropPacket
    {
        DragAndDropPacket()
        {
            userData = nullptr;
            draggable = false;
            widget = nullptr;
            holdOffset = Point(0, 0);
        }

        PiString name;
        void* userData;
        bool draggable;

        Widget* widget;
        Point holdOffset;
    };

    /**
     * @brief Provides alignment configuration.
     */
    enum class Alignment
    {
        None = 0,
        Left = (1 << 1),
        Right = (1 << 2),
        Top = (1 << 3),
        Bottom = (1 << 4),
        CenterV = (1 << 5),
        CenterH = (1 << 6),
        Fill = (1 << 7),
        Center = CenterV | CenterH
    };

    inline Alignment operator|(Alignment a, Alignment b)
    {
        return Alignment(static_cast<PiInt32>(a) | static_cast<PiInt32>(b));
    }

    inline bool operator&(Alignment a, Alignment b)
    {
        return (static_cast<PiInt32>(a) & static_cast<PiInt32>(b)) != 0;
    }

    /**
     * @brief Maps strings to event listeners.
     */
    typedef std::map<PiString, EventListener*> AccelMap;

} // namespace SparkyStudios::UI::Pixel

#endif // PIXEL_UI_BASE_H
