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

#include <SparkyStudios/UI/Pixel/Core/Application.h>
#include <SparkyStudios/UI/Pixel/Core/Clipboard.h>

#include <allegro5/allegro5.h>
#include <allegro5/allegro_native_dialog.h>

namespace SparkyStudios::UI::Pixel::Clipboard
{
    PiString GetText()
    {
        PiString str;

        auto* handle = piApp->GetMainWindow()->GetNativeHandle();
        if (handle == nullptr)
            return str;

        if (al_clipboard_has_text(static_cast<ALLEGRO_DISPLAY*>(handle)))
        {
            char* clip = al_get_clipboard_text(static_cast<ALLEGRO_DISPLAY*>(handle));

            if (clip != nullptr)
            {
                str = clip;
                al_free(clip);
            }
        }

        return str;
    }

    bool SetText(const PiString& text)
    {
        auto* handle = piApp->GetMainWindow()->GetNativeHandle();
        if (handle == nullptr)
            return false;

        return al_set_clipboard_text(static_cast<ALLEGRO_DISPLAY*>(handle), text.c_str());
    }
} // namespace SparkyStudios::UI::Pixel::Clipboard
