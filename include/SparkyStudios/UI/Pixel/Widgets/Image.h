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

#ifndef PIXEL_UI_IMAGE_H
#define PIXEL_UI_IMAGE_H

#include <SparkyStudios/UI/Pixel/Widgets/Widget.h>

namespace SparkyStudios::UI::Pixel
{
    class PI_EXPORT Image : public Widget
    {
    public:
        PI_WIDGET_INLINE(Image, Widget)
        {
            SetUV(0, 0, 1, 1);
            SetMouseInputEnabled(false);
            m_drawColor = Colors::White;
            SetStretch(true);
            m_texWidth = 0.0f;
            m_texHeight = 0.0f;
        }

        virtual ~Image()
        {
            GetSkin()->GetRenderer()->GetLoader().FreeTexture(m_texture);
        }

        virtual void SetUV(float u1, float v1, float u2, float v2)
        {
            m_uv[0] = u1;
            m_uv[1] = v1;
            m_uv[2] = u2;
            m_uv[3] = v2;
        }

        virtual void SetImage(const PiString& imageName)
        {
            m_texture.name = imageName;
            IResourceLoader& loader = GetSkin()->GetRenderer()->GetLoader();
            m_status = loader.LoadTexture(m_texture);
            switch (m_status)
            {
            case IResourceLoader::LoadStatus::Loaded:
                {
                    TextureData texData = loader.GetTextureData(m_texture);
                    m_texWidth = texData.width;
                    m_texHeight = texData.height;
                    break;
                }
            default:;
            }
        }

        virtual const PiString& GetImage() const
        {
            return m_texture.name;
        }

        virtual int TextureWidth() const
        {
            return m_texWidth;
        }

        virtual int TextureHeight() const
        {
            return m_texHeight;
        }

        virtual const PiString& GetImageName()
        {
            return m_texture.name;
        }

        void Render(Skin* skin) override
        {
            BaseRenderer* renderer = skin->GetRenderer();

            renderer->SetDrawColor(m_drawColor);

            if (m_bStretch)
            {
                renderer->DrawTexturedRect(m_texture, RenderBounds(), m_uv[0], m_uv[1], m_uv[2], m_uv[3]);
            }
            else
            {
                renderer->DrawTexturedRect(m_texture, Rect(0, 0, m_texWidth, m_texHeight), m_uv[0], m_uv[1], m_uv[2], m_uv[3]);
            }
        }

        virtual void SizeToContents()
        {
            SetSize(m_texWidth, m_texHeight);
        }

        virtual void SetDrawColor(Color color)
        {
            m_drawColor = color;
        }

        virtual bool FailedToLoad()
        {
            return m_status != IResourceLoader::LoadStatus::Loaded;
        }

        virtual bool GetStretch()
        {
            return m_bStretch;
        }

        virtual void SetStretch(bool b)
        {
            m_bStretch = b;
        }

    protected:
        Texture m_texture;
        float m_uv[4];
        Color m_drawColor;

        bool m_bStretch;
        float m_texWidth, m_texHeight;
        IResourceLoader::LoadStatus m_status;
    };
} // namespace SparkyStudios::UI::Pixel

#endif // PIXEL_UI_IMAGE_H
