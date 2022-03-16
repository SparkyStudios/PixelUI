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

#include <SparkyStudios/UI/Pixel/Widgets/Label.h>
#include <SparkyStudios/UI/Pixel/Widgets/StatusBar.h>

namespace SparkyStudios::UI::Pixel
{
    PI_WIDGET_CONSTRUCTOR(StatusBar)
    {
        const Skin::Data& skinData = GetSkin()->GetSkinData();

        SetHeight(skinData.StatusBar.height);
        SetPadding(skinData.StatusBar.padding);

        SetDock(Alignment::Bottom);
    }

    void StatusBar::SetText(const PiString& text, Alignment alignment, Alignment dock)
    {
        ClearChildren();
        auto* label = new Label(this, "StatusBar.Label");
        label->SetText(text);
        label->SetDock(dock);
        label->SetAlignment(alignment);
        AddWidget(label);
    }

    void StatusBar::AddWidget(Widget* widget)
    {
        widget->SetParent(this);
    }

    void StatusBar::Render(Skin* skin)
    {
        BaseRenderer* renderer = skin->GetRenderer();
        const Skin::Data& skinData = skin->GetSkinData();

        const Rect& rect = RenderBounds();

        if (m_disabled)
            renderer->SetDrawColor(skinData.StatusBar.backgroundColorDisabled);
        else
            renderer->SetDrawColor(skinData.StatusBar.backgroundColorNormal);

        renderer->DrawFilledRect(rect, Size(0, 0));
    }
} // namespace SparkyStudios::UI::Pixel
