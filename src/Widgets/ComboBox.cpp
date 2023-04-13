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
#include <SparkyStudios/UI/Pixel/Widgets/ComboBox.h>
#include <SparkyStudios/UI/Pixel/Widgets/Containers/Menu.h>
#include <SparkyStudios/UI/Pixel/Widgets/Shapes/DownArrow.h>

namespace SparkyStudios::UI::Pixel
{
    const PiString ComboBox::SelectionChangeEvent = "ComboBox::Events::SelectionChange";

    PI_WIDGET_CONSTRUCTOR(ComboBox)
    {
        SetSize(100, 24);

        const Skin::Data& skinData = GetSkin()->GetSkinData();

        m_menu = new Menu(this);
        m_menu->SetHidden(true);
        m_menu->SetDisableIconMargin(true);

        auto* arrow = new DownArrow(this);
        arrow->SetSize(skinData.ComboBox.arrowSize, skinData.ComboBox.arrowSize);
        arrow->SetBackgroundColor(skinData.ComboBox.arrowColorNormal);
        m_arrow = arrow;

        m_selectedItem = nullptr;

        SetAlignment(Alignment::Left | Alignment::CenterV);
        SetPadding(skinData.ComboBox.padding);
        SetKeyboardInputEnabled(true);
    }

    void ComboBox::SelectItem(MenuItem* item, bool fireChangeEvents)
    {
        if (m_selectedItem == item)
            return;

        m_selectedItem = item;
        SetText(m_selectedItem->GetText());
        m_menu->SetHidden(true);
        Invalidate();

        if (fireChangeEvents)
        {
            On(SelectionChangeEvent)->Call(this);
            Focus();
        }
    }

    void ComboBox::SelectItemByName(const PiString& name, bool fireChangeEvents)
    {
        for (auto&& control : m_menu->GetChildren())
        {
            MenuItem* child = pi_cast<MenuItem*>(control);

            if (child->GetName() == name)
                return SelectItem(child, fireChangeEvents);
        }
    }

    MenuItem* ComboBox::GetSelectedItem()
    {
        return m_selectedItem;
    }

    bool ComboBox::IsOpen() const
    {
        return m_menu && !m_menu->IsHidden();
    }

    void ComboBox::OpenList()
    {
        if (!m_menu)
            return;

        m_menu->SetParent(GetCanvas());
        m_menu->SetHidden(false);
        m_menu->BringToFront();
        Point p = LocalPositionToWindow(Point(0, 0));
        m_menu->SetBounds(Rect(p.x, p.y + Height(), Width(), m_menu->Height()));
    }

    void ComboBox::CloseList()
    {
        if (!m_menu)
            return;

        m_menu->Hide();
    }

    void ComboBox::ClearItems()
    {
        if (m_menu)
            m_menu->ClearItems();
    }

    MenuItem* ComboBox::AddItem(const PiString& label, const PiString& name)
    {
        MenuItem* item = m_menu->AddItem(label);
        item->SetName(name);
        item->On(MenuItem::SelectedEvent)->Add(this, &ComboBox::OnItemSelected);

        return item;
    }

    void ComboBox::OnItemSelected(EventInfo info)
    {
        // Convert selected to a menu item
        MenuItem* item = pi_cast<MenuItem*>(info.source);

        if (item)
            SelectItem(item);
    }

    void ComboBox::OnPress(EventInfo)
    {
        if (IsOpen())
            return GetCanvas()->CloseMenus();

        const bool bWasMenuHidden = m_menu->IsHidden();
        GetCanvas()->CloseMenus();

        if (bWasMenuHidden)
            OpenList();
    }

    bool ComboBox::OnKeyDown(Key key)
    {
        if (key == Key::Up)
        {
            const Widget::List& children = m_menu->GetChildren();
            auto it = std::find(children.rbegin(), children.rend(), m_selectedItem);

            if (it != children.rend() && (++it != children.rend()))
            {
                Widget* upElement = *it;
                OnItemSelected(EventInfo(upElement));
            }
        }
        else if (key == Key::Down)
        {
            const Widget::List& children = m_menu->GetChildren();
            auto it = std::find(children.begin(), children.end(), m_selectedItem);

            if (it != children.end() && (++it != children.end()))
            {
                Widget* upElement = *it;
                OnItemSelected(EventInfo(upElement));
            }
        }

        return true;
    }

    void ComboBox::OnLostKeyboardFocus()
    {}

    void ComboBox::OnGetKeyboardFocus()
    {}

    void ComboBox::Render(Skin* skin)
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
                SetTextColorOverride(skinData.ComboBox.textColorDisabled);
            else if (drawPressed)
                SetTextColorOverride(skinData.ComboBox.textColorPressed);
            else if (drawHovered)
                SetTextColorOverride(skinData.ComboBox.textColorHovered);
            else
                SetTextColorOverride(Colors::Transparent);
        }

        // Arrow
        {
            if (m_disabled)
                pi_cast<DownArrow*>(m_arrow)->SetBackgroundColor(skinData.ComboBox.arrowColorDisabled);
            else if (drawPressed)
                pi_cast<DownArrow*>(m_arrow)->SetBackgroundColor(skinData.ComboBox.arrowColorPressed);
            else if (drawHovered)
                pi_cast<DownArrow*>(m_arrow)->SetBackgroundColor(skinData.ComboBox.arrowColorHovered);
            else
                pi_cast<DownArrow*>(m_arrow)->SetBackgroundColor(skinData.ComboBox.arrowColorNormal);
        }

        if (!m_bDrawBackground)
            return;

        Rect rect = RenderBounds();

        // Background
        {
            const PiUInt32 offset = skinData.ComboBox.borderWidth;

            if (m_disabled)
                renderer->SetDrawColor(skinData.ComboBox.backgroundColorDisabled);
            else if (drawPressed)
                renderer->SetDrawColor(skinData.ComboBox.backgroundColorPressed);
            else if (drawHovered)
                renderer->SetDrawColor(skinData.ComboBox.backgroundColorHovered);
            else
                renderer->SetDrawColor(skinData.ComboBox.backgroundColorNormal);

            renderer->DrawFilledRect(rect + Rect(offset, offset, -offset * 2, -offset * 2), skinData.ComboBox.borderRadius);
        }

        // Border
        if (skinData.ComboBox.borderWidth > 0)
        {
            if (m_disabled)
                renderer->SetDrawColor(skinData.ComboBox.borderColorDisabled);
            else if (drawPressed)
                renderer->SetDrawColor(skinData.ComboBox.borderColorPressed);
            else if (drawHovered)
                renderer->SetDrawColor(skinData.ComboBox.borderColorHovered);
            else
                renderer->SetDrawColor(skinData.ComboBox.borderColorNormal);

            renderer->DrawLinedRect(rect, skinData.ComboBox.borderWidth, skinData.ComboBox.borderRadius);
        }
    }

    void ComboBox::RenderFocus(Skin* skin)
    {}

    void ComboBox::Layout(Skin* skin)
    {
        const Skin::Data& skinData = skin->GetSkinData();

        m_arrow->SetPosition(Alignment::Right | Alignment::CenterV, skinData.ComboBox.padding.right, 0);
        ParentClass::Layout(skin);
    }
} // namespace SparkyStudios::UI::Pixel
