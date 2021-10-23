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
    const Skin::ColorData Skin::ColorData::Default = {
        Color(80, 80, 80, 255), // Border
        Color(144, 144, 144, 255), // WidgetOutlineLight
        Color(210, 210, 210, 255), // WidgetOutlineLighter
        Color(248, 248, 248, 255), // Background
        Color(235, 235, 235, 255), // BackgroundDark
        Color(248, 248, 248, 255), // Widget
        Color(51, 153, 255, 255), // WidgetBorderHighlight
        Color(180, 180, 180, 255), // WidgetDarker
        Color(112, 112, 112, 255), // WidgetOutlineNormal
        Color(255, 255, 255, 255), // WidgetBright
        Color(214, 214, 214, 255), //  WidgetDark
        Color(192, 221, 252, 255), //  HighlightBackground
        Color(51, 153, 255, 255), //  HighlightBorder
        Color(255, 255, 225, 255), // TooltipBackground
        Color(0, 0, 0, 255), //  TooltipBorder
        Color(25, 25, 25, 150), //  Modal
    };

    Skin::Skin(Skin::Mode mode, BaseRenderer* renderer)
        : m_defaultFont()
        , m_skinMode(mode)
        , m_renderer(renderer)
        , m_colorData(Skin::ColorData::Default)
    {
        m_defaultFont.facename = "Arial";
        m_defaultFont.size = 10.0f;
    }

    Skin::~Skin()
    {
        ReleaseFont(&m_defaultFont);
    }

    void Skin::SetMode(Mode mode)
    {
        m_skinMode = mode;
    }

    void Skin::ReleaseFont(Font* font)
    {
        if (font != nullptr && m_renderer != nullptr)
        {
            m_renderer->FreeFont(*font);
        }
    }

    void Skin::SetColorData(const ColorData& data)
    {
        m_colorData = data;
    }

    const Skin::ColorData& Skin::GetColorData() const
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