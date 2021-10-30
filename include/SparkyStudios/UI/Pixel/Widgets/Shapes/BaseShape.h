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

#ifndef PIXEL_UI_BASESHAPE_H
#define PIXEL_UI_BASESHAPE_H

#include <SparkyStudios/UI/Pixel/Widgets/Widget.h>

namespace SparkyStudios::UI::Pixel
{
    /**
     * @brief Base class for all shape widgets.
     *
     * Shape widgets are just used to draw a single shape on screen
     * (arrows, ellipses, circles, etc...). You can configure the shape
     * background color, border color, and border width. Other settings
     * may be specified in each specific shape widget.
     */
    class PI_EXPORT BaseShape : public Widget
    {
    public:
        /**
         * @brief Set this shape to capture mouse events or not.
         *
         * If mouse events are captured, the shape will receive click
         * and hover events like any other widget. In some cases, this
         * is not what you would expect.
         *
         * @param value Whether to capture mouse events.
         */
        void CaptureMouseEvent(bool value);

        /**
         * @brief Set if the shape should render with a background
         * color.
         *
         * The background color is specified using SetBackgroundColor().
         * If the background color is transparent, the shape will not render
         * as filled to optimize performances.
         *
         * @param value Whether to render with a background color.
         */
        void DrawBackground(bool value);

        /**
         * @brief Set the shape background color.
         *
         * This color will be used only when drawing the shape with
         * a background color is enabled.
         *
         * @param color The background color.
         */
        void SetBackgroundColor(const Color& color);

        /**
         * @brief Set if the shape should render with a border.
         *
         * The border color and thickness are specified using SetBorder().
         * If the border color is transparent, the shape will not render
         * with a border to optimize performances.
         *
         * @param value Whether to render with a border.
         */
        void DrawBorder(bool value);

        /**
         * @brief Set the border color and thickness.
         *
         * @param color The border color.
         * @param thickness The border thickness.
         */
        void SetBorder(const Color& color, PiUInt32 thickness);

        Widget* GetWidgetAt(PiInt32 x, PiInt32 y, bool onlyIfMouseEnabled) override;

    protected:
        PI_WIDGET(BaseShape, Widget);

        /**
         * @brief Whether to capture mouse events.
         */
        bool m_captureMouseEvents;

        /**
         * @brief The shape background color.
         */
        Color m_backgroundColor;

        /**
         * @brief Whether to render with a border.
         */
        bool m_drawBorder;

        /**
         * @brief The shape border color.
         */
        Color m_borderColor;

        /**
         * @brief The shape border thickness.
         */
        PiUInt32 m_borderThickness;
    };
} // namespace SparkyStudios::UI::Pixel

#endif // PIXEL_UI_BASESHAPE_H
