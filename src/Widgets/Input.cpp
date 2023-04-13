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

#include <SparkyStudios/UI/Pixel/Core/Animation/Animation.h>
#include <SparkyStudios/UI/Pixel/Core/Application.h>
#include <SparkyStudios/UI/Pixel/Core/Clipboard.h>

#include <SparkyStudios/UI/Pixel/Widgets/Canvas.h>
#include <SparkyStudios/UI/Pixel/Widgets/Input.h>

namespace SparkyStudios::UI::Pixel
{
    static constexpr PiInt32 kNoMaxLength = -1;

#if PI_ENABLE_ANIMATION
    class CaretColorAnimation : public Animation
    {
    public:
        CaretColorAnimation()
            : Animation(0.5, 0.0, TransitionFunction::Linear, true)
        {}

        void OnStart() override
        {
            _caretVisible = !_caretVisible;
        }

        void Run(PiTime percent) override
        {}

        void OnFinish() override
        {
            pi_cast<Input*>(m_widget)->SetCaretVisible(_caretVisible);
        }

    private:
        bool _caretVisible;
    };
#endif // PI_ENABLE_ANIMATION

    const char* const Input::ReturnKeyPressedEvent = "Input::Events::ReturnKeyPressed";
    const char* const Input::TextChangedEvent = "Input::Events::TextChanged";

    PI_WIDGET_CONSTRUCTOR(Input)
    {
        SetSize(200, 24);
        SetMouseInputEnabled(true);
        SetKeyboardInputEnabled(true);
        SetAlignment(Alignment::Left | Alignment::CenterV);
        SetPadding(Padding(4, 2));
        m_cursorPos = 0;
        m_cursorEnd = 0;
        m_cursorLine = 0;
        m_editable = true;
        m_selectAll = false;
        m_maxTextLength = kNoMaxLength;
        _caretVisible = true;
        m_forceCaretVisible = false;
        SetTextColor(GetSkin()->GetSkinData().Input.textColorNormal); // TODO: From Skin
        // SetTabable(true);
        AddAccelerator("Ctrl + C", &Input::OnCopy);
        AddAccelerator("Ctrl + X", &Input::OnCut);
        AddAccelerator("Ctrl + V", &Input::OnPaste);
        AddAccelerator("Ctrl + A", &Input::OnSelectAll);
        Animation::Add(this, new CaretColorAnimation());
    }

    void Input::SetCaretVisible(bool visible)
    {
        _caretVisible = visible;
    }

    bool Input::IsCaretVisible() const
    {
        return _caretVisible;
    }

    void Input::SetMaxLength(PiInt32 maxLength)
    {
        m_maxTextLength = maxLength;
    }

    PiInt32 Input::GetMaxLength() const
    {
        return m_maxTextLength;
    }

    void Input::InsertText(const PiString& text)
    {
        if (!m_editable)
            return;

        if (HasSelection())
            EraseSelection();

        if (m_cursorPos > TextLength())
            m_cursorPos = TextLength();

        if (!IsTextAllowed(text, m_cursorPos))
            return;

        auto insertSize = static_cast<PiInt32>(text.size());
        if (m_maxTextLength > kNoMaxLength && TextLength() + insertSize > m_maxTextLength)
        {
            insertSize = m_maxTextLength - TextLength();

            if (insertSize <= 0)
                return;
        }

        PiString str = GetText();
        str.insert(m_cursorPos, text, 0, insertSize);
        SetText(str);
        m_cursorPos += insertSize;
        m_cursorEnd = m_cursorPos;
        m_cursorLine = 0;
        RefreshCursorBounds();
    }

    void Input::DeleteText(PiInt32 start, PiInt32 length)
    {
        if (!m_editable)
            return;

        PiString str = GetText();
        str.erase(start, length);
        SetText(str);

        if (m_cursorPos > start)
            SetSelectionStart(m_cursorPos - length);

        SetSelectionEnd(m_cursorPos);
    }

    void Input::EraseSelection()
    {
        PiInt32 start = (std::min)(m_cursorPos, m_cursorEnd);
        PiInt32 end = (std::max)(m_cursorPos, m_cursorEnd);
        DeleteText(start, end - start);

        // Move the cursor to the start of the selection,
        // since the end is probably outside of the string now.
        m_cursorPos = start;
        m_cursorEnd = start;
    }

    bool Input::HasSelection() const
    {
        return m_cursorPos != m_cursorEnd;
    }

    PiString Input::GetSelection()
    {
        if (!HasSelection())
            return "";

        PiInt32 start = (std::min)(m_cursorPos, m_cursorEnd);
        PiInt32 end = (std::max)(m_cursorPos, m_cursorEnd);
        const PiString& str = GetText();

        return str.substr(start, end - start);
    }

    void Input::SetSelectionStart(PiInt32 pos)
    {
        if (m_cursorPos == pos)
            return;

        m_cursorPos = pos;
        m_cursorLine = 0;
        RefreshCursorBounds();
    }

    void Input::SetSelectionEnd(PiInt32 pos)
    {
        if (m_cursorEnd == pos)
            return;

        m_cursorEnd = pos;
        RefreshCursorBounds();
    }

    void Input::SetSelectionRange(const Range<PiInt32>& range)
    {
        SetSelectionRange(range.start, range.end);
    }

    void Input::SetSelectionRange(PiInt32 start, PiInt32 end)
    {
        if (m_cursorPos == start && m_cursorEnd == end)
            return;

        m_cursorPos = start;
        m_cursorEnd = end;

        RefreshCursorBounds();
    }

    void Input::SetCaretPosition(PiInt32 position)
    {
        m_cursorPos = position;
        m_cursorEnd = position;
        RefreshCursorBounds();
    }

    void Input::MoveCaretToStart()
    {
        SetCaretPosition(0);
    }

    void Input::MoveCaretToEnd()
    {
        SetCaretPosition(TextLength());
    }

    void Input::EnsureCaretVisible()
    {
        if (m_text->Width() < Width())
        {
            m_text->SetPosition(m_align, 0, 0);
        }
        else
        {
            const PiInt32 caretPos = m_text->GetCharacterPosition(m_cursorPos).x;
            const PiInt32 realCaretPos = caretPos + m_text->X();
            const PiInt32 slidingZone = m_text->GetFont().size + 1; // Width()*0.1f

            // If the carat is already in a semi-good position, leave it.
            if (realCaretPos >= slidingZone && realCaretPos <= Width() - slidingZone)
                return;

            PiInt32 x = 0;

            if (realCaretPos > Width() - slidingZone)
                x = Width() - caretPos - slidingZone;

            if (realCaretPos < slidingZone)
                x = -caretPos + slidingZone;

            // Don't show too much whitespace to the right
            if (x + m_text->Width() < Width() - GetPadding().right)
                x = -m_text->Width() + (Width() - GetPadding().right);

            // Or the left
            if (x > GetPadding().left)
                x = GetPadding().left;

            PiInt32 y = 0;

            if (m_align & Alignment::Top)
                y = GetPadding().top;

            if (m_align & Alignment::Bottom)
                y = Height() - m_text->Height() - GetPadding().bottom;

            if (m_align & Alignment::CenterV)
                y = (Height() - m_text->Height()) / 2;

            m_text->SetPosition(x, y);
        }
    }

    void Input::RefreshCursorBounds()
    {
        m_caretColor = GetSkin()->GetSkinData().Input.caretColor;
        EnsureCaretVisible();

        const Rect pB = GetCharacterPosition(m_cursorEnd);
        const Rect pA = GetCharacterPosition(m_cursorPos);

        m_rectSelectionBounds.x = (std::min)(pA.x, pB.x);
        m_rectSelectionBounds.y = m_text->Y() - 1;
        m_rectSelectionBounds.w = (std::max)(pA.x, pB.x) - m_rectSelectionBounds.x;
        m_rectSelectionBounds.h = m_text->Height() + 2;
        m_rectCaretBounds.x = pA.x;
        m_rectCaretBounds.y = pA.y;
        m_rectCaretBounds.w = 1;
        m_rectCaretBounds.h = pA.h;

        Redraw();
    }

    void Input::OnPaste(EventInfo)
    {
        InsertText(Clipboard::GetText());
    }

    void Input::OnCopy(EventInfo)
    {
        if (!HasSelection())
            return;

        Clipboard::SetText(GetSelection());
    }

    void Input::OnCut(EventInfo)
    {
        if (!HasSelection())
            return;

        Clipboard::SetText(GetSelection());
        EraseSelection();
    }

    void Input::OnSelectAll(EventInfo)
    {
        m_cursorEnd = 0;
        m_cursorPos = TextLength();
        m_cursorLine = 0;
        RefreshCursorBounds();
    }

    void Input::OnEnter()
    {
        On(ReturnKeyPressedEvent)->Call(this);
    }

    bool Input::OnCharacter(char chr)
    {
        if (chr == '\t')
            return false;

        InsertText(PiString{ chr });
        return true;
    }

    bool Input::OnKey(Key key, KeyPressMode mode)
    {
        switch (key)
        {
        case Key::Return:
            if (mode == KeyPressMode::Pressed)
                return true;

            // Try to move to the next control, as if tab had been pressed
            OnKey(Key::Tab, KeyPressMode::Pressed);

            // If we still have focus, unfocus it.
            if (IsFocused())
                UnFocus();

            OnEnter();
            return true;

        case Key::Backspace:
            if (mode == KeyPressMode::Released)
            {
                m_forceCaretVisible = false;
                return true;
            }

            m_forceCaretVisible = true;

            if (HasSelection())
            {
                EraseSelection();
                return true;
            }

            if (m_cursorPos == 0)
                return true;

            DeleteText(m_cursorPos - 1, 1);
            return true;

        case Key::Delete:
            if (mode == KeyPressMode::Released)
            {
                m_forceCaretVisible = false;
                return true;
            }

            m_forceCaretVisible = true;

            if (HasSelection())
            {
                EraseSelection();
                return true;
            }

            if (m_cursorPos >= TextLength())
                return true;

            DeleteText(m_cursorPos, 1);
            return true;

        case Key::Left:
            if (mode == KeyPressMode::Released)
            {
                m_forceCaretVisible = false;
                return true;
            }

            m_forceCaretVisible = true;

            if (m_cursorPos > 0)
                m_cursorPos--;

            if (!Canvas::Input::IsShiftDown())
                m_cursorEnd = m_cursorPos;

            RefreshCursorBounds();
            return true;

        case Key::Right:
            if (mode == KeyPressMode::Released)
            {
                m_forceCaretVisible = false;
                return true;
            }

            m_forceCaretVisible = true;

            if (m_cursorPos < TextLength())
                m_cursorPos++;

            if (!Canvas::Input::IsShiftDown())
                m_cursorEnd = m_cursorPos;

            RefreshCursorBounds();
            return true;

        case Key::Home:
            if (mode == KeyPressMode::Released)
            {
                m_forceCaretVisible = false;
                return true;
            }

            m_forceCaretVisible = true;

            m_cursorPos = 0;

            if (!Canvas::Input::IsShiftDown())
                m_cursorEnd = m_cursorPos;

            RefreshCursorBounds();
            return true;

        case Key::End:
            if (mode == KeyPressMode::Released)
            {
                m_forceCaretVisible = false;
                return true;
            }

            m_forceCaretVisible = true;

            m_cursorPos = TextLength();

            if (!Canvas::Input::IsShiftDown())
                m_cursorEnd = m_cursorPos;

            RefreshCursorBounds();
            return true;

        default:
            return ParentClass::OnKey(key, mode);
        }
    }

    void Input::OnMouseEnter()
    {
        ParentClass::OnMouseEnter();
        piApp->GetMainWindow()->ApplyCursorStyle(CursorStyle::Edit);
    }

    void Input::OnMouseLeave()
    {
        piApp->GetMainWindow()->ApplyDefaultCursorStyle();
        ParentClass::OnMouseLeave();
    }

    void Input::OnTextChanged()
    {
        if (m_cursorPos > TextLength())
            m_cursorPos = TextLength();

        if (m_cursorEnd > TextLength())
            m_cursorEnd = TextLength();

        On(TextChangedEvent)->Call(this);
    }

    void Input::Render(Skin* skin)
    {
        BaseRenderer* renderer = skin->GetRenderer();
        const Skin::Data& skinData = skin->GetSkinData();

        Rect rect = RenderBounds();

        const bool drawFocused = IsFocused();
        const bool drawHovered = IsHovered();

        // Background
        if (m_bDrawBackground)
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
                skin->GetRenderer()->SetDrawColor(skin->GetSkinData().Input.selectionColor);
                skin->GetRenderer()->DrawFilledRect(m_rectSelectionBounds, Size());
            }
        }

        // Caret
        {
            skin->GetRenderer()->SetDrawColor(m_forceCaretVisible || _caretVisible ? m_caretColor : Colors::Transparent);
            skin->GetRenderer()->DrawFilledRect(m_rectCaretBounds, Size());
        }
    }

    void Input::RenderFocus(Skin* skin)
    {}

    void Input::Layout(Skin* skin)
    {
        ParentClass::Layout(skin);
        RefreshCursorBounds();
    }

    void Input::PostLayout(Skin* skin)
    {}
} // namespace SparkyStudios::UI::Pixel
