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
        struct PI_EXPORT Data
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

            // ProgressBar
            struct
            {
                // Borders
                Color borderColorNormal;
                Color borderColorDisabled;
                Size borderRadius;
                PiUInt32 borderWidth;

                // Shape
                Size size;
                Padding padding;

                // Background
                Color backgroundColorNormal;
                Color backgroundColorDisabled;

                // Progress
                Color progressColorNormal;
                Color progressColorDisabled;

                // Text
                Color textColorNormal;
                Color textColorDisabled;
                PiUInt32 textSize;
            } ProgressBar;

            // GroupBox
            struct
            {
                // Borders
                Color borderColor;
                PiUInt32 borderWidth;

                // Text
                Color textColor;
                PiUInt32 textSize;

                // Padding & Margin
                Padding padding;
                PiUInt32 titleMargin;
            } GroupBox;

            // StatusBar
            struct
            {
                // Background
                Color backgroundColorNormal;
                Color backgroundColorDisabled;

                // Shape
                Padding padding;
                PiUInt32 height;
            } StatusBar;

            // ScrollContainer
            struct
            {
                // ScrollBar
                Color scrollBarBackgroundNormal;
                Color scrollBarBackgroundDisabled;
                Color scrollBarBackgroundHovered;
                Color scrollBarBackgroundPressed;
                Size scrollBarRadius;

                // Scroll
                Color scrollBackgroundNormal;
                Color scrollBackgroundDisabled;
                Color scrollBackgroundHovered;
                Color scrollBackgroundPressed;
                Size scrollRadius;

                // Buttons
                Color buttonBackgroundNormal;
                Color buttonBackgroundDisabled;
                Color buttonBackgroundHovered;
                Color buttonBackgroundPressed;

                // Arrows
                Color arrowColorNormal;
                Color arrowColorDisabled;
                Color arrowColorHovered;
                Color arrowColorPressed;

                // Background
                Color backgroundColor;

                // Shape
                Size scrollSize;
                Padding padding;
            } ScrollContainer;

            // MenuStrip
            struct
            {
                // Background
                Color backgroundColorNormal;
                Color backgroundColorDisabled;
                Color backgroundColorHovered;

                // Items
                Color itemsBackgroundNormal;
                Color itemsBackgroundDisabled;
                Color itemsBackgroundHovered;
                Color itemsBackgroundPressed;
                Color itemsTextColor;
                Color separatorColor;
            } MenuStrip;

            // Menu
            struct
            {
                // Background
                Color backgroundColorNormal;
                Color backgroundColorDisabled;
                Color backgroundColorHovered;

                // Shape
                Size radius;

                // Items
                Color itemsBackgroundNormal;
                Color itemsBackgroundDisabled;
                Color itemsBackgroundHovered;
                Color itemsBackgroundPressed;
                Color itemsTextColor;
                Color separatorColor;
                Color arrowColor;
                Color acceleratorColor;

                // Shadow
                Color shadowColor;
                Point shadowOffset;
                Size shadowRadius;
            } Menu;

            // ComboBox
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

                // Arrow
                Color arrowColorNormal;
                Color arrowColorDisabled;
                Color arrowColorHovered;
                Color arrowColorPressed;
                PiUInt32 arrowSize;
            } ComboBox;

            // Input
            struct
            {
                // Caret
                Color caretColor;

                // Selection
                Color selectionColor;

                // Borders
                Color borderColorNormal;
                Color borderColorDisabled;
                Color borderColorHovered;
                Color borderColorFocused;
                Size borderRadius;
                PiUInt32 borderWidth;

                // Padding
                Padding padding;
                Size size;

                // Background
                Color backgroundColorNormal;
                Color backgroundColorDisabled;
                Color backgroundColorHovered;
                Color backgroundColorFocused;

                // Text
                Color textColorNormal;
                Color textColorDisabled;
                Color textColorHovered;
                Color textColorFocused;
                PiUInt32 textSize;
            } Input;
        };

        /**
         * @brief Constructor
         *
         * @param skinData The skin data.
         * @param renderer The renderer to use when drawing the skin.
         */
        explicit Skin(const Data& skinData, BaseRenderer* renderer = nullptr);

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
        [[nodiscard]] virtual const Data& GetSkinData() const;

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
        [[nodiscard]] virtual BaseRenderer* GetRenderer() const;

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
        [[nodiscard]] virtual const Font& GetDefaultFont() const;

    protected:
        Font m_defaultFont;
        BaseRenderer* m_renderer;
        Data m_colorData;
    };

    inline const Skin::Data Skin::Data::Default = {
        "resources",
        // Canvas
        {
            Padding(0), // Padding
            Color(41, 50, 68, 255), // Background Color
            {
                "OpenSans",
                12,
                Font::Weight::Regular,
                Font::Style::Normal,
            }, // Default Font
        },
        // Tooltip
        {
            Color(118, 118, 118, 255), // Border Color
            Size(0, 0), // Border Radius
            1, // Border Width
            Padding(8, 4), // Padding
            Color(255, 255, 255, 255), // Background Color
            Color(87, 87, 87, 255), // Text Color
            10, // Text Size
        },
        // Button
        {
            // Borders
            Color(192, 192, 192, 255), // Normal Border Color
            Color(84, 91, 105, 255), // Disabled Border Color
            Color(51, 153, 255, 255), // Hovered Border Color
            Color(51, 153, 255, 255), // Pressed Border Color
            Size(0, 0), // Border Radius
            1, // Border Width

            // Padding
            Padding(4),

            // Background
            Color(57, 68, 85, 255), // Normal Background Color
            Color(49, 59, 76, 255), // Disabled Background Color
            Color(65, 65, 65, 255), // Hovered Background Color
            Color(51, 153, 255, 255), // Pressed Background Color

            // Text
            Color(255, 255, 255, 255), // Normal Text Color
            Color(84, 91, 105, 255), // Disabled Text Color
            Color(255, 255, 255, 255), // Hovered Text Color
            Color(255, 255, 255, 255), // Pressed Text Color
            12, // Text Size
        },
        // CheckBox
        {
            // Borders
            Color(224, 224, 224, 255), // Normal Border Color
            Color(126, 131, 140, 255), // Disabled Border Color
            Color(51, 153, 255, 255), // Hovered Border Color
            Color(51, 153, 255, 255), // Pressed Border Color
            Size(0, 0), // Border Radius
            1, // Border Width

            Padding(4), // Padding
            Size(16, 16), // Size

            // Background
            Color(12, 21, 36, 255), // Normal Background Color
            Color(20, 29, 46, 255), // Disabled Background Color
            Color(65, 65, 65, 255), // Hovered Background Color
            Color(51, 153, 255, 255), // Pressed Background Color

            // Text
            Color(255, 255, 255, 255), // Normal Text Color
            Color(84, 91, 105, 255), // Disabled Text Color
            Color(255, 255, 255, 255), // Hovered Text Color
            Color(255, 255, 255, 255), // Pressed Text Color
            12, // Text Size
        },
        // Label
        {
            Color(255, 255, 255, 255), // Text Color
        },
        // ProgressBar
        {
            // Borders
            Color(0, 0, 0, 0), // Normal Border Color
            Color(0, 0, 0, 0), // Disabled Border Color
            Size(0, 0), // Border Radius
            0, // Border Width

            // Shape
            Size(24, 24), // Size
            Padding(4), // Padding

            // Background
            Color(19, 23, 31, 255), // Normal Background Color
            Color(20, 29, 46, 255), // Disabled Background Color

            // Progress
            Color(100, 149, 237, 255), // Normal Progress Color
            Color(49, 59, 76, 125), // Disabled Progress Color

            // Text
            Color(255, 255, 255, 255), // Normal Text Color
            Color(84, 91, 105, 255), // Disabled Text Color
            12,
        },
        // GroupBox
        {
            // Borders
            Color(129, 147, 181, 255), // Border Color
            1, // Border Width

            // Text
            Color(255, 255, 255, 255), // Normal Text Color
            14,

            // Padding
            Padding(8, 20, 8, 8), // Padding
            16, // Title Margin
        },
        // StatusBar
        {
            // Background
            Color(100, 149, 237, 255), // Normal Progress Color
            Color(49, 59, 76, 125), // Disabled Progress Color

            // Shape
            Padding(8, 4), // Padding
            24, // Height
        },
        // ScrollContainer
        {
            // ScrollBar
            Color(14, 22, 36, 255), // Normal Scrollbar Color
            Color(19, 23, 31, 255), // Disabled Scrollbar Color
            Color(14, 22, 36, 255), // Hovered Scrollbar Color
            Color(14, 22, 36, 255), // Pressed Scrollbar Color
            Size(0, 0), // Scrollbar Radius

            // Scroll
            Color(76, 76, 76, 255), // Normal Scroll Color
            Color(76, 76, 76, 125), // Disabled Scroll Color
            Color(166, 166, 166, 255), // Hovered Scroll Color
            Color(76, 76, 76, 255), // Pressed Scroll Color
            Size(0, 0), // Scroll Radius

            // Buttons
            Color(0, 0, 0, 0), // Normal Button Color
            Color(0, 0, 0, 0), // Disabled Button Color
            Color(218, 218, 218, 255), // Hovered Button Color
            Color(0, 0, 0, 0), // Pressed Button Color

            // Arrows
            Color(117, 133, 165, 255), // Normal Arrow Color
            Color(255, 255, 255, 125), // Disabled Arrow Color
            Color(24, 24, 24, 255), // Hovered Arrow Color
            Color(255, 255, 255, 255), // Pressed Arrow Color

            // Background
            Color(14, 22, 36, 255), // Background Color

            // Shape
            Size(16, 16), // Size
            Padding(4, 4), // Padding
        },
        // MenuStrip
        {
            // Background
            Color(7, 71, 166, 255), // Normal Background Color
            Color(7, 71, 166, 255), // Disabled Background Color
            Color(7, 71, 166, 255), // Hovered Background Color

            // Items
            Color(0, 0, 0, 0), // Normal Item Background Color
            Color(0, 0, 0, 0), // Disabled Item Background Color
            Color(5, 53, 124, 255), // Hovered Item Background Color
            Color(5, 53, 124, 255), // Pressed Item Background Color
            Color(255, 255, 255, 255), // Item Text Color
            Color(255, 255, 255, 255), // Separator Color
        },
        // Menu
        {
            // Background
            Color(43, 43, 43, 255), // Normal Background Color
            Color(43, 43, 43, 255), // Disabled Background Color
            Color(43, 43, 43, 255), // Hovered Background Color

            // Shape
            Size(0, 0), // Radius

            // Items
            Color(0, 0, 0, 0), // Normal Item Background Color
            Color(0, 0, 0, 0), // Disabled Item Background Color
            Color(65, 65, 65, 255), // Hovered Item Background Color
            Color(65, 65, 65, 255), // Pressed Item Background Color
            Color(255, 255, 255, 255), // Item Text Color
            Color(255, 255, 255, 255), // Separator Color
            Color(255, 255, 255, 255), // Arrow Color
            Color(255, 255, 255, 125), // Accelerator Color

            // Shadow
            Color(0, 0, 0, 75), // Shadow Background Color
            Point(2, 2), // Shadow Offset
            Size(0, 0), // Shadow Radius
        },
        // ComboBox
        {
            // Borders
            Color(192, 192, 192, 255), // Normal Border Color
            Color(84, 91, 105, 255), // Disabled Border Color
            Color(51, 153, 255, 255), // Hovered Border Color
            Color(51, 153, 255, 255), // Pressed Border Color
            Size(0, 0), // Border Radius
            1, // Border Width

            // Padding
            Padding(8),

            // Background
            Color(12, 21, 36, 255), // Normal Background Color
            Color(25, 34, 50, 255), // Disabled Background Color
            Color(65, 65, 65, 255), // Hovered Background Color
            Color(51, 153, 255, 255), // Pressed Background Color

            // Text
            Color(255, 255, 255, 255), // Normal Text Color
            Color(84, 91, 105, 255), // Disabled Text Color
            Color(255, 255, 255, 255), // Hovered Text Color
            Color(255, 255, 255, 255), // Pressed Text Color
            12, // Text Size

            // Arrow
            Color(255, 255, 255, 255), // Normal Arrow Color
            Color(255, 255, 255, 125), // Disabled Arrow Color
            Color(255, 255, 255, 255), // Hovered Arrow Color
            Color(255, 255, 255, 255), // Pressed Arrow Color
            4, // Arrow Size
        },
        // Input
        {
            // Caret
            Color(255, 255, 255, 255), // Caret Color

            // Selection
            Color(50, 170, 225, 200), // Selection Color

            // Borders
            Color(224, 224, 224, 255), // Normal Border Color
            Color(126, 131, 140, 255), // Disabled Border Color
            Color(51, 153, 255, 255), // Hovered Border Color
            Color(51, 153, 255, 255), // Focused Border Color
            Size(0, 0), // Border Radius
            1, // Border Width

            Padding(4), // Padding
            Size(16, 16), // Size

            // Background
            Color(12, 21, 36, 255), // Normal Background Color
            Color(20, 29, 46, 255), // Disabled Background Color
            Color(12, 21, 36, 255), // Hovered Background Color
            Color(12, 21, 36, 255), // Focused Background Color

            // Text
            Color(255, 255, 255, 255), // Normal Text Color
            Color(84, 91, 105, 255), // Disabled Text Color
            Color(255, 255, 255, 255), // Hovered Text Color
            Color(255, 255, 255, 255), // Focused Text Color
            12, // Text Size
        },
    };
} // namespace SparkyStudios::UI::Pixel

#endif // PIXEL_UI_BASESKIN_H
