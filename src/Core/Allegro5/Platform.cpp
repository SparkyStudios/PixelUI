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

#include <SparkyStudios/UI/Pixel/Core/Platform.h>

#include <allegro5/allegro5.h>

namespace SparkyStudios::UI::Pixel
{
    namespace Platform
    {
        void Sleep(PiUInt32 ms)
        {
            al_rest(ms * 0.001);
        }

        PI_EXPORT PiTime GetTimeInSeconds()
        {
            return al_get_time();
        }

        PI_EXPORT PiString GetExecutableDir()
        {
            if (!al_is_system_installed())
                return "";

            ALLEGRO_PATH* path = al_get_standard_path(ALLEGRO_EXENAME_PATH);
            al_set_path_filename(path, NULL);
            const PiString dir(al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP));
            al_destroy_path(path);

            return dir;
        }

        PI_EXPORT Size GetDesktopSize()
        {
            if (!al_is_system_installed())
                return Size();

            ALLEGRO_MONITOR_INFO info;
            al_get_monitor_info(0, &info);

            Size s;
            s.x = info.x2 - info.x1;
            s.y = info.y2 - info.y1;

            return s;
        }
    } // namespace Platform
} // namespace SparkyStudios::UI::Pixel
