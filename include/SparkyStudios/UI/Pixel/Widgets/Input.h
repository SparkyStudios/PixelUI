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

#ifndef PIXEL_UI_WIDGET_INPUT_H
#define PIXEL_UI_WIDGET_INPUT_H

#include <SparkyStudios/UI/Pixel/Widgets/Label.h>

namespace SparkyStudios::UI::Pixel
{
    class PI_EXPORT Input : public Label
    {
    public:
        /**
         * @brief The Return key pressed event.
         */
        static const char* const ReturnKeyPressedEvent;
        static const char* const TextChangedEvent;

        PI_WIDGET(Input, Label);

        /**
         * @brief Sets the visible state of the caret.
         */
        void SetCaretVisible(bool visible);

        /**
         * @brief Checks whether the caret is visible or not.
         *
         * @return The visible state of the caret.
         */
        [[nodiscard]] bool IsCaretVisible() const;

        /**
         * @brief Set the max text length of the text field.
         *
         * @param maxLength The max text length.
         */
        void SetMaxLength(PiInt32 maxLength);

        /**
         * @brief Get the max text length of the text field.
         *
         * @return The max text length.
         */
        [[nodiscard]] PiInt32 GetMaxLength() const;

        /**
         * @brief Inserts a text at the end of the Input.
         *
         * @param text The text to insert.
         */
        virtual void InsertText(const PiString& text);

        /**
         * @brief Deletes the text in the given range.
         *
         * @param start The position where to start to delete the text.
         * @param length The number of characters to delete.
         */
        virtual void DeleteText(PiInt32 start, PiInt32 length);

        /**
         * @brief Erases the current selected text.
         */
        virtual void EraseSelection();

        /**
         * @brief Checks if there is an active text selection.
         */
        [[nodiscard]] virtual bool HasSelection() const;

        /**
         * @brief Get the currently selected text.
         *
         * @return The selected text.
         */
        virtual PiString GetSelection();

        /**
         * @brief Sets the start position of the selection.
         *
         * @param pos The position of the start of the selection.
         */
        virtual void SetSelectionStart(PiInt32 pos);

        /**
         * @brief Sets the end position of the selection.
         */
        virtual void SetSelectionEnd(PiInt32 pos);

        /**
         * @brief Sets the selection range.
         *
         * @param range The selection range.
         */
        virtual void SetSelectionRange(const Range<PiInt32>& range);

        /**
         * @brief Sets the selection range.
         *
         * @param start The start position of the selection.
         * @param end The end position of the selection.
         */
        virtual void SetSelectionRange(PiInt32 start, PiInt32 end);

        /**
         * @brief Sets the position of the caret.
         */
        virtual void SetCaretPosition(PiInt32 position);

        /**
         * @brief Moves the caret to the start of the text.
         */
        virtual void MoveCaretToStart();

        /**
         * @brief Moves the caret to the end of the text.
         */
        virtual void MoveCaretToEnd();

    protected:
        virtual void EnsureCaretVisible();

        virtual void RefreshCursorBounds();

        /**
         * @brief Event triggered when a text is pasted in the field.
         */
        virtual void OnPaste(EventInfo info);

        /**
         * @brief Event triggered when a text is copied in the field.
         */
        virtual void OnCopy(EventInfo info);

        /**
         * @brief Event triggered when a text is cuted in the field.
         */
        virtual void OnCut(EventInfo info);

        /**
         * @brief Event triggered when all the text is selected in the field.
         */
        virtual void OnSelectAll(EventInfo info);

        /**
         * @brief Event triggered when the Enter key is pressed.
         */
        virtual void OnEnter();

        bool OnCharacter(char chr) override;

        bool OnKey(Key key, KeyPressMode mode) override;

        void OnMouseEnter() override;

        void OnMouseLeave() override;

        void OnTextChanged() override;

        void Render(Skin* skin) override;

        void RenderFocus(Skin* skin) override;

        void Layout(Skin* skin) override;

        void PostLayout(Skin* skin) override;

        virtual bool IsTextAllowed(const PiString& /*str*/, int /*iPos*/)
        {
            return true;
        }

        bool m_editable;
        bool m_selectAll;

        PiInt32 m_cursorPos;
        PiInt32 m_cursorEnd;
        PiInt32 m_cursorLine;

        PiInt32 m_maxTextLength;

        Rect m_rectSelectionBounds;
        Rect m_rectCaretBounds;

        PiReal32 m_nextCaretColorChange;
        Color m_caretColor;
        bool m_forceCaretVisible;

    private:
        bool _caretVisible;
    };
} // namespace SparkyStudios::UI::Pixel

#endif // PIXEL_UI_WIDGET_INPUT_H
