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

#ifndef PIXEL_UI_WIDGET_H
#define PIXEL_UI_WIDGET_H

#include <list>
#include <type_traits>

#include <SparkyStudios/UI/Pixel/Core/Events/EventHandler.h>
#include <SparkyStudios/UI/Pixel/Core/Events/EventListener.h>
#include <SparkyStudios/UI/Pixel/Core/Input/IInputEventListener.h>
#include <SparkyStudios/UI/Pixel/Core/Renderer/Skin.h>

#include <SparkyStudios/UI/Pixel/Widgets/Base.h>

/**
 * @brief Provides pi_cast dynamic cast features on a PixelUI widget.
 */
#define PI_DYNAMIC(_this_name_, _base_name_)                                                                                               \
    static const char* GetIdentifier()                                                                                                     \
    {                                                                                                                                      \
        static const char* ident = #_base_name_ ":" #_this_name_;                                                                          \
        return ident;                                                                                                                      \
    }                                                                                                                                      \
                                                                                                                                           \
    SparkyStudios::UI::Pixel::Widget* DynamicCast(const char* identifier) override                                                         \
    {                                                                                                                                      \
        if (GetIdentifier() == identifier)                                                                                                 \
            return this;                                                                                                                   \
                                                                                                                                           \
        return ParentClass::DynamicCast(identifier);                                                                                       \
    }

/**
 * @brief Defines standard type names for a PixelUI widget class and its parent.
 */
#define PI_CLASS(_this_name_, _base_name_)                                                                                                 \
    typedef _base_name_ ParentClass;                                                                                                       \
    typedef _this_name_ ThisClass;

/**
 * @brief Initializes a PixelUI widget class with some default settings.
 *
 * This macro will typically:
 *   - Setup standard type names using PI_CLASS
 *   - Setup pi_cast support using PI_DYNAMIC
 *   - Setup the widget type identifier based on the class name.
 *   - Setup the default class constructor
 */
#define PI_WIDGET(_this_name_, _base_name_)                                                                                                \
public:                                                                                                                                    \
    PI_CLASS(_this_name_, _base_name_)                                                                                                     \
    PI_DYNAMIC(_this_name_, _base_name_)                                                                                                   \
    static const char* GetTypeNameStatic()                                                                                                 \
    {                                                                                                                                      \
        return #_this_name_;                                                                                                               \
    }                                                                                                                                      \
    const char* GetTypeName() const override                                                                                               \
    {                                                                                                                                      \
        return GetTypeNameStatic();                                                                                                        \
    }                                                                                                                                      \
    const char* GetParentTypeName() const override                                                                                         \
    {                                                                                                                                      \
        return ParentClass::GetTypeNameStatic();                                                                                           \
    }                                                                                                                                      \
    explicit _this_name_(SparkyStudios::UI::Pixel::Widget* parent = nullptr, SparkyStudios::UI::Pixel::PiString name = "")

/**
 * @brief Similar as PI_WIDGET, but optimized for inlined constructors
 */
#define PI_WIDGET_INLINE(_this_name_, _base_name)                                                                                          \
    PI_WIDGET(_this_name_, _base_name)                                                                                                     \
        : ParentClass(parent, name)

/**
 * @brief Generates the widget constructor in a cpp file.
 *
 * This macro can be used only if PI_WIDGET was also used in the header file.
 */
#define PI_WIDGET_CONSTRUCTOR(_this_name_)                                                                                                 \
    _this_name_::_this_name_(SparkyStudios::UI::Pixel::Widget* parent, SparkyStudios::UI::Pixel::PiString name)                            \
        : ParentClass(parent, std::move(name))

namespace SparkyStudios::UI::Pixel
{
#if PI_ENABLE_ANIMATION
    class Animation;
#endif // PI_ENABLE_ANIMATION

    class Canvas;
    class Widget;

    /**
     * @brief The base class for every PixelUI widgets.
     */
    class PI_EXPORT Widget : public EventHandler
    {
        friend class Canvas;

    public:
        /**
         * @brief The mouse enter event.
         */
        static const char* const MouseEnterEvent;

        /**
         * @brief The mouse leave event.
         */
        static const char* const MouseLeaveEvent;

        /**
         * @brief The mouse button pressed event.
         */
        static const char* const MouseButtonDownEvent;

        /**
         * @brief The mouse button released event.
         */
        static const char* const MouseButtonUpEvent;

        /**
         * @brief The mouse button double click event.
         */
        static const char* const MouseDoubleClickEvent;

        /**
         * @brief The key pressed event.
         */
        static const char* const KeyDownEvent;

        /**
         * @brief The key released event.
         */
        static const char* const KeyUpEvent;

        /**
         * @brief A collection of widgets.
         */
        typedef std::list<Widget*> List;

        /**
         * @brief Construct a new PixelUI Widget.
         *
         * @param parent The parent widget.
         * @param name The widget name.
         */
        explicit Widget(Widget* parent, PiString name = "");
        ~Widget() override;

        /**
         * @brief Get the type name of this widget.
         *
         * @return The widget type name.
         */
        static const char* GetTypeNameStatic();

        /**
         * @brief Get the widget class identifier for use in pi_cast.
         */
        static const char* GetIdentifier()
        {
            return GetTypeNameStatic();
        }

        /**
         * @brief Casts this widget to the type identified by the given name.
         *
         * @param typeName The name of the type.
         *
         * @return A widget of the type identified by the given name.
         */
        virtual Widget* DynamicCast(const char* typeName)
        {
            return nullptr;
        }

        /**
         * @brief Get the type name of this widget.
         *
         * @return The widget type name.
         */
        [[nodiscard]] virtual const char* GetTypeName() const;

        /**
         * @brief Get the type name of the parent widget.
         *
         * @return The parent widget type name.
         */
        [[nodiscard]] virtual const char* GetParentTypeName() const;

        virtual void DelayedDelete();
        virtual void PreDelete(Skin* skin);

        /**
         * @brief Gets the event listener associated to the given event name.
         *
         * @param event The event name.
         *
         * @return An event listener for the event.
         */
        EventListener* On(const PiString& event);

        /**
         * @brief Set the parent of this widget.
         *
         * @param parent The parent widget.
         */
        virtual void SetParent(Widget* parent);

        /**
         * @brief Get the parent of this widget.
         *
         * @return The parent widget.
         */
        [[nodiscard]] virtual Widget* GetParent() const;

        /**
         * @brief Set the string to display inside the tooltip.
         *
         * The given string will be wrapped inside a label.
         *
         * @param str The string to display in the tooltip.
         */
        virtual void SetTooltipText(const PiString& str);

        /**
         * @brief Set the widget to display inside the tooltip.
         *
         * @param tooltip The widget to display inside the tooltip.
         */
        virtual void SetTooltip(Widget* tooltip);

        /**
         * @brief Get the tooltip widget.
         */
        virtual Widget* GetTooltip();

        /**
         * @brief Get the root widget of the widget tree.
         *
         * @return The root widget.
         */
        virtual Canvas* GetCanvas();

        /**
         * @brief Get the the list of children of this widget.
         *
         * @return The children of this widget
         */
        [[nodiscard]] virtual const List& GetChildren() const;

        /**
         * @brief Checks if the given widget is a child of this widget.
         *
         * @param child The widget to check.
         *
         * @return true if the given widget is a child of this widget, false otherwise.
         */
        virtual bool IsChild(Widget* child) const;

        /**
         * @brief Gets the number of children of this widget.
         *
         * @return The number of children of this widget.
         */
        [[nodiscard]] virtual PiUInt32 ChildCount() const;

        /**
         * @brief Get the child at the given index.
         *
         * @param index The child index.
         *
         * @return The child widget at the given index.
         */
        [[nodiscard]] virtual Widget* GetChild(PiUInt32 index) const;

        /**
         * @brief Resizes this widget to fit its content.
         *
         * @param width Whether to resize the width.
         * @param height Whether to resize the height.
         *
         * @return Whether the resize was successful.
         */
        [[maybe_unused]] virtual bool SizeToChildren(bool width, bool height);

        /**
         * @brief Gets the size of the content of this widget.
         *
         * @return The size of the content of this widget.
         */
        [[nodiscard]] virtual Size ChildrenSize() const;

        /**
         * @brief Finds a child widget with the given name.
         *
         * @param name The name of the child widget to find.
         * @param recursive Whether to search for children recursively.
         *
         * @return The found child widget, or nullptr if no child widget with the given name exists.
         */
        [[nodiscard]] virtual Widget* FindChildByName(const PiString& name, bool recursive) const;

        /**
         * @brief Finds a child widget with the given name, and
         * cast it to the given type.
         *
         * @tparam T The type of the child widget to find.
         * @param name The name of the child widget to find.
         * @param recursive Whether to search for children recursively.
         *
         * @return The found child widget, or nullptr if no child widget with the given name exists.
         */
        template<class T, std::enable_if_t<std::is_base_of_v<Widget, T>, bool> = true>
        T* FindChild(const PiString& name, bool recursive = false) const;

        /**
         * @brief Get the child widget at the given position.
         *
         * @param x The x position.
         * @param y The y position.
         * @param onlyIfMouseEnabled Whether to find the child widget only if mouse input is enabled.
         *
         * @return The child widget at the given position.
         */
        virtual Widget* GetWidgetAt(PiInt32 x, PiInt32 y, bool onlyIfMouseEnabled);

        /**
         * @brief Removes all children of this widget.
         */
        virtual void ClearChildren();

        /**
         * @brief Sets the widget name.
         *
         * @param name The new name.
         */
        virtual void SetName(const PiString& name);

        /**
         * @brief Gets the widget name.
         *
         * @return The name of the widget.
         */
        [[nodiscard]] virtual const PiString& GetName() const;

        /**
         * @brief Updates the current widget state.
         *
         * This method is called automatically before the rendering process.
         */
        virtual void Think();

        /**
         * @brief Moves this widget to the back position of its parent.
         */
        virtual void SendToBack();

        /**
         * @brief Moves this widget to the front position of its parent.
         */
        virtual void BringToFront();

        /**
         * @brief Moves this widget position near the given widget.
         *
         * @param child The child widget to move this widget near to.
         * @param behind Whether this widget should be placed behind the given child.
         */
        virtual void MoveNextToWidget(Widget* child, bool behind);

        /**
         * @brief Translates a point from widget coordinates to the root window coordinates.
         *
         * @param position The position to translate.
         *
         * @return The translated position.
         */
        virtual Point LocalPositionToWindow(const Point& position);

        /**
         * @brief Translates a point from window coordinates to widget coordinates.
         *
         * @param position
         * @return Point
         */
        virtual Point WindowPositionToLocal(const Point& position);

        /**
         * @brief Set the dock position of this widget.
         *
         * @param dock The dock position.
         */
        virtual void SetDock(Alignment dock);

        /**
         * @brief Get the current dock position.
         *
         * @return The dock position.
         */
        [[nodiscard]] virtual Alignment GetDock() const;

        /**
         * @brief Defines if the widget should be constrained into its parent
         * when using MoveTo() and MoveBy().
         *
         * @param value Whether the widget should be constrained into its parent.
         */
        [[maybe_unused]] virtual void RestrictToParent(bool value);

        /**
         * @brief Checks if the widget is constrained into its parent.
         *
         * @return true if the widget is constrained into its parent, false otherwise.
         */
        [[nodiscard]] virtual bool IsRestrictedToParent() const;

        /**
         * @brief Set the bounds of this widget in window coordinates.
         *
         * @param x The x position of the widget.
         * @param y The y position of the widget.
         * @param width The width of the widget.
         * @param height The height of the widget.
         *
         * @return True if the bounds has been changed, false otherwise.
         */
        virtual bool SetBounds(PiInt32 x, PiInt32 y, PiInt32 width, PiInt32 height);

        /**
         * @brief Set the bounds of this widget in window coordinates.
         *
         * @param bounds The widget bounds.
         *
         * @return True if the bounds has been changed, false otherwise.
         */
        virtual bool SetBounds(const Rect& bounds);

        /**
         * @brief Get the bounds of this widget.
         *
         * @return The widget bounds.
         */
        [[nodiscard]] virtual const Rect& GetBounds() const;

        /**
         * @brief Get the position of this widget over the X-axis
         * in window coordinates.
         *
         * @return The x position of the widget.
         */
        [[nodiscard]] virtual PiInt32 X() const;

        /**
         * @brief Get the position of this widget over the Y-axis
         * in window coordinates.
         *
         * @return The y position of the widget.
         */
        [[nodiscard]] virtual PiInt32 Y() const;

        /**
         * @brief Get the width of this widget.
         *
         * @return The widget width.
         */
        [[nodiscard]] virtual PiInt32 Width() const;

        /**
         * @brief Get the height of this widget.
         *
         * @return The widget height.
         */
        [[nodiscard]] virtual PiInt32 Height() const;

        /**
         * @brief Get the position of the left edge of
         * this widget. This include the margin.
         *
         * @return The left edge of the widget, including margin.
         */
        [[nodiscard]] virtual PiInt32 Left() const;

        /**
         * @brief Get the position of the right edge of
         * this widget. This include the margin.
         *
         * @return The right edge of the widget, including margin.
         */
        [[nodiscard]] virtual PiInt32 Right() const;

        /**
         * @brief Get the position of the bottom edge of
         * this widget. This include the margin.
         *
         * @return The bottom edge of the widget, including margin.
         */
        [[nodiscard]] virtual PiInt32 Bottom() const;

        /**
         * @brief Get the position of the top edge of
         * this widget. This include the margin.
         *
         * @return The top edge of the widget, including margin.
         */
        [[nodiscard]] virtual PiInt32 Top() const;

        /**
         * @brief Get the area inside this widget that doesn't have
         * child widgets docked to it.
         *
         * @return The widget inner bounds.
         */
        [[nodiscard]] virtual const Rect& InnerBounds() const;

        /**
         * @brief Get the bounds passed to the renderer when drawing this
         * widget.
         *
         * @return The render bounds of this widget.
         */
        [[nodiscard]] virtual const Rect& RenderBounds() const;

        /**
         * @brief Set the widget position within the bounds of its parent.
         *
         * @param pos The dock position.
         * @param xOffset The x offset from the dock position.
         * @param yOffset The y offset from the dock position.
         */
        virtual void SetPosition(Alignment pos, int xOffset, int yOffset);

        /**
         * @brief Set the widget position in window coordinates.
         *
         * @param x The x position.
         * @param y The y position.
         */
        virtual void SetPosition(PiInt32 x, PiInt32 y);

        /**
         * @brief Set the widget position in window coordinates.
         *
         * @param position The widget position.
         */
        virtual void SetPosition(const Point& position);

        /**
         * @brief Get the widget position in window coordinates.
         *
         * @return The widget position.
         */
        [[nodiscard]] virtual Point GetPosition() const;

        /**
         * @brief Set the widget width.
         *
         * @param width The width of the widget.
         */
        virtual void SetWidth(PiInt32 width);

        /**
         * @brief Set the widget height.
         *
         * @param height The height of the widget.
         */
        virtual void SetHeight(PiInt32 height);

        /**
         * @brief Set the widget size.
         *
         * @param width The width of the widget.
         * @param height The height of the widget.
         */
        virtual bool SetSize(PiInt32 width, PiInt32 height);

        /**
         * @brief Set the widget size.
         *
         * @param size The size of the widget.
         */
        virtual bool SetSize(const Size& size);

        /**
         * @brief Get the widget size.
         *
         * @return The size of the widget.
         */
        [[nodiscard]] virtual Size GetSize() const;

        /**
         * @brief Set the widget padding.
         *
         * @param padding The widget padding.
         */
        virtual void SetPadding(const Padding& padding);

        /**
         * @brief Get the widget padding.
         *
         * @return The widget padding.
         */
        [[nodiscard]] virtual const Padding& GetPadding() const;

        /**
         * @brief Set the widget margin.
         *
         * @param margin The widget margin.
         */
        virtual void SetMargin(const Margin& margin);

        /**
         * @brief Get the widget margin.
         *
         * @return The widget margin.
         */
        [[nodiscard]] virtual const Margin& GetMargin() const;

        /**
         * @brief Moves this widget to the specified position. This method
         * is similar to SetPosition(), except that it uses IsRestrictedToParent().
         *
         * @param x The x position.
         * @param y The y position.
         */
        virtual void MoveTo(PiInt32 x, PiInt32 y);

        /**
         * @brief Moves this widget to the specified position. This method
         * is similar to SetPosition(), except that it uses IsRestrictedToParent().
         *
         * @param position The position.
         */
        virtual void MoveTo(const Point& position);

        /**
         * @brief Moves from the current position with the specified offset.
         *
         * @param x The horizontal offset.
         * @param y The vertical offset.
         */
        virtual void MoveBy(PiInt32 x, PiInt32 y);

        /**
         * @brief Moves from the current position with the specified offset.
         *
         * @param offset The offset.
         */
        virtual void MoveBy(const Point& offset);

        /**
         * @brief Set the visibility of this widget.
         *
         * @param value Whether to show or hide the widget.
         */
        virtual void SetHidden(bool value);

        /**
         * @brief Makes this widget visible.
         */
        virtual void Show();

        /**
         * @brief Makes this widget hidden.
         */
        virtual void Hide();

        /**
         * @brief Checks if this widget is hidden.
         *
         * @return true if hidden, false otherwise.
         */
        [[nodiscard]] virtual bool IsHidden() const;

        /**
         * @brief Checks if this widget is visible.
         *
         * @return true if visible, false otherwise.
         */
        [[nodiscard]] virtual bool IsVisible() const;

        /**
         * @brief Sets if this widget is disabled or not.
         *
         * @param value The disabled state.
         */
        virtual void SetDisabled(bool value);

        /**
         * @brief Gets the disabled state of this widget.
         */
        [[nodiscard]] virtual bool IsDisabled() const;

        /**
         * @brief Applies a custom skin to this widget.
         *
         * @param skin The custom skin.
         * @param affectChildren Defines if the custom skin should be applied to child widgets too.
         */
        virtual void SetSkin(Skin* skin, bool affectChildren);

        /**
         * @brief Get the custom skin of this widget, if any.
         *
         * @return The custom widget skin.
         */
        [[nodiscard]] virtual Skin* GetSkin() const;

        /**
         * @brief Sets if mouse inputs are enabled on this widget.
         *
         * @param value Whether mouse inputs are enabled.
         */
        virtual void SetMouseInputEnabled(bool value);

        /**
         * @brief Get whether this widget has mouse events enabled.
         */
        [[nodiscard]] virtual bool GetMouseInputEnabled() const;

        /**
         * @brief Sets if keyboard inputs are enabled on this widget.
         *
         * @param value Whether keyboard inputs are enabled.
         */
        virtual void SetKeyboardInputEnabled(bool value);

        /**
         * @brief Get whether this widget has keyboard events enabled.
         */
        [[nodiscard]] virtual bool GetKeyboardInputEnabled() const;

        /**
         * @brief Signals this widgets and all its ancestors that it has been touched (clicked).
         */
        virtual void Touch();

        /**
         * @brief Focus keyboard input on this widget.
         */
        virtual void Focus();

        /**
         * @brief Make this widget lose the keyboard input focus.
         */
        virtual void UnFocus();

        /**
         * @brief Get whether this widget is currently the one hovered.
         */
        [[nodiscard]] virtual bool IsHovered() const;

        /**
         * @brief Get whether this widget currently have the keyboard focus.
         */
        [[nodiscard]] virtual bool IsFocused() const;

        /**
         * @brief Checks if this widget is on the top of its parent.
         */
        [[nodiscard]] virtual bool IsOnTop() const;

        /**
         * @brief Enables cache to texture optimization on this widget.
         */
        virtual void EnableCacheToTexture();

        /**
         * @brief Disables cache to texture optimization on this widget.
         */
        virtual void DisableCacheToTexture();

        /**
         * @brief Check if cache to texture optimization is enabled.
         */
        [[nodiscard]] virtual bool IsCachedToTextureEnabled() const;

        /**
         * @brief Checks if this widget needs a layout pass.
         */
        [[nodiscard]] bool NeedsLayout() const;

        /**
         * @brief Invalidates this widget and force a re-render.
         */
        void Invalidate();

        /**
         * @brief Invalidates this widget's parent and force a re-render.
         */
        void InvalidateParent();

        /**
         * @brief Invalidates this widget's children and force them to be re-rendered.
         *
         * @param recursive Defines if we should recursively invalidate the children.
         */
        void InvalidateChildren(bool recursive = false);

        void SetShouldIncludeInSize(bool value);

        [[nodiscard]] bool ShouldIncludeInSize() const;

        /**
         * @brief Process an accelerator string.
         *
         * @param accelerator The accelerator.
         *
         * @return Whether the accelerator has been handled.
         */
        virtual bool RunAccelerator(const PiString& accelerator);

        /**
         * @brief Set the widget value in string representation.
         *
         * @param value The string representation of the value.
         */
        virtual void SetValue(const PiString& value);

        /**
         * @brief Get the value of this widget in string representation.
         */
        virtual PiString GetValue();

#if PI_ENABLE_ANIMATION

        /**
         * @brief Animates this widget using the given animation
         *
         * @param animation The animation.
         */
        void Animate(Animation* animation);

#endif // PI_ENABLE_ANIMATION

        /**
         * @brief Adds an event listener using an accelerator.
         *
         * @tparam T The event handler class type.
         *
         * @param accelerator The accelerator command as string.
         * @param func A pointer to a class method from the event handler.
         * @param handler The event handler.
         */
        template<typename T>
        void AddAccelerator(const PiString& accelerator, void (T::*func)(EventInfo), EventHandler* handler = nullptr)
        {
            if (handler == nullptr)
                handler = this;

            EventListener* listener = new EventListener();
            listener->Add(handler, func);
            PiString str = accelerator;
            Utility::Strings::ToUpper(str);
            Utility::Strings::Strip(str, " ");
            m_accelerators[str] = listener;
        }

    protected:
        /**
         * @brief Adds a child widget to this widget.
         *
         * @param child The child widget.
         */
        virtual void AddChild(Widget* child);

        /**
         * @brief Removes the given child widget from this widget.
         *
         * @param child The child widget to remove.
         */
        virtual void RemoveChild(Widget* child);

        /**
         * @brief Notify a child widget that this widget's bounds have changed.
         *
         * @param old The old bounds of this widget.
         * @param child The child widget to notify.
         */
        virtual void NotifyBoundsChanged(const Rect& old, Widget* child);

        /**
         * @brief Updates the render bounds of this widget.
         */
        virtual void UpdateRenderBounds();

        /**
         * @brief Event triggered when a child is added to this widget.
         *
         * @param child The added child.
         */
        virtual void OnChildAdded(Widget* child);

        /**
         * @brief Event triggered when a child is added to this widget.
         *
         * @param child The added child.
         */
        virtual void OnChildRemoved(Widget* child);

        /**
         * @brief Event triggered when this widget bounds are changed.
         *
         * @param old The old bounds of this widget.
         */
        virtual void OnBoundsChanged(const Rect& old);

        /**
         * @brief Event triggered when the UI scale has changed.
         */
        virtual void OnScaleChanged();

        /**
         * @brief Event triggered when the custom skin of this widget has changed.
         *
         * @param skin The new custom skin of this widget.
         */
        virtual void OnSkinChanged(Skin* skin);

        /**
         * @brief Event triggered when the mouse cursor enters the widget bounds.
         */
        virtual void OnMouseEnter();

        /**
         * @brief Event triggered when the mouse cursor is moving inside the bounds of
         * this widget.
         *
         * @param position The previous mouse position.
         * @param deltaX The amount to move the cursor over the x axis.
         * @param deltaY The amount to move the cursor over the y axis.
         */
        virtual void OnMouseMove(const Point& position, PiInt32 deltaX, PiInt32 deltaY);

        /**
         * @brief Event triggered when a mouse button is pressed or released within the
         * bounds of this widget.
         *
         * @param position The mouse position.
         * @param button The mouse button.
         * @param mode The state of the mouse button.
         */
        virtual void OnMouseButton(const Point& position, MouseButton button, MouseButtonPressMode mode);

        /**
         * @brief Event triggered when a double click occurs on this widget.
         *
         * @param position The mouse position.
         * @param button The double clicked mouse button.
         */
        virtual void OnMouseDoubleClick(const Point& position, MouseButton button);

        /**
         * @brief Event triggered when a scroll event occurs on this widget.
         *
         * @param delta The scroll delta over the horizontal and vertical axis.
         *
         * @return Whether the event has been handled.
         */
        virtual bool OnMouseWheel(const Point& delta);

        /**
         * @brief Event triggered when the mouse cursor exits the widget bounds.
         */
        virtual void OnMouseLeave();

        /**
         * @brief Event triggered when a keyboard key is pressed or released while
         * this widget has focus.
         *
         * @param key The keyboard key.
         * @param mode The state of the keyboard key.
         *
         * @return Whether the event has been handled.
         */
        virtual bool OnKey(Key key, KeyPressMode mode);

        /**
         * @brief Event triggered when the user presses a key while this widget
         * has the keyboard focus.
         *
         * @param key The key that was pressed.
         *
         * @return true if the widget has handled the event, false otherwise.
         */
        virtual bool OnKeyDown(Key key);

        /**
         * @brief Event triggered when the user releases a key while this widget
         * has the keyboard focus.
         *
         * @param key The key that was released.
         *
         * @return true if the widget has handled the event, false otherwise.
         */
        virtual bool OnKeyUp(Key key);

        /**
         * @brief Event triggered when a character input is received when this widget has focus.
         *
         * @param character The input character.
         *
         * @return Whether the event has been handled.
         */
        virtual bool OnCharacter(char character);

        /**
         * @brief Event triggered when one child of this widget has bee touched (clicked).
         *
         * @param child The touched child.
         */
        virtual void OnChildTouched(Widget* child);

        /**
         * @brief Event triggered when this widget get the keyboard focus.
         */
        virtual void OnGetKeyboardFocus();

        /**
         * @brief Event triggered when this widget lost the keyboard focus.
         */
        virtual void OnLostKeyboardFocus();

        /**
         * @brief Defines whether accelerators should be processed only on focus.
         */
        virtual bool AccelOnlyOnFocus();

        /**
         * @brief Handle the accelerator with the specified name.
         *
         * @param accel The accelerator name.
         *
         * @return Whether the accelerator has been handled.
         */
        virtual bool HandleAccelerator(const PiString& accel);

        /**
         * @brief Returns whether the renderer should use a clip region
         * while drawing this widget.
         *
         * @return true
         * @return false
         */
        virtual bool ShouldClip();

        /**
         * @brief Renders this widget and all children recursively.
         *
         * @param skin The skin to apply during the render.
         */
        void RenderRecursive(Skin* skin);

        /**
         * @brief Renders this widget on screen.
         *
         * @param skin The skin to use when drawing the widget. This is
         * ignored when the widget provide its own skin.
         */
        virtual void Render(Skin* skin);

        /**
         * @brief Renders this widget on screen.
         *
         * @param skin The skin to use when drawing the widget. This is
         * ignored when the widget provide its own skin.
         */
        virtual void RenderOverlay(Skin* skin);

        /**
         * @brief Renders this widget on screen.
         *
         * @param skin The skin to use when drawing the widget. This is
         * ignored when the widget provide its own skin.
         */
        virtual void RenderUnder(Skin* skin);

        /**
         * @brief Renders this widget on screen.
         *
         * @param skin The skin to use when drawing the widget. This is
         * ignored when the widget provide its own skin.
         */
        virtual void RenderFocus(Skin* skin);

        /**
         * @brief Ask the renderer to redraw this widget.
         */
        virtual void Redraw();

        /**
         * @brief Updates the widget layout and texture cache
         * if enabled.
         *
         * @param skin The skin to use when drawing the widget.
         */
        virtual void Layout(Skin* skin);

        /**
         * @brief Updates the widget layout and texture cache recursively
         * in all children widgets.
         *
         * @param skin The skin to use when drawing the widget. This is ignored
         * when the widget provides its own skin.
         */
        virtual void RecurseLayout(Skin* skin);

        /**
         * @brief Called right after the widget layout has been updated.
         *
         * @param skin The skin used during the layout update.
         */
        virtual void PostLayout(Skin* skin);

        /**
         * @brief The logical parent of this widget.
         */
        Widget* m_parent;

        /**
         * @brief The list of widgets that are children of this widget.
         */
        List m_children;

        /**
         * @brief If the inner panel exists the children of this widget will automatically
         * become children of that instead of this widget - allowing this widget to move
         * them all around by moving that panel (useful for scrolling etc).
         */
        Widget* m_innerPanel;

        /**
         * @brief This is the real parent of this widget, most likely the inner panel
         * of the logical parent (if it has one).
         */
        Widget* m_actualParent;

        /**
         * @brief The widget's tooltip.
         */
        Widget* m_tooltip;

        /**
         * @brief The custom skin of the widget.
         */
        Skin* m_skin;

        /**
         * @brief The widget bounds in window coordinates.
         */
        Rect m_bounds;

        Rect m_innerBounds;

        /**
         * @brief The widget bounds passed to the renderer.
         */
        Rect m_renderBounds;

        /**
         * @brief The widget padding.
         */
        Padding m_padding;

        /**
         * @brief The widget margin.
         */
        Margin m_margin;

        /**
         * @brief The widget name.
         */
        PiString m_name;

        bool m_restrictToParent;

        /**
         * @brief Defines if this widget is disabled.
         *
         * A disabled event cannot trigger events nor process inputs.
         */
        bool m_disabled;

        /**
         * @brief Defines if this widget is visible.
         */
        bool m_hidden;

        /**
         * @brief Defines if this widget can process mouse inputs.
         */
        bool m_mouseInputEnabled;

        /**
         * @brief Defines if this widget can process keyboard inputs.
         */
        bool m_keyboardInputEnabled;

        /**
         * @brief Defines if this widget should draw background.
         */
        bool m_drawBackground;

        /**
         * @brief Defines if this widget needs a layout pass.
         */
        bool m_needsLayout;

        /**
         * @brief Defines if this widget should update the cached texture.
         */
        bool m_cacheTextureDirty;

        /**
         * @brief Defines if this widget is using a cached texture.
         */
        bool m_cacheToTexture;

        /**
         * @brief The map of registered events.
         */
        AccelMap m_eventsMap;

        /**
         * @brief The list of registered accelerators.
         */
        AccelMap m_accelerators;

        /**
         * @brief The docking position of this widget inside his parent.
         */
        Alignment m_dock;

        bool m_includeInSize;

    private:
        void DoRender(Skin* skin);
        void DoCacheRender(Skin* skin, Widget* root);
    };

    /**
     * @brief To avoid using dynamic_cast we have pi_cast.
     *
     * @parblock
     *  Each class in PixelUI includes PI_DYNAMIC. You don't have to include this
     *  macro anywhere as it's automatically included in the PI_WIDGET macro.
     *
     *  PI_DYNAMIC adds 2 functions:
     *
     *  * GetIdentifier() :-
     *      A static function with a static variable inside, which returns
     *      the address of the static variable. The variable is defined
     *      as a string containing "BaseClassName:ClassName". This string
     *      should be as unique as possible - or the compiler might optimize the
     *      variables together - which means that when this function returns the
     *      address it could be the same on one or more variables. Something to
     *      bear in mind.
     *
     *  * DynamicCast() :-
     *      Non static, takes an address returned by GetIdentifier().
     *      Will return an address of a control if the control can safely be cast to
     *      the class from which the identifier was taken.
     *
     *  Really you shouldn't actually have to concern yourself with that stuff.
     *  The only thing you should use in theory is pi_cast - which is used
     *  just the same as dynamic cast:
     *
     *      pi_cast<MyWidget*>(widget)
     *      dynamic_cast<MyWidget*>(widget)
     * @endparblock
     *
     * @tparam T The widget type. Should be a pointer type.
     *
     * @param widget The widget to cast.
     *
     * @return The cast address of the widget.
     */
    template<class T, std::enable_if_t<std::is_pointer_v<T> && std::is_base_of_v<Widget, std::remove_pointer_t<T>>, bool> = true>
    inline T pi_cast(Widget* widget)
    {
        if (!widget)
            return nullptr;

        Widget* result = widget->DynamicCast(std::remove_pointer_t<T>::GetIdentifier());

        return result != nullptr ? static_cast<T>(result) : nullptr;
    }

    template<class T, std::enable_if_t<std::is_base_of_v<Widget, T>, bool>>
    inline T* Widget::FindChild(const PiString& name, bool recursive) const
    {
        return pi_cast<T*>(FindChildByName(name, recursive));
    }
} // namespace SparkyStudios::UI::Pixel

#endif // PIXEL_UI_WIDGET_H
