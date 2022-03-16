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

#ifndef PIXEL_UI_WIDGET_TEXTAREA_H
#define PIXEL_UI_WIDGET_TEXTAREA_H

#include <SparkyStudios/UI/Pixel/Widgets/Input.h>

namespace SparkyStudios::UI::Pixel
{
    class PI_EXPORT TextArea : public Input
    {
    public:
        PI_WIDGET(TextArea, Input);

        virtual PiInt32 GetCurrentLine();

    protected:
        void EnsureCaretVisible() override;

        bool OnKey(Key key, KeyPressMode mode) override;

        void Render(Skin* skin) override;
    };
} // namespace SparkyStudios::UI::Pixel

#endif // PIXEL_UI_WIDGET_TEXTAREA_H
