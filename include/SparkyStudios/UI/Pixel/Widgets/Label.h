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

#ifndef PIXEL_UI_WIDGET_LABEL_H
#define PIXEL_UI_WIDGET_LABEL_H

#include <SparkyStudios/UI/Pixel/Widgets/Widget.h>

namespace SparkyStudios::UI::Pixel
{
    /**
     * @brief Internal widget implementation to display text.
     *
     * Don't use it directly. If you need to display a text, use
     * the Label widget instead.
     */
    class PI_EXPORT Text : public Widget
    {
        friend class Label;

    private:
        typedef std::list<Text*> Lines;

        PI_WIDGET(Text, Widget);
        virtual ~Text();

        /**
         * @brief Set the font used to draw the text.
         *
         * @param font The font.
         */
        void SetFont(const Font& font);

        /**
         * @brief Get the font used to draw the text.
         *
         * @return The font.
         */
        const Font& GetFont() const;

        /**
         * @brief Set the text to display.
         *
         * @param text The text to display.
         */
        void SetText(const PiString& text);

        /**
         * @brief Get the displayed text.
         *
         * @return The displayed text.
         */
        const PiString& GetText() const;

        /**
         * @brief Recalculates bounds based on the text and the font.
         */
        void RefreshSize();

        /**
         * @brief Get the position of the character at the given index in the text.
         *
         * @param index The character index.
         *
         * @return The character bounds.
         */
        Rect GetCharacterPosition(PiUInt32 index);

        /**
         * @brief Get the closest character index at the given position.
         *
         * @param p The position of the character.
         *
         * @return The character index.
         */
        PiInt32 GetClosestCharacter(Point p);

        /**
         * @brief Returns the encapsulating box of the given line.
         *
         * @remark This function is necessary to implement multiline selection.
         */
        virtual Rect GetLineBox(PiInt32 i);

        /**
         * @brief Gets the length of the rendered text.
         *
         * @return The text length.
         */
        PiInt32 Length() const;

        /**
         * @brief Sets the text color.
         *
         * @param col The text color to apply.
         */
        virtual void SetTextColor(const Color& col);

        /**
         * @brief Sets the overriden text color.
         *
         * This can be used when the text color should not be permanently
         * changed (ie. changing the color of the text due to an event, and
         * revert it back on another event)
         *
         * @param col The overriden color. Set it as transparent to use the
         * default color instead.
         */
        virtual void SetTextColorOverride(const Color& col);

        /**
         * @brief Gets the current text color.
         *
         * @return The text color.
         */
        const Color& TextColor() const;

        /**
         * @brief Checks if the text has changed.
         *
         * @return true if the text has changed, false otherwise.
         */
        virtual void TextChanged();

        /**
         * @brief Checks if the text is wrapped on overflow.
         *
         * @return true if the text is wrapped, false otherwise.
         */
        virtual bool Wrap();

        /**
         * @brief Sets if the text should be wrapped on overflow.
         *
         * @param wrap Whether the text should wrap.
         */
        virtual void SetWrap(bool wrap);

        /**
         * @brief Returns a Text component with the text at the given line.
         * 
         * @param i The line number.
         * 
         * @return A Text widget with the text at the given line of this one.
         */
        virtual Text* GetLine(PiInt32 i);

        /**
         * @brief Gets the line number on which is rendered the character with the given index.
         * 
         * @param i The character index.
         * 
         * @return The line number of the character.
         */
        virtual PiInt32 GetLineFromChar(PiInt32 i);

        /**
         * @brief Gets the index of the first character of the given line number.
         * 
         * @param i The line number.
         * 
         * @return The index of the first character of the line.
         */
        virtual PiInt32 GetStartCharFromLine(PiInt32 i);

        /**
         * @brief Gets the index of the last character of the given line number.
         * 
         * @param i The line number.
         * 
         * @return The index of the last character of the line.
         */
        virtual PiInt32 GetEndCharFromLine(PiInt32 i);

        /**
         * @brief Gets the character index in the line which stores the character at the
         * given index in the whole text.
         * 
         * @param i The character index in the text.
         * 
         * @return The character index in the line.
         */
        virtual PiInt32 GetCharPosOnLine(PiInt32 i);

        /**
         * @brief Gets the total number of lines.
         * 
         * @return The number of lines.
         */
        virtual PiUInt32 NumLines();

        /**
         * @brief Splits the given string into words (splitting using spaces and line breaks)
         * 
         * @param s The string to split.
         * @param elems The vector in which store the words.
         */
        virtual void SplitWords(const PiString& s, std::vector<PiString>& elems);

        /**
         * @brief Invalidates and recalculates text lines and widget size.
         */
        virtual void RefreshSizeWrap();

        Widget* GetWidgetAt(PiInt32 x, PiInt32 y, bool onlyIfMouseEnabled) override;

        void Render(Skin* skin) override;
        void Layout(Skin* skin) override;

        void OnScaleChanged() override;

        PiString m_string;
        const Font* m_font;
        Color m_color;
        Color m_colorOverride;

        bool m_wrap;
        bool m_textChanged;

        Lines m_lines;
    };

    /**
     * @brief A widget that displays a text.
     */
    class PI_EXPORT Label : public Widget
    {
    public:
        PI_WIDGET(Label, Widget);

        /**
         * @brief Sets the text to display.
         * 
         * @param text The displayed text.
         * @param doEvents Defines if the TextChanged event should be processed.
         */
        virtual void SetText(const PiString& text, bool doEvents = true);

        /**
         * @brief Gets the displayed text.
         * 
         * @return The displayed text.
         */
        [[nodiscard]] virtual const PiString& GetText() const;

        void PreDelete(Skin* skin) override;

        void Render(Skin* skin) override;

        void PostLayout(Skin* skin) override;

        virtual void SizeToContents();

        virtual void SetAlignment(Alignment alignment);

        virtual Alignment GetAlignment();

        virtual void SetFont(PiString fontName, PiInt32 size, Font::Weight weight);

        virtual void SetFont(const Font& font);

        virtual const Font& GetFont() const;

        void SetFontSize(PiUInt32 size);

        void SetFontWeight(Font::Weight weight);

        void SetFontStyle(Font::Style style);

        virtual void SetTextColor(const Color& color);

        virtual void SetTextColorOverride(const Color& color);

        const Color& GetTextColor() const;

        virtual PiInt32 GetTextWidth() const;

        virtual PiInt32 TextRight() const;

        virtual PiInt32 GetTextHeight() const;

        virtual PiInt32 TextX() const;

        virtual PiInt32 TextY() const;

        virtual PiInt32 TextLength() const;

        Rect GetCharacterPosition(PiInt32 index);

        virtual void SetPadding(const Padding& padding) override;

        [[nodiscard]] Alignment GetAlignment() const;

        PiString GetValue() override;

        void SetValue(const PiString& value) override;

        virtual bool Wrap();

        virtual void SetWrap(bool b);

        void OnBoundsChanged(const Rect& old) override;

    protected:
        virtual void OnTextChanged()
        {}

        Font* m_createdFont;
        Text* m_text;
        Alignment m_align;
        bool m_textDirty;
    };

} // namespace SparkyStudios::UI::Pixel

#endif // PIXEL_UI_WIDGET_LABEL_H
