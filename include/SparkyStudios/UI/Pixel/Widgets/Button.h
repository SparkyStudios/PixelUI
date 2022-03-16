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

#ifndef PIXEL_UI_BUTTON_H
#define PIXEL_UI_BUTTON_H

#include <SparkyStudios/UI/Pixel/Widgets/Label.h>

namespace SparkyStudios::UI::Pixel
{
    class PI_EXPORT Button : public Label
    {
        friend class LabeledCheckBox;

    public:
        static const PiString DoubleClickEvent;
        static const PiString ClickEvent;
        static const PiString RightClickEvent;

        PI_WIDGET(Button, Label);

        void Render(Skin* skin) override;
        void OnMouseButton(const Point& position, MouseButton button, MouseButtonPressMode mode) override;
        void OnMouseDoubleClick(const Point& position, MouseButton button) override;
        bool OnKeyDown(Key key) override;

        virtual void OnPress(EventInfo info);
        virtual void OnRightPress(EventInfo info);

        // void AcceleratePressed();

        //! Is the button currently being pressed?
        virtual bool IsPressed() const
        {
            return m_pressed;
        }

        virtual void SetDepressed(bool b);

        //
        /// Buttons can be toggle type, which means that it is
        /// toggled on and off. Its toggle status is in IsDepressed.
        //
        virtual void SetIsToggle(bool b)
        {
            m_bToggle = b;
        }

        virtual bool IsToggle() const
        {
            return m_bToggle;
        }

        virtual bool GetToggleState() const
        {
            return m_toggleStatus;
        }

        virtual void SetToggleState(bool b);
        virtual void Toggle()
        {
            SetToggleState(!GetToggleState());
        }

        virtual void SetImage(const PiString& strName, bool bCenter = false);

        void SizeToContents() override;
        void PostLayout(Skin* skin) override;
        // void UpdateColors() override;

        virtual void SetImageAlpha(float fMultiply);

    public:
        EventListener onToggle;
        EventListener onToggleOn;
        EventListener onToggleOff;

    protected:
        // Image* m_image; ///< Optional image to be displayed on button.

        bool m_pressed; ///< Is button currently being pressed?
        bool m_bToggle; ///< Is this a toggling button, i.e. on or off.
        bool m_toggleStatus;

        bool m_centerImage; ///< Centre the image in the button?
    };
} // namespace SparkyStudios::UI::Pixel

#endif // PIXEL_UI_BUTTON_H
