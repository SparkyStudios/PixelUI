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

#include <SparkyStudios/UI/Pixel/Widgets/InputNumber.h>

namespace SparkyStudios::UI::Pixel
{
    PI_WIDGET_CONSTRUCTOR(InputNumber)
    {
        SetInputValue(0);
    }

    void InputNumber::SetInputValue(PiReal64 value)
    {
        SetText(Utility::ToString(value));
    }

    PiReal64 InputNumber::GetInputValue()
    {
        return Utility::Strings::To::Float(GetText());
    }

    bool InputNumber::IsTextAllowed(const PiString& text, const PiInt32 index)
    {
        const PiString& strString = GetText();

        if (text.length() == 0)
            return true;

        for (size_t i = 0; i < text.length(); i++)
        {
            if (text[i] == '-')
            {
                // Has to be at the start
                if (i != 0 || index != 0)
                    return false;

                // Can only be one
                if (std::count(strString.begin(), strString.end(), '-') > 0)
                    return false;

                continue;
            }

            if (text[i] >= '0' && text[i] <= '9')
                continue;

            if (text[i] == '.')
            {
                // Already a fullstop
                if (std::count(strString.begin(), strString.end(), '.') > 0)
                    return false;

                continue;
            }

            return false;
        }

        return true;
    }
} // namespace SparkyStudios::UI::Pixel