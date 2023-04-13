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

#ifndef PIXEL_UI_CHECKBOX_H
#define PIXEL_UI_CHECKBOX_H

#include <SparkyStudios/UI/Pixel/Widgets/Button.h>

namespace SparkyStudios::UI::Pixel
{
    class PI_EXPORT CheckBox : public Button
    {
    public:
        static const PiString CheckEvent;
        static const PiString UncheckEvent;
        static const PiString CheckChangedEvent;

        PI_WIDGET(CheckBox, Button);

        void Render(Skin* skin) override;

        void OnPress(EventInfo) override;

        virtual void SetChecked(bool checked);

        void Toggle() override
        {
            SetChecked(!m_checked);
        }

        virtual bool IsChecked()
        {
            return m_checked;
        }

    protected:
        virtual bool AllowUncheck()
        {
            return true;
        }

    private:
        void OnCheckStatusChanged();

        bool m_checked;
    };

    class PI_EXPORT LabeledCheckBox : public Widget
    {
    public:
        PI_WIDGET_INLINE(LabeledCheckBox, Widget)
        {
            const Skin::Data& skinData = GetSkin()->GetSkinData();
            SetSize(200, (std::max)(static_cast<PiUInt32>(skinData.CheckBox.size.h), skinData.CheckBox.textSize));

            m_checkbox = new CheckBox(this);
            m_checkbox->SetDock(Alignment::Left);
            m_checkbox->SetMargin(Margin(0, 0, 8, 0));
            // m_checkbox->SetTabable(false);

            m_label = new Button(this);
            m_label->SetPadding(Padding(0));
            m_label->m_bDrawBackground = false;
            m_label->SetDock(Alignment::Fill);
            m_label->SetAlignment(Alignment::Left);
            m_label->On(Button::ClickEvent)->Add(m_checkbox, &CheckBox::OnPress);

            // TODO
            // m_label->SetTabable(false);
            // SetTabable(false);
        }

        Widget* GetWidgetAt(PiInt32 x, PiInt32 y, bool onlyIfMouseEnabled) override
        {
            Widget* child = ParentClass::GetWidgetAt(x, y, onlyIfMouseEnabled);

            if (child == m_label || child == this)
                return m_checkbox;

            return child;
        }

        virtual CheckBox* Checkbox()
        {
            return m_checkbox;
        }

        virtual Label* Label()
        {
            return m_label;
        }

        void SetDisabled(bool value) override
        {
            m_checkbox->SetDisabled(value);
            m_label->SetDisabled(value);
        }

        void SetText(const PiString& label)
        {
            m_label->SetText(label);
        }

        bool OnKeyDown(Key key) override
        {
            ParentClass::OnKeyDown(key);

            if (key == Key::Space)
            {
                m_checkbox->SetChecked(!m_checkbox->IsChecked());
            }

            return true;
        }

    private:
        CheckBox* m_checkbox;
        Button* m_label;
    };

} // namespace SparkyStudios::UI::Pixel

#endif // PIXEL_UI_CHECKBOX_H
