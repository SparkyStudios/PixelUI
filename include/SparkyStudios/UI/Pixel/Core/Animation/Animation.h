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

#ifndef PIXEL_UI_BASEANIMATION_H
#define PIXEL_UI_BASEANIMATION_H

#include <SparkyStudios/UI/Pixel/Core/Common.h>

#if PI_ENABLE_ANIMATION

#include <list>
#include <map>

namespace SparkyStudios::UI::Pixel
{
    class Widget;

    /**
     * @brief Base class for all animations.
     */
    class PI_EXPORT Animation
    {
    public:
        using List = std::list<Animation*>;
        using WidgetMap = std::map<Widget*, List>;

        /**
         * @brief Animation transition functions.
         */
        enum class TransitionFunction
        {
            Custom = -1,
            Linear,
            None = Linear,
            Ease,
            EaseIn,
            EaseOut,
            EaseInOut,
        };

        /**
         * @brief Create a custom animation transition function using
         * a one-dimensional cubic bezier curve.
         *
         * This use the exact same algorithm as in CSS. The first and last
         * control points of the cubic bezier curve are fixed to (0,0)
         * and (1,1) respectively.
         */
        struct Transition
        {
        public:
            /**
             * @brief Construct a new Transition curve.
             *
             * @param x1 The x coordinate of the second control point.
             * @param y1 The y coordinate of the second control point.
             * @param x2 The x coordinate of the third control point.
             * @param y2 The y coordinate of the third control point.
             */
            Transition(PiReal32 x1, PiReal32 y1, PiReal32 x2, PiReal32 y2);

            /**
             * @brief Given an animation duration percentage (in the range [0, 1]),
             * it calculates the animation progression percentage from the configured curve.
             *
             * @param t The animation duration percentage (in the range [0, 1]).
             *
             * @return The animation progress percentage (in the range [0, 1]).
             */
            PiTime Ease(PiTime t) const;

            /**
             * @brief The x coordinate of the second control point.
             */
            PiReal32 x1;

            /**
             * @brief The y coordinate of the second control point.
             */
            PiReal32 y1;

            /**
             * @brief The x coordinate of the third control point.
             */
            PiReal32 x2;

            /**
             * @brief The y coordinate of the third control point.
             */
            PiReal32 y2;

        private:
            PiTime GetTFromX(PiReal64 x) const;

            PiReal64 _samples[11];
        };

        /**
         * @brief Add a new animation to a widget.
         *
         * @param widget The widget to animate.
         * @param animation The animation.
         */
        static void Add(Widget* widget, Animation* animation);

        /**
         * @brief Cancels all animations assigned to a widget.
         *
         * This will let the widget at the state the animations has
         * set it before they has been cancelled.
         *
         * If the second parameter is not null, only that animation
         * will be cancelled, if it has been registered before on the
         * given widget.
         *
         * @param widget The widget to use when cancelling the animation.
         * @param animation The animation to cancel. Pass null to cancel all registered animations.
         */
        static void Cancel(Widget* widget, Animation* animation = nullptr);

        /**
         * @brief Update all animations from all widgets.
         *
         * @param time The total elapsed time since the start of the application.
         */
        static void Tick(PiTime time);

        /**
         * @brief Construct a new animation.
         *
         * @param duration The animation duration in seconds.
         * @param delay The animation delay in seconds.
         * @param ease The animation ease transition function.
         * @param loop Defines if the animation should loop.
         */
        Animation(PiTime duration, PiTime delay = 0.0f, TransitionFunction function = TransitionFunction::Linear, bool loop = false);

        virtual ~Animation() = default;

        /**
         * @brief Set a custom animation transition.
         *
         * @param curve The custom animation transition curve.
         */
        void SetTransition(const Transition& curve);

        /**
         * @brief Set the animation transition function.
         *
         * @param function The transition function.
         */
        void SetTransition(TransitionFunction function);

        /**
         * @brief Get the animation transition.
         *
         * @return The animation transition curve.
         */
        const Transition& GetTransition() const;

        /**
         * @brief Check if the animation has started.
         *
         * @return Whether the animation has started.
         */
        virtual bool Started();

        /**
         * @brief Trigerred when the animation starts.
         */
        virtual void OnStart() = 0;

        /**
         * @brief Runs the animation, providing the percentage of time
         * elapsed between the start and the end of the animation.
         *
         * @param percent The percentage of time elapsed since the start of the animation, in the range [0, 1].
         */
        virtual void Run(PiTime percent) = 0;

        /**
         * @brief Trigerred when the animation finishes.
         */
        virtual void OnFinish() = 0;

        /**
         * @brief Check if this animation is finished.
         *
         * @return Whether this animation is finished.
         */
        virtual bool Finished();

    protected:
        /**
         * @brief Starts the animation.
         *
         * @param time The total elapsed time since the start of the application.
         */
        virtual void Start(PiTime time);

        /**
         * @brief Process frames for this animation.
         *
         * @param time The total elapsed time since the start of the application.
         */
        virtual void Update(PiTime time);

        /**
         * @brief Finishes the animation.
         */
        virtual void Finish(PiTime time);

        /**
         * @brief The widget on which this animation is applied.
         */
        Widget* m_widget;

        /**
         * @brief Indicates if the animation has started.
         */
        bool m_started;

        /**
         * @brief Indicates if the animation has ended.
         */
        bool m_finished;

        /**
         * @brief The start time of the animation.
         */
        PiTime m_start;

        /**
         * @brief The animation duration in seconds.
         */
        PiTime m_duration;

        /**
         * @brief The animation start delay in seconds.
         */
        PiTime m_delay;

        /**
         * @brief The defined tramsition function.
         */
        TransitionFunction m_ease;

        /**
         * @brief The custom animation transition curve.
         *
         * This is only used when the animation transition function
         * is set to Custom.
         */
        Transition m_customCurve;

        /**
         * @brief The animation loop behavior.
         */
        bool m_loop;
    };
} // namespace SparkyStudios::UI::Pixel

#endif // PI_ENABLE_ANIMATION

#endif // PIXEL_UI_BASEANIMATION_H
