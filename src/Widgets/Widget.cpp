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

#include <algorithm>
#include <cmath>
#include <utility>

#include <SparkyStudios/UI/Pixel/Widgets/Canvas.h>
#include <SparkyStudios/UI/Pixel/Widgets/Label.h>

#if PI_ENABLE_ANIMATION
#include <SparkyStudios/UI/Pixel/Core/Animation/Animation.h>
#endif // PI_ENABLE_ANIMATION

namespace SparkyStudios::UI::Pixel
{
    const char* const Widget::MouseEnterEvent = "Widget::Events::MouseEnter";
    const char* const Widget::MouseLeaveEvent = "Widget::Events::MouseLeave";
    const char* const Widget::MouseButtonDownEvent = "Widget::Events::MouseButtonDown";
    const char* const Widget::MouseButtonUpEvent = "Widget::Events::MouseButtonUp";
    const char* const Widget::MouseDoubleClickEvent = "Widget::Events::MouseDoubleClick";
    const char* const Widget::KeyDownEvent = "Widget::Events::KeyDown";
    const char* const Widget::KeyUpEvent = "Widget::Events::KeyUp";

    Widget::Widget(Widget* parent, PiString name)
        : m_parent(nullptr)
        , m_actualParent(nullptr)
        , m_innerPanel(nullptr)
        , m_skin(nullptr)
        , m_hidden(false)
        , m_bounds(Rect(0, 0, 10, 10))
        , m_padding(Padding(0, 0, 0, 0))
        , m_margin(Margin(0, 0, 0, 0))
        , m_dock(Alignment::None)
        , m_disabled(false)
        , m_cacheTextureDirty(true)
        , m_cacheToTexture(false)
        , m_includeInSize(true)
        , m_tooltip(nullptr)
        , m_name(std::move(name))
        , m_restrictToParent(false)
        , m_mouseInputEnabled(true)
        , m_keyboardInputEnabled(false)
        , m_drawBackground(true)
        , m_needsLayout(true)
    {
        SetParent(parent);
        // m_dragAndDrop_Package = nullptr;
        Invalidate();
        // SetCursor(Gwk::CursorType::Normal);
        // SetTabable(false);
    }

    Widget::~Widget()
    {
        {
            Canvas* canvas = GetCanvas();

            if (canvas)
                canvas->PreDeleteCanvas(this);
        }

        auto iter = m_children.begin();

        while (iter != m_children.end())
        {
            Widget* child = *iter;
            iter = m_children.erase(iter);
            delete child;
        }

        for (auto&& m_accelerator : m_accelerators)
        {
            delete m_accelerator.second;
        }

        m_accelerators.clear();
        SetParent(nullptr);

        if (Canvas::GetHoveredWidget() == this)
            Canvas::SetHoveredWidget(nullptr);

        if (Canvas::GetKeyboardFocusedWidget() == this)
            Canvas::SetKeyboardFocusedWidget(nullptr);

        if (Canvas::GetMouseFocusedWidget() == this)
            Canvas::SetMouseFocusedWidget(nullptr);

            // DragAndDrop::ControlDeleted(this);
            // Tooltip::ControlDeleted(this);

#if PI_ENABLE_ANIMATION
        Animation::Cancel(this);
#endif // PI_ENABLE_ANIMATION

        // if (m_dragAndDrop_Package)
        // {
        //     delete m_dragAndDrop_Package;
        //     m_dragAndDrop_Package = nullptr;
        // }
    }

    const char* Widget::GetTypeNameStatic()
    {
        return "Widget";
    }

    const char* Widget::GetTypeName() const
    {
        return Widget::GetTypeNameStatic();
    }

    const char* Widget::GetParentTypeName() const
    {
        return nullptr;
    }

    void Widget::DelayedDelete()
    {
        Canvas* canvas = GetCanvas();
        canvas->AddDelayedDelete(this);
    }

    void Widget::PreDelete(Skin* skin)
    {}

    EventListener* Widget::On(const PiString& event)
    {
        if (m_eventsMap[event] == nullptr)
            m_eventsMap[event] = new EventListener();

        return m_eventsMap[event];
    }

    void Widget::SetParent(Widget* parent)
    {
        if (m_parent == parent)
            return;

        if (m_parent)
            m_parent->RemoveChild(this);

        m_parent = parent;
        m_actualParent = nullptr;

        if (m_parent)
            m_parent->AddChild(this);
    }

    Widget* Widget::GetParent() const
    {
        return m_parent;
    }

    void Widget::SetTooltipText(const PiString& text)
    {
        auto* tooltip = new Label(this);
        tooltip->SetText(text);
        tooltip->SetFontSize(GetSkin()->GetSkinData().Tooltip.textSize);
        tooltip->SetTextColorOverride(GetSkin()->GetSkinData().Tooltip.textColor);
        tooltip->SetPadding(GetSkin()->GetSkinData().Tooltip.padding);
        tooltip->SizeToContents();
        SetTooltip(tooltip);
    }

    void Widget::SetTooltip(Widget* widget)
    {
        m_tooltip = widget;

        if (m_tooltip)
        {
            m_tooltip->SetParent(this);
            m_tooltip->SetHidden(true);
        }
    }

    Widget* Widget::GetTooltip()
    {
        return m_tooltip;
    }

    Canvas* Widget::GetCanvas()
    {
        Widget* root = m_parent;

        if (root == nullptr)
            return pi_cast<Canvas*>(this);

        return root->GetCanvas();
    }

    const Widget::List& Widget::GetChildren() const
    {
        if (m_innerPanel)
            return m_innerPanel->GetChildren();

        return m_children;
    }

    bool Widget::IsChild(Widget* child) const
    {
        return std::any_of(
            m_children.begin(), m_children.end(),
            [=](Widget* c) -> bool
            {
                return (c == child);
            });
    }

    PiUInt32 Widget::ChildCount() const
    {
        return static_cast<PiUInt32>(m_children.size());
    }

    Widget* Widget::GetChild(PiUInt32 index) const
    {
        if (index >= ChildCount())
            return nullptr;

        for (auto&& child : m_children)
        {
            if (index == 0)
                return child;

            index--;
        }

        // Should never happen
        PI_ASSERT(false);

        return nullptr;
    }

    [[maybe_unused]] bool Widget::SizeToChildren(bool width, bool height)
    {
        Size size = ChildrenSize();
        return SetSize(width ? size.w + m_padding.right : m_bounds.w, height ? size.h + m_padding.bottom : m_bounds.h);
    }

    Size Widget::ChildrenSize() const
    {
        Size size{};

        for (auto&& child : m_children)
        {
            if (child->m_hidden)
                continue;

            if (!child->ShouldIncludeInSize())
                continue;

            size.w = (std::max)(size.w, child->Right());
            size.h = (std::max)(size.h, child->Bottom());
        }

        return size;
    }

    Widget* Widget::FindChildByName(const PiString& name, bool recursive) const
    {
        for (auto&& child : m_children)
        {
            if (!child->GetName().empty() && child->GetName() == name)
                return child;

            if (recursive)
            {
                Widget* subChild = child->FindChildByName(name, recursive);

                if (subChild != nullptr)
                    return subChild;
            }
        }

        return nullptr;
    }

    Widget* Widget::GetWidgetAt(PiInt32 x, PiInt32 y, bool onlyIfMouseEnabled)
    {
        if (m_hidden)
            return nullptr;

        if (x < 0 || y < 0 || x >= m_bounds.w || y >= m_bounds.h)
            return nullptr;

        if (onlyIfMouseEnabled && !m_mouseInputEnabled)
            return nullptr;

        for (auto it = m_children.rbegin(); it != m_children.rend(); ++it)
        {
            Widget* child = *it;
            Widget* found = child->GetWidgetAt(x - child->m_bounds.x, y - child->m_bounds.y, onlyIfMouseEnabled);

            if (found != nullptr)
                return found;
        }

        return this;
    }

    void Widget::ClearChildren()
    {
        while (!m_children.empty())
        {
            RemoveChild(*m_children.begin());
        }
    }

    void Widget::SetName(const PiString& name)
    {
        m_name = name;
    }

    const PiString& Widget::GetName() const
    {
        return m_name;
    }

    void Widget::Think()
    {}

    void Widget::SendToBack()
    {
        if (m_actualParent == nullptr)
            return;

        if (m_actualParent->m_children.front() == this)
            return;

        m_actualParent->m_children.remove(this);
        m_actualParent->m_children.push_front(this);

        InvalidateParent();
    }

    void Widget::BringToFront()
    {
        if (m_actualParent == nullptr)
            return;

        if (m_actualParent->m_children.front() == this)
            return;

        m_actualParent->m_children.remove(this);
        m_actualParent->m_children.push_back(this);

        InvalidateParent();
        Redraw();
    }

    void Widget::MoveNextToWidget(Widget* child, bool behind)
    {
        if (m_actualParent == nullptr)
            return;

        m_actualParent->m_children.remove(this);
        auto it = std::find(m_actualParent->m_children.begin(), m_actualParent->m_children.end(), child);

        if (it == m_actualParent->m_children.end())
            return BringToFront();

        if (behind)
        {
            ++it;

            if (it == m_actualParent->m_children.end())
                return BringToFront();
        }

        m_actualParent->m_children.insert(it, this);
        InvalidateParent();
    }

    Point Widget::LocalPositionToWindow(const Point& position)
    {
        if (m_parent)
        {
            int x = position.x + X();
            int y = position.y + Y();

            // If our parent has an inner panel, and we're a child of it
            // add its offset onto us.
            if (m_parent->m_innerPanel && m_parent->m_innerPanel->IsChild(this))
            {
                x += m_parent->m_innerPanel->X();
                y += m_parent->m_innerPanel->Y();
            }

            return m_parent->LocalPositionToWindow(Point(x, y));
        }

        return position;
    }

    Point Widget::WindowPositionToLocal(const Point& position)
    {
        if (m_parent)
        {
            int x = position.x - X();
            int y = position.y - Y();

            // If our parent has an inner panel, and we're a child of it
            // add its offset onto us.
            if (m_parent->m_innerPanel && m_parent->m_innerPanel->IsChild(this))
            {
                x -= m_parent->m_innerPanel->X();
                y -= m_parent->m_innerPanel->Y();
            }

            return m_parent->WindowPositionToLocal(Point(x, y));
        }

        return position;
    }

    void Widget::SetDock(Alignment dock)
    {
        if (m_dock == dock)
            return;

        m_dock = dock;

        Invalidate();
        InvalidateParent();
    }

    Alignment Widget::GetDock() const
    {
        return m_dock;
    }

    [[maybe_unused]] void Widget::RestrictToParent(bool value)
    {
        m_restrictToParent = value;
    }

    bool Widget::IsRestrictedToParent() const
    {
        return m_restrictToParent;
    }

    bool Widget::SetBounds(PiInt32 x, PiInt32 y, PiInt32 width, PiInt32 height)
    {
        return SetBounds(Rect(x, y, width, height));
    }

    bool Widget::SetBounds(const Rect& bounds)
    {
        if (m_bounds == bounds)
            return false;

        const auto old = GetBounds();
        m_bounds = bounds;

        OnBoundsChanged(old);

        return true;
    }

    const Rect& Widget::GetBounds() const
    {
        return m_bounds;
    }

    PiInt32 Widget::X() const
    {
        return m_bounds.x;
    }

    PiInt32 Widget::Y() const
    {
        return m_bounds.y;
    }

    PiInt32 Widget::Width() const
    {
        return m_bounds.w;
    }

    PiInt32 Widget::Height() const
    {
        return m_bounds.h;
    }

    PiInt32 Widget::Left() const
    {
        return m_bounds.x - m_margin.left;
    }

    PiInt32 Widget::Right() const
    {
        return m_bounds.x + m_bounds.w + m_margin.right;
    }

    PiInt32 Widget::Bottom() const
    {
        return m_bounds.y + m_bounds.h + m_margin.bottom;
    }

    PiInt32 Widget::Top() const
    {
        return m_bounds.y - m_margin.top;
    }

    const Rect& Widget::InnerBounds() const
    {
        return m_innerBounds;
    }

    const Rect& Widget::RenderBounds() const
    {
        return m_renderBounds;
    }

    void Widget::SetPosition(Alignment pos, int xOffset, int yOffset)
    {
        const Rect& bounds = m_parent->InnerBounds();
        const Margin& margin = GetMargin();

        int x = m_bounds.x;
        int y = m_bounds.y;

        if (pos & Alignment::Left)
            x = bounds.x + xOffset + margin.left;

        if (pos & Alignment::Right)
            x = bounds.x + (bounds.w - m_bounds.w - xOffset - margin.right);

        if (pos & Alignment::CenterH)
            x = bounds.x + (bounds.w - m_bounds.w - margin.right + margin.left) / 2 + xOffset;

        if (pos & Alignment::Top)
            y = bounds.y + yOffset + margin.top;

        if (pos & Alignment::Bottom)
            y = bounds.y + (bounds.h - m_bounds.h - yOffset - margin.bottom);

        if (pos & Alignment::CenterV)
            y = bounds.y + (bounds.h - m_bounds.h - margin.bottom + margin.top) / 2 + yOffset;

        SetPosition(x, y);
    }

    void Widget::SetPosition(PiInt32 x, PiInt32 y)
    {
        SetBounds(x, y, m_bounds.w, m_bounds.h);
    }

    void Widget::SetPosition(const Point& position)
    {
        SetPosition(position.x, position.y);
    }

    Point Widget::GetPosition() const
    {
        return Point(m_bounds.x, m_bounds.y);
    }

    void Widget::SetWidth(PiInt32 width)
    {
        SetSize(width, m_bounds.h);
    }

    void Widget::SetHeight(PiInt32 height)
    {
        SetSize(m_bounds.w, height);
    }

    bool Widget::SetSize(PiInt32 width, PiInt32 height)
    {
        return SetBounds(m_bounds.x, m_bounds.y, width, height);
    }

    bool Widget::SetSize(const Size& size)
    {
        return SetSize(size.w, size.h);
    }

    Size Widget::GetSize() const
    {
        return Size(m_bounds.w, m_bounds.h);
    }

    void Widget::SetPadding(const Padding& padding)
    {
        if (m_padding.left == padding.left && m_padding.top == padding.top && m_padding.right == padding.right &&
            m_padding.bottom == padding.bottom)
            return;

        m_padding = padding;
        Invalidate();
        InvalidateParent();
    }

    const Padding& Widget::GetPadding() const
    {
        return m_padding;
    }

    void Widget::SetMargin(const Margin& margin)
    {
        if (m_margin.top == margin.top && m_margin.left == margin.left && m_margin.bottom == margin.bottom &&
            m_margin.right == margin.right)
            return;

        m_margin = margin;
        Invalidate();
        InvalidateParent();
    }

    const Margin& Widget::GetMargin() const
    {
        return m_margin;
    }

    void Widget::MoveTo(PiInt32 x, PiInt32 y)
    {
        if (m_restrictToParent && m_parent != nullptr)
        {
            Widget* parent = m_parent;

            if (x - m_padding.left < parent->m_margin.left)
                x = parent->m_margin.left + m_padding.left;

            if (y - m_padding.top < parent->m_margin.top)
                y = parent->m_margin.top + m_padding.top;

            if (x + m_bounds.w + m_padding.right > parent->m_bounds.w - parent->m_margin.right)
                x = parent->m_bounds.w - parent->m_margin.right - m_bounds.w - m_padding.right;

            if (y + m_bounds.h + m_padding.bottom > parent->m_bounds.h - parent->m_margin.bottom)
                y = parent->m_bounds.h - parent->m_margin.bottom - m_bounds.h - m_padding.bottom;
        }

        SetPosition(x, y);
    }

    void Widget::MoveTo(const Point& position)
    {
        MoveTo(position.x, position.y);
    }

    void Widget::MoveBy(PiInt32 x, PiInt32 y)
    {
        MoveTo(m_bounds.x + x, m_bounds.y + y);
    }

    void Widget::MoveBy(const Point& offset)
    {
        MoveBy(offset.x, offset.y);
    }

    void Widget::SetHidden(bool value)
    {
        if (m_hidden == value)
            return;

        m_hidden = value;
        Invalidate();
        Redraw();
    }

    void Widget::Show()
    {
        SetHidden(false);
    }

    void Widget::Hide()
    {
        SetHidden(true);
    }

    bool Widget::IsHidden() const
    {
        return m_hidden;
    }

    bool Widget::IsVisible() const
    {
        if (m_hidden)
            return false;

        if (m_parent)
            return m_parent->IsVisible();

        return true;
    }

    bool Widget::IsDisabled() const
    {
        return m_disabled;
    }

    void Widget::SetDisabled(bool value)
    {
        if (m_disabled == value)
            return;

        m_disabled = value;
        Redraw();
    }

    void Widget::SetSkin(Skin* skin, bool affectChildren)
    {
        if (m_skin == skin)
            return;

        m_skin = skin;

        Invalidate();
        Redraw();

        OnSkinChanged(skin);

        if (affectChildren)
        {
            for (auto&& child : m_children)
            {
                child->SetSkin(skin, true);
            }
        }
    }

    Skin* Widget::GetSkin() const
    {
        if (m_skin != nullptr)
            return m_skin;

        if (m_parent != nullptr)
            return m_parent->GetSkin();

        return nullptr;
    }

    void Widget::SetMouseInputEnabled(bool value)
    {
        m_mouseInputEnabled = value;
    }

    bool Widget::GetMouseInputEnabled() const
    {
        return m_mouseInputEnabled;
    }

    void Widget::SetKeyboardInputEnabled(bool value)
    {
        m_keyboardInputEnabled = value;
    }

    bool Widget::GetKeyboardInputEnabled() const
    {
        return m_keyboardInputEnabled;
    }

    void Widget::Touch()
    {
        if (m_parent != nullptr)
            m_parent->OnChildTouched(this);
    }

    void Widget::Focus()
    {
        if (IsFocused())
            return;

        if (Canvas::GetKeyboardFocusedWidget() != nullptr)
            Canvas::GetKeyboardFocusedWidget()->OnLostKeyboardFocus();

        Canvas::SetKeyboardFocusedWidget(this);
        OnGetKeyboardFocus();

        Redraw();
    }

    void Widget::UnFocus()
    {
        if (!IsFocused())
            return;

        Canvas::SetKeyboardFocusedWidget(nullptr);
        OnLostKeyboardFocus();

        Redraw();
    }

    bool Widget::IsHovered() const
    {
        return Canvas::GetHoveredWidget() == this;
    }

    bool Widget::IsFocused() const
    {
        return Canvas::GetKeyboardFocusedWidget() == this;
    }

    bool Widget::IsOnTop() const
    {
        if (m_parent == nullptr)
            return false;

        auto it = m_parent->m_children.begin();
        Widget* child = *it;

        return (child == this);
    }

    void Widget::EnableCacheToTexture()
    {
        m_cacheToTexture = true;
    }

    void Widget::DisableCacheToTexture()
    {
        m_cacheToTexture = false;
    }

    bool Widget::IsCachedToTextureEnabled() const
    {
        return m_cacheToTexture;
    }

    bool Widget::NeedsLayout() const
    {
        return m_needsLayout;
    }

    void Widget::Invalidate()
    {
        m_needsLayout = true;
        m_cacheTextureDirty = true;
    }

    void Widget::InvalidateParent()
    {
        if (m_parent)
            m_parent->Invalidate();
    }

    void Widget::InvalidateChildren(bool recursive)
    {
        for (auto&& child : m_children)
        {
            child->Invalidate();

            if (recursive)
                child->InvalidateChildren(recursive);
        }

        if (m_innerPanel)
        {
            for (auto&& child : m_innerPanel->m_children)
            {
                child->Invalidate();

                if (recursive)
                    child->InvalidateChildren(recursive);
            }
        }
    }

    void Widget::SetShouldIncludeInSize(bool value)
    {
        m_includeInSize = value;
    }

    bool Widget::ShouldIncludeInSize() const
    {
        return m_includeInSize;
    }

    bool Widget::RunAccelerator(const PiString& accelerator)
    {
        if (Canvas::GetKeyboardFocusedWidget() == this || !AccelOnlyOnFocus())
        {
            if (auto it = m_accelerators.find(accelerator); it != m_accelerators.end())
            {
                it->second->Call(this);
                return true;
            }
        }

        for (auto&& child : m_children)
        {
            if (child->RunAccelerator(accelerator))
                return true;
        }

        return false;
    }

    void Widget::SetValue(const PiString& value)
    {}

    PiString Widget::GetValue()
    {
        return "";
    }

#if PI_ENABLE_ANIMATION

    void Widget::Animate(Animation* animation)
    {
        Animation::Add(this, animation);
    }

#endif // PI_ENABLE_ANIMATION

    void Widget::AddChild(Widget* child)
    {
        if (m_innerPanel)
        {
            m_innerPanel->AddChild(child);
            return;
        }

        m_children.push_back(child);
        OnChildAdded(child);
        child->m_actualParent = this;
    }

    void Widget::RemoveChild(Widget* child)
    {
        if (m_innerPanel == child)
            m_innerPanel = nullptr;

        if (m_innerPanel)
            m_innerPanel->RemoveChild(child);

        m_children.remove(child);
        OnChildRemoved(child);
    }

    void Widget::NotifyBoundsChanged(const Rect& old, Widget* child)
    {}

    void Widget::UpdateRenderBounds()
    {
        m_renderBounds.x = 0;
        m_renderBounds.y = 0;
        m_renderBounds.w = m_bounds.w;
        m_renderBounds.h = m_bounds.h;

        m_innerBounds.x = m_renderBounds.x + m_padding.left;
        m_innerBounds.w = m_renderBounds.w - (m_padding.left + m_padding.right);
        m_innerBounds.y = m_renderBounds.y + m_padding.top;
        m_innerBounds.h = m_renderBounds.h - (m_padding.top + m_padding.bottom);
    }

    void Widget::OnChildAdded(Widget* child)
    {
        Invalidate();
    }

    void Widget::OnChildRemoved(Widget* child)
    {
        Invalidate();
    }

    void Widget::OnBoundsChanged(const Rect& old)
    {
        if (m_parent != nullptr)
            m_parent->NotifyBoundsChanged(old, this);

        if (m_bounds.w != old.w || m_bounds.h != old.h)
            Invalidate();

        Redraw();
        UpdateRenderBounds();
    }

    void Widget::OnScaleChanged()
    {
        for (auto&& child : m_children)
        {
            child->OnScaleChanged();
        }
    }

    void Widget::OnSkinChanged(Skin* skin)
    {}

    void Widget::OnMouseEnter()
    {
        On(MouseEnterEvent)->Call(this);

        if (m_tooltip != nullptr)
            Canvas::SetTooltipWidget(this);
        else if (m_parent != nullptr && m_parent->m_tooltip != nullptr)
            Canvas::SetTooltipWidget(m_parent);

        Redraw();
    }

    void Widget::OnMouseMove(const Point& position, PiInt32 deltaX, PiInt32 deltaY)
    {}

    void Widget::OnMouseButton(const Point& position, MouseButton button, MouseButtonPressMode mode)
    {
        EventInfo info(this);
        info.data.point = position;
        info.data.mouseButton = button;
        On(mode == MouseButtonPressMode::Pressed ? MouseButtonDownEvent : MouseButtonUpEvent)->Call(this, info);
    }

    void Widget::OnMouseDoubleClick(const Point& position, MouseButton button)
    {
        OnMouseButton(position, button, KeyPressMode::Pressed);

        EventInfo info(this);
        info.data.point = position;
        info.data.mouseButton = button;
        On(MouseDoubleClickEvent)->Call(this, info);
    }

    bool Widget::OnMouseWheel(const Point& delta)
    {
        if (m_actualParent)
            return m_actualParent->OnMouseWheel(delta);

        return false;
    }

    void Widget::OnMouseLeave()
    {
        On(MouseLeaveEvent)->Call(this);

        if (m_tooltip != nullptr)
            Canvas::UnsetTooltipWidget(this);

        Redraw();
    }

    bool Widget::OnKey(Key key, KeyPressMode mode)
    {
        bool handled = false;

        switch (mode)
        {
        case KeyPressMode::Pressed:
            handled = OnKeyDown(key);
            break;
        case KeyPressMode::Released:
            handled = OnKeyUp(key);
            break;
        }

        if (!handled && m_parent != nullptr)
            handled = m_parent->OnKey(key, mode);

        return handled;
    }

    bool Widget::OnKeyDown(Key key)
    {
        EventInfo info(this);
        info.data.key = key;
        On(KeyDownEvent)->Call(this, info);
        return false;
    }

    bool Widget::OnKeyUp(Key key)
    {
        EventInfo info(this);
        info.data.key = key;
        On(KeyUpEvent)->Call(this, info);
        return false;
    }

    bool Widget::OnCharacter(char character)
    {
        return false;
    }

    void Widget::OnChildTouched(Widget* child)
    {
        Touch();
    }

    void Widget::OnGetKeyboardFocus()
    {}

    void Widget::OnLostKeyboardFocus()
    {}

    bool Widget::AccelOnlyOnFocus()
    {
        return false;
    }

    bool Widget::HandleAccelerator(const PiString& accelerator)
    {
        if (Canvas::GetKeyboardFocusedWidget() == this || !AccelOnlyOnFocus())
        {
            AccelMap::iterator iter = m_accelerators.find(accelerator);

            if (iter != m_accelerators.end())
            {
                iter->second->Call(this);
                return true;
            }
        }

        for (auto&& child : m_children)
        {
            if (child->HandleAccelerator(accelerator))
                return true;
        }

        return false;
    }

    bool Widget::ShouldClip()
    {
        return true;
    }

    void Widget::DoRender(Skin* skin)
    {
        // Use the custom widget skin if any
        if (m_skin != nullptr)
            skin = m_skin;

        Think();
        BaseRenderer* renderer = skin->GetRenderer();

        if (renderer->GetCTT() != nullptr && IsCachedToTextureEnabled())
        {
            DoCacheRender(skin, this);
            return;
        }

        RenderRecursive(skin);
    }

    void Widget::DoCacheRender(Skin* skin, Widget* root)
    {
        BaseRenderer* renderer = skin->GetRenderer();
        ICacheToTexture* cache = renderer->GetCTT();

        if (cache == nullptr)
            return;

        Point oldOffset = renderer->GetRenderOffset();
        Rect oldRegion = renderer->ClipRegion();

        if (this != root)
        {
            renderer->AddRenderOffset(m_bounds);
            renderer->AddClipRegion(m_bounds);
        }
        else
        {
            renderer->SetRenderOffset(Point(0, 0));
            renderer->SetClipRegion(m_bounds);
        }

        if (m_cacheTextureDirty && renderer->ClipRegionVisible())
        {
            renderer->StartClip();
            {
                if (IsCachedToTextureEnabled())
                    cache->SetupCacheTexture(this);

                Render(skin);

                if (!m_children.empty())
                {
                    for (auto&& child : m_children)
                    {
                        if (!child->m_hidden)
                        {
                            child->DoRender(skin);
                        }
                    }
                }

                if (IsCachedToTextureEnabled())
                {
                    cache->FinishCacheTexture(this);
                    m_cacheTextureDirty = false;
                }
            }
            renderer->EndClip();
        }

        renderer->SetClipRegion(oldRegion);
        renderer->StartClip();
        {
            renderer->SetRenderOffset(oldOffset);
            cache->DrawCachedWidgetTexture(this);
        }
        renderer->EndClip();
    }

    void Widget::RenderRecursive(Skin* skin)
    {
        BaseRenderer* renderer = skin->GetRenderer();
        Point oldOffset = renderer->GetRenderOffset();

        renderer->AddRenderOffset(m_bounds);
        RenderUnder(skin);

        Rect oldRegion = renderer->ClipRegion();

        // If this control is clipping, change the clip rect to ourselves
        // else clip using our parents clip rect.
        if (ShouldClip())
        {
            renderer->AddClipRegion(RenderBounds());

            if (!renderer->ClipRegionVisible())
            {
                renderer->SetRenderOffset(oldOffset);
                renderer->SetClipRegion(oldRegion);
                return;
            }
        }

        // Render this control and children controls
        renderer->StartClip();
        {
            Render(skin);

            if (!m_children.empty())
            {
                for (auto&& child : m_children)
                {
                    if (child->m_hidden)
                        continue;

                    child->DoRender(skin);
                }
            }
        }
        renderer->EndClip();

        // Render overlay/focus
        renderer->SetClipRegion(oldRegion);
        renderer->StartClip();
        {
            RenderOverlay(skin);
            RenderFocus(skin);
        }
        renderer->EndClip();
        renderer->SetRenderOffset(oldOffset);
    }

    void Widget::Render(Skin* skin)
    {}

    void Widget::RenderOverlay(Skin* skin)
    {}

    void Widget::RenderUnder(Skin* skin)
    {}

    void Widget::RenderFocus(Skin* skin)
    {}

    void Widget::Redraw()
    {
        m_cacheTextureDirty = true;

        if (m_parent != nullptr)
            m_parent->Redraw();
    }

    void Widget::Layout(Skin* skin)
    {
        if (IsCachedToTextureEnabled() && skin->GetRenderer()->GetCTT())
            skin->GetRenderer()->GetCTT()->CreateWidgetCacheTexture(this, m_bounds.GetSize());
    }

    void Widget::RecurseLayout(Skin* skin)
    {
        // Use custom skin if any
        if (m_skin != nullptr)
            skin = m_skin;

        if (m_hidden)
            return;

        if (NeedsLayout())
        {
            m_needsLayout = false;
            Layout(skin);
        }

        Rect rBounds = RenderBounds();

        // Adjust bounds for padding
        rBounds.x += m_padding.left;
        rBounds.w -= m_padding.left + m_padding.right;
        rBounds.y += m_padding.top;
        rBounds.h -= m_padding.top + m_padding.bottom;

        m_innerBounds = rBounds;

        for (auto&& child : m_children)
        {
            if (child->m_hidden)
                continue;

            Alignment dock = child->m_dock;

            if (dock & Alignment::Fill)
                continue;

            if (dock & Alignment::Top)
            {
                const Margin& margin = child->m_margin;
                child->SetBounds(rBounds.x + margin.left, rBounds.y + margin.top, rBounds.w - margin.left - margin.right, child->Height());
                int iHeight = margin.top + margin.bottom + child->Height();
                rBounds.y += iHeight;
                rBounds.h -= iHeight;
            }

            if (dock & Alignment::Left)
            {
                const Margin& margin = child->m_margin;
                child->SetBounds(rBounds.x + margin.left, rBounds.y + margin.top, child->Width(), rBounds.h - margin.top - margin.bottom);
                int iWidth = margin.left + margin.right + child->Width();
                rBounds.x += iWidth;
                rBounds.w -= iWidth;
            }

            if (dock & Alignment::Right)
            {
                // TODO: THIS MARGIN CODE MIGHT NOT BE FULLY FUNCTIONAL
                const Margin& margin = child->m_margin;
                child->SetBounds(
                    (rBounds.x + rBounds.w) - child->Width() - margin.right, rBounds.y + margin.top, child->Width(),
                    rBounds.h - margin.top - margin.bottom);
                int iWidth = margin.left + margin.right + child->Width();
                rBounds.w -= iWidth;
            }

            if (dock & Alignment::Bottom)
            {
                // TODO: THIS MARGIN CODE MIGHT NOT BE FULLY FUNCTIONAL
                const Margin& margin = child->m_margin;
                child->SetBounds(
                    rBounds.x + margin.left, (rBounds.y + rBounds.h) - child->Height() - margin.bottom,
                    rBounds.w - margin.left - margin.right, child->Height());
                rBounds.h -= child->Height() + margin.bottom + margin.top;
            }

            child->RecurseLayout(skin);
        }

        // Fill uses the leftover space, so do that now.
        for (auto&& child : m_children)
        {
            Alignment dock = child->m_dock;

            if (!(dock & Alignment::Fill))
                continue;

            const Margin& margin = child->GetMargin();
            child->SetBounds(
                rBounds.x + margin.left, rBounds.y + margin.top, rBounds.w - margin.left - margin.right,
                rBounds.h - margin.top - margin.bottom);
            child->RecurseLayout(skin);
        }

        PostLayout(skin);

        // TODO
        // if (IsTabable() && !IsDisabled())
        // {
        //     if (!GetCanvas()->FirstTab)
        //         GetCanvas()->FirstTab = this;
        //
        //     if (!GetCanvas()->NextTab)
        //         GetCanvas()->NextTab = this;
        // }

        if (Canvas::GetKeyboardFocusedWidget() == this)
            GetCanvas()->NextTab = nullptr;
    }

    void Widget::PostLayout(Skin* skin)
    {}
} // namespace SparkyStudios::UI::Pixel
