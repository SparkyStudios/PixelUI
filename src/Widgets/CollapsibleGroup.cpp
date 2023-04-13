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

#include <SparkyStudios/UI/Pixel/Widgets/CollapsibleGroup.h>

namespace SparkyStudios::UI::Pixel
{
    class CollapsibleGroupButton : public Button
    {
        PI_WIDGET_INLINE(CollapsibleGroupButton, Button)
        , m_bAlt(false)
        {
            SetAlignment(Alignment::Left | Alignment::CenterV);
        }

        void Render(Skin* skin) override
        {
            BaseRenderer* renderer = skin->GetRenderer();
            const Skin::Data& skinData = skin->GetSkinData();

            // TODO: Alt colors

            if (IsPressed() || GetToggleState())
                renderer->SetDrawColor(skinData.Button.backgroundColorPressed);
            else if (IsHovered())
                renderer->SetDrawColor(skinData.Button.backgroundColorHovered);
            else
                renderer->SetDrawColor(skinData.Button.backgroundColorNormal);

            renderer->DrawFilledRect(RenderBounds(), skinData.Button.borderRadius);
        }

        bool m_bAlt;
    };

    class CollapsibleGroupHeaderButton : public Button
    {
        PI_WIDGET_INLINE(CollapsibleGroupHeaderButton, Button)
        {
            m_bDrawBackground = false;
            SetIsToggle(true);
            SetAlignment(Alignment::Center);
        }
    };

    PI_WIDGET_CONSTRUCTOR(CollapsibleGroup)
    , m_list(nullptr)
    , m_button(new CollapsibleGroupHeaderButton(this))
    {
        m_button->SetText("Group Title");
        m_button->SetDock(Alignment::Top);
        m_button->SetHeight(20);
        SetPadding(Padding(1, 0, 1, 5));
        SetSize(512, 512);
    }

    Button* CollapsibleGroup::Add(const PiString& name)
    {
        auto* button = new CollapsibleGroupButton(this);
        button->SetText(name);
        button->SetDock(Alignment::Top);
        button->SizeToContents();
        button->SetSize(button->Width()+4, button->Height()+4);
        button->SetPadding(Padding(5, 2, 2, 2));
        button->On(Button::ClickEvent)->Add(this, &ThisClass::OnSelection);
        return button;
    }

    void CollapsibleGroup::OnSelection(EventInfo info)
    {
        auto* child = pi_cast<CollapsibleGroupButton*>(info.source);

        if (child == nullptr)
            return;

        if (m_list != nullptr)
            m_list->UnselectAll();
        else
            UnselectAll();

        child->SetToggleState(true);
        On(CollapsibleGroup::SelectionEvent)->Call(this);
    }

    void CollapsibleGroup::UnselectAll()
    {
        const List& list = GetChildren();

        for (const auto& widget : list)
        {
            auto* child = pi_cast<CollapsibleGroupButton*>(widget);

            if (child!= nullptr)
                child->SetToggleState(false);
        }
    }

    void CollapsibleGroup::Render(Skin* skin)
    {
       // TODO
    }

    void CollapsibleGroup::SetText(const PiString& text)
    {
        m_button->SetText(text);
    }

    void CollapsibleGroup::PostLayout(Skin* skin)
    {
        if (m_button->GetToggleState())
            SetHeight(m_button->Height());
        else
            SizeToChildren(false, true);

        const List& list = GetChildren();
        bool isAlt = true;

        for (const auto& widget : list)
        {
            auto* child = pi_cast<CollapsibleGroupButton*>(widget);

            if (child!= nullptr)
            {
                child->m_bAlt = isAlt;
                isAlt = !isAlt;
            }
        }
    }
} // namespace SparkyStudios::UI::Pixel