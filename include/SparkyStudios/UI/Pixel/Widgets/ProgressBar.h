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

#ifndef PIXEL_UI_PROGRESSBAR_H
#define PIXEL_UI_PROGRESSBAR_H

#include <SparkyStudios/UI/Pixel/Widgets/Label.h>

namespace SparkyStudios::UI::Pixel
{
    /**
     * @brief Displays a progress bar, with a progress label.
     */
    class PI_EXPORT ProgressBar : public Label
    {
        friend class ProgressBarAnimation;

    public:
        /**
         * @brief The progress bar orientation.
         */
        enum class Orientation
        {
            Horizontal = 0,
            Vertical,
        };

        /**
         * @brief The progress bar direction.
         */
        enum class Direction
        {
            /**
             * @brief Start to end progressing.
             *
             * When the progress bar is in the Horizontal orientation, the bar moves
             * from the left to right.
             *
             * When the progress bar is in the Vertical orientation, the bar moves
             * from the bottom to top.
             */
            StartToEnd = 0,

            /**
             * @brief End to start progressing.
             *
             * When the progress bar is in the Horizontal orientation, the bar moves
             * from the right to the left.
             *
             * When the progress bar is in the Vertical orientation, the bar moves
             * from the top to the bottom.
             */
            EndToStart,
        };

        PI_WIDGET(ProgressBar, Label);

        /**
         * @brief Changes the progress bar orientation.
         *
         * @param orientation The progress bar orientation.
         */
        virtual void SetOrientation(Orientation orientation);

        /**
         * @brief Get the progress bar orientation.
         *
         * @return The progress bar orientation.
         */
        [[nodiscard]] virtual Orientation GetOrientation() const;

        /**
         * @brief Changes the progress bar direction.
         *
         * @param direction The progress bar direction.
         */
        virtual void SetDirection(Direction direction);

        /**
         * @brief Get the progress bar direction.
         *
         * @return The progress bar direction.
         */
        [[nodiscard]] virtual Direction GetDirection() const;

        /**
         * @brief Set the progress bar value range.
         *
         * @param range The progress bar value range.
         */
        virtual void SetRange(const Range<PiReal64>& range);

        /**
         * @brief Get the progress bar value range.
         *
         * @return The progress bar value range.
         */
        [[nodiscard]] virtual const Range<PiReal64>& GetRange() const;

        /**
         * @brief Set the progress level of the progress bar.
         *
         * @param progress The progress level.
         */
        virtual void SetProgress(PiReal64 progress);

        /**
         * @brief Get the progress level of the progress bar.
         *
         * @return The progress level.
         */
        [[nodiscard]] virtual PiReal64 GetProgress() const;

        /**
         * @brief Defines if the progress bar should automatically display
         * a label based on the progress level.
         *
         * If this option is enabled, label will display a completion percentage
         * in the form 00%. This has no effect on undetermined p
         *
         * @param value Whether to activate the auto label mode.
         */
        virtual void AutoLabel(bool value);

        /**
         * @brief Checks if the progress bar is automatically displaying a label
         * based on the progress level.
         *
         * @return Whether the auto label mode is enabled.
         */
        [[nodiscard]] virtual bool IsAutoLabelEnabled() const;

        /**
         * @brief Defines whether the progress bar should display a label.
         *
         * @param value Whether to display a label inside the progress bar.
         */
        virtual void ShowLabel(bool value);

        /**
         * @brief Checks if the progress bar is displaying a label.
         *
         * @return Whether the progress bar label is visible.
         */
        [[nodiscard]] virtual bool IsLabelVisible() const;

        /**
         * @brief Defines whether the progress bar has an undetermined progress
         * level.
         *
         * @param value Whether to make the progress bar in undetermined state.
         */
        virtual void SetUndeterminedProgress(bool value);

        /**
         * @brief Checks if the progress bar is in undetermined state.
         *
         * @return Whether the progress bar is in undetermined state.
         */
        [[nodiscard]] virtual bool IsUndetermined() const;

    protected:
        void Render(Skin* skin) override;

        Range<PiReal64> m_range;
        PiReal64 m_progress;

        Orientation m_orientation;
        Direction m_direction;
        bool m_autoLabel;
        bool m_showLabel;

        bool m_undetermined;

    private:
        PiReal64 _undeterminedCycleLevel;
    };
} // namespace SparkyStudios::UI::Pixel

#endif // PIXEL_UI_PROGRESSBAR_H
