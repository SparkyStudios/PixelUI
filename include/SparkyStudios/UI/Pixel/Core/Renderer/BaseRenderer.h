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

#ifndef PIXEL_UI_BASERENDERER_H
#define PIXEL_UI_BASERENDERER_H

#include <SparkyStudios/UI/Pixel/Core/Common.h>
#include <SparkyStudios/UI/Pixel/Core/Resource.h>

#include <SparkyStudios/UI/Pixel/Graphics/Color.h>
#include <SparkyStudios/UI/Pixel/Graphics/Font.h>
#include <SparkyStudios/UI/Pixel/Graphics/Point.h>
#include <SparkyStudios/UI/Pixel/Graphics/Rect.h>
#include <SparkyStudios/UI/Pixel/Graphics/Texture.h>

namespace SparkyStudios::UI::Pixel
{
    class MainWindow;

    class BaseRenderer;

    class ICacheToTexture
    {
    public:
        typedef void* CacheHandle;

        virtual ~ICacheToTexture() = default;

        virtual void Initialize() = 0;
        virtual void ShutDown() = 0;
        virtual void SetupCacheTexture(CacheHandle widget) = 0;
        virtual void FinishCacheTexture(CacheHandle widget) = 0;
        virtual void DrawCachedWidgetTexture(CacheHandle widget) = 0;
        virtual void CreateWidgetCacheTexture(CacheHandle widget, const Point& size) = 0;
        virtual void UpdateWidgetCacheTexture(CacheHandle widget) = 0;
        virtual void SetRenderer(BaseRenderer* renderer) = 0;
    };

    /**
     * @brief Base class for platform specific renderer implementations.
     */
    class PI_EXPORT BaseRenderer : public IResourceLoader
    {
    protected:
        /**
         * @brief Constructor
         *
         * @param paths The resources path manager to use in this
         * renderer.
         */
        explicit BaseRenderer(ResourcePaths& paths);

    public:
        virtual ~BaseRenderer();

        /**
         * @brief Initializes the renderer.
         */
        virtual void Init();

        /**
         * @brief Begins the rendering process.
         *
         * Every needed resource should be loaded in this method.
         */
        virtual void Begin();

        /**
         * @brief Ends the rendering process.
         *
         * Every loaded resource should be released in this method.
         */
        virtual void End();

        /**
         * @brief Gets the IResourceLoader object.
         *
         * @return The resource loader instance associated with this renderer.
         */
        IResourceLoader& GetLoader();

        /**
         * @brief Gets the Resource Paths object
         *
         * @return ResourcePaths&
         */
        [[nodiscard]] const ResourcePaths& GetResourcePaths() const;

        /**
         * @brief Sets the color of next draw operations.
         *
         * @param color The color of next draw operations.
         */
        virtual void SetDrawColor(const Color& color);

        /**
         * @brief Clips the renderer inside the region defined
         * by BaseRenderer::SetClipRegion().
         */
        virtual void StartClip();

        /**
         * @brief Restores the clip region of the renderer.
         */
        virtual void EndClip();

        /**
         * @brief Set the clip region rectangle to use in BaseRenderer::StartClip().
         *
         * @param rect The clip region.
         */
        void SetClipRegion(Rect const& rect);

        /**
         * @brief Add a clip region to the current one.
         *
         * @param rect The clip region to add.
         */
        void AddClipRegion(Rect rect);

        /**
         * @brief Check if the clip region is valid and visible.
         *
         * @return Whether the clip region is visible.
         */
        bool ClipRegionVisible();

        /**
         * @brief Gets the clip region.
         *
         * @return The clip region.
         */
        [[nodiscard]] const Rect& ClipRegion() const;

        /**
         * @brief Gets the color of the pixel at the given position.
         *
         * @param texture The texture to fetch the pixel from.
         * @param position The position of the pixel.
         * @param defaultColor The default color to use as fallback.
         *
         * @return The pixel color.
         */
        virtual Color PixelColor(const Texture& texture, const Point& position, const Color& defaultColor = Colors::White);

        /**
         * @brief Fallback drawer used when trying to draw an unloaded or missing image.
         *
         * @param rect The target space to draw into.
         */
        virtual void DrawMissingImage(Rect rect);

        /**
         * @brief Draws a rectangle filled with the drawing color.
         *
         * @param rect The target space to draw into.
         * @param radii The radius of the rectangle.
         */
        virtual void DrawFilledRect(Rect rect, const Size& radii);

        /**
         * @brief Draws a texture.
         *
         * @param texture The texture to draw.
         * @param rect The target space to draw into.
         * @param u1 The X-axis upper left coordinate inside the texture.
         * @param v1 The Y-axis upper left coordinate inside the texture.
         * @param u2 The X-axis lower right coordinate inside the texture.
         * @param v2 The Y-axis lower right coordinate inside the texture.
         * @param radii The radius of the rectangle.
         */
        virtual void DrawTexturedRect(const Texture& texture, Rect rect, PiReal32 u1, PiReal32 v1, PiReal32 u2, PiReal32 v2);

        /**
         * @brief Draws a transparent rectangle with borders of width 1.
         *
         * @param rect The target space to draw into.
         * @param thickness The width of the rectangle borders.
         * @param radii The radius of the rectangle.
         */
        virtual void DrawLinedRect(Rect rect, PiUInt32 thickness, const Size& radii);

        /**
         * @brief Draws a single pixel with the drawing color.
         *
         * @param position The pixel position.
         */
        virtual void DrawPixel(const Point& position);

        /**
         * @brief Draws a circle filled with the drawing color.
         *
         * @param rect The target space to draw into. If the target is a square,
         * the function will draw a circle.
         */
        virtual void DrawFilledEllipse(Rect rect);

        /**
         * @brief Draws a circle lined with the drawing color.
         *
         * @param rect The target space to draw into. If the target is a square,
         * the function will draw a circle.
         * @param thickness The width of the ellipse borders.
         */
        virtual void DrawLinedEllipse(Rect rect, PiUInt32 thickness);

        /**
         * @brief Draws a triangle filled with the drawing color.
         *
         * @param p1 The first point of the triangle.
         * @param p2 The second point of the triangle.
         * @param p3 The third point of the triangle.
         */
        virtual void DrawFilledTriangle(Point p1, Point p2, Point p3);

        /**
         * @brief Draws a triangle lined with the drawing color.
         *
         * @param p1 The first point of the triangle.
         * @param p2 The second point of the triangle.
         * @param p3 The third point of the triangle.
         * @param thickness The width of the ellipse borders.
         */
        virtual void DrawLinedTriangle(Point p1, Point p2, Point p3, PiUInt32 thickness);

        /**
         * @brief Draws a rectangle with shaved corners.
         *
         * @param rect The target space to draw into.
         * @param slight
         */
        virtual void DrawShavedCornerRect(Rect rect, bool slight = false);

        /**
         * @brief Draws a text with the given font at the given position.
         *
         * @param font The font to use when drawing the text.
         * @param pos The position of the text.
         * @param text The text to draw.
         */
        virtual void DrawString(const Font& font, Point pos, const PiString& text);

        /**
         * @brief Gets the size of the text.
         *
         * @param font The font to use when computing the size.
         * @param text The text.
         *
         * @return The size of the text in pixels.
         */
        virtual Size MeasureText(const Font& font, const PiString& text);

        /**
         * @brief Converts a position in pixels to render space coordinates.
         *
         * @param x The x coordinate.
         * @param y The y coordinate.
         */
        void Translate(PiInt32& x, PiInt32& y);

        /**
         * @brief Converts a rectangle in pixels to render space coordinates.
         *
         * @param rect The rectangle.
         */
        void Translate(Rect& rect);

        /**
         * @brief Sets the render offset.
         *
         * @param offset The offset in pixels.
         */
        void SetRenderOffset(const Point& offset);

        /**
         * @brief Adds the given position to the render offset.
         *
         * @param offset The offset to add.
         */
        void AddRenderOffset(const Rect& offset);

        /**
         * @brief Gets the current render offset.
         *
         * @return The current render offset.
         */
        [[nodiscard]] const Point& GetRenderOffset() const;

        /**
         * @brief Sets the UI scale factor.
         *
         * @param scale The scale factor.
         */
        void SetScale(PiReal32 scale);

        /**
         * @brief Gets the UI scale factor.
         *
         * @return The scale factor.
         */
        [[nodiscard]] PiReal32 GetScale() const;

        /**
         * @brief Initializes the rendering context.
         *
         * @param window The window which hold the rendering context.
         *
         * @return Whether the operation was successful.
         */
        virtual bool InitializeContext(MainWindow* window);

        /**
         * @brief Destroys the rendering context.
         *
         * @param window The window which holds the rendering context.
         *
         * @return Whether the operation was successful.
         */
        virtual bool DestroyContext(MainWindow* window);

        /**
         * @brief Resizes the rendering context.
         *
         * @param window The window which holds the rendering context.
         * @param size The new size of the rendering context.
         *
         * @return Whether the operation was successful.
         */
        virtual bool ResizedContext(MainWindow* window, const Size& size);

        /**
         * @brief Starts a rendering session.
         *
         * @param window The window which holds the rendering context.
         *
         * @return Whether the operation was successful.
         */
        virtual bool BeginContext(MainWindow* window);

        /**
         * @brief Ends a rendering session.
         *
         * @param window The window which holds the rendering context.
         *
         * @return Whether the operation was successful.
         */
        virtual bool EndContext(MainWindow* window);

        /**
         * @brief Swap front and back buffers in the rendering context.
         *
         * @param window The window which holds the rendering context.
         *
         * @return Whether the operation was successful.
         */
        virtual bool PresentContext(MainWindow* window);

        /**
         * @brief Gets the widgets texture cache.
         *
         * @return ICacheToTexture*
         */
        virtual ICacheToTexture* GetCTT();

    protected:
        /**
         * @brief Ensures that a font resource is loaded and available.
         *
         * @param font The font resource.
         *
         * @return true when the font resource is loaded, false otherwise.
         */
        virtual bool EnsureFont(const Font& font);

        /**
         * @brief Ensures that a texture resource is loaded and available.
         *
         * @param texture The texture resource.
         *
         * @return true when the texture resource is loaded, false otherwise.
         */
        virtual bool EnsureTexture(const Texture& texture);

        PiReal32 m_scale;

    private:
        ResourcePaths& _paths;
        Point _renderOffset;
        Rect _rectClipRegion;
    };
} // namespace SparkyStudios::UI::Pixel

#endif // PIXEL_UI_BASERENDERER_H
