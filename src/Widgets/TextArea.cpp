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

#include <SparkyStudios/UI/Pixel/Widgets/Canvas.h>
#include <SparkyStudios/UI/Pixel/Widgets/TextArea.h>

namespace SparkyStudios::UI::Pixel
{
    PI_WIDGET_CONSTRUCTOR(TextArea)
    {
        SetSize(200, 100);
        SetWrap(true);
        SetPadding(Padding(4, 2));
        SetAlignment(Alignment::Left | Alignment::Top);
    }

    PiInt32 TextArea::GetCurrentLine()
    {
        return m_text->GetLineFromChar(m_cursorPos);
    }

    void TextArea::EnsureCaretVisible()
    {
        if (m_text->Height() < Height())
        {
            m_text->SetPosition(m_align, 0, 0);
        }
        else
        {
            const Point offset = Point(GetPadding().left, GetPadding().top);
            const Rect pos = m_text->GetCharacterPosition(m_cursorPos) + offset;

            PiInt32 caretPos = pos.y;
            PiInt32 realCaretPos = caretPos + m_text->Y();

            if (realCaretPos >= GetPadding().top && realCaretPos <= Height() - pos.h - GetPadding().bottom)
                return;

            PiInt32 y = 0;

            // Bottom of carat too low
            if (realCaretPos > Height() - pos.h - GetPadding().bottom)
            {
                // Align bottom
                y = Height() - caretPos - pos.h - GetPadding().bottom;
            }

            // Top of carat too low
            if (realCaretPos < GetPadding().top)
                y = -caretPos + GetPadding().top;

            // Don't show too much whitespace to the bottom
            if (y + m_text->Height() < Height() - GetPadding().bottom)
                y = -m_text->Height() + (Height() - GetPadding().bottom);

            // Or the top
            if (y > GetPadding().top)
                y = GetPadding().top;

            PiInt32 x = 0;
            if (m_align & Alignment::Left)
                x = offset.x;
            if (m_align & Alignment::Right)
                x = Width() - m_text->Width() - GetPadding().right;
            if (m_align & Alignment::CenterH)
                x = (Width() - m_text->Width()) * 0.5f;

            m_text->SetPosition(x, y);
        }
    }

    bool TextArea::OnKey(Key key, KeyPressMode mode)
    {
        switch (key)
        {
        case Key::Return:
            {
                if (mode == KeyPressMode::Pressed)
                {
                    m_forceCaretVisible = true;
                    InsertText("\n");
                }
                else
                {
                    m_forceCaretVisible = false;
                }

                return true;
            }
        case Key::Home:
            {
                if (mode == KeyPressMode::Released)
                {
                    m_forceCaretVisible = false;
                    return true;
                }

                m_forceCaretVisible = true;

                PiInt32 currentLine = GetCurrentLine();
                PiInt32 characterIndex = m_text->GetStartCharFromLine(currentLine);

                m_cursorLine = 0;
                m_cursorPos = characterIndex;

                if (!Canvas::Input::IsShiftDown())
                    m_cursorEnd = m_cursorPos;

                RefreshCursorBounds();
                return true;
            }
        case Key::End:
            {
                if (mode == KeyPressMode::Released)
                {
                    m_forceCaretVisible = false;
                    return true;
                }

                m_forceCaretVisible = true;

                PiInt32 currentLine = GetCurrentLine();
                PiInt32 characterIndex = m_text->GetStartCharFromLine(currentLine);

                m_cursorLine = 0;
                m_cursorPos = characterIndex;

                PiInt32 lastLine = m_text->NumLines() - 1;

                if (currentLine < lastLine && characterIndex > 0)
                    m_cursorPos = characterIndex - 1;
                else
                    m_cursorPos = TextLength();

                if (!Canvas::Input::IsShiftDown())
                    m_cursorEnd = m_cursorPos;

                RefreshCursorBounds();
                return true;
            }
        case Key::Up:
            {
                if (mode == KeyPressMode::Released)
                {
                    m_forceCaretVisible = false;
                    return true;
                }

                m_forceCaretVisible = true;

                m_cursorLine = m_text->GetCharPosOnLine(m_cursorPos);

                PiInt32 line = m_text->GetLineFromChar(m_cursorPos);
                if (line == 0)
                    return true;

                m_cursorPos = m_text->GetStartCharFromLine(line - 1);
                m_cursorPos += Clamp(m_cursorLine, 0, m_text->GetLine(line - 1)->Length() - 1);
                m_cursorPos = Clamp(m_cursorPos, 0, m_text->Length());

                if (!Canvas::Input::IsShiftDown())
                    m_cursorEnd = m_cursorPos;

                RefreshCursorBounds();
                return true;
            }
        case Key::Down:
            {
                if (mode == KeyPressMode::Released)
                {
                    m_forceCaretVisible = false;
                    return true;
                }

                m_forceCaretVisible = true;

                m_cursorLine = m_text->GetCharPosOnLine(m_cursorPos);

                PiInt32 line = m_text->GetLineFromChar(m_cursorPos);
                PiInt32 lastLine = m_text->NumLines() - 1;
                if (line >= lastLine || lastLine < 1)
                    return true;

                m_cursorPos = m_text->GetStartCharFromLine(line + 1);
                if (line + 1 >= lastLine)
                    m_cursorPos += Clamp(m_cursorLine, 0, m_text->GetLine(line + 1)->Length());
                else
                    m_cursorPos += Clamp(m_cursorLine, 0, m_text->GetLine(line + 1)->Length() - 1);
                m_cursorPos = Clamp(m_cursorPos, 0, m_text->Length());

                if (!Canvas::Input::IsShiftDown())
                    m_cursorEnd = m_cursorPos;

                RefreshCursorBounds();
                return true;
            }
        default:
            return ParentClass::OnKey(key, mode);
        }
    }

    void TextArea::Render(Skin* skin)
    {
        BaseRenderer* renderer = skin->GetRenderer();
        const Skin::Data& skinData = skin->GetSkinData();

        Rect rect = RenderBounds();

        const bool drawFocused = IsFocused();
        const bool drawHovered = IsHovered();

        // Background
        if (m_drawBackground)
        {
            const PiUInt32 offset = skinData.Input.borderWidth;

            if (m_disabled)
                renderer->SetDrawColor(skinData.Input.backgroundColorDisabled);
            else if (drawFocused)
                renderer->SetDrawColor(skinData.Input.backgroundColorFocused);
            else if (drawHovered)
                renderer->SetDrawColor(skinData.Input.backgroundColorHovered);
            else
                renderer->SetDrawColor(skinData.Input.backgroundColorNormal);

            renderer->DrawFilledRect(rect + Rect(offset, offset, -offset * 2, -offset * 2), skinData.Input.borderRadius);
        }

        // Border
        if (skinData.Input.borderWidth > 0)
        {
            if (m_disabled)
                renderer->SetDrawColor(skinData.Input.borderColorDisabled);
            else if (drawFocused)
                renderer->SetDrawColor(skinData.Input.borderColorFocused);
            else if (drawHovered)
                renderer->SetDrawColor(skinData.Input.borderColorHovered);
            else
                renderer->SetDrawColor(skinData.Input.borderColorNormal);

            renderer->DrawLinedRect(rect, skinData.Tooltip.borderWidth, skinData.Input.borderRadius);
        }

        if (!drawFocused)
            return;

        // Selection
        {
            if (HasSelection())
            {
                PiUInt32 cursorStartLine = m_text->GetLineFromChar(m_cursorPos);
                PiUInt32 cursorEndLine = m_text->GetLineFromChar(m_cursorEnd);

                if (cursorStartLine > m_text->NumLines() - 1)
                    cursorStartLine = m_text->NumLines() - 1;

                if (cursorEndLine > m_text->NumLines() - 1)
                    cursorEndLine = m_text->NumLines() - 1;

                PiInt32 selectionStartLine = (m_cursorPos < m_cursorEnd) ? cursorStartLine : cursorEndLine;
                PiInt32 selectionEndLine = (m_cursorPos < m_cursorEnd) ? cursorEndLine : cursorStartLine;

                PiInt32 selectionStartPos = (m_cursorPos < m_cursorEnd) ? m_cursorPos : m_cursorEnd;
                PiInt32 selectionEndPos = (m_cursorPos < m_cursorEnd) ? m_cursorEnd : m_cursorPos;

                renderer->SetDrawColor(Color(50, 170, 255, 200));
                m_rectSelectionBounds.h = m_text->GetFont().size + 2;

                for (PiInt32 line = selectionStartLine; line <= selectionEndLine; ++line)
                {
                    Rect box = m_text->GetLineBox(line);
                    box.x += m_text->X();
                    box.y += m_text->Y();

                    if (line == selectionStartLine)
                    {
                        Rect pos = GetCharacterPosition(selectionStartPos);
                        m_rectSelectionBounds.x = pos.x;
                        m_rectSelectionBounds.y = pos.y - 1;
                    }
                    else
                    {
                        m_rectSelectionBounds.x = box.x;
                        m_rectSelectionBounds.y = box.y - 1;
                    }

                    if (line == selectionEndLine)
                    {
                        Rect pos = GetCharacterPosition(selectionEndPos);
                        m_rectSelectionBounds.w = pos.x - m_rectSelectionBounds.x;
                    }
                    else
                    {
                        m_rectSelectionBounds.w = box.x + box.w - m_rectSelectionBounds.x;
                    }
                    if (m_rectSelectionBounds.w < 1)
                        m_rectSelectionBounds.w = 1;

                    renderer->DrawFilledRect(m_rectSelectionBounds, Size(0, 0));
                }
            }
        }

        // Caret
        {
            skin->GetRenderer()->SetDrawColor(m_forceCaretVisible || IsCaretVisible() ? m_caretColor : Colors::Transparent);
            skin->GetRenderer()->DrawFilledRect(m_rectCaretBounds, Size(0, 0));
        }
    }
} // namespace SparkyStudios::UI::Pixel