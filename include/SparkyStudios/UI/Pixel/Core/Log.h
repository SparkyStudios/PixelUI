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

#ifndef PIXEL_UI_LOG_H
#define PIXEL_UI_LOG_H

namespace SparkyStudios::UI::Pixel
{
    namespace Log
    {
        /**
         * @brief Log message classification
         */
        enum class Level
        {
            Info,
            Warning,
            Error,
            Fatal
        };

        typedef void (*LogListener)(Level, const char*);

        /**
         * @brief Writes a message to the platform logging system.
         *
         * @param level The log message level.
         * @param message A message, using printf() style formatting
         * @param ... Message variables.
         */
        void Write(Level level, const char* message, ...);

        /**
         * @brief Set the Log Listener, where messages are sent to.
         *
         * @param listener The log listener.
         */
        void SetLogListener(LogListener listener);
    } // namespace Log
} // namespace SparkyStudios::UI::Pixel

#endif // PIXEL_UI_LOG_H
