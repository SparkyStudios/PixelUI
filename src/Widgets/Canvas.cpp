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

#include <SparkyStudios/UI/Pixel/Core/MainWindow.h>
#include <SparkyStudios/UI/Pixel/Core/Platform.h>
#include <SparkyStudios/UI/Pixel/Widgets/Canvas.h>

namespace SparkyStudios::UI::Pixel
{
    struct Action
    {
        PiUInt8 type;
        PiInt32 x, y;
        char chr;
    };

    struct KeyData
    {
        KeyData()
        {
            for (PiInt32 i = 0; i < (PiUInt32)Key::MAX; i++)
            {
                KeyState[i] = false;
                NextRepeat[i] = 0;
            }

            Target = nullptr;
            LeftMouseDown = false;
            RightMouseDown = false;
        }

        bool KeyState[(PiUInt32)Key::MAX]{};
        PiTime NextRepeat[(PiUInt32)Key::MAX]{};
        Widget* Target;
        bool LeftMouseDown;
        bool RightMouseDown;
    };

    static constexpr PiReal32 kDoubleClickSpeed = 0.5f;
    static constexpr PiReal32 kKeyRepeatRate = 0.03f;
    static constexpr PiReal32 kKeyRepeatDelay = 0.3f;
    static constexpr PiUInt32 kMaxMouseButtons = 5;

    static Widget* gTooltipWidget = nullptr;
    static Widget* gKeyboardFocusedWidget = nullptr;
    static Widget* gMouseFocusedWidget = nullptr;
    static Widget* gHoveredWidget = nullptr;

    static KeyData gKeyData;
    static Point gMousePosition;

    static PiTime gLastClickTime[kMaxMouseButtons];
    static Point gPntLastClickPos;

    static bool FindKeyboardFocus(Widget* control)
    {
        if (control == nullptr)
            return false;

        if (control->GetKeyboardInputEnabled())
        {
            // Make sure none of our children have keyboard focus first
            //  todo recursive
            for (auto&& child : control->GetChildren())
            {
                if (child == gKeyboardFocusedWidget)
                    return false;
            }

            control->Focus();
            return true;
        }

        return FindKeyboardFocus(control->GetParent());
    }

    static Rect ClampRectToRect(Rect inside, const Rect& outside, bool clampSize)
    {
        if (inside.x < outside.x)
            inside.x = outside.x;

        if (inside.y < outside.y)
            inside.y = outside.y;

        if (inside.x + inside.w > outside.x + outside.w)
        {
            if (clampSize)
                inside.w = outside.w;
            else
                inside.x = outside.x + outside.w - inside.w;
        }

        if (inside.y + inside.h > outside.y + outside.h)
        {
            if (clampSize)
                inside.h = outside.h;
            else
                inside.y = outside.w + outside.h - inside.h;
        }

        return inside;
    }

    Point Canvas::Input::GetMousePosition()
    {
        return gMousePosition;
    }

    void Canvas::Input::OnCanvasThink(Widget* widget)
    {
        if (gMouseFocusedWidget != nullptr && !gMouseFocusedWidget->IsVisible())
            gMouseFocusedWidget = nullptr;

        if (gKeyboardFocusedWidget != nullptr &&
            (!gKeyboardFocusedWidget->IsVisible() || !gKeyboardFocusedWidget->GetKeyboardInputEnabled()))
            gKeyboardFocusedWidget = nullptr;

        if (gKeyboardFocusedWidget == nullptr)
            return;

        if (gKeyboardFocusedWidget->GetCanvas() != widget)
            return;

        PiTime time = Platform::GetTimeInSeconds();

        // Simulate Key-Repeats
        for (PiUInt32 i = 0; i < (PiUInt32)Key::MAX; i++)
        {
            if (gKeyData.KeyState[i] && gKeyData.Target != gKeyboardFocusedWidget)
            {
                gKeyData.KeyState[i] = false;
                continue;
            }

            if (gKeyData.KeyState[i] && time > gKeyData.NextRepeat[i])
            {
                gKeyData.NextRepeat[i] = Platform::GetTimeInSeconds() + kKeyRepeatRate;

                if (gKeyboardFocusedWidget)
                    gKeyboardFocusedWidget->OnKey((Key)i, KeyPressMode::Pressed);
            }
        }
    }

    bool Canvas::Input::IsKeyDown(Key key)
    {
        return gKeyData.KeyState[(PiUInt32)key];
    }

    bool Canvas::Input::IsLeftMouseDown()
    {
        return gKeyData.LeftMouseDown;
    }

    bool Canvas::Input::IsRightMouseDown()
    {
        return gKeyData.RightMouseDown;
    }

    void Canvas::Input::OnMouseMove(Widget* canvas, PiInt32 x, PiInt32 y, PiInt32 /*deltaX*/ deltaX, PiInt32 /*deltaY*/ deltaY)
    {
        gMousePosition.x = x;
        gMousePosition.y = y;
        UpdateHoveredControl(canvas);
    }

    bool Canvas::Input::OnMouseButton(Widget* canvas, MouseButton mouseButton, MouseButtonPressMode mode)
    {
        // If we click on a control that isn't a menu we want to close
        // all the open menus. Menus are children of the canvas.
        // TODO
        //        if (down && (!gHoveredWidget || !gHoveredWidget->IsMenuComponent()))
        //            canvas->CloseMenus();

        if (!gHoveredWidget)
            return false;

        if (gHoveredWidget->GetCanvas() != canvas)
            return false;

        if (!gHoveredWidget->IsVisible())
            return false;

        if (gHoveredWidget == canvas)
            return false;

        if (static_cast<PiUInt32>(mouseButton) > kMaxMouseButtons)
            return false;

        bool down = mode == MouseButtonPressMode::Pressed;

        if (static_cast<PiUInt32>(mouseButton) == 0)
            gKeyData.LeftMouseDown = down;
        else if (static_cast<PiUInt32>(mouseButton) == 1)
            gKeyData.RightMouseDown = down;

        // Double click.
        // TODO - Shouldn't double click if mouse has moved significantly
        bool isDoubleClick = false;

        if (down && gPntLastClickPos.x == gMousePosition.x && gPntLastClickPos.y == gMousePosition.y &&
            (Platform::GetTimeInSeconds() - gLastClickTime[static_cast<PiUInt32>(mouseButton)]) < kDoubleClickSpeed)
        {
            isDoubleClick = true;
        }

        if (down && !isDoubleClick)
        {
            gLastClickTime[static_cast<PiUInt32>(mouseButton)] = Platform::GetTimeInSeconds();
            gPntLastClickPos = gMousePosition;
        }

        if (down)
        {
            if (!FindKeyboardFocus(gHoveredWidget))
            {
                if (gKeyboardFocusedWidget)
                    gKeyboardFocusedWidget->UnFocus();
            }
        }

        // TODO
        // gHoveredWidget->UpdateCursor();

        // This tells the child it has been touched, which
        // in turn tells its parents, who tell their parents.
        // This is basically so that Windows can pop themselves
        // to the top when one of their children has been clicked.
        if (down)
            gHoveredWidget->Touch();

#if GWK_HOOKSYSTEM

        if (bDown)
        {
            if (Hook::CallHook(&Hook::BaseHook::OnControlClicked, gHoveredWidget, g_mousePosition.x, g_mousePosition.y))
                return true;
        }

#endif

        if (mouseButton == MouseButton::Left)
        {
            // if (DragAndDrop::OnMouseButton(gHoveredWidget, g_mousePosition.x, g_mousePosition.y, down))
            // {
            //     return true;
            // }
        }

        if (isDoubleClick)
            gHoveredWidget->OnMouseDoubleClick(gMousePosition, mouseButton);
        else
            gHoveredWidget->OnMouseButton(gMousePosition, mouseButton, mode);

        return true;
    }

    bool Canvas::Input::HandleAccelerator(Widget* canvas, char chr)
    {
        // Build the accelerator search string
        PiString accelString;

        if (IsControlDown())
            accelString += "CTRL+";

        if (IsShiftDown())
            accelString += "SHIFT+";

        chr = towupper(chr);
        accelString += chr;

        if (gKeyboardFocusedWidget != nullptr && gKeyboardFocusedWidget->RunAccelerator(accelString))
            return true;

        if (gMouseFocusedWidget != nullptr && gMouseFocusedWidget->RunAccelerator(accelString))
            return true;

        if (canvas->RunAccelerator(accelString))
            return true;

        return false;
    }

    bool Canvas::Input::OnKey(Widget* canvas, Key key, KeyPressMode mode)
    {
        Widget* target = gKeyboardFocusedWidget;

        if (target && target->GetCanvas() != canvas)
            target = nullptr;

        if (target && !target->IsVisible())
            target = nullptr;

        bool down = mode == KeyPressMode::Pressed;

        if (down)
        {
            if (!gKeyData.KeyState[static_cast<PiUInt32>(key)])
            {
                gKeyData.KeyState[static_cast<PiUInt32>(key)] = true;
                gKeyData.NextRepeat[static_cast<PiUInt32>(key)] = Platform::GetTimeInSeconds() + kKeyRepeatDelay;
                gKeyData.Target = target;
            }
        }
        else
        {
            if (gKeyData.KeyState[static_cast<PiUInt32>(key)])
            {
                gKeyData.KeyState[static_cast<PiUInt32>(key)] = false;

                //! @bug This causes shift left arrow in textboxes
                //! to not work. What is disabling it here breaking?
                //! `gKeyData.Target = nullptr;`
            }
        }

        if (target)
            return target->OnKey(key, mode);

        return false;
    }

    void Canvas::UpdateHoveredControl(Widget* widget)
    {
        Widget* hovered = widget->GetWidgetAt(gMousePosition.x, gMousePosition.y, false);

        if (hovered != gHoveredWidget)
        {
            if (gHoveredWidget != nullptr)
            {
                Widget* oldHover = gHoveredWidget;
                gHoveredWidget = nullptr;
                oldHover->OnMouseLeave();
            }

            gHoveredWidget = hovered;

            if (gHoveredWidget != nullptr)
                gHoveredWidget->OnMouseEnter();
        }

        if (gMouseFocusedWidget != nullptr && gMouseFocusedWidget->GetCanvas() == widget)
        {
            if (gHoveredWidget != nullptr)
            {
                Widget* oldHover = gHoveredWidget;
                gHoveredWidget = nullptr;
                oldHover->Redraw();
            }

            gHoveredWidget = gMouseFocusedWidget;
        }
    }

    bool Canvas::IsTooltipActive()
    {
        return gTooltipWidget != nullptr;
    }

    void Canvas::SetTooltipWidget(Widget* widget)
    {
        if (widget->m_tooltip == nullptr)
            return;

        gTooltipWidget = widget;
    }

    void Canvas::UnsetTooltipWidget(Widget* widget)
    {
        if (gTooltipWidget == widget)
            gTooltipWidget = nullptr;
    }

    void Canvas::SetKeyboardFocusedWidget(Widget* widget)
    {
        gKeyboardFocusedWidget = widget;
    }

    Widget* Canvas::GetKeyboardFocusedWidget()
    {
        return gKeyboardFocusedWidget;
    }

    void Canvas::SetMouseFocusedWidget(Widget* widget)
    {
        gMouseFocusedWidget = widget;
    }

    Widget* Canvas::GetMouseFocusedWidget()
    {
        return gMouseFocusedWidget;
    }

    void Canvas::SetHoveredWidget(Widget* widget)
    {
        gHoveredWidget = widget;
    }

    Widget* Canvas::GetHoveredWidget()
    {
        return gHoveredWidget;
    }

    void Canvas::RenderDragAndDropOverlay(Widget* widget, Skin* skin)
    {}

    void Canvas::RenderTooltip(Skin* skin)
    {
        if (gTooltipWidget == nullptr)
            return;

        const Skin::Data& skinData = skin->GetSkinData();
        BaseRenderer* renderer = skin->GetRenderer();
        Point oldRenderOffset = renderer->GetRenderOffset();
        Point mousePos = Input::GetMousePosition();
        Rect bounds = gTooltipWidget->m_tooltip->GetBounds();
        Rect rOffset = Rect(mousePos.x - bounds.w * 0.5f, mousePos.y - bounds.h - 4, bounds.w, bounds.h);
        rOffset = ClampRectToRect(rOffset, gTooltipWidget->GetCanvas()->GetBounds(), false);

        // Calculate offset on screen bounds
        renderer->AddRenderOffset(rOffset);
        renderer->EndClip();

        Rect rect = gTooltipWidget->m_tooltip->RenderBounds();

        // Background
        {
            const PiUInt32 offset = skinData.Tooltip.borderWidth;

            renderer->SetDrawColor(skinData.Tooltip.backgroundColor);
            renderer->DrawFilledRect(rect + Rect(offset, offset, -offset * 2, -offset * 2), skinData.Tooltip.borderRadius);
        }

        // Border
        if (skinData.Tooltip.borderWidth > 0)
        {
            renderer->SetDrawColor(skinData.Tooltip.borderColor);
            renderer->DrawLinedRect(rect, skinData.Tooltip.borderWidth, skinData.Tooltip.borderRadius);
        }

        gTooltipWidget->GetTooltip()->DoRender(skin);
        renderer->SetRenderOffset(oldRenderOffset);
    }

    Canvas::Canvas(MainWindow* window, Skin* skin)
        : ParentClass(nullptr)
    {
        SetBounds(Rect(0, 0, window->GetWidth(), window->GetHeight()));
        SetScale(1.0f);
        SetBackgroundColor(Colors::White);
        SetDrawBackground(true);

        if (skin != nullptr)
            SetSkin(skin, false);
    }

    Canvas::~Canvas()
    {
        ReleaseChildren();
    }

    void Canvas::RenderCanvas()
    {
        DoThink();
        BaseRenderer* renderer = m_skin->GetRenderer();

        renderer->Begin();
        {
            RecurseLayout(m_skin);
            renderer->SetClipRegion(GetBounds());
            renderer->SetRenderOffset(Point(0, 0));
            renderer->SetScale(GetScale());

            if (m_drawBackground)
            {
                renderer->SetDrawColor(m_backgroundColor);
                renderer->DrawFilledRect(RenderBounds(), Size(0, 0));
            }

            DoRender(m_skin);
            RenderDragAndDropOverlay(this, m_skin);
            RenderTooltip(m_skin);
        }
        renderer->End();
    }

    void Canvas::Render(Skin* render)
    {
        m_needsRedraw = false;
    }

    void Canvas::OnBoundsChanged(const Rect& old)
    {
        ParentClass::OnBoundsChanged(old);
        InvalidateChildren(true);
    }

    void Canvas::DoThink()
    {
        ProcessDelayedDeletes();

        if (m_hidden)
            return;

        // Reset tabbing
        {
            NextTab = nullptr;
            FirstTab = nullptr;
        }

        // Check has focus etc...
        RecurseLayout(m_skin);

        // If we didn't have a next tab, cycle to the start.
        if (NextTab == nullptr)
            NextTab = FirstTab;

        Input::OnCanvasThink(this);
    }

    void Canvas::SetScale(float f)
    {
        if (m_scale == f)
            return;

        m_scale = f;

        if (m_skin && m_skin->GetRenderer())
            m_skin->GetRenderer()->SetScale(m_scale);

        OnScaleChanged();
        Redraw();
    }

    void Canvas::AddDelayedDelete(Widget* control)
    {
        if (!m_anyDelete || m_deleteSet.find(control) == m_deleteSet.end())
        {
            m_anyDelete = true;
            m_deleteSet.insert(control);
            m_deleteList.push_back(control);
        }
    }

    void Canvas::PreDeleteCanvas(Widget* control)
    {
        if (m_anyDelete)
        {
            if (m_deleteSet.find(control) != m_deleteSet.end())
            {
                m_deleteList.remove(control);
                m_deleteSet.erase(control);
                m_anyDelete = !m_deleteSet.empty();
            }
        }
    }

    void Canvas::ProcessDelayedDeletes()
    {
        while (m_anyDelete)
        {
            m_anyDelete = false;
            List deleteList = m_deleteList;
            m_deleteList.clear();
            m_deleteSet.clear();

            for (auto&& control : deleteList)
            {
                control->PreDelete(GetSkin());
                delete control;
                Redraw();
            }
        }
    }

    void Canvas::ReleaseChildren()
    {
        auto it = m_children.begin();

        while (it != m_children.end())
        {
            Widget* child = *it;
            it = m_children.erase(it);
            delete child;
        }
    }

    bool Canvas::OnMouseMove(int x, int y, int deltaX, int deltaY)
    {
        if (m_hidden)
            return false;

        // TODO
        if (IsTooltipActive())
            Redraw();

        // Todo: Handle scaling here..
        // float fScale = 1.0f / Scale();
        Input::OnMouseMove(this, x, y, deltaX, deltaY);

        if (!gHoveredWidget)
            return false;

        if (gHoveredWidget == this)
            return false;

        if (gHoveredWidget->GetCanvas() != this)
            return false;

        gHoveredWidget->OnMouseMove(Point(x, y), deltaX, deltaY);
        // TODO
        // gHoveredWidget->UpdateCursor();
        // DragAndDrop::OnMouseMoved(gHoveredWidget, x, y);
        return true;
    }

    bool Canvas::OnMouseButton(MouseButton button, MouseButtonPressMode mode)
    {
        if (IsHidden())
            return false;

        return Input::OnMouseButton(this, button, mode);
    }

    bool Canvas::OnSpecialKey(Key key, KeyPressMode mode)
    {
        if (IsHidden())
            return false;

        if (key <= Key::Invalid)
            return false;

        if (key >= Key::MAX)
            return false;

        return Input::OnKey(this, key, mode);
    }

    //    bool Canvas::InputCharacter(char chr)
    //    {
    //        if (IsHidden())
    //            return false;
    //
    //        // Check if character is printable, i.e. don't want hidden codes, like backspace.
    //        if (!std::isprint(chr))
    //            return false;
    //
    //        // Handle Accelerators
    //        if (Input::HandleAccelerator(this, chr))
    //            return true;
    //
    //        // Handle characters
    //        if (!gKeyboardFocusedWidget)
    //            return false;
    //
    //        if (gKeyboardFocusedWidget->GetCanvas() != this)
    //            return false;
    //
    //        if (!gKeyboardFocusedWidget->IsVisible())
    //            return false;
    //
    //        if (Input::IsControlDown())
    //            return false;
    //
    //        return gKeyboardFocusedWidget->OnChar(chr);
    //    }

    bool Canvas::OnMouseWheel(PiInt32 val)
    {
        if (IsHidden())
            return false;

        if (!gHoveredWidget)
            return false;

        if (gHoveredWidget == this)
            return false;

        if (gHoveredWidget->GetCanvas() != this)
            return false;

        // TODO: Scroll over horizontal axis
        return gHoveredWidget->OnMouseWheel(Point(val, 0));
    }

    bool Canvas::OnCharacter(char chr)
    {
        if (IsHidden())
            return false;

        // Check if character is printable, i.e. don't want hidden codes, like backspace.
        if (!std::isprint(chr))
            return false;

        // Handle Accelerators
        if (Input::HandleAccelerator(this, chr))
            return true;

        // Handle characters
        if (gKeyboardFocusedWidget == nullptr)
            return false;

        if (gKeyboardFocusedWidget->GetCanvas() != this)
            return false;

        if (!gKeyboardFocusedWidget->IsVisible())
            return false;

        if (Input::IsControlDown())
            return false;

        return gKeyboardFocusedWidget->OnCharacter(chr);
    }

    void Canvas::SetSkin(Skin* skin, bool affectChildren)
    {
        ParentClass::SetSkin(skin, affectChildren);

        Skin::Data data = skin->GetSkinData();
        skin->SetDefaultFont(data.Canvas.defaultFont);

        SetBackgroundColor(data.Canvas.backgroundColor);
        SetPadding(data.Canvas.padding);
    }
} // namespace SparkyStudios::UI::Pixel
