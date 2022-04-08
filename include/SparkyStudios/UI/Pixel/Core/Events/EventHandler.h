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

#ifndef PIXEL_UI_EVENTHANDLER_H
#define PIXEL_UI_EVENTHANDLER_H

#include <list>

#include <SparkyStudios/UI/Pixel/Config/Config.h>

namespace SparkyStudios::UI::Pixel
{
    class EventListener;

    /**
     *  @brief Base class used to allow to receive events.
     */
    class PI_EXPORT EventHandler
    {
    public:
        using Listeners = std::list<EventListener*>;

        virtual ~EventHandler();

        /**
         * @brief Registers a listener on this event handler.
         */
        void RegisterListener(EventListener* listener);

        /**
         * @brief Unregisters a listener from this event handler.
         */
        void UnregisterListener(EventListener* listener);

    protected:
        EventHandler(); // Class needs to be derived.

    private:
        void CleanLinks();

        Listeners _listeners;
    };

} // namespace SparkyStudios::UI::Pixel

#endif // PIXEL_UI_EVENTHANDLER_H
