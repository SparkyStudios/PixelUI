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

#ifndef PIXEL_UI_RENDERER_H
#define PIXEL_UI_RENDERER_H

#include <SparkyStudios/UI/Pixel/Core/Renderer/BaseRenderer.h>

#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>

#include <functional>
#include <map>
#include <memory>
#include <unordered_map>
#include <vector>

namespace SparkyStudios::UI::Pixel
{
    class CacheToTexture_Allegro : public ICacheToTexture
    {
        struct CacheEntry
        {
            ALLEGRO_BITMAP* m_bitmap;
        };

        typedef std::map<CacheHandle, CacheEntry> CacheMap;

    public:
        CacheToTexture_Allegro();
        ~CacheToTexture_Allegro() override = default;

        void Initialize() override;
        void ShutDown() override;

        void SetupCacheTexture(CacheHandle control) override;
        void FinishCacheTexture(CacheHandle control) override;

        void SetRenderer(BaseRenderer* renderer) override;

        void DrawCachedWidgetTexture(CacheHandle control) override;
        void CreateWidgetCacheTexture(CacheHandle control, const Point& size) override;
        void UpdateWidgetCacheTexture(CacheHandle control) override;

        // TODO: Why destroys the cached textures? Does this assume they always exist?

    private:
        BaseRenderer* _renderer;
        CacheMap _cache;
        ALLEGRO_BITMAP* _oldTarget;
    };

    class PI_EXPORT Renderer_Allegro : public BaseRenderer
    {
        template<typename T>
        using deleted_unique_ptr = std::unique_ptr<T, std::function<void(T*)>>;

        struct TextureData_Allegro : public TextureData
        {
            TextureData_Allegro() = default;

            TextureData_Allegro(const TextureData_Allegro&) = delete;

            TextureData_Allegro(TextureData_Allegro&& other)
                : TextureData_Allegro()
            {
                std::swap(width, other.width);
                std::swap(height, other.height);
                std::swap(readable, other.readable);
                texture.swap(other.texture);
            }

            ~TextureData_Allegro()
            {}

            deleted_unique_ptr<ALLEGRO_BITMAP> texture;
        };

        struct FontData_Allegro
        {
            FontData_Allegro() = default;

            FontData_Allegro(const FontData_Allegro&) = delete;

            FontData_Allegro(FontData_Allegro&& other)
                : FontData_Allegro()
            {
                font.swap(other.font);
            }

            ~FontData_Allegro()
            {}

            deleted_unique_ptr<ALLEGRO_FONT> font;
        };

    public:
        Renderer_Allegro(ResourcePaths& paths);
        virtual ~Renderer_Allegro();

        void SetDrawColor(const Color& color) override;

        void StartClip() override;

        void EndClip() override;

        Color PixelColor(const Texture& texture, const Point& position, const Color& defaultColor = Colors::White) override;

        void DrawFilledRect(Rect rect) override;

        void DrawTexturedRect(
            const Texture& texture, Rect rect, PiReal32 u1 = 0.0f, PiReal32 v1 = 0.0f, PiReal32 u2 = 1.0f, PiReal32 v2 = 1.0f) override;

        void DrawLinedRect(Rect rect) override;

        void DrawShavedCornerRect(Rect rect, bool bSlight = false) override;

        void DrawString(const Font& font, Point pos, const PiString& text) override;

        Size MeasureText(const Font& font, const PiString& text) override;

        bool InitializeContext(MainWindow* window) override;

        bool DestroyContext(MainWindow* window);

        bool ResizedContext(MainWindow* window, const Size& size);

        bool BeginContext(MainWindow* window) override;

        bool EndContext(MainWindow* window) override;

        bool PresentContext(MainWindow* window) override;

        ICacheToTexture* GetCTT() override;

        IResourceLoader::LoadStatus LoadFont(const Font& font);

        void FreeFont(const Font& font);

        bool EnsureFont(const Font& font) override;

        IResourceLoader::LoadStatus LoadTexture(const Texture& texture) override;

        void FreeTexture(const Texture& texture) override;

        TextureData GetTextureData(const Texture& texture) const override;

        bool EnsureTexture(const Texture& texture) override;

    private:
        std::unordered_map<Font, FontData_Allegro> _fonts;
        std::unordered_map<Texture, TextureData_Allegro> _textures;
        std::pair<const Font, FontData_Allegro>* _lastFont;
        std::pair<const Texture, TextureData_Allegro>* _lastTexture;

        ALLEGRO_COLOR _color;
        CacheToTexture_Allegro* _ctt;
    };
} // namespace SparkyStudios::UI::Pixel

#endif // PIXEL_UI_RENDERER_H
