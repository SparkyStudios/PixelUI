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
#include <SparkyStudios/UI/Pixel/Widgets/Containers/Menu.h>
#include <SparkyStudios/UI/Pixel/Widgets/Shapes/Line.h>

namespace SparkyStudios::UI::Pixel
{
    PI_WIDGET_CONSTRUCTOR(Menu)
    , m_deleteOnClose(false), m_parentMenuItem(nullptr), m_disableIconMargin(true)
    {
        AutoHideBar(true);
        SetScroll(false, true);
    }

    MenuItem* Menu::AddItem(const PiString& label, const PiString& icon, const PiString& accelerator)
    {
        auto* item = new MenuItem(this);
        item->SetText(label);
        item->SetImage(icon);
        item->SetAccelerator(accelerator);
        OnAddItem(EventInfo(item));
        return item;
    }

    void Menu::AddSeparator()
    {
        auto* item = new Line(this);
        item->SetDock(Alignment::Top);
        item->SetMargin(Margin(4));
        OnAddSeparator(EventInfo(item));
    }

    void Menu::ClearItems()
    {
        for (auto&& child : m_innerPanel->GetChildren())
        {
            if (!child)
                continue;

            child->DelayedDelete();
        }
    }

    void Menu::Open(Alignment pos)
    {
        SetHidden(false);
        BringToFront();

        if (m_parentMenuItem)
        {
            switch (pos)
            {
            case Alignment::Bottom:
                SetPosition(m_parentMenuItem->Left(), m_parentMenuItem->Bottom());
                break;
            case Alignment::Right:
                SetPosition(m_parentMenuItem->GetParent()->Right(), m_parentMenuItem->Top() + m_parentMenuItem->GetParent()->Top());
                break;

            default:
                break;
            }
        }
        else
        {
            Point mousePos = Canvas::Input::GetMousePosition();
            SetPosition(mousePos.x, mousePos.y);
        }
    }

    void Menu::Close()
    {
        SetHidden(true);

        if (m_deleteOnClose)
            DelayedDelete();
    }

    void Menu::CloseHierarchy(bool closeThis)
    {
        for (auto&& child : m_innerPanel->GetChildren())
        {
            MenuItem* item = pi_cast<MenuItem*>(child);

            if (item == nullptr)
                continue;

            item->CloseSubMenu();
        }

        if (closeThis)
            Close();
    }

    bool Menu::IsMenuOpen()
    {
        return !IsHidden();
    }

    void Menu::SetDisableIconMargin(bool value)
    {
        m_disableIconMargin = value;
    }

    void Menu::OnHoverItem(EventInfo info)
    {
        auto* item = pi_cast<MenuItem*>(info.source);

        if (item == nullptr)
            return;

        if (item->GetSubMenuOpenMode() != MenuItem::Mode::OpenOnHover)
            return;

        if (item->IsSubMenuOpen())
            return;

        CloseHierarchy(false);
        item->OpenSubMenu();
    }

    void Menu::OnAddItem(EventInfo info)
    {
        auto* item = pi_cast<MenuItem*>(info.source);

        if (item == nullptr)
            return;

        item->SetPadding(Padding(m_disableIconMargin ? 16 : 32, 4, 16, 4));
        item->SetMargin(Margin(2));
        item->SetDock(Alignment::Top);
        item->SetSubMenuOpenMode(MenuItem::Mode::OpenOnHover);
        item->SizeToContents();
        item->SetAlignment(Alignment::CenterV | Alignment::Left);
        item->On(MenuItem::MouseEnterEvent)->Add(this, &Menu::OnHoverItem);

        // Do this here - after Top Docking these values mean nothing in layout
        const int w = (std::max)(item->Width() + item->GetMargin().left + item->GetMargin().right, Width());

        SetSize(w, Height());
    }

    void Menu::OnAddSeparator(EventInfo info)
    {
        auto* item = pi_cast<Line*>(info.source);

        if (item == nullptr)
            return;

        item->SetBackgroundColor(GetSkin()->GetSkinData().Menu.separatorColor);
    }

    void Menu::Render(Skin* skin)
    {
        BaseRenderer* renderer = skin->GetRenderer();
        const Skin::Data& skinData = skin->GetSkinData();

        if (m_disabled)
            renderer->SetDrawColor(skinData.Menu.backgroundColorDisabled);
        else if (IsHovered())
            renderer->SetDrawColor(skinData.Menu.backgroundColorHovered);
        else
            renderer->SetDrawColor(skinData.Menu.backgroundColorNormal);

        renderer->DrawFilledRect(RenderBounds(), skinData.Menu.radius);
    }

    void Menu::RenderUnder(Skin* skin)
    {
        ParentClass::RenderUnder(skin);

        BaseRenderer* renderer = skin->GetRenderer();
        const Skin::Data& skinData = skin->GetSkinData();

        renderer->SetDrawColor(skinData.Menu.shadowColor);
        renderer->DrawFilledRect(RenderBounds() + skinData.Menu.shadowOffset, skinData.Menu.shadowRadius);
    }

    void Menu::Layout(Skin* skin)
    {
        int menuHeight = 0;

        for (auto&& child : m_innerPanel->GetChildren())
        {
            if (child)
                menuHeight += child->Height() + child->GetMargin().top + child->GetMargin().bottom;
        }

        // if tall menu descends off-screen then clamp to screen height
        if (Y() + menuHeight > GetCanvas()->Height())
        {
            const int dy = GetCanvas()->Height() - Y();
            const int ytop = (std::max)(Y() - dy, 0);

            SetPosition(X(), ytop);
            menuHeight = (std::min)(menuHeight, GetCanvas()->Height());
        }

        SetSize(Width(), menuHeight);

        ParentClass::Layout(skin);
    }

    bool Menu::IsMenuWidget() const
    {
        return true;
    }
} // namespace SparkyStudios::UI::Pixel
