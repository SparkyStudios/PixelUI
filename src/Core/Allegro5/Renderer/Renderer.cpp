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

#include <Core/Allegro5/Renderer/Renderer.h>

namespace SparkyStudios::UI::Pixel
{
    CacheToTexture_Allegro::CacheToTexture_Allegro()
        : _oldTarget(nullptr)
    {}

    void CacheToTexture_Allegro::Initialize()
    {}

    void CacheToTexture_Allegro::ShutDown()
    {
        // TODO: Delete cached textures
    }

    void CacheToTexture_Allegro::SetupCacheTexture(CacheHandle control)
    {
        CacheMap::iterator it = _cache.find(control);
        PI_ASSERT(it != _cache.end());

        if (it != _cache.end())
        {
            // Prepare for rendering
            PI_ASSERT(_oldTarget == nullptr);
            _oldTarget = al_get_target_bitmap();

            auto* alBitmap = it->second.m_bitmap;
            PI_ASSERT(alBitmap != nullptr);
            al_set_target_bitmap(alBitmap);

            al_clear_to_color(al_map_rgb_f(1.0f, 1.0f, 1.0f));
        }
    }

    void CacheToTexture_Allegro::FinishCacheTexture(CacheHandle control)
    {
        // Prepare for rendering
        al_set_target_bitmap(_oldTarget);
        _oldTarget = nullptr;
    }

    void CacheToTexture_Allegro::SetRenderer(BaseRenderer* renderer)
    {
        _renderer = renderer;
    }

    void CacheToTexture_Allegro::DrawCachedWidgetTexture(CacheHandle control)
    {
        CacheMap::iterator it = _cache.find(control);
        PI_ASSERT(it != _cache.end());

        if (it != _cache.end())
        {
            auto* alBitmap = it->second.m_bitmap;
            const Point& offset = _renderer->GetRenderOffset();
            al_draw_bitmap(alBitmap, offset.x, offset.y, 0);
        }
    }

    void CacheToTexture_Allegro::CreateWidgetCacheTexture(CacheHandle control, const Point& size)
    {
        // If we haven't seen this control before, create a new one
        if (_cache.find(control) == _cache.end())
        {
            al_set_new_bitmap_flags(ALLEGRO_VIDEO_BITMAP);
            CacheEntry newEntry = { al_create_bitmap(size.x, size.y) };
            _cache[control] = newEntry;
        }
    }

    void CacheToTexture_Allegro::UpdateWidgetCacheTexture(CacheHandle control)
    {}

    Renderer_Allegro::Renderer_Allegro(ResourcePaths& paths)
        : BaseRenderer(paths)
        , _lastFont(nullptr)
        , _lastTexture(nullptr)
        , _ctt(new CacheToTexture_Allegro())
    {
        _ctt->SetRenderer(this);
        _ctt->Initialize();
    }

    Renderer_Allegro::~Renderer_Allegro()
    {}

    void Renderer_Allegro::SetDrawColor(const Color& color)
    {
        _color = al_map_rgba(color.r, color.g, color.b, color.a);
    }

    void Renderer_Allegro::StartClip()
    {
        Rect rect = ClipRegion();
        al_set_clipping_rectangle(rect.x, rect.y, rect.w, rect.h);
    }

    void Renderer_Allegro::EndClip()
    {
        ALLEGRO_BITMAP* target = al_get_target_bitmap();
        al_set_clipping_rectangle(0, 0, al_get_bitmap_width(target), al_get_bitmap_height(target));
    }

    Color Renderer_Allegro::PixelColor(const Texture& texture, const Point& position, const Color& col_default)
    {
        if (!EnsureTexture(texture))
            return col_default;

        TextureData_Allegro& data = _lastTexture->second;
        ALLEGRO_COLOR color = al_get_pixel(data.texture.get(), position.x, position.y);

        Color c;
        al_unmap_rgba(color, &c.r, &c.g, &c.b, &c.a);

        return c;
    }

    void Renderer_Allegro::DrawFilledRect(Rect rect)
    {
        Translate(rect);
        const PiReal32 fx = rect.x + 0.5f, fy = rect.y + 0.5f;
        al_draw_filled_rectangle(fx, fy, fx + rect.w, fy + rect.h, _color);
    }

    void Renderer_Allegro::DrawTexturedRect(const Texture& texture, Rect rect, PiReal32 u1, PiReal32 v1, PiReal32 u2, PiReal32 v2)
    {
        if (!EnsureTexture(texture))
        {
            DrawMissingImage(rect);
            return;
        }

        Translate(rect);

        TextureData_Allegro& data = _lastTexture->second;

        const PiUInt32 w = data.width;
        const PiUInt32 h = data.height;

        al_draw_scaled_bitmap(
            data.texture.get(), // texture
            u1 * w, v1 * h, (u2 - u1) * w, (v2 - v1) * h, // source
            rect.x, rect.y, rect.w, rect.h, // destination
            0 // flags
        );
    }

    void Renderer_Allegro::DrawLinedRect(Rect rect)
    {
        Translate(rect);
        const PiReal32 fx = rect.x + 0.5f, fy = rect.y + 0.5f;
        al_draw_rectangle(fx, fy, fx + rect.w, fy + rect.h, _color, 0.0f);
    }

    void Renderer_Allegro::DrawShavedCornerRect(Rect rect, bool bSlight)
    {
        // Draw INSIDE the w/h.
        rect.w -= 1;
        rect.h -= 1;

#define SET_VERT(I, X, Y) vtx[I].x = (X), vtx[I].y = (Y), vtx[I].color = _color
#define ADD_LINE(I, X0, Y0, X1, Y1)                                                                                                        \
    SET_VERT(I, X0, Y0);                                                                                                                   \
    SET_VERT(I + 1, X1, Y1)

        const PiReal32 fx = rect.x + 0.5f, fy = rect.y + 0.5f;
        const PiReal32 fw = rect.w, fh = rect.h;

        if (bSlight)
        {
            ALLEGRO_VERTEX vtx[4 * 2];
            ADD_LINE(0, fx + 1.f, fy, fx + fw - 1.f, fy); // top
            ADD_LINE(2, fx + fw, fy + 1.f, fx + fw, fy + fh - 1.f); // right
            ADD_LINE(4, fx + fw - 1.f, fy + fh, fx + 1.f, fy + fh); // bottom
            ADD_LINE(6, fx, fy + fh - 1.f, fx, fy + 1.f); // left
            al_draw_prim(vtx, nullptr, nullptr, 0, 7, ALLEGRO_PRIM_LINE_LOOP);
        }
        else
        {
            ALLEGRO_VERTEX vtx[4 * 2];
            ADD_LINE(0, fx + 2.f, fy, fx + fw - 2.f, fy); // top
            ADD_LINE(2, fx + fw, fy + 2.f, fx + fw, fy + fh - 2.f); // right
            ADD_LINE(4, fx + fw - 2.f, fy + fh, fx + 2.f, fy + fh); // bottom
            ADD_LINE(6, fx, fy + fh - 2.f, fx, fy + 2.f); // left
            al_draw_prim(vtx, nullptr, nullptr, 0, 7, ALLEGRO_PRIM_LINE_LOOP);
        }

#undef SET_VERT
#undef ADD_LINE
    }

    void Renderer_Allegro::DrawString(const Font& font, Point pos, const PiString& text)
    {
        if (!EnsureFont(font))
            return;

        auto it = _fonts.find(font);
        if (it == _fonts.end())
            return;

        FontData_Allegro& data = it->second;
        Translate(pos.x, pos.y);

        al_draw_text(data.font.get(), _color, pos.x, pos.y, ALLEGRO_ALIGN_LEFT, text.c_str());
    }

    Size Renderer_Allegro::MeasureText(const Font& font, const PiString& text)
    {
        if (!EnsureFont(font))
            return Size(0, 0);

        auto it = _fonts.find(font);
        if (it == _fonts.end())
            return Size(0, 0);

        FontData_Allegro& data = it->second;

        return Size(al_get_text_width(data.font.get(), text.c_str()), al_get_font_line_height(data.font.get()));
    }

    bool Renderer_Allegro::InitializeContext(MainWindow* window)
    {
        return true;
    }

    bool Renderer_Allegro::DestroyContext(MainWindow* window)
    {
        return true;
    }

    bool Renderer_Allegro::ResizedContext(MainWindow* window, const Size& size)
    {
        return true;
    }

    bool Renderer_Allegro::BeginContext(MainWindow* window)
    {
        al_clear_to_color(al_map_rgba_f(0.0f, 0.0f, 0.0f, 0.0f));
        return true;
    }

    bool Renderer_Allegro::EndContext(MainWindow* window)
    {
        return true;
    }

    bool Renderer_Allegro::PresentContext(MainWindow* window)
    {
        al_flip_display();
        return true;
    }

    ICacheToTexture* Renderer_Allegro::GetCTT()
    {
        return _ctt;
    }

    IResourceLoader::LoadStatus Renderer_Allegro::LoadFont(const Font& font)
    {
        FreeFont(font);
        _lastFont = nullptr;

        const PiString fileName = GetResourcePaths().GetPath(ResourcePaths::Type::Font, font.facename);

        ALLEGRO_FONT* alFont = al_load_font(fileName.c_str(), font.size * GetScale(), ALLEGRO_TTF_NO_KERNING);

        if (alFont != nullptr)
        {
            FontData_Allegro fontData;
            fontData.font = deleted_unique_ptr<ALLEGRO_FONT>(
                alFont,
                [](ALLEGRO_FONT* f)
                {
                    if (f != nullptr)
                        al_destroy_font(f);
                });

            _lastFont = &(*_fonts.insert({ font, std::move(fontData) }).first);
            return LoadStatus::Loaded;
        }
        else
        {
            Log::Write(Log::Level::Error, "Font file not found: %s", fileName.c_str());
            return LoadStatus::ErrorFileNotFound;
        }
    }

    void Renderer_Allegro::FreeFont(const Font& font)
    {
        if (_lastFont != nullptr && _lastFont->first == font)
            _lastFont = nullptr;

        _fonts.erase(font);
    }

    bool Renderer_Allegro::EnsureFont(const Font& font)
    {
        if (_lastFont != nullptr && _lastFont->first == font)
            return true;

        auto it = _fonts.find(font);
        if (it != _fonts.end())
        {
            _lastFont = &(*it);
            return true;
        }

        return LoadFont(font) == IResourceLoader::LoadStatus::Loaded;
    }

    IResourceLoader::LoadStatus Renderer_Allegro::LoadTexture(const Texture& texture)
    {
        FreeTexture(texture);
        _lastTexture = nullptr;
        const PiString fileName = GetResourcePaths().GetPath(ResourcePaths::Type::Texture, texture.name);

        ALLEGRO_BITMAP* bitmap = al_load_bitmap(fileName.c_str());

        if (bitmap != nullptr)
        {
            TextureData_Allegro data;
            data.texture = deleted_unique_ptr<ALLEGRO_BITMAP>(
                bitmap,
                [](ALLEGRO_BITMAP* b)
                {
                    if (b)
                        al_destroy_bitmap(b);
                });
            data.width = al_get_bitmap_width(bitmap);
            data.height = al_get_bitmap_height(bitmap);
            data.readable = false;
            _lastTexture = &(*_textures.insert({ texture, std::move(data) }).first);

            return IResourceLoader::LoadStatus::Loaded;
        }
        else
        {
            Log::Write(Log::Level::Error, "Texture file not found: %s", fileName.c_str());
            return IResourceLoader::LoadStatus::ErrorFileNotFound;
        }
    }

    void Renderer_Allegro::FreeTexture(const Texture& texture)
    {
        if (_lastTexture != nullptr && _lastTexture->first == texture)
            _lastTexture = nullptr;

        _textures.erase(texture);
    }

    TextureData Renderer_Allegro::GetTextureData(const Texture& texture) const
    {
        if (_lastTexture != nullptr && _lastTexture->first == texture)
            return _lastTexture->second;

        auto it = _textures.find(texture);
        if (it != _textures.end())
        {
            return it->second;
        }

        return TextureData();
    }

    bool Renderer_Allegro::EnsureTexture(const Texture& texture)
    {
        if (_lastTexture != nullptr && _lastTexture->first == texture)
            return true;

        auto it = _textures.find(texture);
        if (it != _textures.end())
        {
            _lastTexture = &(*it);
            return true;
        }

        return LoadTexture(texture) == IResourceLoader::LoadStatus::Loaded;
    }
} // namespace SparkyStudios::UI::Pixel