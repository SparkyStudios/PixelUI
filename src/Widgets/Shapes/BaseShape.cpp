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

#include <SparkyStudios/UI/Pixel/Widgets/Shapes/BaseShape.h>

namespace SparkyStudios::UI::Pixel
{
    PI_WIDGET_CONSTRUCTOR(BaseShape)
    {
        DrawBackground(true);
        CaptureMouseEvent(false);
        SetBackgroundColor(Colors::Black);
        SetSize(16, 16);
    }

    void BaseShape::CaptureMouseEvent(bool value)
    {
        m_captureMouseEvents = value;
        SetMouseInputEnabled(value);
    }

    void BaseShape::DrawBackground(bool value)
    {
        m_drawBackground = value;
    }

    void BaseShape::SetBackgroundColor(const Color& color)
    {
        m_backgroundColor = color;
    }

    void BaseShape::DrawBorder(bool value)
    {
        m_drawBorder = value;
    }

    void BaseShape::SetBorder(const Color& color, PiUInt32 thickness)
    {
        m_borderColor = color;
        m_borderThickness = thickness;
    }

    Widget* BaseShape::GetWidgetAt(PiInt32 x, PiInt32 y, bool onlyIfMouseEnabled)
    {
        Widget* child = ParentClass::GetWidgetAt(x, y, onlyIfMouseEnabled);
        return m_captureMouseEvents ? child : child == this ? m_parent : child;
    }
} // namespace SparkyStudios::UI::Pixel