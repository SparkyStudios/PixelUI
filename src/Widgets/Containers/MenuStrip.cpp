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

#include <SparkyStudios/UI/Pixel/Widgets/Containers/MenuStrip.h>
#include <SparkyStudios/UI/Pixel/Widgets/Shapes/Line.h>

namespace SparkyStudios::UI::Pixel
{
    PI_WIDGET_CONSTRUCTOR(MenuStrip)
    {
        SetHeight(24);
        SetDock(Alignment::Top);
    }

    void MenuStrip::Close()
    {}

    void MenuStrip::OnAddItem(EventInfo info)
    {
        auto* item = pi_cast<MenuItem*>(info.source);

        if (item == nullptr)
            return;

        item->SetDock(Alignment::Left);
        item->SetSubMenuOpenMode(MenuItem::Mode::OpenOnClick);
        item->SetPadding(Padding(8, 0));
        item->SizeToContents();
        item->On(MenuItem::MouseEnterEvent)->Add(this, &MenuStrip::OnHoverItem);
    }

    void MenuStrip::OnAddSeparator(EventInfo info)
    {
        auto* item = pi_cast<Line*>(info.source);

        if (item == nullptr)
            return;

        item->SetBackgroundColor(GetSkin()->GetSkinData().MenuStrip.separatorColor);
    }

    void MenuStrip::Render(Skin* skin)
    {
        BaseRenderer* renderer = skin->GetRenderer();
        const Skin::Data& skinData = skin->GetSkinData();

        if (m_disabled)
            renderer->SetDrawColor(skinData.MenuStrip.backgroundColorDisabled);
        else if (IsHovered())
            renderer->SetDrawColor(skinData.MenuStrip.backgroundColorHovered);
        else
            renderer->SetDrawColor(skinData.MenuStrip.backgroundColorNormal);

        renderer->DrawFilledRect(RenderBounds(), Size());
    }

    void MenuStrip::RenderUnder(Skin* skin)
    {}

    void MenuStrip::Layout(Skin* /*skin*/)
    {
        // TODO: We don't want to do vertical sizing the same as Menu, do nothing
        // for now
    }
} // namespace SparkyStudios::UI::Pixel
