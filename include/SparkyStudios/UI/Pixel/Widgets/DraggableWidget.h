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

#include <SparkyStudios/UI/Pixel/Widgets/Widget.h>

#ifndef PIXEL_UI_DRAGGABLEWIDGET_H
#define PIXEL_UI_DRAGGABLEWIDGET_H

namespace SparkyStudios::UI::Pixel
{
    /**
     * @brief Base class for widgets which adds
     * drag features.
     */
    class PI_EXPORT DraggableWidget : public Widget
    {
    public:
        /**
         * @brief The drag event.
         *
         * This event is triggered when the widget is being dragged.
         * The mouse position delta will be stored in the `point` field
         * of the event data.
         */
        static const char* const DragEvent;

        /**
         * @brief The drag start event.
         *
         * This event is triggered when the drag operation on the widget
         * is started. The mouse position will be stored in the `point` field
         * of the event data.
         */
        static const char* const DragStartEvent;

        /**
         * @brief The drag end event.
         *
         * This event is triggered when the drag operation on the widget
         * is ended (the mouse button is released). The mouse position will be
         * stored in the `point` field of the event data.
         */
        static const char* const DragEndEvent;

        PI_WIDGET(DraggableWidget, Widget);

        /**
         * @brief Set the widget which should move when dragging.
         *
         * @param target The target widget to move on drag.
         */
        virtual void SetDragTarget(Widget* target);

        /**
         * @brief Checks if the widget is currently being dragged.
         *
         * @return Whether the widget is currently being dragged.
         */
        virtual bool IsDragging() const;

    protected:
        void OnMouseMove(const Point& position, PiInt32 deltaX, PiInt32 deltaY) override;
        void OnMouseButton(const Point& position, MouseButton button, MouseButtonPressMode mode) override;

        Widget* m_target;

        Point m_holdPosition;

        bool m_pressed;
        bool m_doMove;
    };
} // namespace SparkyStudios::UI::Pixel

#endif // PIXEL_UI_DRAGGABLEWIDGET_H
