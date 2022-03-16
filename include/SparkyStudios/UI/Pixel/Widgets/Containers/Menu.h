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

#ifndef PIXEL_UI_MENU_H
#define PIXEL_UI_MENU_H

#include <SparkyStudios/UI/Pixel/Widgets/Containers/ScrollContainer.h>
#include <SparkyStudios/UI/Pixel/Widgets/MenuItem.h>

namespace SparkyStudios::UI::Pixel
{
    class PI_EXPORT Menu : public ScrollContainer
    {
        friend class MenuItem;

    public:
        PI_WIDGET(Menu, ScrollContainer);

        virtual MenuItem* AddItem(const PiString& label, const PiString& icon = "", const PiString& accelerator = "");

        virtual void AddSeparator();

        void ClearItems();

        virtual void Open(Alignment alignment);
        virtual void Close();
        void CloseHierarchy(bool closeThis = true);

        bool IsMenuOpen();

        void SetDisableIconMargin(bool value);

    protected:
        void OnHoverItem(EventInfo info);
        virtual void OnAddItem(EventInfo info);
        virtual void OnAddSeparator(EventInfo info);

        void Render(Skin* skin) override;
        void RenderUnder(Skin* skin) override;

        void Layout(Skin* skin) override;

        bool IsMenuWidget() const override;

        bool m_deleteOnClose;
        bool m_disableIconMargin;

        MenuItem* m_parentMenuItem;
    };
} // namespace SparkyStudios::UI::Pixel

#endif // PIXEL_UI_MENU_H
