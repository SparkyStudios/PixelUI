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

#ifndef PIXEL_UI_PLATFORM_H
#define PIXEL_UI_PLATFORM_H

#include <SparkyStudios/UI/Pixel/Core/Common.h>
#include <SparkyStudios/UI/Pixel/Graphics/Point.h>

namespace SparkyStudios::UI::Pixel
{
    namespace Platform
    {
        /**
         * @brief Sleeps the application for a specified number of milliseconds.
         *
         * @param ms The number of milliseconds to sleep
         */
        PI_EXPORT void Sleep(PiUInt32 ms);

        /**
         * @brief Get the total time the application is running.
         */
        PI_EXPORT PiTime GetTimeInSeconds();

        /**
         * @brief Get the directory of the running executable.
         *
         * @return Path string, blank if not known.
         * @note Path is terminated with directory separator.
         */
        PI_EXPORT PiString GetExecutableDir();

        /**
         * @brief Get the size of the desktop in which the main window is located.
         *
         * @return The desktop size
         */
        PI_EXPORT Size GetDesktopSize();
    } // namespace Platform
} // namespace SparkyStudios::UI::Pixel

#endif // PIXEL_UI_PLATFORM_H
