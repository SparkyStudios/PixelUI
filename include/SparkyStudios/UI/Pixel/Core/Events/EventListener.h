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

#ifndef PIXEL_UI_EVENTLISTENER_H
#define PIXEL_UI_EVENTLISTENER_H

#include <functional>
#include <list>

#include <SparkyStudios/UI/Pixel/Config/Config.h>
#include <SparkyStudios/UI/Pixel/Config/Types.h>
#include <SparkyStudios/UI/Pixel/Core/Input/Keyboard.h>
#include <SparkyStudios/UI/Pixel/Core/Input/Mouse.h>

namespace SparkyStudios::UI::Pixel
{
    class EventHandler;
    class Widget;

    struct EventData
    {
        union
        {
            Key key;
            MouseButton mouseButton;
        };

        Point point;

        PiString string;
        PiInt8 int8;
        PiInt16 int16;
        PiInt32 int32;
        PiInt64 int64;
        PiUInt8 uint8;
        PiUInt16 uint16;
        PiUInt32 uint32;
        PiUInt64 uint64;
        PiReal32 real32;
        PiReal64 real64;
        PiVoidPtr userData;
    };

    /**
     * @brief The event info passed to the event listener.
     */
    struct EventInfo
    {
        explicit EventInfo(Widget* widget = nullptr)
            : source(widget)
        {}

        /**
         * @brief The event source.
         *
         * It will always be the widget which have triggered the event.
         */
        Widget* source;

        /**
         * @brief The event data defined by the event hook. e.g. :-
         * ~~~
         * EventData data{};
         * data.string = "A simple event";
         * onSomeEvent.Add(handler, &HandlerClass::CallbackMethod, data);
         * ~~~
         */
        const EventData* hookData;

        /**
         * @brief The event data.
         *
         * This is set by the widget and may be empty.
         */
        EventData data;
    };

    typedef std::function<void(EventHandler&, EventInfo)> EventCallback;
    typedef void (EventHandler::*EventListenerMethod)(EventInfo info);

    /**
     * @brief Event listener.
     *
     * Event handlers are registered to the event listener through a callback to receive
     * event notifications.
     *
     * The generic event notification flow is:
     * Widget (trigger event) -> EventListener (receive event) -> EventHandler (execute event)
     */
    class PI_EXPORT EventListener
    {
    public:
        EventListener();
        ~EventListener();

        /**
         * @brief Registers a custom callback to this event listener.
         *
         * @param handler The event handler.
         * @param callback The event callback.
         * @param data The additional data to pass to the handler when this listener receive the event.
         */
        void AddCallback(EventHandler* handler, const EventCallback& callback, const EventData& data = EventData());

        /**
         * @brief Registers a callback to this event listener using a
         * method from the event handler class.
         *
         * @tparam T The event handler class type.
         *
         * @param handler The event handler.
         * @param f A pointer to a class method from the event handler.
         * @param data The additional data to pass to the handler when this listener receive the event.
         */
        template<typename T>
        void Add(EventHandler* handler, void (T::*f)(EventInfo), const EventData& data = EventData())
        {
            auto callback = [=](EventHandler& h, const EventInfo& i) -> void
            {
                (static_cast<T&>(h).*f)(i);
            };

            AddCallback(handler, EventCallback(callback), data);
        }

        /**
         * @brief Unregisters an event handler from this event listener.
         *
         * @param handler The event handler to remove.
         */
        void RemoveHandler(EventHandler* handler);

        /**
         * @brief Executes the event by calling all the registered event handlers.
         *
         * @param widget The widget which have triggered the event.
         */
        void Call(Widget* widget);

        /**
         * @brief Executes the event by calling all the registered event handlers.
         *
         * @param widget The widget which have triggered the event.
         * @param info The event info to pass to the event handler.
         */
        void Call(Widget* widget, EventInfo& info);

    private:
        void AddInternal(EventHandler* handler, const EventCallback& callback, const EventData& data);

        void CleanLinks();

        struct HandlerInstance
        {
            EventCallback callback;
            EventData data;
            EventHandler* handler;
        };

        std::list<HandlerInstance> _handlers;
    };
} // namespace SparkyStudios::UI::Pixel

#endif // PIXEL_UI_EVENTLISTENER_H
