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

#ifndef PIXEL_UI_COLLAPSIBLEGROUP_H
#define PIXEL_UI_COLLAPSIBLEGROUP_H

#include <SparkyStudios/UI/Pixel/Widgets/Button.h>

namespace SparkyStudios::UI::Pixel
{
    class CollapsibleList;

    class PI_EXPORT CollapsibleGroup : public Widget
    {
    public:
        static const PiString SelectionEvent;

        PI_WIDGET(CollapsibleGroup, Widget);

        void Render(Skin* skin) override;
        virtual void SetText(const PiString& text);
        virtual Button* Add(const PiString& name);

        void PostLayout(Skin* skin) override;

        /**
         * @brief Sets the items list.
         *
         * @param list The items list to set.
         */
        virtual void SetList(CollapsibleList* list)
        {
            m_list = list;
        }

        /**
         * @brief Unselects all items.
         */
        virtual void UnselectAll();

        /**
         * @brief Get the selected item.
         *
         * @return The selected item.
         */
        virtual Button* GetSelected();

        /**
         * @brief Sets the expanded state of the group.
         *
         * @param expanded The expanded state of the group.
         */
        void SetExpanded(bool expanded);

        /**
         * @brief Gets the current expanded state of the group.
         *
         * @return True if the group is expanded, false otherwise.
         */
        bool IsExpanded() const;

    protected:
        virtual void OnSelection(EventInfo event);

        Button* m_button;
        CollapsibleList* m_list;
    };
} // namespace SparkyStudios::UI::Pixel

#endif // PIXEL_UI_COLLAPSIBLEGROUP_H
