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

#include <SparkyStudios/UI/Pixel/Widgets/Canvas.h>
#include <SparkyStudios/UI/Pixel/Widgets/DraggableWidget.h>

namespace SparkyStudios::UI::Pixel
{
    const char* const DraggableWidget::DragEvent = "DraggableWidget::Events::Drag";
    const char* const DraggableWidget::DragStartEvent = "DraggableWidget::Events::DragStart";
    const char* const DraggableWidget::DragEndEvent = "DraggableWidget::Events::DragEnd";

    PI_WIDGET_CONSTRUCTOR(DraggableWidget)
    , m_target(nullptr), m_pressed(false), m_doMove(true)
    {}

    void DraggableWidget::SetDragTarget(Widget* target)
    {
        m_target = target;
    }

    bool DraggableWidget::IsDragging() const
    {
        return m_pressed;
    }

    void DraggableWidget::OnMouseMove(const Point& position, PiInt32 deltaX, PiInt32 deltaY)
    {
        ParentClass::OnMouseMove(position, deltaX, deltaY);

        if (m_disabled)
            return;

        if (!m_pressed)
            return;

        if (m_doMove && m_target != nullptr)
        {
            Point p = position - m_holdPosition;

            // Translate to parent
            if (m_target->GetParent() != nullptr)
                p = m_target->GetParent()->WindowPositionToLocal(p);

            m_target->MoveTo(p);
        }

        EventInfo info(this);
        info.data.point = Point(deltaX, deltaY);
        On(DragEvent)->Call(this, info);
    }

    void DraggableWidget::OnMouseButton(const Point& position, MouseButton button, MouseButtonPressMode mode)
    {
        ParentClass::OnMouseButton(position, button, mode);

        if (button != MouseButton::Left)
            return;

        if (m_disabled)
            return;

        EventInfo info(this);
        info.data.point = position;

        if (mode == MouseButtonPressMode::Pressed)
        {
            m_pressed = true;

            if (m_target != nullptr)
                m_holdPosition = m_target->WindowPositionToLocal(position);

            Canvas::SetMouseFocusedWidget(this);

            On(DragStartEvent)->Call(this, info);
        }
        else
        {
            m_pressed = false;

            Canvas::SetMouseFocusedWidget(nullptr);

            On(DragEndEvent)->Call(this);
        }
    }
} // namespace SparkyStudios::UI::Pixel