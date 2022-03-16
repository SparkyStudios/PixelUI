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

#ifndef PIXEL_UI_SCROLLCONTAINER_H
#define PIXEL_UI_SCROLLCONTAINER_H

#include <SparkyStudios/UI/Pixel/Widgets/Widget.h>

namespace SparkyStudios::UI::Pixel
{
    class PI_EXPORT ScrollContainer : public Widget
    {
    public:
        PI_WIDGET(ScrollContainer, Widget);

        virtual void SetScroll(bool horizontal, bool vertical);

        virtual void SetContentSize(int width, int height);

        virtual void SetContentSize(const Size& size);

        virtual void AutoHideBar(bool value);

        virtual bool CanScrollX() const;

        virtual bool CanScrollY() const;

        virtual void ScrollToTop();

        virtual void ScrollToBottom();

        virtual void ScrollToLeft();

        virtual void ScrollToRight();

        void ClearChildren() override;

    protected:
        virtual void OnVerticalScrollMove(EventInfo);

        virtual void OnHorizontalScrollMove(EventInfo);

        bool OnMouseWheel(const Point& delta) override;

        void NotifyBoundsChanged(const Rect& old, Widget* child) override;

        void Layout(Skin* skin) override;

        void Render(Skin* skin) override;

        bool m_scrollH;
        bool m_scrollV;
        bool m_autoHideScrollbars;

        Widget* m_scrollbarH;
        Widget* m_scrollbarV;

    private:
        void UpdateScroll();
        void SetVScrollRequired(bool value);
        void SetHScrollRequired(bool value);
    };
} // namespace SparkyStudios::UI::Pixel

#endif // PIXEL_UI_SCROLLCONTAINER_H
