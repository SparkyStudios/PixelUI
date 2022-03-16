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

#ifndef PIXEL_UI_STATUSBAR_H
#define PIXEL_UI_STATUSBAR_H

#include <SparkyStudios/UI/Pixel/Widgets/Widget.h>

namespace SparkyStudios::UI::Pixel
{
    /**
     * @brief Displays a status bar.
     *
     * The status bar is placed at the bottom of the window by default.
     * You can add more widgets to the status bar by using the AddWidget() method.
     *
     * You can also display a single text inside the status bar by using the SetText() method.
     * This method will automatically remove all existing widgets before setting the text.
     */
    class PI_EXPORT StatusBar : public Widget
    {
    public:
        PI_WIDGET(StatusBar, Widget);

        /**
         * @brief Set the text to display in the status bar.
         *
         * This will automatically remove all existing widgets before setting the text.
         *
         * @param text The text to display.
         * @param alignment The text alignment.
         * @param dock The position of the text in the status bar.
         */
        virtual void SetText(const PiString& text, Alignment alignment, Alignment dock);

        /**
         * @brief Add a widget to the status bar.
         *
         * @param widget The widget to add.
         */
        virtual void AddWidget(Widget* widget);

    protected:
        void Render(Skin* skin) override;
    };
} // namespace SparkyStudios::UI::Pixel

#endif // PIXEL_UI_STATUSBAR_H
