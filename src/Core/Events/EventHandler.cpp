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
    EventHandler::~EventHandler()
    {
        CleanLinks();
    }

    void EventHandler::RegisterListener(EventListener* listener)
    {
        _listeners.push_back(listener);
    }

    void EventHandler::UnregisterListener(EventListener* listener)
    {
        _listeners.remove(listener);
    }

    EventHandler::EventHandler()
        : _listeners()
    {}

    void EventHandler::CleanLinks()
    {
        auto it = _listeners.begin();

        while (it != _listeners.end())
        {
            EventListener* listener = *it;
            UnregisterListener(listener);
            listener->RemoveHandler(this);
            it = _listeners.begin();
        }
    }
} // namespace SparkyStudios::UI::Pixel