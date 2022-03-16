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

#ifndef PIXEL_UI_COMBOBOX_H
#define PIXEL_UI_COMBOBOX_H

#include <SparkyStudios/UI/Pixel/Widgets/Button.h>

namespace SparkyStudios::UI::Pixel
{
    class Menu;
    class MenuItem;

    /**
     * @brief Displays a list of selectable items.
     */
    class PI_EXPORT ComboBox : public Button
    {
    public:
        static const PiString SelectionChangeEvent;

        PI_WIDGET(ComboBox, Button);

        /**
         * @brief Set the selected item to the specified one.
         *
         * @param item The item to be selected.
         * @param fireChangeEvents Defines if we should trigger a change event.
         */
        virtual void SelectItem(MenuItem* item, bool fireChangeEvents = true);

        /**
         * @brief Set the selected item to the one which have the specified name.
         *
         * @param item The name of the item to set as selected.
         * @param fireChangeEvents Defines if we should trigger a change event.
         */
        virtual void SelectItemByName(const PiString& name, bool fireChangeEvents = true);

        /**
         * @brief Get the selected item.
         *
         * @return The selected item, or null if there is no selected item.
         */
        virtual MenuItem* GetSelectedItem();

        /**
         * @brief Checks if the items list is opened.
         *
         * @return Whether the items list is opened.
         */
        virtual bool IsOpen() const;

        /**
         * @brief Open the list of items.
         */
        virtual void OpenList();

        /**
         * @brief Close the list of items.
         */
        virtual void CloseList();

        /**
         * @brief Clear the list of items.
         */
        virtual void ClearItems();

        /**
         * @brief Adds an item in the combo box.
         *
         * @param label The item label.
         * @param name The item name.
         *
         * @return The added item object.
         */
        virtual MenuItem* AddItem(const PiString& label, const PiString& name = "");

    protected:
        /**
         * @brief Event triggered when the selected item change.
         *
         * @param info The event info.
         */
        virtual void OnItemSelected(EventInfo info);

        void OnPress(EventInfo info) override;

        bool OnKeyDown(Key key) override;

        void OnLostKeyboardFocus() override;
        void OnGetKeyboardFocus() override;

        void Render(Skin* skin) override;
        void RenderFocus(Skin* skin) override;
        void Layout(Skin* skin) override;

        /**
         * @brief The menu component wrapped in this widget.
         */
        Menu* m_menu;

        /**
         * @brief The selected item.
         */
        MenuItem* m_selectedItem;

        /**
         * @brief The button component which trigger the open/close
         * of the combo box.
         */
        Widget* m_arrow;
    };
} // namespace SparkyStudios::UI::Pixel

#endif // PIXEL_UI_COMBOBOX_H
