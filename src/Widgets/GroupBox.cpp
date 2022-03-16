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

#include <SparkyStudios/UI/Pixel/Widgets/GroupBox.h>

namespace SparkyStudios::UI::Pixel
{
    PI_WIDGET_CONSTRUCTOR(GroupBox)
    {
        const Skin::Data& skinData = GetSkin()->GetSkinData();
        const Font& font = GetFont();

        SetMouseInputEnabled(true);
        SetAlignment(Alignment::Top | Alignment::Left);

        SetTextColor(skinData.GroupBox.textColor);
        SetFont(font.facename, skinData.GroupBox.textSize, font.weight);

        m_innerPanel = new Widget(this);
        m_innerPanel->SetDock(Alignment::Fill);

        Invalidate();
    }

    void GroupBox::SetAlignment(Alignment alignment)
    {
        const Skin::Data& skinData = GetSkin()->GetSkinData();

        if (alignment & Alignment::Left)
        {
            SetPadding(Padding(skinData.GroupBox.titleMargin, 0, 0, 0));
        }
        else if (alignment & Alignment::Right)
        {
            SetPadding(Padding(0, 0, 0, skinData.GroupBox.titleMargin));
        }

        ParentClass::SetAlignment(alignment);
    }

    void GroupBox::Render(Skin* skin)
    {
        Rect rect = RenderBounds();

        const Skin::Data& skinData = skin->GetSkinData();
        BaseRenderer* renderer = skin->GetRenderer();

        const PiInt32 textStart = TextX() + m_text->GetPadding().left;
        const PiInt32 textWidth = GetTextWidth() - m_text->GetPadding().left;
        const PiInt32 textHeight = GetTextHeight();

        rect.y += textHeight / 2;
        rect.h -= textHeight / 2;

        const Size radii = Size(0, 0);
        const PiUInt32& borderWidth = skinData.GroupBox.borderWidth;

        renderer->SetDrawColor(skinData.GroupBox.borderColor);
        renderer->DrawFilledRect(Rect(rect.x, rect.y, textStart - 4, borderWidth), radii);
        renderer->DrawFilledRect(
            Rect(rect.x + textStart + textWidth + 4, rect.y, rect.w - (textStart + textWidth + 4), borderWidth), radii);
        renderer->DrawFilledRect(Rect(rect.x, (rect.y + rect.h) - 1, rect.w, borderWidth), radii);
        renderer->DrawFilledRect(Rect(rect.x, rect.y + 1, borderWidth, rect.h - 1), radii);
        renderer->DrawFilledRect(Rect((rect.x + rect.w) - 1, rect.y + 1, borderWidth, rect.h - 1), radii);
    }

    void GroupBox::Layout(Skin* skin)
    {
        m_innerPanel->SetMargin(skin->GetSkinData().GroupBox.padding);
        ParentClass::Layout(skin);
    }
} // namespace SparkyStudios::UI::Pixel
