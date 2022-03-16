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

#include <SparkyStudios/UI/Pixel/Core/Renderer/Skin.h>

namespace SparkyStudios::UI::Pixel
{
    //const Skin::Data Skin::Data::Default = {
    //    "resources",
    //    // Canvas
    //    {
    //        Padding(0), // Padding
    //        Color(41, 50, 68, 255), // Background Color
    //        {
    //            "OpenSans",
    //            12,
    //            Font::Weight::Regular,
    //            Font::Style::Normal,
    //        }, // Default Font
    //    },
    //    // Tooltip
    //    {
    //        Color(118, 118, 118, 255), // Border Color
    //        Size(0, 0), // Border Radius
    //        1, // Border Width
    //        Padding(8, 4), // Padding
    //        Color(255, 255, 255, 255), // Background Color
    //        Color(87, 87, 87, 255), // Text Color
    //        10, // Text Size
    //    },
    //    // Button
    //    {
    //        // Borders
    //        Color(192, 192, 192, 255), // Normal Border Color
    //        Color(84, 91, 105, 255), // Disabled Border Color
    //        Color(51, 153, 255, 255), // Hovered Border Color
    //        Color(51, 153, 255, 255), // Pressed Border Color
    //        Size(0, 0), // Border Radius
    //        1, // Border Width

    //        // Padding
    //        Padding(4),

    //        // Background
    //        Color(57, 68, 85, 255), // Normal Background Color
    //        Color(49, 59, 76, 255), // Disabled Background Color
    //        Color(65, 65, 65, 255), // Hovered Background Color
    //        Color(51, 153, 255, 255), // Pressed Background Color

    //        // Text
    //        Color(255, 255, 255, 255), // Normal Text Color
    //        Color(84, 91, 105, 255), // Disabled Text Color
    //        Color(255, 255, 255, 255), // Hovered Text Color
    //        Color(255, 255, 255, 255), // Pressed Text Color
    //        12, // Text Size
    //    },
    //    // CheckBox
    //    {
    //        // Borders
    //        Color(224, 224, 224, 255), // Normal Border Color
    //        Color(126, 131, 140, 255), // Disabled Border Color
    //        Color(51, 153, 255, 255), // Hovered Border Color
    //        Color(51, 153, 255, 255), // Pressed Border Color
    //        Size(0, 0), // Border Radius
    //        1, // Border Width

    //        Padding(4), // Padding
    //        Size(16, 16), // Size

    //        // Background
    //        Color(12, 21, 36, 255), // Normal Background Color
    //        Color(20, 29, 46, 255), // Disabled Background Color
    //        Color(65, 65, 65, 255), // Hovered Background Color
    //        Color(51, 153, 255, 255), // Pressed Background Color

    //        // Text
    //        Color(255, 255, 255, 255), // Normal Text Color
    //        Color(84, 91, 105, 255), // Disabled Text Color
    //        Color(255, 255, 255, 255), // Hovered Text Color
    //        Color(255, 255, 255, 255), // Pressed Text Color
    //        12, // Text Size
    //    },
    //    // Label
    //    {
    //        Color(255, 255, 255, 255), // Text Color
    //    },
    //    // ProgressBar
    //    {
    //        // Borders
    //        Color(0, 0, 0, 0), // Normal Border Color
    //        Color(0, 0, 0, 0), // Disabled Border Color
    //        Size(0, 0), // Border Radius
    //        0, // Border Width

    //        // Shape
    //        Size(24, 24), // Size
    //        Padding(4), // Padding

    //        // Background
    //        Color(19, 23, 31, 255), // Normal Background Color
    //        Color(20, 29, 46, 255), // Disabled Background Color

    //        // Progress
    //        Color(100, 149, 237, 255), // Normal Progress Color
    //        Color(49, 59, 76, 125), // Disabled Progress Color

    //        // Text
    //        Color(255, 255, 255, 255), // Normal Text Color
    //        Color(84, 91, 105, 255), // Disabled Text Color
    //        12,
    //    },
    //    // GroupBox
    //    {
    //        // Borders
    //        Color(129, 147, 181, 255), // Border Color
    //        1, // Border Width

    //        // Text
    //        Color(255, 255, 255, 255), // Normal Text Color
    //        14,

    //        // Padding
    //        Padding(8, 20, 8, 8), // Padding
    //        16, // Title Margin
    //    },
    //    // StatusBar
    //    {
    //        // Background
    //        Color(100, 149, 237, 255), // Normal Progress Color
    //        Color(49, 59, 76, 125), // Disabled Progress Color

    //        // Shape
    //        Padding(8, 4), // Padding
    //        24, // Height
    //    },
    //    // ScrollContainer
    //    {
    //        // ScrollBar
    //        Color(14, 22, 36, 255), // Normal Scrollbar Color
    //        Color(19, 23, 31, 255), // Disabled Scrollbar Color
    //        Color(14, 22, 36, 255), // Hovered Scrollbar Color
    //        Color(14, 22, 36, 255), // Pressed Scrollbar Color
    //        Size(0, 0), // Scrollbar Radius

    //        // Scroll
    //        Color(76, 76, 76, 255), // Normal Scroll Color
    //        Color(76, 76, 76, 125), // Disabled Scroll Color
    //        Color(166, 166, 166, 255), // Hovered Scroll Color
    //        Color(76, 76, 76, 255), // Pressed Scroll Color
    //        Size(0, 0), // Scroll Radius

    //        // Buttons
    //        Color(0, 0, 0, 0), // Normal Button Color
    //        Color(0, 0, 0, 0), // Disabled Button Color
    //        Color(218, 218, 218, 255), // Hovered Button Color
    //        Color(0, 0, 0, 0), // Pressed Button Color

    //        // Arrows
    //        Color(117, 133, 165, 255), // Normal Arrow Color
    //        Color(255, 255, 255, 125), // Disabled Arrow Color
    //        Color(24, 24, 24, 255), // Hovered Arrow Color
    //        Color(255, 255, 255, 255), // Pressed Pressed Color

    //        // Background
    //        Color(14, 22, 36, 255), // Background Color

    //        // Shape
    //        Size(16, 16), // Size
    //        Padding(4, 4), // Padding
    //    },
    //    // MenuStrip
    //    {
    //        // Background
    //        Color(7, 71, 166, 255), // Normal Background Color
    //        Color(7, 71, 166, 255), // Disabled Background Color
    //        Color(7, 71, 166, 255), // Hovered Background Color

    //        // Items
    //        Color(0, 0, 0, 0), // Normal Item Background Color
    //        Color(0, 0, 0, 0), // Disabled Item Background Color
    //        Color(5, 53, 124, 255), // Hovered Item Background Color
    //        Color(5, 53, 124, 255), // Pressed Item Background Color
    //        Color(255, 255, 255, 255), // Item Text Color
    //        Color(255, 255, 255, 255), // Separator Color
    //    },
    //    // Menu
    //    {
    //        // Background
    //        Color(43, 43, 43, 255), // Normal Background Color
    //        Color(43, 43, 43, 255), // Disabled Background Color
    //        Color(43, 43, 43, 255), // Hovered Background Color

    //        // Shape
    //        Size(0, 0), // Radius

    //        // Items
    //        Color(0, 0, 0, 0), // Normal Item Background Color
    //        Color(0, 0, 0, 0), // Disabled Item Background Color
    //        Color(65, 65, 65, 255), // Hovered Item Background Color
    //        Color(65, 65, 65, 255), // Pressed Item Background Color
    //        Color(255, 255, 255, 255), // Item Text Color
    //        Color(255, 255, 255, 255), // Separator Color
    //        Color(255, 255, 255, 255), // Arrow Color
    //        Color(255, 255, 255, 125), // Accelerator Color

    //        // Shadow
    //        Color(0, 0, 0, 75), // Shadow Background Color
    //        Point(2, 2), // Shadow Offset
    //        Size(0, 0), // Shadow Radius
    //    },
    //    // ComboBox
    //    {
    //        // Borders
    //        Color(192, 192, 192, 255), // Normal Border Color
    //        Color(84, 91, 105, 255), // Disabled Border Color
    //        Color(51, 153, 255, 255), // Hovered Border Color
    //        Color(51, 153, 255, 255), // Pressed Border Color
    //        Size(0, 0), // Border Radius
    //        1, // Border Width

    //        // Padding
    //        Padding(8),

    //        // Background
    //        Color(12, 21, 36, 255), // Normal Background Color
    //        Color(25, 34, 50, 255), // Disabled Background Color
    //        Color(65, 65, 65, 255), // Hovered Background Color
    //        Color(51, 153, 255, 255), // Pressed Background Color

    //        // Text
    //        Color(255, 255, 255, 255), // Normal Text Color
    //        Color(84, 91, 105, 255), // Disabled Text Color
    //        Color(255, 255, 255, 255), // Hovered Text Color
    //        Color(255, 255, 255, 255), // Pressed Text Color
    //        12, // Text Size

    //        // Arrow
    //        Color(255, 255, 255, 255), // Normal Arrow Color
    //        Color(255, 255, 255, 125), // Disabled Arrow Color
    //        Color(255, 255, 255, 255), // Hovered Arrow Color
    //        Color(255, 255, 255, 255), // Pressed Pressed Color
    //        4, // Arrow Size
    //    },
    //};

    Skin::Skin(const Skin::Data& skinData, BaseRenderer* renderer)
        : m_defaultFont()
        , m_renderer(renderer)
        , m_colorData(skinData)
    {}

    Skin::~Skin()
    {
        ReleaseFont(&m_defaultFont);
    }

    void Skin::ReleaseFont(Font* font)
    {
        if (font != nullptr && m_renderer != nullptr)
        {
            m_renderer->FreeFont(*font);
        }
    }

    void Skin::SetSkinData(const Data& data)
    {
        m_colorData = data;
    }

    const Skin::Data& Skin::GetSkinData() const
    {
        return m_colorData;
    }

    void Skin::SetRenderer(BaseRenderer* renderer)
    {
        m_renderer = renderer;
    }

    BaseRenderer* Skin::GetRenderer() const
    {
        return m_renderer;
    }

    void Skin::SetDefaultFont(const Font& font)
    {
        m_defaultFont = font;
    }

    const Font& Skin::GetDefaultFont() const
    {
        return m_defaultFont;
    }
} // namespace SparkyStudios::UI::Pixel
