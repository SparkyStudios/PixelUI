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

#ifndef PIXEL_UI_COLLAPSIBLEITEMSCONTAINER_H
#define PIXEL_UI_COLLAPSIBLEITEMSCONTAINER_H

#include <SparkyStudios/UI/Pixel/Widgets/CollapsibleGroup.h>
#include <SparkyStudios/UI/Pixel/Widgets/Containers/ScrollContainer.h>

namespace SparkyStudios::UI::Pixel
{
    class PI_EXPORT CollapsibleItemsContainer : public ScrollContainer
    {
    public:
        static const PiString SelectionEvent;

        PI_WIDGET(CollapsibleItemsContainer, ScrollContainer);

        virtual void Add(CollapsibleGroup* category);

        virtual CollapsibleGroup* Add(const PiString& name);

        void Render() override;

        void UnselectAll();

        Button* GetSelected();

    protected:
        virtual void OnSelection(CollapsibleGroup* control, Button* selected)
        {
            On(SelectionEvent)->Call(this);
        }

        void OnSelectionEvent(EventInfo info)
        {
            auto child = pi_cast<CollapsibleGroup*>(info.source);

            if (child)
            {
                OnSelection(child, child->GetSelected());
            }
        }
    };
} // namespace SparkyStudios::UI::Pixel

#endif // PIXEL_UI_COLLAPSIBLEITEMSCONTAINER_H
