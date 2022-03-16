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

#include <SparkyStudios/UI/Pixel/Widgets/Shapes/LeftArrow.h>

namespace SparkyStudios::UI::Pixel
{
    PI_WIDGET_CONSTRUCTOR(LeftArrow)
    {}

    void LeftArrow::Render(Skin* skin)
    {
        BaseRenderer* renderer = skin->GetRenderer();
        const Rect& r = RenderBounds();

        renderer->SetDrawColor(m_backgroundColor);

        renderer->DrawFilledTriangle(Point(r.x, r.y + (r.h / 2)), Point(r.x + r.w, r.y), Point(r.x + r.w, r.y + r.h));
    }
} // namespace SparkyStudios::UI::Pixel
