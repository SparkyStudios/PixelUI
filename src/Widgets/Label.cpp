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

#include <SparkyStudios/UI/Pixel/Core/Log.h>
#include <SparkyStudios/UI/Pixel/Core/Utility.h>
#include <SparkyStudios/UI/Pixel/Widgets/Label.h>

namespace SparkyStudios::UI::Pixel
{
    PI_WIDGET_CONSTRUCTOR(Text)
    {
        m_font = &GetSkin()->GetDefaultFont();
        m_colorOverride = Colors::Transparent;
        m_color = GetSkin()->GetSkinData().Label.textColor;
        SetMouseInputEnabled(false);
        SetWrap(false);
    }

    Text::~Text()
    {
        // NOTE: This font doesn't need to be released
        // Because it's a pointer to another font somewhere.
    }

    void Text::SetFont(const Font& font)
    {
        if (m_font == &font)
            return;

        m_font = &font;
        m_textChanged = true;
        // Change the font of multilines too!
        for (auto&& line : m_lines)
        {
            line->SetFont(*m_font);
        }

        Invalidate();
    }

    const Font& Text::GetFont() const
    {
        return *m_font;
    }

    void Text::SetText(const PiString& str)
    {
        if (m_string == str)
            return;

        m_string = str;
        m_textChanged = true;
        Invalidate();
    }

    const PiString& Text::GetText() const
    {
        return m_string;
    }

    void Text::RefreshSize()
    {
        if (m_wrap)
            return RefreshSizeWrap();

        if (!m_font)
        {
            // TODO: Implement assert with message
            Log::Write(Log::Level::Fatal, "Text::RefreshSize() - No Font!");
            PI_ASSERT(false);
            return;
        }

        Size p(1, GetFont().size);

        if (Length() > 0)
            p = GetSkin()->GetRenderer()->MeasureText(GetFont(), m_string);

        p.w += m_padding.left + m_padding.right;
        p.h += m_padding.top + m_padding.bottom;

        if (p.w == m_bounds.w && p.h == m_bounds.h)
            return;

        if (p.h < GetFont().size)
            p.h = GetFont().size;

        SetSize(p.w, p.h);
        InvalidateParent();
        Invalidate();
    }

    Rect Text::GetCharacterPosition(PiUInt32 characterIndex)
    {
        if (!m_lines.empty())
        {
            auto it = m_lines.begin();
            PiUInt32 chars = 0;

            Text* line = nullptr;
            while (it != m_lines.end())
            {
                line = *it;
                ++it;
                chars += line->Length();

                if (chars <= characterIndex)
                    continue;

                chars -= line->Length();
                Rect rect = line->GetCharacterPosition(characterIndex - chars);
                rect.x += line->X();
                rect.y += line->Y();
                return rect;
            }

            // manage special case of the last character
            Rect rect = line->GetCharacterPosition(line->Length());
            rect.x += line->X();
            rect.y += line->Y();
            return rect;
        }

        if (Length() == 0 || characterIndex == 0)
        {
            Size p = GetSkin()->GetRenderer()->MeasureText(GetFont(), " ");
            return Rect(0, 0, 0, p.h);
        }

        PiString sub = m_string.substr(0, characterIndex);
        Size p = GetSkin()->GetRenderer()->MeasureText(GetFont(), sub);

        return Rect(p.w, 0, 0, p.h);
    }

    PiInt32 Text::GetClosestCharacter(Point p)
    {
        if (!m_lines.empty())
        {
            auto it = m_lines.begin();
            PiInt32 chars = 0;

            Text* line = nullptr;
            while (it != m_lines.end())
            {
                line = *it;
                ++it;
                chars += line->Length();

                if (p.y < line->Y())
                    continue;
                if (p.y > line->Bottom())
                    continue;
                if (p.y < line->Bottom())
                    break;
            }

            chars -= line->Length();
            int linePos = line->GetClosestCharacter(Point(p.x - line->X(), p.y - line->Y()));
            return chars + linePos;
        }

        PiInt32 distance = 4096;
        PiInt32 index = 0;

        for (PiInt32 i = 0; i < m_string.length() + 1; i++)
        {
            Rect cp = GetCharacterPosition(i);
            const PiInt32 dist = std::abs(cp.x - p.x) + std::abs(cp.y - p.y); // this isn't proper

            if (dist > distance)
                continue;

            distance = dist;
            index = i;
        }

        return index;
    }

    Rect Text::GetLineBox(PiInt32 index)
    {
        Text* line = GetLine(index);
        if (line != nullptr)
        {
            Size p = GetSkin()->GetRenderer()->MeasureText(GetFont(), line->m_string);
            return Rect(line->X(), line->Y(), Clamp(p.w, 1, p.w), Clamp(p.h, 1, p.h));
        }
        else
        {
            Size p = GetSkin()->GetRenderer()->MeasureText(GetFont(), m_string);
            return Rect(0, 0, Clamp(p.w, 1, p.w), Clamp(p.h, 1, p.h));
        }
    }

    PiInt32 Text::Length() const
    {
        return static_cast<PiInt32>(m_string.size());
    }

    void Text::SetTextColor(const Color& col)
    {
        m_color = col;
    }

    void Text::SetTextColorOverride(const Color& col)
    {
        m_colorOverride = col;
    }

    const Color& Text::TextColor() const
    {
        return m_color;
    }

    void Text::TextChanged()
    {
        m_textChanged = true;
    }

    bool Text::Wrap()
    {
        return m_wrap;
    }

    void Text::SetWrap(bool wrap)
    {
        if (m_wrap == wrap)
            return;

        m_wrap = wrap;
        m_textChanged = true;
        Invalidate();
    }

    Text* Text::GetLine(PiInt32 i)
    {
        auto it = m_lines.begin();

        while (it != m_lines.end())
        {
            if (i == 0)
                return *it;

            ++it;
            i--;
        }

        return nullptr;
    }

    PiInt32 Text::GetLineFromChar(PiInt32 i)
    {
        PiInt32 chars = 0;
        PiInt32 lineNum = 0;

        for (auto&& line : m_lines)
        {
            chars += line->Length();

            if (chars > i)
                return lineNum;

            lineNum++;
        }

        if (lineNum > 0)
            return lineNum - 1;

        return lineNum;
    }

    PiInt32 Text::GetStartCharFromLine(PiInt32 i)
    {
        PiInt32 chars = 0;

        for (auto&& line : m_lines)
        {
            if (i == 0)
                return Clamp(chars, 0, Length());

            chars += line->Length();
            i--;
        }

        return Clamp(chars, 0, Length());
    }

    PiInt32 Text::GetEndCharFromLine(PiInt32 i)
    {
        PiInt32 start = GetStartCharFromLine(i);
        Text* line = GetLine(i);

        if (line)
            start += line->Length();

        return Clamp(start, 0, Length());
    }

    PiInt32 Text::GetCharPosOnLine(PiInt32 i)
    {
        PiInt32 lineNum = GetLineFromChar(i);
        Text* line = GetLine(lineNum);

        if (!line)
            return 0;

        PiInt32 start = GetStartCharFromLine(lineNum);
        return i - start;
    }

    PiUInt32 Text::NumLines()
    {
        return static_cast<PiUInt32>(m_lines.size());
    }

    void Text::SplitWords(const PiString& s, std::vector<PiString>& elems)
    {
        PiString str;

        int w = m_parent->Width() - (m_parent->GetPadding().left + m_parent->GetPadding().right);

        for (size_t i = 0; i < s.length(); i++)
        {
            if (s[i] == '\n')
            {
                if (!str.empty())
                    elems.push_back(str);

                elems.emplace_back("\n");
                str.clear();
                continue;
            }

            if (s[i] == ' ')
            {
                str += s[i];
                elems.push_back(str);
                str.clear();
                continue;
            }

            str += s[i];

            // if adding character makes the word bigger than the textbox size
            Size p = GetSkin()->GetRenderer()->MeasureText(GetFont(), str);
            if (p.w > w)
            {
                // split words
                str.pop_back();
                elems.push_back(str);
                str.clear();
                --i;
                continue;
            }
        }

        if (!str.empty())
            elems.push_back(str);
    }

    void Text::RefreshSizeWrap()
    {
        ClearChildren();

        for (auto&& line : m_lines)
        {
            delete line;
        }
        m_lines.clear();

        std::vector<PiString> words;
        SplitWords(GetText(), words);

        // Adding a word to the end simplifies the code below
        // which is anything but simple.
        words.emplace_back("");

        if (!m_font)
        {
            // TODO: Implement assert with message
            Log::Write(Log::Level::Fatal, "Text::RefreshSize() - No Font!");
            PI_ASSERT(false);
            return;
        }

        Size fontSize = GetSkin()->GetRenderer()->MeasureText(GetFont(), " ");
        int w = m_parent->Width() - m_parent->GetPadding().left - m_parent->GetPadding().right;
        int x = 0, y = 0;
        PiString strLine;

        for (auto&& it = words.begin(); it != words.end(); ++it)
        {
            bool bFinishLine = false;
            bool bWrapped = false;

            // If this word is a newline - make a newline (we still add it to the
            // text)
            if ((*it).c_str()[0] == '\n')
                bFinishLine = true;

            // Does adding this word drive us over the width?
            {
                strLine += *it;
                Size p = GetSkin()->GetRenderer()->MeasureText(GetFont(), strLine);

                if (p.w > Width() && p.w > w)
                {
                    bFinishLine = true;
                    bWrapped = true;
                }
            }

            // If this is the last word then finish the line
            if (--words.end() == it)
                bFinishLine = true;

            if (bFinishLine)
            {
                Text* t = new Text(this);
                t->SetFont(GetFont());
                t->SetTextColor(TextColor());
                if (bWrapped)
                {
                    t->SetText(strLine.substr(0, strLine.length() - (*it).length()));
                    // newline should start with the word that was too big
                    strLine = *it;
                }
                else
                {
                    t->SetText(strLine.substr(0, strLine.length()));
                    // new line is empty
                    strLine.clear();
                }
                t->RefreshSize();
                t->SetPosition(x, y);
                m_lines.push_back(t);
                // newline should start with the word that was too big
                // strLine = *it;

                // Position the newline
                y += fontSize.h;
                x = 0;
                // if ( strLine[0] == ' ' ) x -= fontSize.x;
            }
        }

        // Size to children height and parent width
        {
            Size childsize = ChildrenSize();
            SetSize(w, childsize.h);
        }
        InvalidateParent();
        Invalidate();
    }

    Widget* Text::GetWidgetAt(PiInt32 x, PiInt32 y, bool onlyIfMouseEnabled)
    {
        Widget* child = ParentClass::GetWidgetAt(x, y, onlyIfMouseEnabled);

        if (child == this)
            return m_parent;

        return child;
    }

    void Text::Render(Skin* skin)
    {
        if (m_hidden)
            return;

        if (m_wrap)
            return;

        if (Length() == 0)
            return;

        if (m_colorOverride.a == 0)
            skin->GetRenderer()->SetDrawColor(m_color);
        else
            skin->GetRenderer()->SetDrawColor(m_colorOverride);

        skin->GetRenderer()->DrawString(GetFont(), Point(m_padding.left, m_padding.top), m_string);
    }

    void Text::Layout(Skin* skin)
    {
        if (m_textChanged)
        {
            RefreshSize();
            m_textChanged = false;
        }
    }

    void Text::OnScaleChanged()
    {
        Invalidate();
    }

    PI_WIDGET_CONSTRUCTOR(Label)
    {
        const Skin::Data& skinData = GetSkin()->GetSkinData();
        m_createdFont = nullptr;
        m_text = new Text(this);
        SetFont(GetSkin()->GetDefaultFont());
        SetMouseInputEnabled(false);
        SetBounds(0, 0, 100, 10);
        SetAlignment(Alignment::Left | Alignment::Top);
        SetTextColor(skinData.Label.textColor);
    }

    void Label::SetText(const PiString& str, bool doEvents)
    {
        if (m_text->GetText() == str)
            return;

        m_text->SetText(str);
        m_textDirty = true;

        Redraw();

        if (doEvents)
            OnTextChanged();
    }

    const PiString& Label::GetText() const
    {
        return m_text->GetText();
    }

    void Label::PreDelete(Skin* skin)
    {
        if (m_createdFont)
        {
            skin->ReleaseFont(m_createdFont);
            delete m_createdFont;
            m_createdFont = nullptr;
            SetFont(skin->GetDefaultFont());
        }
    }

    void Label::Render(Skin* skin)
    {}

    void Label::PostLayout(Skin* skin)
    {
        m_text->SetPosition(m_align, 0, 0);
    }

    void Label::SizeToContents()
    {
        m_text->SetPosition(m_padding.left, m_padding.top);
        m_text->RefreshSize();
        SetSize(m_text->Width() + m_padding.left + m_padding.right, m_text->Height() + m_padding.top + m_padding.bottom);
    }

    void Label::SetAlignment(Alignment alignment)
    {
        if (m_align == alignment)
            return;

        m_align = alignment;
        Invalidate();
    }

    Alignment Label::GetAlignment()
    {
        return m_align;
    }

    void Label::SetFont(PiString fontName, PiInt32 size, Font::Weight weight)
    {
        if (m_createdFont)
        {
            GetSkin()->ReleaseFont(m_createdFont);
            delete m_createdFont;
            m_createdFont = nullptr;
            SetFont(GetSkin()->GetDefaultFont());
        }

        m_createdFont = new Font();
        // TODO: PI_ASSERT_MSG(m_createdFont != nullptr, "Couldn't Create Font!");
        PI_ASSERT(m_createdFont != nullptr);
        m_createdFont->weight = weight;
        m_createdFont->facename = fontName;
        m_createdFont->size = size;
        SetFont(*m_createdFont);
        m_text->RefreshSize();
    }

    void Label::SetFont(const Font& font)
    {
        m_text->SetFont(font);
        m_textDirty = true;
    }

    const Font& Label::GetFont() const
    {
        return m_text->GetFont();
    }

    void Label::SetFontSize(PiUInt32 size)
    {
        Font f = *m_text->m_font;
        SetFont(f.facename, size, f.weight);
    }

    void Label::SetFontWeight(Font::Weight weight)
    {
        Font f = *m_text->m_font;
        SetFont(f.facename, f.size, weight);
    }

    void Label::SetFontStyle(Font::Style style)
    {
        Font f = *m_text->m_font;

        f.style = style;
        SetFont(f);
        m_text->RefreshSize();
    }

    void Label::SetTextColor(const Color& color)
    {
        m_text->SetTextColor(color);
    }

    void Label::SetTextColorOverride(const Color& color)
    {
        m_text->SetTextColorOverride(color);
    }

    const Color& Label::GetTextColor() const
    {
        return m_text->TextColor();
    }

    PiInt32 Label::GetTextWidth() const
    {
        return m_text->Width();
    }

    PiInt32 Label::TextRight() const
    {
        return m_text->Right();
    }

    PiInt32 Label::GetTextHeight() const
    {
        return m_text->Height();
    }

    PiInt32 Label::TextX() const
    {
        return m_text->X();
    }

    PiInt32 Label::TextY() const
    {
        return m_text->Y();
    }

    PiInt32 Label::TextLength() const
    {
        return m_text->Length();
    }

    Rect Label::GetCharacterPosition(int index)
    {
        Rect p = m_text->GetCharacterPosition(index);
        p.x += m_text->X();
        p.y += m_text->Y();
        return p;
    }

    void Label::SetPadding(const Padding& padding)
    {
        ParentClass::SetPadding(padding);
        m_textDirty = true;
        m_text->RefreshSize();
    }

    Alignment Label::GetAlignment() const
    {
        return m_align;
    }

    PiString Label::GetValue()
    {
        return GetText();
    }

    void Label::SetValue(const PiString& value)
    {
        SetText(value);
    }

    bool Label::Wrap()
    {
        return m_text->Wrap();
    }

    void Label::SetWrap(bool wrap)
    {
        m_text->SetWrap(wrap);
    }

    void Label::OnBoundsChanged(const Rect& oldChildBounds)
    {
        ParentClass::OnBoundsChanged(oldChildBounds);

        if (m_text->Wrap())
        {
            m_text->RefreshSize();
            Invalidate();
        }
    }
} // namespace SparkyStudios::UI::Pixel
