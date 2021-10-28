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

#include <SparkyStudios/UI/Pixel/Widgets/Base.h>

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
         * @brief The skin data. Defines the template for each core widgets
         * of PixelUI.
         */
        struct Data
        {
            static const Data Default;

            PiString resourcesDir;

            // Canvas
            struct
            {
                // Padding
                Padding padding;

                // Background
                Color backgroundColor;

                // Font
                Font defaultFont;
            } Canvas;

            // Tooltip
            struct
            {
                // Borders
                Color borderColor;
                Size borderRadius;
                PiUInt32 borderWidth;

                // Padding
                Padding padding;

                // Background
                Color backgroundColor;

                // Text
                Color textColor;
                PiUInt32 textSize;
            } Tooltip;

            // Button
            struct
            {
                // Borders
                Color borderColorNormal;
                Color borderColorDisabled;
                Color borderColorHovered;
                Color borderColorPressed;
                Size borderRadius;
                PiUInt32 borderWidth;

                // Padding & Size
                Padding padding;

                // Background
                Color backgroundColorNormal;
                Color backgroundColorDisabled;
                Color backgroundColorHovered;
                Color backgroundColorPressed;

                // Text
                Color textColorNormal;
                Color textColorDisabled;
                Color textColorHovered;
                Color textColorPressed;
                PiUInt32 textSize;
            } Button;

            // CheckBox
            struct
            {
                // Borders
                Color borderColorNormal;
                Color borderColorDisabled;
                Color borderColorHovered;
                Color borderColorPressed;
                Size borderRadius;
                PiUInt32 borderWidth;

                // Padding
                Padding padding;
                Size size;

                // Background
                Color backgroundColorNormal;
                Color backgroundColorDisabled;
                Color backgroundColorHovered;
                Color backgroundColorPressed;

                // Text
                Color textColorNormal;
                Color textColorDisabled;
                Color textColorHovered;
                Color textColorPressed;
                PiUInt32 textSize;
            } CheckBox;

            // Label
            struct
            {
                Color textColor;
            } Label;
        };

        /**
         * @brief Constructor
         *
         * @param skinData The skin data.
         * @param renderer The renderer to use when drawing the skin.
         */
        Skin(const Data& skinData, BaseRenderer* renderer = nullptr);

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
        virtual void SetSkinData(const Data& data);

        /**
         * @brief Get the color of the skin.
         *
         * @return The color data.
         */
        virtual const Data& GetSkinData() const;

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
        Data m_colorData;
    };
} // namespace SparkyStudios::UI::Pixel

#endif // PIXEL_UI_BASESKIN_H
