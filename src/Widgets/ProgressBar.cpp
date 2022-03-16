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
#include <SparkyStudios/UI/Pixel/Core/Utility.h>

#include <SparkyStudios/UI/Pixel/Widgets/ProgressBar.h>

namespace SparkyStudios::UI::Pixel
{
#if PI_ENABLE_ANIMATION

    class ProgressBarAnimation : public Animation
    {
    public:
        ProgressBarAnimation(PiTime duration)
            : Animation(duration, 0.0, Animation::TransitionFunction::Linear, true)
            , m_reverse(false)
        {}

        void OnStart() override
        {}

        void Run(PiTime percent) override
        {
            auto* p = pi_cast<ProgressBar*>(m_widget);
            if (p->IsUndetermined())
            {
                p->_undeterminedCycleLevel = m_reverse ? 1.0 - percent : percent;
            }
        }

        void OnFinish() override
        {
            // m_reverse = !m_reverse;
        }

        bool m_reverse;
    };

#endif // PI_ENABLE_ANIMATION

    PI_WIDGET_CONSTRUCTOR(ProgressBar)
    , m_progress(0.0), m_autoLabel(true), m_undetermined(false), _undeterminedCycleLevel(0.0)
    {
        const Skin::Data& skinData = GetSkin()->GetSkinData();
        SetMouseInputEnabled(false);
        SetOrientation(Orientation::Horizontal);
        SetDirection(Direction::StartToEnd);
        SetAlignment(Alignment::Center);
        SetTextColor(skinData.ProgressBar.textColorNormal);
        SetPadding(skinData.ProgressBar.padding);
#if PI_ENABLE_ANIMATION
        Animate(new ProgressBarAnimation(1.2));
#endif // PI_ENABLE_ANIMATION
    }

    void ProgressBar::SetOrientation(Orientation orientation)
    {
        m_orientation = orientation;
        const Skin::Data& skinData = GetSkin()->GetSkinData();

        switch (m_orientation)
        {
        case Orientation::Vertical:
            SetBounds(Rect(m_bounds.x, m_bounds.y, skinData.ProgressBar.size.w, (std::max)(m_bounds.w, m_bounds.h)));
            break;
        case Orientation::Horizontal:
            SetBounds(Rect(m_bounds.x, m_bounds.y, (std::max)(m_bounds.w, m_bounds.h), skinData.ProgressBar.size.h));
            break;
        }
    }

    ProgressBar::Orientation ProgressBar::GetOrientation() const
    {
        return m_orientation;
    }

    void ProgressBar::SetDirection(ProgressBar::Direction direction)
    {
        m_direction = direction;
    }

    ProgressBar::Direction ProgressBar::GetDirection() const
    {
        return m_direction;
    }

    void ProgressBar::SetRange(const Range<PiReal64>& range)
    {
        m_range = range;
    }

    const Range<PiReal64>& ProgressBar::GetRange() const
    {
        return m_range;
    }

    void ProgressBar::SetProgress(PiReal64 progress)
    {
        m_progress = Clamp(progress, m_range.start, m_range.end);
        m_progress = Clamp((m_progress - m_range.start) / (m_range.end - m_range.start), 0.0, 1.0);

        if (m_autoLabel)
        {
            const PiInt32 displayValue = static_cast<PiInt32>(m_progress * 100.0);
            SetText(Utility::ToString(displayValue) + "%");
        }
    }

    PiReal64 ProgressBar::GetProgress() const
    {
        return m_progress * (m_range.end - m_range.start) + m_range.start;
    }

    void ProgressBar::AutoLabel(bool value)
    {
        m_autoLabel = value;
    }

    bool ProgressBar::IsAutoLabelEnabled() const
    {
        return m_autoLabel;
    }

    void ProgressBar::ShowLabel(bool value)
    {
        m_showLabel = value;
    }

    bool ProgressBar::IsLabelVisible() const
    {
        return m_showLabel;
    }

    void ProgressBar::SetUndeterminedProgress(bool value)
    {
        m_undetermined = value;
        AutoLabel(!value);
    }

    bool ProgressBar::IsUndetermined() const
    {
        return m_undetermined;
    }

    void ProgressBar::Render(Skin* skin)
    {
        const Rect& rect = RenderBounds();

        const Skin::Data& skinData = skin->GetSkinData();
        BaseRenderer* renderer = skin->GetRenderer();

        // Text
        {
            if (m_disabled)
                SetTextColorOverride(skinData.ProgressBar.textColorDisabled);
            else
                SetTextColorOverride(Colors::Transparent);
        }

        const PiUInt32& offset = skinData.ProgressBar.borderWidth;
        const Rect& rOffset = Rect(offset, offset, -offset * 2, -offset * 2);

        if (m_orientation == Orientation::Horizontal)
        {
            // Background
            {
                if (m_disabled)
                    renderer->SetDrawColor(skinData.ProgressBar.backgroundColorDisabled);
                else
                    renderer->SetDrawColor(skinData.ProgressBar.backgroundColorNormal);

                renderer->DrawFilledRect(rect + rOffset, skinData.ProgressBar.borderRadius);
            }

            // Progress
            if (m_undetermined)
            {
                if (!m_disabled)
                {
                    const PiReal64 s = rect.w / 4.0;
                    renderer->SetDrawColor(skinData.ProgressBar.progressColorNormal);

                    if (m_direction == Direction::StartToEnd)
                    {
                        const PiInt32 l = rect.w * _undeterminedCycleLevel;
                        renderer->DrawFilledRect(Rect(l, rect.y, s, rect.h) + rOffset, skinData.ProgressBar.borderRadius);

                        if (l + s > rect.w)
                            renderer->DrawFilledRect(Rect(l - rect.w, rect.y, s, rect.h) + rOffset, skinData.ProgressBar.borderRadius);
                    }
                    else
                    {
                        const PiInt32 l = rect.w - rect.w * _undeterminedCycleLevel - s;
                        renderer->DrawFilledRect(Rect(l, rect.y, s, rect.h) + rOffset, skinData.ProgressBar.borderRadius);

                        if (l < rect.x)
                            renderer->DrawFilledRect(
                                Rect(rect.w - (rect.x - l), rect.y, s, rect.h) + rOffset, skinData.ProgressBar.borderRadius);
                    }
                }
            }
            else
            {
                if (m_disabled)
                    renderer->SetDrawColor(skinData.ProgressBar.progressColorDisabled);
                else
                    renderer->SetDrawColor(skinData.ProgressBar.progressColorNormal);

                if (m_direction == Direction::EndToStart)
                    renderer->DrawFilledRect(
                        Rect(rect.w * (1.0 - m_progress), rect.y, rect.w * m_progress, rect.h) + rOffset,
                        skinData.ProgressBar.borderRadius);
                else
                    renderer->DrawFilledRect(
                        Rect(rect.x, rect.y, rect.w * m_progress, rect.h) + rOffset, skinData.ProgressBar.borderRadius);
            }
        }
        else
        {
            // Background
            {
                if (m_disabled)
                    renderer->SetDrawColor(skinData.ProgressBar.backgroundColorDisabled);
                else
                    renderer->SetDrawColor(skinData.ProgressBar.backgroundColorNormal);

                renderer->DrawFilledRect(rect + rOffset, skinData.ProgressBar.borderRadius);
            }

            // Progress
            if (m_undetermined)
            {
                if (!m_disabled)
                {
                    const PiReal64 s = rect.h / 4.0;
                    renderer->SetDrawColor(skinData.ProgressBar.progressColorNormal);

                    if (m_direction == Direction::EndToStart)
                    {
                        const PiInt32 l = rect.h * _undeterminedCycleLevel;
                        renderer->DrawFilledRect(Rect(rect.x, l, rect.w, s) + rOffset, skinData.ProgressBar.borderRadius);

                        if (l + s > rect.h)
                            renderer->DrawFilledRect(Rect(rect.x, l - rect.h, rect.w, s) + rOffset, skinData.ProgressBar.borderRadius);
                    }
                    else
                    {
                        const PiInt32 l = rect.h - rect.h * _undeterminedCycleLevel - s;
                        renderer->DrawFilledRect(Rect(rect.x, l, rect.w, s) + rOffset, skinData.ProgressBar.borderRadius);

                        if (l < rect.y)
                            renderer->DrawFilledRect(
                                Rect(rect.x, rect.h - (rect.y - l), rect.w, s) + rOffset, skinData.ProgressBar.borderRadius);
                    }
                }
            }
            else
            {
                if (m_disabled)
                    renderer->SetDrawColor(skinData.ProgressBar.progressColorDisabled);
                else
                    renderer->SetDrawColor(skinData.ProgressBar.progressColorNormal);

                if (m_direction == Direction::EndToStart)
                    renderer->DrawFilledRect(
                        Rect(rect.x, rect.y, rect.w, rect.h * m_progress) + rOffset, skinData.ProgressBar.borderRadius);
                else
                    renderer->DrawFilledRect(
                        Rect(rect.x, rect.h * (1.0 - m_progress), rect.w, rect.h * m_progress) + rOffset,
                        skinData.ProgressBar.borderRadius);
            }
        }

        // Border
        if (skinData.ProgressBar.borderWidth > 0)
        {
            if (m_disabled)
                renderer->SetDrawColor(skinData.ProgressBar.borderColorDisabled);
            else
                renderer->SetDrawColor(skinData.ProgressBar.borderColorNormal);

            renderer->DrawLinedRect(rect, skinData.ProgressBar.borderWidth, skinData.ProgressBar.borderRadius);
        }
    }
} // namespace SparkyStudios::UI::Pixel
