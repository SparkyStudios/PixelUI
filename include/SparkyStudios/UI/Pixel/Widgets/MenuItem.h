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

#ifndef PIXEL_UI_MENUITEM_H
#define PIXEL_UI_MENUITEM_H

#include <SparkyStudios/UI/Pixel/Widgets/Button.h>

namespace SparkyStudios::UI::Pixel
{
    class Menu;

    /**
     * @brief Menu Item Widget.
     *
     * This widget provides useful functionalities for displaying a menu item inside
     * any menu-base widgets.
     */
    class PI_EXPORT MenuItem : public Button
    {
        friend class MenuStrip;

    public:
        /**
         * @brief The menu item selected event.
         *
         * This event is triggered when the user clicks on the menu item.
         */
        static const char* const SelectedEvent;

        /**
         * @brief The menu item checked event.
         *
         * This event is triggered when the user checks the checkbox
         * associated with the menu item. The menu item should be checkable.
         */
        static const char* const CheckedEvent;

        /**
         * @brief The menu item unchecked event.
         *
         * This event is triggered when the user uncheck the checkbox
         * associated with the menu item. The menu item should be checkable.
         */
        static const char* const UncheckedEvent;

        /**
         * @brief The menu item checkbox change event.
         *
         * This event is triggered when the user clicks on the checkbox
         * associated with the menu item. The menu item should be checkable.
         */
        static const char* const CheckChangedEvent;

        /**
         * @brief The submenu open mode.
         */
        enum class Mode
        {
            OpenOnClick,
            OpenOnHover,
        };

        PI_WIDGET(MenuItem, Button);

        virtual ~MenuItem();

        virtual void SetCheckable(bool checkable);

        virtual void SetChecked(bool checked);

        bool IsChecked() const;

        Menu* GetSubMenu();

        bool IsSubMenuOpen() const;

        void OpenSubMenu();

        void CloseSubMenu();

        void SetSubMenuOpenMode(Mode mode);
        Mode GetSubMenuOpenMode() const;

        template<typename T>
        MenuItem* SetAction(EventHandler* handler, T fn)
        {
            if (m_accelerator != nullptr)
                AddAccelerator(m_accelerator->GetText(), fn, handler);

            On(SelectedEvent)->Add(handler, fn);

            return this;
        }

        void SetAccelerator(const PiString& accel);

        bool InMenuStrip() const;

        void SizeToContents() override;

    protected:
        void OnPress(EventInfo info) override;

        void Render(Skin* skin) override;
        void Layout(Skin* skin) override;

        bool IsMenuWidget() const override;

        Menu* m_subMenu;
        Mode m_subMenuOpenMode;

        bool m_checkable;
        bool m_checked;

        Label* m_accelerator;

        Widget* m_submenuArrow;

    private:
        void CloseMenuHierarchy();
    };
} // namespace SparkyStudios::UI::Pixel

#endif // PIXEL_UI_MENUITEM_H
