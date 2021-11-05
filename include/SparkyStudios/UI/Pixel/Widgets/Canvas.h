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

#ifndef PIXEL_UI_CANVAS_H
#define PIXEL_UI_CANVAS_H

#include <set>

#include <SparkyStudios/UI/Pixel/Core/Input/IInputEventListener.h>
#include <SparkyStudios/UI/Pixel/Widgets/Containers/Menu.h>
#include <SparkyStudios/UI/Pixel/Widgets/Widget.h>

namespace SparkyStudios::UI::Pixel
{
    class MainWindow;

    class PI_EXPORT Canvas
        : public Widget
        , public IInputEventListener
    {
        friend class Widget;

    public:
        PI_WIDGET(Canvas, Widget);

        class PI_EXPORT Input
        {
        public:
            enum class Message
            {
                Copy,
                Paste,
                Cut,
                Undo,
                Redo,
                SelectAll
            };

            // For use in panels
            static bool PI_EXPORT IsKeyDown(Key key);
            static bool PI_EXPORT IsLeftMouseDown();
            static bool PI_EXPORT IsRightMouseDown();
            static Point PI_EXPORT GetMousePosition();

            static inline bool IsShiftDown()
            {
                return IsKeyDown(Key::Shift);
            }

            static inline bool IsControlDown()
            {
                return IsKeyDown(Key::Control);
            }

            static inline bool IsAltDown()
            {
                return IsKeyDown(Key::Alt);
            }

            // Does copy, paste etc
            static bool PI_EXPORT HandleAccelerator(Widget* canvas, char chr);

            // Send input to canvas for study
            static void PI_EXPORT OnMouseMove(Widget* canvas, int x, int y, int deltaX, int deltaY);
            static bool PI_EXPORT OnMouseButton(Widget* canvas, MouseButton mouseButton, MouseButtonPressMode mode);
            static bool PI_EXPORT OnKey(Widget* canvas, Key key, KeyPressMode mode);
            static void PI_EXPORT OnCanvasThink(Widget* widget);
        };

        static void UpdateHoveredControl(Widget* widget);

        static bool IsTooltipActive();
        static void SetTooltipWidget(Widget* widget);
        static void UnsetTooltipWidget(Widget* widget);

        static void SetKeyboardFocusedWidget(Widget* widget);
        static Widget* GetKeyboardFocusedWidget();

        static void SetMouseFocusedWidget(Widget* widget);
        static Widget* GetMouseFocusedWidget();

        static void SetHoveredWidget(Widget* widget);
        static Widget* GetHoveredWidget();

        static void RenderDragAndDropOverlay(Widget* widget, Skin* skin);
        static void RenderTooltip(Skin* skin);

        explicit Canvas(MainWindow* window, Skin* skin = nullptr);
        ~Canvas() override;

        /// For additional initialization
        /// (which is sometimes not appropriate in the constructor)
        virtual void Initialize()
        {}

        /// You should call this to render your canvas.
        virtual void RenderCanvas();

        /// Call this whenever you want to process input. This
        /// is usually once a frame..
        virtual void DoThink();

        /// In most situations you will be rendering the canvas
        /// every frame. But in some situations you will only want
        /// to render when there have been changes. You can do this
        /// by checking NeedsRedraw().
        virtual bool NeedsRedraw()
        {
            return m_needsRedraw;
        }

        void Redraw() override
        {
            m_needsRedraw = true;
        }

        // Internal. Do not call directly.
        void Render(Skin* render) override;

        /// Child panels call parent->GetCanvas() until they get to
        /// this top level function.
        Canvas* GetCanvas() override
        {
            return this;
        }

        void CloseMenus()
        {
            for (auto&& child : m_children)
            {
                if (auto* menu = pi_cast<Menu*>(child); menu != nullptr)
                    menu->Close();
            }
        }

        virtual void SetScale(float f);

        virtual float GetScale() const
        {
            return m_scale;
        }

        void SetSkin(Skin* skin, bool affectChildren) override;

        void OnBoundsChanged(const Rect& old) override;

        /// Delete all children (this is done called in the destructor too)
        virtual void ReleaseChildren();

        /// Delayed deletes
        virtual void AddDelayedDelete(Widget* control);
        virtual void ProcessDelayedDeletes();

        Widget* FirstTab;
        Widget* NextTab;

        ////////////////////
        // IInputEventListener implementation start
        ////////////////////

        bool OnMouseMove(PiInt32 x, PiInt32 y, PiInt32 deltaX, PiInt32 deltaY) override;

        bool OnMouseButton(MouseButton button, MouseButtonPressMode pressMode) override;

        bool OnMouseWheel(PiReal32 x, PiReal32 y) override;

        bool OnSpecialKey(Key key, KeyPressMode pressedMode) override;

        bool OnCharacter(char character) override;

        ////////////////////
        // IInputEventListener implementation end
        ////////////////////

        // Background
        virtual void SetBackgroundColor(const Color& color)
        {
            m_backgroundColor = color;
        }

        virtual void SetDrawBackground(bool shouldDraw)
        {
            m_drawBackground = shouldDraw;
        }

    protected:
        void PreDeleteCanvas(Widget* widget);

        bool m_needsRedraw;
        bool m_anyDelete;
        float m_scale;

        List m_deleteList;
        std::set<Widget*> m_deleteSet;

        Color m_backgroundColor;
    };
} // namespace SparkyStudios::UI::Pixel

#endif // PIXEL_UI_CANVAS_H
