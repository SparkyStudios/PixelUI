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

#include <SparkyStudios/UI/Pixel/Widgets/CheckBox.h>

namespace SparkyStudios::UI::Pixel
{
    const PiString CheckBox::CheckEvent = "CheckBox::Events::Check";
    const PiString CheckBox::UncheckEvent = "CheckBox::Events::Uncheck";
    const PiString CheckBox::CheckChangedEvent = "CheckBox::Events::CheckChanged";

    PI_WIDGET_CONSTRUCTOR(CheckBox)
    , m_checked(false)
    {
        const Size& size = GetSkin()->GetSkinData().CheckBox.size;
        SetSize(size);
    }

    void CheckBox::Render(Skin* skin)
    {
        BaseRenderer* renderer = skin->GetRenderer();
        const Skin::Data& skinData = skin->GetSkinData();

        Rect rect = RenderBounds();

        const bool drawPressed = IsPressed() && IsHovered();
        const bool drawHovered = IsHovered();

        // Background
        {
            const PiUInt32 offset = skinData.CheckBox.borderWidth;

            if (m_disabled)
                renderer->SetDrawColor(skinData.CheckBox.backgroundColorDisabled);
            else if (drawPressed)
                renderer->SetDrawColor(skinData.CheckBox.backgroundColorPressed);
            else if (drawHovered)
                renderer->SetDrawColor(skinData.CheckBox.backgroundColorHovered);
            else
                renderer->SetDrawColor(skinData.CheckBox.backgroundColorNormal);

            renderer->DrawFilledRect(rect + Rect(offset, offset, -offset * 2, -offset * 2), skinData.CheckBox.borderRadius);
        }

        // Border
        if (skinData.CheckBox.borderWidth > 0)
        {
            if (m_disabled)
                renderer->SetDrawColor(skinData.CheckBox.borderColorDisabled);
            else if (drawPressed)
                renderer->SetDrawColor(skinData.CheckBox.borderColorPressed);
            else if (drawHovered)
                renderer->SetDrawColor(skinData.CheckBox.borderColorHovered);
            else
                renderer->SetDrawColor(skinData.CheckBox.borderColorNormal);

            renderer->DrawLinedRect(rect, skinData.Tooltip.borderWidth, skinData.CheckBox.borderRadius);
        }

        {
            Rect r(
                skinData.CheckBox.padding.left, skinData.CheckBox.padding.top,
                rect.w - (skinData.CheckBox.padding.left + skinData.CheckBox.padding.right),
                rect.h - (skinData.CheckBox.padding.top + skinData.CheckBox.padding.bottom));

            if (drawPressed)
                renderer->SetDrawColor(skinData.CheckBox.textColorPressed);
            else if (m_checked)
                renderer->SetDrawColor(skinData.CheckBox.textColorNormal);

            if (drawPressed || m_checked)
            {
                renderer->DrawFilledRect(r, Size(0, 0));
            }
        }
    }

    void CheckBox::OnPress(EventInfo)
    {
        if (m_disabled)
            return;

        if (m_checked && !AllowUncheck())
            return;

        Toggle();
    }

    void CheckBox::OnCheckStatusChanged()
    {
        if (m_checked)
            On(CheckEvent)->Call(this);
        else
            On(UncheckEvent)->Call(this);

        On(CheckChangedEvent)->Call(this);
    }

    void CheckBox::SetChecked(bool bChecked)
    {
        if (m_checked == bChecked)
            return;

        m_checked = bChecked;
        OnCheckStatusChanged();
    }
} // namespace SparkyStudios::UI::Pixel
