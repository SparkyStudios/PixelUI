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
#include <SparkyStudios/UI/Pixel/Widgets/Containers/MenuStrip.h>
#include <SparkyStudios/UI/Pixel/Widgets/MenuItem.h>
#include <SparkyStudios/UI/Pixel/Widgets/Shapes/RightArrow.h>

namespace SparkyStudios::UI::Pixel
{
    const char* const MenuItem::SelectedEvent = "MenuItem::Events::Selected";
    const char* const MenuItem::CheckedEvent = "MenuItem::Events::Checked";
    const char* const MenuItem::UncheckedEvent = "MenuItem::Events::Unchecked";
    const char* const MenuItem::CheckChangedEvent = "MenuItem::Events::CheckChanged";

    PI_WIDGET_CONSTRUCTOR(MenuItem)
    , m_subMenu(nullptr), m_submenuArrow(nullptr), m_accelerator(nullptr), m_checkable(false), m_checked(false)
    {}

    MenuItem::~MenuItem()
    {}

    void MenuItem::SetCheckable(bool checkable)
    {
        m_checkable = checkable;
    }

    void MenuItem::SetChecked(bool checked)
    {
        if (checked == m_checked)
            return;

        m_checked = checked;

        EventInfo info(this);
        info.data.boolean = checked;
        On(CheckChangedEvent)->Call(this, info);

        if (checked)
            On(CheckedEvent)->Call(this);
        else
            On(UncheckedEvent)->Call(this);
    }

    bool MenuItem::IsChecked() const
    {
        return m_checked;
    }

    Menu* MenuItem::GetSubMenu()
    {
        if (m_subMenu == nullptr)
        {
            m_subMenu = new Menu(GetCanvas());
            m_subMenu->m_parentMenuItem = this;
            m_subMenu->SetDisableIconMargin(!InMenuStrip());
            m_subMenu->Close();

            if (!InMenuStrip())
            {
                m_submenuArrow = new RightArrow(this);
                m_submenuArrow->SetSize(8, 8);
                m_submenuArrow->SetPosition(Alignment::CenterV | Alignment::Right, 8, 0);
                pi_cast<RightArrow*>(m_submenuArrow)->SetBackgroundColor(GetSkin()->GetSkinData().Menu.arrowColor);
            }
        }

        return m_subMenu;
    }

    bool MenuItem::IsSubMenuOpen() const
    {
        if (m_subMenu == nullptr)
            return false;

        return m_subMenu->IsMenuOpen();
    }

    void MenuItem::OpenSubMenu()
    {
        if (m_subMenu == nullptr)
            return;

        m_subMenu->SetHidden(false);
        m_subMenu->Open(InMenuStrip() ? Alignment::Bottom : Alignment::Right);
    }

    void MenuItem::CloseSubMenu()
    {
        if (m_subMenu == nullptr)
            return;

        m_subMenu->CloseHierarchy();
    }

    void MenuItem::SetSubMenuOpenMode(Mode mode)
    {
        m_subMenuOpenMode = mode;
    }

    MenuItem::Mode MenuItem::GetSubMenuOpenMode() const
    {
        return m_subMenuOpenMode;
    }

    void MenuItem::SetAccelerator(const PiString& accel)
    {
        if (m_accelerator != nullptr)
        {
            m_accelerator->DelayedDelete();
            m_accelerator = nullptr;
        }

        if (accel.empty())
            return;

        m_accelerator = new Label(this);
        m_accelerator->SetMargin(Margin(16, 0, 8, 0));
        m_accelerator->SetDock(Alignment::Right);
        m_accelerator->SetAlignment(Alignment::Right | Alignment::CenterV);
        m_accelerator->SetText(accel);
        m_accelerator->SetFontSize(10);
        m_accelerator->SetTextColor(GetSkin()->GetSkinData().Menu.acceleratorColor);
    }

    bool MenuItem::InMenuStrip() const
    {
        auto* menu = pi_cast<MenuStrip*>(m_parent);
        return menu != nullptr;
    }

    void MenuItem::SizeToContents()
    {
        ParentClass::SizeToContents();

        PiInt32 w = Width();

        if (m_accelerator != nullptr)
        {
            m_accelerator->SizeToContents();
            w += m_accelerator->Width() + m_accelerator->GetMargin().left + m_accelerator->GetMargin().right;
        }

        if (m_submenuArrow != nullptr)
        {
            w += m_submenuArrow->Width() + m_submenuArrow->GetMargin().left + m_submenuArrow->GetMargin().right;
        }

        SetWidth(w);
    }

    void MenuItem::OnPress(EventInfo info)
    {
        const bool inStrip = InMenuStrip();

        if (inStrip)
        {
            CloseMenuHierarchy();
        }

        if (m_subMenu != nullptr)
        {
            OpenSubMenu();
        }
        else if (!inStrip)
        {
            if (m_checkable)
                SetChecked(!m_checked);

            On(SelectedEvent)->Call(this);
            CloseMenuHierarchy();
        }

        ParentClass::OnPress(info);
    }

    void MenuItem::Render(Skin* skin)
    {
        BaseRenderer* renderer = skin->GetRenderer();
        const Skin::Data& skinData = skin->GetSkinData();

        if (InMenuStrip())
        {
            if (m_disabled)
                renderer->SetDrawColor(skinData.MenuStrip.itemsBackgroundDisabled);
            else if (m_pressed)
                renderer->SetDrawColor(skinData.MenuStrip.itemsBackgroundPressed);
            else if (IsHovered())
                renderer->SetDrawColor(skinData.MenuStrip.itemsBackgroundHovered);
            else
                renderer->SetDrawColor(skinData.MenuStrip.itemsBackgroundNormal);
        }
        else
        {
            if (m_disabled)
                renderer->SetDrawColor(skinData.Menu.itemsBackgroundDisabled);
            else if (m_pressed)
                renderer->SetDrawColor(skinData.Menu.itemsBackgroundPressed);
            else if (IsHovered())
                renderer->SetDrawColor(skinData.Menu.itemsBackgroundHovered);
            else
                renderer->SetDrawColor(skinData.Menu.itemsBackgroundNormal);
        }

        renderer->DrawFilledRect(RenderBounds(), Size());
    }

    void MenuItem::Layout(Skin* skin)
    {
        if (m_submenuArrow != nullptr)
            m_submenuArrow->SetPosition(Alignment::Right | Alignment::CenterV, 8, 0);

        ParentClass::Layout(skin);
    }

    bool MenuItem::IsMenuWidget() const
    {
        return true;
    }

    void MenuItem::CloseMenuHierarchy()
    {
        auto* menu = pi_cast<Menu*>(m_parent);
        if (menu != nullptr)
        {
            if (menu->m_parentMenuItem != nullptr)
                menu->m_parentMenuItem->CloseMenuHierarchy();
            else
                menu->CloseHierarchy();
        }
    }
} // namespace SparkyStudios::UI::Pixel
