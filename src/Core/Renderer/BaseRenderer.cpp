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

#include <cmath>

#include <SparkyStudios/UI/Pixel/Core/Renderer/BaseRenderer.h>
#include <SparkyStudios/UI/Pixel/Core/Utility.h>

namespace SparkyStudios::UI::Pixel
{
    BaseRenderer::BaseRenderer(ResourcePaths& paths)
        : m_scale(1.0f)
        , _paths(paths)
        , _renderOffset(Point(0, 0))
    {}

    BaseRenderer::~BaseRenderer()
    {
        if (GetCTT())
            GetCTT()->ShutDown();
    }

    void BaseRenderer::Init()
    {}

    void BaseRenderer::Begin()
    {}

    void BaseRenderer::End()
    {}

    IResourceLoader& BaseRenderer::GetLoader()
    {
        return *reinterpret_cast<IResourceLoader*>(this);
    }

    const ResourcePaths& BaseRenderer::GetResourcePaths() const
    {
        return _paths;
    }

    void BaseRenderer::SetDrawColor(const Color& color)
    {}

    void BaseRenderer::StartClip()
    {}

    void BaseRenderer::EndClip()
    {}

    void BaseRenderer::SetClipRegion(const Rect& rect)
    {
        _rectClipRegion = rect;
    }

    void BaseRenderer::AddClipRegion(Rect rect)
    {
        rect.x += _renderOffset.x;
        rect.y += _renderOffset.y;
        Rect out = rect;

        if (rect.x < _rectClipRegion.x)
        {
            out.w -= _rectClipRegion.x - out.x;
            out.x = _rectClipRegion.x;
        }

        if (rect.y < _rectClipRegion.y)
        {
            out.h -= _rectClipRegion.y - out.y;
            out.y = _rectClipRegion.y;
        }

        if (rect.x + rect.w > _rectClipRegion.x + _rectClipRegion.w)
            out.w = (_rectClipRegion.x + _rectClipRegion.w) - out.x;

        if (rect.y + rect.h > _rectClipRegion.y + _rectClipRegion.h)
            out.h = (_rectClipRegion.y + _rectClipRegion.h) - out.y;

        _rectClipRegion = out;
    }

    bool BaseRenderer::ClipRegionVisible()
    {
        if (_rectClipRegion.w <= 0 || _rectClipRegion.h <= 0)
            return false;

        return true;
    }

    const Rect& BaseRenderer::ClipRegion() const
    {
        return _rectClipRegion;
    }

    Color BaseRenderer::PixelColor(const Texture& texture, const Point& position, const Color& defaultColor)
    {
        return defaultColor;
    }

    void BaseRenderer::DrawMissingImage(Rect rect)
    {
        SetDrawColor(Colors::Red);
        DrawFilledRect(rect, Size(0, 0));
    }

    void BaseRenderer::DrawFilledRect(Rect rect, const Size& radii)
    {}

    void BaseRenderer::DrawTexturedRect(const Texture& texture, Rect rect, PiReal32 u1, PiReal32 v1, PiReal32 u2, PiReal32 v2)
    {}

    void BaseRenderer::DrawLinedRect(Rect rect, PiUInt32 thickness, const Size& radii)
    {
        DrawFilledRect(Rect(rect.x, rect.y, rect.w, 1), radii);
        DrawFilledRect(Rect(rect.x, rect.y + rect.h - 1, rect.w, 1), radii);
        DrawFilledRect(Rect(rect.x, rect.y, 1, rect.h), radii);
        DrawFilledRect(Rect(rect.x + rect.w - 1, rect.y, 1, rect.h), radii);
    }

    void BaseRenderer::DrawPixel(const Point& position)
    {
        DrawFilledRect(Rect(position.x, position.y, 1, 1), Size(0, 0));
    }

    void BaseRenderer::DrawShavedCornerRect(Rect rect, bool slight)
    {
        const auto radii = Size(0, 0);

        // Draw INSIDE the w/h.
        rect.w -= 1;
        rect.h -= 1;

        if (slight)
        {
            DrawFilledRect(Rect(rect.x + 1, rect.y, rect.w - 1, 1), radii);
            DrawFilledRect(Rect(rect.x + 1, rect.y + rect.h, rect.w - 1, 1), radii);
            DrawFilledRect(Rect(rect.x, rect.y + 1, 1, rect.h - 1), radii);
            DrawFilledRect(Rect(rect.x + rect.w, rect.y + 1, 1, rect.h - 1), radii);
        }
        else
        {
            DrawPixel(Point(rect.x + 1, rect.y + 1));
            DrawPixel(Point(rect.x + rect.w - 1, rect.y + 1));
            DrawPixel(Point(rect.x + 1, rect.y + rect.h - 1));
            DrawPixel(Point(rect.x + rect.w - 1, rect.y + rect.h - 1));
            DrawFilledRect(Rect(rect.x + 2, rect.y, rect.w - 3, 1), radii);
            DrawFilledRect(Rect(rect.x + 2, rect.y + rect.h, rect.w - 3, 1), radii);
            DrawFilledRect(Rect(rect.x, rect.y + 2, 1, rect.h - 3), radii);
            DrawFilledRect(Rect(rect.x + rect.w, rect.y + 2, 1, rect.h - 3), radii);
        }
    }

    void BaseRenderer::DrawString(const Font& font, Point pos, const PiString& text)
    {
        const PiReal32 size = font.size * GetScale();

        for (PiUInt32 i = 0; i < text.length(); i++)
        {
            char chr = text[i];

            if (chr == ' ')
                continue;

            Rect r(pos.x + i * size * 0.4f, pos.y, size * 0.4f - 1.0f, size);

            // This isn't important, it's just me messing around changing
            // the shape of the rect based on the letter.. just for fun.
            if (chr == 'l' || chr == 'i' || chr == '!' || chr == 't')
            {
                r.w = 1;
            }
            else if (chr >= 'a' && chr <= 'z')
            {
                r.y += size * 0.5f;
                r.h -= size * 0.4f;
            }
            else if (chr == '.' || chr == ',')
            {
                r.x += 2;
                r.y += r.h - 2;
                r.w = 2;
                r.h = 2;
            }
            else if (chr == '\'' || chr == '`' || chr == '"')
            {
                r.x += 3;
                r.w = 2;
                r.h = 2;
            }

            if (chr == 'o' || chr == 'O' || chr == '0')
                DrawLinedRect(r, 1, Size(0, 0));
            else
                DrawFilledRect(r, Size(0, 0));
        }
    }

    Size BaseRenderer::MeasureText(const Font& font, const PiString& text)
    {
        Size p;
        p.x = font.size * GetScale() * text.length() * 0.4f;
        p.y = font.size * GetScale();
        return p;
    }

    void BaseRenderer::Translate(PiInt32& x, PiInt32& y)
    {
        x += _renderOffset.x;
        y += _renderOffset.y;
        x = std::ceil(static_cast<PiReal32>(x) * m_scale);
        y = std::ceil(static_cast<PiReal32>(y) * m_scale);
    }

    void BaseRenderer::Translate(Rect& rect)
    {
        Translate(rect.x, rect.y);
        rect.w = std::ceil(static_cast<PiReal32>(rect.w) * m_scale);
        rect.h = std::ceil(static_cast<PiReal32>(rect.h) * m_scale);
    }

    void BaseRenderer::SetRenderOffset(const Point& offset)
    {
        _renderOffset = offset;
    }

    void BaseRenderer::AddRenderOffset(const Rect& offset)
    {
        _renderOffset.x += offset.x;
        _renderOffset.y += offset.y;
    }

    const Point& BaseRenderer::GetRenderOffset() const
    {
        return _renderOffset;
    }

    void BaseRenderer::SetScale(PiReal32 scale)
    {
        m_scale = scale;
    }

    PiReal32 BaseRenderer::GetScale() const
    {
        return m_scale;
    }

    bool BaseRenderer::InitializeContext(MainWindow* window)
    {
        return false;
    }

    bool BaseRenderer::DestroyContext(MainWindow* window)
    {
        return false;
    }

    bool BaseRenderer::ResizedContext(MainWindow* window, const Size& size)
    {
        return false;
    }

    bool BaseRenderer::BeginContext(MainWindow* window)
    {
        return false;
    }

    bool BaseRenderer::EndContext(MainWindow* window)
    {
        return false;
    }

    bool BaseRenderer::PresentContext(MainWindow* window)
    {
        return false;
    }

    ICacheToTexture* BaseRenderer::GetCTT()
    {
        return nullptr;
    }

    bool BaseRenderer::EnsureFont(const Font& font)
    {
        return false;
    }

    bool BaseRenderer::EnsureTexture(const Texture& texture)
    {
        return false;
    }
} // namespace SparkyStudios::UI::Pixel