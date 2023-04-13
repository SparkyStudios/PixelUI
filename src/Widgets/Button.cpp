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

#include <SparkyStudios/UI/Pixel/Widgets/Button.h>
#include <SparkyStudios/UI/Pixel/Widgets/Canvas.h>

namespace SparkyStudios::UI::Pixel
{
    const PiString Button::DoubleClickEvent = "Button::Events::DoubleClick";
    const PiString Button::ClickEvent = "Button::Events::Click";
    const PiString Button::RightClickEvent = "Button::Events::RightClick";

    PI_WIDGET_CONSTRUCTOR(Button)
    , m_bToggle(false), m_pressed(false), m_centerImage(false), m_toggleStatus(false) //, m_image(nullptr)
    {
        const Skin::Data& skinData = GetSkin()->GetSkinData();

        SetSize(100, 20);
        SetMouseInputEnabled(true);
        SetIsToggle(false);
        SetAlignment(Alignment::Center);
        SetPadding(skinData.Button.padding);
        SetKeyboardInputEnabled(false);
        SetFontSize(skinData.Button.textSize);
        // SetTabable(false);
    }

    void Button::Render(Skin* skin)
    {
        bool drawPressed = IsPressed() && IsHovered();

        if (IsToggle())
            drawPressed = drawPressed || GetToggleState();

        const bool drawHovered = IsHovered();
        BaseRenderer* renderer = skin->GetRenderer();
        const Skin::Data& skinData = skin->GetSkinData();

        // Text
        {
            if (m_disabled)
                SetTextColorOverride(skinData.Button.textColorDisabled);
            else if (drawPressed)
                SetTextColorOverride(skinData.Button.textColorPressed);
            else if (drawHovered)
                SetTextColorOverride(skinData.Button.textColorHovered);
            else
                SetTextColorOverride(Colors::Transparent);
        }

        if (!m_bDrawBackground)
            return;

        Rect rect = RenderBounds();

        // Background
        {
            const PiUInt32 offset = skinData.Button.borderWidth;

            if (m_disabled)
                renderer->SetDrawColor(skinData.Button.backgroundColorDisabled);
            else if (drawPressed)
                renderer->SetDrawColor(skinData.Button.backgroundColorPressed);
            else if (drawHovered)
                renderer->SetDrawColor(skinData.Button.backgroundColorHovered);
            else
                renderer->SetDrawColor(skinData.Button.backgroundColorNormal);

            renderer->DrawFilledRect(rect + Rect(offset, offset, -offset * 2, -offset * 2), skinData.Button.borderRadius);
        }

        // Border
        if (skinData.Button.borderWidth > 0)
        {
            if (m_disabled)
                renderer->SetDrawColor(skinData.Button.borderColorDisabled);
            else if (drawPressed)
                renderer->SetDrawColor(skinData.Button.borderColorPressed);
            else if (drawHovered)
                renderer->SetDrawColor(skinData.Button.borderColorHovered);
            else
                renderer->SetDrawColor(skinData.Button.borderColorNormal);

            renderer->DrawLinedRect(rect, skinData.Button.borderWidth, skinData.Button.borderRadius);
        }
    }

    void Button::OnMouseButton(const Point& position, MouseButton button, MouseButtonPressMode mode)
    {
        if (IsDisabled())
            return;

        if (mode == MouseButtonPressMode::Pressed)
        {
            SetDepressed(true);
            Canvas::SetMouseFocusedWidget(this);
        }
        else
        {
            if (IsHovered() && m_pressed)
            {
                if (button == MouseButton::Left)
                    OnPress(EventInfo(this));
                else if (button == MouseButton::Right)
                    OnRightPress(EventInfo(this));
            }

            SetDepressed(false);
            Canvas::SetMouseFocusedWidget(nullptr);
        }

        ParentClass::OnMouseButton(position, button, mode);
    }

    void Button::SetDepressed(bool b)
    {
        if (m_pressed == b)
            return;

        m_pressed = b;
        Redraw();
    }

    void Button::OnRightPress(EventInfo info)
    {
        On(RightClickEvent)->Call(this, info);
    }

    void Button::OnPress(EventInfo info)
    {
        if (IsToggle())
            SetToggleState(!GetToggleState());

        On(ClickEvent)->Call(this, info);
    }

    void Button::SetImage(const PiString& strName, bool bCenter)
    {
        if (strName.empty())
        {
            // if (m_image)
            // {
            //     delete m_image;
            //     m_image = nullptr;
            // }

            return;
        }

        // if (!m_image)
        //     m_image = new Image(this);

        // m_image->SetImage(strName);
        // m_image->SizeToContents();
        // m_image->SetMargin(Margin(2, 0, 2, 0));
        // m_centerImage = bCenter;
        // // Ugh.
        // Padding padding = GetPadding();
        // padding.left = m_image->Right() + 2;
        // SetPadding(padding);
    }

    void Button::SetToggleState(bool b)
    {
        if (m_toggleStatus == b)
            return;

        m_toggleStatus = b;
        onToggle.Call(this);

        if (m_toggleStatus)
            onToggleOn.Call(this);
        else
            onToggleOff.Call(this);

        Redraw();
    }

    void Button::SizeToContents()
    {
        ParentClass::SizeToContents();

        // if (m_image)
        // {
        //     int height = m_image->Height() + 4;

        //     if (Height() < height)
        //         SetHeight(height);
        // }
    }

    bool Button::OnKeyDown(Key key)
    {
        ParentClass::OnKeyDown(key);

        if (key == Key::Space)
        {
            OnPress(EventInfo(this));
        }

        return true;
    }

    // void Button::AcceleratePressed()
    // {
    //     OnPress(EventInfo(this));
    // }

    // void Button::UpdateColors()
    // {
    //     if (IsDisabled())
    //         return SetTextColor(GetSkin()->Colors.Button.Disabled);

    //     if (IsPressed() || GetToggleState())
    //         return SetTextColor(GetSkin()->Colors.Button.Down);

    //     if (IsHovered())
    //         return SetTextColor(GetSkin()->Colors.Button.Hover);

    //     SetTextColor(GetSkin()->Colors.Button.Normal);
    // }

    void Button::PostLayout(Skin* skin)
    {
        ParentClass::PostLayout(skin);

        // if (m_image)
        // {
        //     if (m_centerImage)
        //         m_image->SetPosition(Position::Center);
        //     else
        //         m_image->SetPosition(Position::Left | Position::CenterV);
        // }
    }

    void Button::OnMouseDoubleClick(const Point& position, MouseButton button)
    {
        if (button != MouseButton::Left)
            return;

        if (IsDisabled())
            return;

        OnMouseButton(position, button, MouseButtonPressMode::Pressed);
        On(DoubleClickEvent)->Call(this);
    }

    void Button::SetImageAlpha(float f)
    {
        // if (!m_image)
        //     return;

        // m_image->SetDrawColor(Gwk::Color(255, 255, 255, 255.0f * f));
    }
} // namespace SparkyStudios::UI::Pixel
