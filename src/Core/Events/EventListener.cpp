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

#include <SparkyStudios/UI/Pixel/Core/Events/EventHandler.h>
#include <SparkyStudios/UI/Pixel/Core/Events/EventListener.h>

namespace SparkyStudios::UI::Pixel
{
    EventListener::EventListener()
        : _handlers()
    {}

    EventListener::~EventListener()
    {}

    void EventListener::AddCallback(EventHandler* handler, const EventCallback& callback, const EventData& packet)
    {
        AddInternal(handler, callback, packet);
    }

    template<typename T>
    void EventListener::Add(EventHandler* handler, void (T::*f)(EventInfo), const EventData& packet)
    {
        auto callback = [=](EventHandler& h, Info const& i) -> void
        {
            (static_cast<T&>(h).*f)(i);
        };

        AddCallback(handler, EventCallback(callback), packet);
    }

    void EventListener::RemoveHandler(EventHandler* handler)
    {
        handler->UnregisterListener(this);
        auto it = _handlers.begin();

        while (it != _handlers.end())
        {
            const HandlerInstance& h = *it;

            if (h.handler == handler)
                it = _handlers.erase(it);
            else
                ++it;
        }
    }

    void EventListener::Call(Widget* widget)
    {
        EventInfo info{};
        Call(widget, info);
    }

    void EventListener::Call(Widget* widget, EventInfo& info)
    {
        info.source = widget;

        for (auto&& h : _handlers)
        {
            info.hookData = &h.data;

            if (h.callback)
                h.callback(*h.handler, info);
        }
    }

    void EventListener::AddInternal(EventHandler* handler, const EventCallback& callback, const EventData& data)
    {
        HandlerInstance h = { callback, data, handler };
        _handlers.push_back(h);
        handler->RegisterListener(this);
    }

    void EventListener::CleanLinks()
    {
        for (auto&& h : _handlers)
        {
            h.handler->UnregisterListener(this);
        }

        _handlers.clear();
    }
} // namespace SparkyStudios::UI::Pixel