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

#ifndef PIXEL_UI_BASESKIN_H
#define PIXEL_UI_BASESKIN_H

#include <SparkyStudios/UI/Pixel/Core/Renderer/BaseRenderer.h>

namespace SparkyStudios::UI::Pixel
{
    class Widget;

    /**
     * @brief Application skin.
     *
     * This class holds needed methods to setup an application
     * skin using colors, or a PixelUI skin texture image.
     */
    class PI_EXPORT Skin
    {
    public:
        /**
         * @brief The skin mode. Either colored or textured.
         */
        enum class Mode
        {
            Colored,
            Textured,
        };

        /**
         * @brief The color of the skin. Used when in Colored mode.
         */
        struct ColorData
        {
            static const ColorData Default;

            Color m_Border;
            Color m_WidgetOutlineLight;
            Color m_WidgetOutlineLighter;
            Color m_Background;
            Color m_BackgroundDark;
            Color m_Widget;
            Color m_WidgetBorderHighlight;
            Color m_WidgetDarker;
            Color m_WidgetOutlineNormal;
            Color m_WidgetBright;
            Color m_WidgetDark;
            Color m_HighlightBackground;
            Color m_HighlightBorder;
            Color m_TooltipBackground;
            Color m_TooltipBorder;
            Color m_Modal;
        };

        /**
         * @brief Constructor
         *
         * @param renderer The renderer to use when drawing the skin.
         */
        Skin(Mode mode = Mode::Colored, BaseRenderer* renderer = nullptr);

        ~Skin();

        /**
         * @brief Releases a loaded font from the renderer.
         *
         * @param font The font to release.
         */
        virtual void ReleaseFont(Font* font);

        /**
         * @brief Sets the color of the skin.
         *
         * @param data The skin colors.
         */
        virtual void SetColorData(const ColorData& data);

        /**
         * @brief Get the color of the skin.
         *
         * @return The color data.
         */
        virtual const ColorData& GetColorData() const;

        /**
         * @brief Sets the renderer to use for drawing the skin.
         *
         * @param renderer The renderer.
         */
        virtual void SetRenderer(BaseRenderer* renderer);

        /**
         * @brief Gets the renderer used by the skin.
         *
         * @return The renderer.
         */
        virtual BaseRenderer* GetRenderer() const;

        /**
         * @brief Set the default font to use for the skin.
         *
         * @param font The default font.
         */
        virtual void SetDefaultFont(const Font& font);

        /**
         * @brief Get the default font of the skin.
         *
         * @return The default font.
         */
        virtual const Font& GetDefaultFont() const;

    protected:
        Font m_defaultFont;
        BaseRenderer* m_renderer;
        Mode m_skinMode;
        ColorData m_colorData;
    };
} // namespace SparkyStudios::UI::Pixel

#endif // PIXEL_UI_BASESKIN_H
