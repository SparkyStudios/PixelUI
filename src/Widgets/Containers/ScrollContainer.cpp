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

#include <SparkyStudios/UI/Pixel/Core/Utility.h>
#include <SparkyStudios/UI/Pixel/Widgets/Button.h>
#include <SparkyStudios/UI/Pixel/Widgets/Canvas.h>
#include <SparkyStudios/UI/Pixel/Widgets/Containers/ScrollContainer.h>
#include <SparkyStudios/UI/Pixel/Widgets/DraggableWidget.h>
#include <SparkyStudios/UI/Pixel/Widgets/Shapes/DownArrow.h>
#include <SparkyStudios/UI/Pixel/Widgets/Shapes/LeftArrow.h>
#include <SparkyStudios/UI/Pixel/Widgets/Shapes/RightArrow.h>
#include <SparkyStudios/UI/Pixel/Widgets/Shapes/UpArrow.h>

namespace SparkyStudios::UI::Pixel
{
    class ScrollButton : public Button
    {
    protected:
        PI_WIDGET_INLINE(ScrollButton, Button)
        {}

        void Render(Skin* skin) override
        {
            BaseRenderer* renderer = skin->GetRenderer();
            const Skin::Data& skinData = skin->GetSkinData();

            const Rect& r = RenderBounds();

            if (m_disabled)
                renderer->SetDrawColor(skinData.ScrollContainer.buttonBackgroundDisabled);
            else if (m_pressed)
                renderer->SetDrawColor(skinData.ScrollContainer.buttonBackgroundPressed);
            else if (IsHovered())
                renderer->SetDrawColor(skinData.ScrollContainer.buttonBackgroundHovered);
            else
                renderer->SetDrawColor(skinData.ScrollContainer.buttonBackgroundNormal);

            renderer->DrawFilledRect(r, Size(0, 0));
        }

        bool IsMenuWidget() const override
        {
            return true;
        }
    };

    class Scroll : public DraggableWidget
    {
    public:
        PI_WIDGET_INLINE(Scroll, DraggableWidget)
        {
            m_restrictToParent = true;
            m_target = this;
        }

        void OnMouseMove(const Point& position, PiInt32 deltaX, PiInt32 deltaY) override
        {
            ParentClass::OnMouseMove(position, deltaX, deltaY);

            if (!m_pressed)
                return;

            InvalidateParent();
        }

        void OnMouseButton(const Point& position, MouseButton button, MouseButtonPressMode state) override
        {
            ParentClass::OnMouseButton(position, button, state);
            InvalidateParent();
        }

        void Render(Skin* skin) override
        {
            BaseRenderer* renderer = skin->GetRenderer();
            const Skin::Data& skinData = skin->GetSkinData();

            if (m_disabled)
                renderer->SetDrawColor(skinData.ScrollContainer.scrollBackgroundDisabled);
            else if (m_pressed)
                renderer->SetDrawColor(skinData.ScrollContainer.scrollBackgroundPressed);
            else if (IsHovered())
                renderer->SetDrawColor(skinData.ScrollContainer.scrollBackgroundHovered);
            else
                renderer->SetDrawColor(skinData.ScrollContainer.scrollBackgroundNormal);

            renderer->DrawFilledRect(RenderBounds(), skinData.ScrollContainer.scrollRadius);
        }

        void Layout(Skin* skin) override
        {
            if (m_parent == nullptr)
                return;

            // TODO: MoveTo()
        }

        bool IsMenuWidget() const override
        {
            return true;
        }
    };

    class BaseScrollBar : public Widget
    {
    public:
        static const char* const ScrollMoveEvent;
        static const char* const NudgeNegativeEvent;
        static const char* const NudgePositiveEvent;

        PI_WIDGET_INLINE(BaseScrollBar, Widget)
        {
            m_scrollButtonNegative = new ScrollButton(this);
            m_scroll = new Scroll(this);
            m_scrollButtonPositive = new ScrollButton(this);

            m_pressed = false;

            m_scrollAmount = 0.0f;
            m_contentSize = 0.0f;
            m_viewableContentSize = 0.0f;

            SetNudgeAmount(20);
        }

        virtual void SetScrollSize(int size) = 0;
        virtual int GetScrollSize() = 0;
        virtual int GetScrollPosition() = 0;

        virtual void ScrollToStart()
        {}

        virtual void ScrollToEnd()
        {}

        virtual PiReal32 GetNudgeAmount()
        {
            return m_nudgeAmount / m_contentSize;
        }

        virtual void SetNudgeAmount(PiReal32 nudge)
        {
            m_nudgeAmount = nudge;
        }

        virtual bool SetScrollAmount(PiReal32 amount, bool forceUpdate)
        {
            if (m_scrollAmount == amount && !forceUpdate)
                return false;

            m_scrollAmount = amount;
            Invalidate();
            NotifyScrollMove();

            return true;
        }

        virtual void SetContentSize(PiReal32 size)
        {
            if (m_contentSize != size)
                Invalidate();

            m_contentSize = size;
        }

        virtual void SetViewableContentSize(PiReal32 size)
        {
            if (m_viewableContentSize != size)
                Invalidate();

            m_viewableContentSize = size;
        }

        virtual PiReal32 GetScrollAmount()
        {
            return m_scrollAmount;
        }

    protected:
        virtual PiReal32 CalculateScrollAmount()
        {
            return 0;
        }

        virtual int CalculateScrollSize()
        {
            return 0;
        }

        virtual void NotifyScrollMove()
        {
            OnScrollMove(EventInfo(this));
        }

        virtual void OnNudgeNegative(EventInfo info)
        {
            On(NudgeNegativeEvent)->Call(this, info);
        }

        virtual void OnNudgePositive(EventInfo info)
        {
            On(NudgePositiveEvent)->Call(this, info);
        }

        virtual void OnScrollMove(EventInfo info)
        {
            On(ScrollMoveEvent)->Call(this, info);
        }

        void OnMouseButton(const Point& position, MouseButton button, MouseButtonPressMode state) override
        {}

        void Render(Skin* skin) override
        {
            BaseRenderer* renderer = skin->GetRenderer();
            const Skin::Data& skinData = skin->GetSkinData();

            if (m_disabled)
                renderer->SetDrawColor(skinData.ScrollContainer.scrollBarBackgroundDisabled);
            else if (m_pressed)
                renderer->SetDrawColor(skinData.ScrollContainer.scrollBarBackgroundPressed);
            else if (IsHovered())
                renderer->SetDrawColor(skinData.ScrollContainer.scrollBarBackgroundHovered);
            else
                renderer->SetDrawColor(skinData.ScrollContainer.scrollBarBackgroundNormal);

            renderer->DrawFilledRect(RenderBounds(), skinData.ScrollContainer.scrollBarRadius);
        }

        bool IsMenuWidget() const override
        {
            return true;
        }

        Button* m_scrollButtonNegative;
        Scroll* m_scroll;
        Button* m_scrollButtonPositive;

        bool m_pressed;
        PiReal32 m_scrollAmount;
        PiReal32 m_contentSize;
        PiReal32 m_viewableContentSize;
        PiReal32 m_nudgeAmount;
    };

    const char* const BaseScrollBar::ScrollMoveEvent = "BaseScrollBar::Events::ScrollMove";
    const char* const BaseScrollBar::NudgeNegativeEvent = "BaseScrollBar::Events::NudgeNegative";
    const char* const BaseScrollBar::NudgePositiveEvent = "BaseScrollBar::Events::NudgePositive";

    class VScrollBar : public BaseScrollBar
    {
        PI_WIDGET_INLINE(VScrollBar, BaseScrollBar)
        {
            const Skin::Data& skinData = GetSkin()->GetSkinData();

            m_upArrow = new UpArrow(m_scrollButtonNegative);
            m_upArrow->DrawBackground(true);
            m_upArrow->DrawBorder(false);
            m_upArrow->SetDock(Alignment::Center);
            m_upArrow->SetBackgroundColor(skinData.ScrollContainer.arrowColorNormal);

            m_downArrow = new DownArrow(m_scrollButtonPositive);
            m_downArrow->DrawBackground(true);
            m_downArrow->DrawBorder(false);
            m_downArrow->SetDock(Alignment::Center);
            m_downArrow->SetBackgroundColor(skinData.ScrollContainer.arrowColorNormal);

            m_scrollButtonNegative->On(ScrollButton::ClickEvent)->Add(this, &VScrollBar::OnNudgeNegative);
            m_scrollButtonNegative->On(ScrollButton::MouseEnterEvent)->Add(this, &VScrollBar::OnUpArrowEntered);
            m_scrollButtonNegative->On(ScrollButton::MouseButtonDownEvent)->Add(this, &VScrollBar::OnUpArrowPressed);
            m_scrollButtonNegative->On(ScrollButton::MouseButtonUpEvent)->Add(this, &VScrollBar::OnUpArrowReleased);
            m_scrollButtonNegative->On(ScrollButton::MouseLeaveEvent)->Add(this, &VScrollBar::OnUpArrowLeaved);
            m_scroll->On(Scroll::DragEvent)->Add(this, &VScrollBar::OnScrollMove);
            m_scrollButtonPositive->On(ScrollButton::ClickEvent)->Add(this, &VScrollBar::OnNudgePositive);
            m_scrollButtonPositive->On(ScrollButton::MouseEnterEvent)->Add(this, &VScrollBar::OnDownArrowEntered);
            m_scrollButtonPositive->On(ScrollButton::MouseButtonDownEvent)->Add(this, &VScrollBar::OnDownArrowPressed);
            m_scrollButtonPositive->On(ScrollButton::MouseButtonUpEvent)->Add(this, &VScrollBar::OnDownArrowReleased);
            m_scrollButtonPositive->On(ScrollButton::MouseLeaveEvent)->Add(this, &VScrollBar::OnDownArrowLeaved);
        }

        int GetScrollSize() override
        {
            return m_scroll->Height();
        }

        int GetScrollPosition() override
        {
            return m_scroll->Y() - Width();
        }

        void SetScrollSize(int size) override
        {
            m_scroll->SetHeight(size);
        }

        void ScrollToStart() override
        {
            SetScrollAmount(0.0f, true);
        }

        void ScrollToEnd() override
        {
            SetScrollAmount(1.0f, true);
        }

        PiReal32 GetNudgeAmount() override
        {
            if (m_pressed)
                return m_viewableContentSize / m_contentSize;
            else
                return ParentClass::GetNudgeAmount();
        }

        bool SetScrollAmount(PiReal32 amount, bool forceUpdate) override
        {
            amount = Clamp(amount, 0.f, 1.f);

            if (!ParentClass::SetScrollAmount(amount, forceUpdate))
                return false;

            if (forceUpdate)
            {
                const Skin::Data& skinData = GetSkin()->GetSkinData();
                const PiInt32 scrollWidth = skinData.ScrollContainer.scrollSize.w;

                const int newY = scrollWidth + amount * (Height() - m_scroll->Height() - scrollWidth * 2);
                m_scroll->MoveTo(m_scroll->X(), newY);
            }

            return true;
        }

    protected:
        void OnScrollMove(EventInfo info) override
        {
            if (m_scroll->IsDragging())
            {
                SetScrollAmount(CalculateScrollAmount(), false);
                ParentClass::OnScrollMove(info);
            }
            else
            {
                InvalidateParent();
            }
        }

        void OnMouseButton(const Point& position, MouseButton button, MouseButtonPressMode state) override
        {
            if (state == MouseButtonPressMode::Pressed)
            {
                m_pressed = true;
                Canvas::SetMouseFocusedWidget(this);
            }
            else
            {
                Point clickPos = WindowPositionToLocal(position);

                if (clickPos.y < m_scroll->Y())
                    OnNudgeNegative(EventInfo(this));
                else if (clickPos.y > m_scroll->Y() + m_scroll->Height())
                    OnNudgePositive(EventInfo(this));

                m_pressed = false;
                Canvas::SetMouseFocusedWidget(nullptr);
            }
        }

        void Layout(Skin* skin) override
        {
            ParentClass::Layout(skin);

            const Skin::Data& skinData = skin->GetSkinData();
            const PiInt32 scrollWidth = skinData.ScrollContainer.scrollSize.w;

            SetWidth(scrollWidth);

            m_upArrow->SetSize(scrollWidth / 2, scrollWidth / 2);
            m_upArrow->SetPosition(Alignment::Center, 0, 0);

            m_downArrow->SetSize(scrollWidth / 2, scrollWidth / 2);
            m_downArrow->SetPosition(Alignment::Center, 0, 0);

            m_scrollButtonNegative->SetDock(Alignment::Top);
            m_scrollButtonNegative->SetSize(scrollWidth, scrollWidth);

            m_scroll->SetWidth(scrollWidth);
            m_scroll->SetPadding(Padding(0, scrollWidth));

            PiReal32 barHeight = (m_viewableContentSize / m_contentSize) * (Height() - scrollWidth);

            m_scroll->SetHeight(barHeight);
            m_scroll->SetHidden(Height() - (scrollWidth * 2) <= barHeight);

            // Based on our last scroll amount, produce a position for the bar
            if (!m_scroll->IsDragging())
                SetScrollAmount(GetScrollAmount(), true);

            m_scrollButtonPositive->SetDock(Alignment::Bottom);
            m_scrollButtonPositive->SetSize(scrollWidth, scrollWidth);
        }

        virtual void OnNudgeNegative(EventInfo info) override
        {
            if (!m_disabled)
            {
                SetScrollAmount(GetScrollAmount() - GetNudgeAmount(), true);
                ParentClass::OnNudgeNegative(info);
            }
        }

        virtual void OnNudgePositive(EventInfo info) override
        {
            if (!m_disabled)
            {
                SetScrollAmount(GetScrollAmount() + GetNudgeAmount(), true);
                ParentClass::OnNudgePositive(info);
            }
        }

        PiReal32 CalculateScrollAmount() override
        {
            const Skin::Data& skinData = GetSkin()->GetSkinData();
            const PiInt32 scrollWidth = skinData.ScrollContainer.scrollSize.w;

            return static_cast<PiReal32>((m_scroll->Y() - scrollWidth)) / (Height() - m_scroll->Height() - scrollWidth * 2);
        }

    private:
        void OnUpArrowEntered(EventInfo)
        {
            const Skin::Data& skinData = GetSkin()->GetSkinData();
            m_upArrow->SetBackgroundColor(skinData.ScrollContainer.arrowColorHovered);
        }

        void OnUpArrowPressed(EventInfo)
        {
            const Skin::Data& skinData = GetSkin()->GetSkinData();
            m_upArrow->SetBackgroundColor(skinData.ScrollContainer.arrowColorPressed);
        }

        void OnUpArrowReleased(EventInfo)
        {
            const Skin::Data& skinData = GetSkin()->GetSkinData();
            m_upArrow->SetBackgroundColor(skinData.ScrollContainer.arrowColorHovered);
        }

        void OnUpArrowLeaved(EventInfo)
        {
            const Skin::Data& skinData = GetSkin()->GetSkinData();
            m_upArrow->SetBackgroundColor(skinData.ScrollContainer.arrowColorNormal);
        }

        void OnDownArrowEntered(EventInfo)
        {
            const Skin::Data& skinData = GetSkin()->GetSkinData();
            m_downArrow->SetBackgroundColor(skinData.ScrollContainer.arrowColorHovered);
        }

        void OnDownArrowPressed(EventInfo)
        {
            const Skin::Data& skinData = GetSkin()->GetSkinData();
            m_downArrow->SetBackgroundColor(skinData.ScrollContainer.arrowColorPressed);
        }

        void OnDownArrowReleased(EventInfo)
        {
            const Skin::Data& skinData = GetSkin()->GetSkinData();
            m_downArrow->SetBackgroundColor(skinData.ScrollContainer.arrowColorHovered);
        }

        void OnDownArrowLeaved(EventInfo)
        {
            const Skin::Data& skinData = GetSkin()->GetSkinData();
            m_downArrow->SetBackgroundColor(skinData.ScrollContainer.arrowColorNormal);
        }

        UpArrow* m_upArrow;
        DownArrow* m_downArrow;
    };

    class HScrollBar : public BaseScrollBar
    {
        PI_WIDGET_INLINE(HScrollBar, BaseScrollBar)
        {
            const Skin::Data& skinData = GetSkin()->GetSkinData();

            m_leftArrow = new LeftArrow(m_scrollButtonNegative);
            m_leftArrow->DrawBackground(true);
            m_leftArrow->DrawBorder(false);
            m_leftArrow->SetDock(Alignment::Center);
            m_leftArrow->SetBackgroundColor(skinData.ScrollContainer.arrowColorNormal);

            m_rightArrow = new RightArrow(m_scrollButtonPositive);
            m_rightArrow->DrawBackground(true);
            m_rightArrow->DrawBorder(false);
            m_rightArrow->SetDock(Alignment::Center);
            m_rightArrow->SetBackgroundColor(skinData.ScrollContainer.arrowColorNormal);

            m_scrollButtonNegative->On(ScrollButton::ClickEvent)->Add(this, &HScrollBar::OnNudgeNegative);
            m_scrollButtonNegative->On(ScrollButton::MouseEnterEvent)->Add(this, &HScrollBar::OnLeftArrowEntered);
            m_scrollButtonNegative->On(ScrollButton::MouseButtonDownEvent)->Add(this, &HScrollBar::OnLeftArrowPressed);
            m_scrollButtonNegative->On(ScrollButton::MouseButtonUpEvent)->Add(this, &HScrollBar::OnLeftArrowReleased);
            m_scrollButtonNegative->On(ScrollButton::MouseLeaveEvent)->Add(this, &HScrollBar::OnLeftArrowLeaved);
            m_scroll->On(Scroll::DragEvent)->Add(this, &HScrollBar::OnScrollMove);
            m_scrollButtonPositive->On(ScrollButton::ClickEvent)->Add(this, &HScrollBar::OnNudgePositive);
            m_scrollButtonPositive->On(ScrollButton::MouseEnterEvent)->Add(this, &HScrollBar::OnRightArrowEntered);
            m_scrollButtonPositive->On(ScrollButton::MouseButtonDownEvent)->Add(this, &HScrollBar::OnRightArrowPressed);
            m_scrollButtonPositive->On(ScrollButton::MouseButtonUpEvent)->Add(this, &HScrollBar::OnRightArrowReleased);
            m_scrollButtonPositive->On(ScrollButton::MouseLeaveEvent)->Add(this, &HScrollBar::OnRightArrowLeaved);
        }

        int GetScrollSize() override
        {
            return m_scroll->Width();
        }

        int GetScrollPosition() override
        {
            return m_scroll->X() - Height();
        }

        void SetScrollSize(int size) override
        {
            m_scroll->SetWidth(size);
        }

        void ScrollToStart() override
        {
            SetScrollAmount(0.0f, true);
        }

        void ScrollToEnd() override
        {
            SetScrollAmount(1.0f, true);
        }

        PiReal32 GetNudgeAmount() override
        {
            if (m_pressed)
                return m_viewableContentSize / m_contentSize;
            else
                return ParentClass::GetNudgeAmount();
        }

        bool SetScrollAmount(PiReal32 amount, bool forceUpdate) override
        {
            amount = Clamp(amount, 0.f, 1.f);

            if (!ParentClass::SetScrollAmount(amount, forceUpdate))
                return false;

            if (forceUpdate)
            {
                const Skin::Data& skinData = GetSkin()->GetSkinData();
                const PiInt32 scrollHeight = skinData.ScrollContainer.scrollSize.h;

                const int newX = scrollHeight + amount * (Width() - m_scroll->Width() - scrollHeight * 2);
                m_scroll->MoveTo(newX, m_scroll->Y());
            }

            return true;
        }

    protected:
        void OnScrollMove(EventInfo info) override
        {
            if (m_scroll->IsDragging())
            {
                SetScrollAmount(CalculateScrollAmount(), false);
                ParentClass::OnScrollMove(info);
            }
            else
            {
                InvalidateParent();
            }
        }

        void OnMouseButton(const Point& position, MouseButton button, MouseButtonPressMode state) override
        {
            if (state == MouseButtonPressMode::Pressed)
            {
                m_pressed = true;
                Canvas::SetMouseFocusedWidget(this);
            }
            else
            {
                Point clickPos = WindowPositionToLocal(position);

                if (clickPos.x < m_scroll->X())
                    OnNudgeNegative(EventInfo(this));
                else if (clickPos.x > m_scroll->X() + m_scroll->Width())
                    OnNudgePositive(EventInfo(this));

                m_pressed = false;
                Canvas::SetMouseFocusedWidget(nullptr);
            }
        }

        void Layout(Skin* skin) override
        {
            ParentClass::Layout(skin);

            const Skin::Data& skinData = skin->GetSkinData();
            const PiInt32 scrollHeight = skinData.ScrollContainer.scrollSize.h;

            SetHeight(scrollHeight);

            m_leftArrow->SetSize(scrollHeight / 2, scrollHeight / 2);
            m_leftArrow->SetPosition(Alignment::Center, 0, 0);

            m_rightArrow->SetSize(scrollHeight / 2, scrollHeight / 2);
            m_rightArrow->SetPosition(Alignment::Center, 0, 0);

            m_scrollButtonNegative->SetDock(Alignment::Left);
            m_scrollButtonNegative->SetSize(scrollHeight, scrollHeight);

            m_scroll->SetHeight(scrollHeight);
            m_scroll->SetPadding(Padding(scrollHeight, 0));

            PiReal32 barWidth = (m_viewableContentSize / m_contentSize) * (Width() - scrollHeight);

            m_scroll->SetWidth(barWidth);
            m_scroll->SetHidden(Width() - (scrollHeight * 2) <= barWidth);

            // Based on our last scroll amount, produce a position for the bar
            if (!m_scroll->IsDragging())
                SetScrollAmount(GetScrollAmount(), true);

            m_scrollButtonPositive->SetDock(Alignment::Right);
            m_scrollButtonPositive->SetSize(scrollHeight, scrollHeight);
        }

        virtual void OnNudgeNegative(EventInfo info) override
        {
            if (!m_disabled)
            {
                SetScrollAmount(GetScrollAmount() - GetNudgeAmount(), true);
                ParentClass::OnNudgeNegative(info);
            }
        }

        virtual void OnNudgePositive(EventInfo info) override
        {
            if (!m_disabled)
            {
                SetScrollAmount(GetScrollAmount() + GetNudgeAmount(), true);
                ParentClass::OnNudgePositive(info);
            }
        }

        PiReal32 CalculateScrollAmount() override
        {
            const Skin::Data& skinData = GetSkin()->GetSkinData();
            const PiInt32 scrollHeight = skinData.ScrollContainer.scrollSize.h;

            return static_cast<PiReal32>((m_scroll->X() - scrollHeight)) / (Width() - m_scroll->Width() - scrollHeight * 2);
        }

    private:
        void OnLeftArrowEntered(EventInfo)
        {
            const Skin::Data& skinData = GetSkin()->GetSkinData();
            m_leftArrow->SetBackgroundColor(skinData.ScrollContainer.arrowColorHovered);
        }

        void OnLeftArrowPressed(EventInfo)
        {
            const Skin::Data& skinData = GetSkin()->GetSkinData();
            m_leftArrow->SetBackgroundColor(skinData.ScrollContainer.arrowColorPressed);
        }

        void OnLeftArrowReleased(EventInfo)
        {
            const Skin::Data& skinData = GetSkin()->GetSkinData();
            m_leftArrow->SetBackgroundColor(skinData.ScrollContainer.arrowColorHovered);
        }

        void OnLeftArrowLeaved(EventInfo)
        {
            const Skin::Data& skinData = GetSkin()->GetSkinData();
            m_leftArrow->SetBackgroundColor(skinData.ScrollContainer.arrowColorNormal);
        }

        void OnRightArrowEntered(EventInfo)
        {
            const Skin::Data& skinData = GetSkin()->GetSkinData();
            m_rightArrow->SetBackgroundColor(skinData.ScrollContainer.arrowColorHovered);
        }

        void OnRightArrowPressed(EventInfo)
        {
            const Skin::Data& skinData = GetSkin()->GetSkinData();
            m_rightArrow->SetBackgroundColor(skinData.ScrollContainer.arrowColorPressed);
        }

        void OnRightArrowReleased(EventInfo)
        {
            const Skin::Data& skinData = GetSkin()->GetSkinData();
            m_rightArrow->SetBackgroundColor(skinData.ScrollContainer.arrowColorHovered);
        }

        void OnRightArrowLeaved(EventInfo)
        {
            const Skin::Data& skinData = GetSkin()->GetSkinData();
            m_rightArrow->SetBackgroundColor(skinData.ScrollContainer.arrowColorNormal);
        }

        LeftArrow* m_leftArrow;
        RightArrow* m_rightArrow;
    };

    PI_WIDGET_CONSTRUCTOR(ScrollContainer)
    {
        SetMouseInputEnabled(false);

        m_scrollbarV = new VScrollBar(this);
        m_scrollbarV->SetDock(Alignment::Right);
        m_scrollbarV->On(VScrollBar::ScrollMoveEvent)->Add(this, &ScrollContainer::OnVerticalScrollMove);
        pi_cast<VScrollBar*>(m_scrollbarV)->SetNudgeAmount(30);
        m_scrollV = true;

        m_scrollbarH = new HScrollBar(this);
        m_scrollbarH->SetDock(Alignment::Bottom);
        m_scrollbarH->On(HScrollBar::ScrollMoveEvent)->Add(this, &ScrollContainer::OnHorizontalScrollMove);
        pi_cast<HScrollBar*>(m_scrollbarH)->SetNudgeAmount(30);
        m_scrollH = true;

        m_innerPanel = new Widget(this);
        m_innerPanel->SetPosition(0, 0);
        m_innerPanel->SetMargin(Margin(4, 4, 4, 4));
        m_innerPanel->SendToBack();
        m_innerPanel->SetMouseInputEnabled(true);

        m_autoHideScrollbars = true;
    }

    void ScrollContainer::SetScroll(bool horizontal, bool vertical)
    {
        m_scrollH = horizontal;
        m_scrollV = vertical;

        m_scrollbarV->SetHidden(!vertical);
        m_scrollbarH->SetHidden(!horizontal);
    }

    void ScrollContainer::SetContentSize(PiInt32 width, PiInt32 height)
    {
        m_innerPanel->SetSize(width, height);
    }

    void ScrollContainer::SetContentSize(const Size& size)
    {
        SetContentSize(size.w, size.h);
    }

    void ScrollContainer::AutoHideBar(bool value)
    {
        m_autoHideScrollbars = value;
    }

    bool ScrollContainer::CanScrollX() const
    {
        return m_scrollH;
    }

    bool ScrollContainer::CanScrollY() const
    {
        return m_scrollV;
    }

    void ScrollContainer::ScrollToTop()
    {
        if (CanScrollY())
        {
            UpdateScroll();
            pi_cast<VScrollBar*>(m_scrollbarV)->ScrollToStart();
        }
    }

    void ScrollContainer::ScrollToBottom()
    {
        if (CanScrollY())
        {
            UpdateScroll();
            pi_cast<VScrollBar*>(m_scrollbarV)->ScrollToEnd();
        }
    }

    void ScrollContainer::ScrollToLeft()
    {
        if (CanScrollX())
        {
            UpdateScroll();
            pi_cast<HScrollBar*>(m_scrollbarH)->ScrollToStart();
        }
    }

    void ScrollContainer::ScrollToRight()
    {
        if (CanScrollX())
        {
            UpdateScroll();
            pi_cast<HScrollBar*>(m_scrollbarH)->ScrollToEnd();
        }
    }

    void ScrollContainer::ClearChildren()
    {
        m_innerPanel->ClearChildren();
    }

    void ScrollContainer::OnVerticalScrollMove(EventInfo)
    {
        Invalidate();
    }

    void ScrollContainer::OnHorizontalScrollMove(EventInfo)
    {
        Invalidate();
    }

    bool ScrollContainer::OnMouseWheel(const Point& delta)
    {
        auto* sV = pi_cast<VScrollBar*>(m_scrollbarV);
        auto* sH = pi_cast<HScrollBar*>(m_scrollbarH);

        bool handled = false;

        if (m_scrollH && sH->IsVisible())
        {
            if (sH->SetScrollAmount(sH->GetScrollAmount() - sH->GetNudgeAmount() * delta.x, true))
                handled = true;
        }

        if (m_scrollV && sV->IsVisible())
        {
            if (sV->SetScrollAmount(sV->GetScrollAmount() - sV->GetNudgeAmount() * delta.y, true))
                handled = true;
        }

        return handled;
    }

    void ScrollContainer::NotifyBoundsChanged(const Rect& old, Widget* child)
    {
        UpdateScroll();
        Invalidate();
    }

    void ScrollContainer::Layout(Skin* skin)
    {
        UpdateScroll();
        ParentClass::Layout(skin);
    }

    void ScrollContainer::Render(Skin* skin)
    {
        BaseRenderer* renderer = skin->GetRenderer();
        const Rect& r = RenderBounds();

        renderer->SetDrawColor(skin->GetSkinData().ScrollContainer.backgroundColor);
        renderer->DrawFilledRect(r, Size(0, 0));
    }

    void ScrollContainer::UpdateScroll()
    {
        if (m_innerPanel == nullptr)
            return;

        // Not sure what this is here to fix/change. Adding it breaks auto-scrollbars for
        // menus, controls, etc. -- BQ
        //
        //    if (ContentsAreDocked())
        //    {
        //        sV->SetHidden(false);
        //        sH->SetHidden(false);
        //        m_innerPanel->SetSize(GetSize());
        //        m_innerPanel->SetPos(0, 0);
        //        return;
        //    }

        int childrenWidth = 0;
        int childrenHeight = 0;

        auto* sV = pi_cast<VScrollBar*>(m_scrollbarV);
        auto* sH = pi_cast<HScrollBar*>(m_scrollbarH);

        // Get the max size of all our children together
        for (auto&& child : m_innerPanel->GetChildren())
        {
            childrenWidth = (std::max)(childrenWidth, child->Right());
            childrenHeight = (std::max)(childrenHeight, child->Bottom());
        }

        if (m_scrollH)
        {
            m_innerPanel->SetSize((std::max)(Width(), childrenWidth), (std::max)(Height(), childrenHeight));
        }
        else
        {
            m_innerPanel->SetSize(Width() - (sV->IsHidden() ? 0 : sV->Width() - 1), (std::max)(Height(), childrenHeight));
        }

        const float wPercent = static_cast<float>(Width()) / (childrenWidth + (sV->IsHidden() ? 0 : sV->Width()));
        const float hPercent = static_cast<float>(Height()) / (childrenHeight + (sH->IsHidden() ? 0 : sH->Height()));

        if (m_scrollV)
            SetVScrollRequired(hPercent >= 1);
        else
            sV->SetHidden(true);

        if (m_scrollH)
            SetHScrollRequired(wPercent >= 1);
        else
            sH->SetHidden(true);

        sV->SetContentSize(m_innerPanel->Height());
        sV->SetViewableContentSize(Height() - (sH->IsHidden() ? 0 : sH->Height()));
        sH->SetContentSize(m_innerPanel->Width());
        sH->SetViewableContentSize(Width() - (sV->IsHidden() ? 0 : sV->Width()));
        int newInnerPanelPosX = 0;
        int newInnerPanelPosY = 0;

        if (m_scrollV && !sV->IsHidden())
        {
            newInnerPanelPosY = -(m_innerPanel->Height() - Height() + (sH->IsHidden() ? 0 : sH->Height())) * sV->GetScrollAmount();
        }

        if (m_scrollH && !sH->IsHidden())
        {
            newInnerPanelPosX = -(m_innerPanel->Width() - Width() + (sV->IsHidden() ? 0 : sV->Width())) * sH->GetScrollAmount();
        }

        m_innerPanel->SetPosition(newInnerPanelPosX, newInnerPanelPosY);
    }

    void ScrollContainer::SetVScrollRequired(bool req)
    {
        auto* sV = pi_cast<VScrollBar*>(m_scrollbarV);

        if (req)
        {
            sV->SetScrollAmount(0, true);
            sV->SetDisabled(true);

            if (m_autoHideScrollbars)
                sV->SetHidden(true);
        }
        else
        {
            sV->SetHidden(false);
            sV->SetDisabled(false);
        }
    }

    void ScrollContainer::SetHScrollRequired(bool req)
    {
        auto* sH = pi_cast<HScrollBar*>(m_scrollbarH);

        if (req)
        {
            sH->SetScrollAmount(0, true);
            sH->SetDisabled(true);

            if (m_autoHideScrollbars)
                sH->SetHidden(true);
        }
        else
        {
            sH->SetHidden(false);
            sH->SetDisabled(false);
        }
    }
} // namespace SparkyStudios::UI::Pixel
