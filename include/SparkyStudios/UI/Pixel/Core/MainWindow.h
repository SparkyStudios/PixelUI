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

#ifndef PIXEL_UI_MAINWINDOW_H
#define PIXEL_UI_MAINWINDOW_H

#include <SparkyStudios/UI/Pixel/Config/Config.h>
#include <SparkyStudios/UI/Pixel/Config/Types.h>

#include <SparkyStudios/UI/Pixel/Core/Input/Mouse.h>
#include <SparkyStudios/UI/Pixel/Core/Renderer/Skin.h>
#include <SparkyStudios/UI/Pixel/Core/Resource.h>

#include <SparkyStudios/UI/Pixel/Graphics/Point.h>
#include <SparkyStudios/UI/Pixel/Graphics/Rect.h>

#include <SparkyStudios/UI/Pixel/Widgets/Canvas.h>

namespace SparkyStudios::UI::Pixel
{
    enum MAIN_WINDOW_FLAGS
    {
        MAIN_WINDOW_WINDOWED = (1 << 1),
        MAIN_WINDOW_FULLSCREEN = (1 << 2),
        MAIN_WINDOW_RESIZABLE = (1 << 3),
        MAIN_WINDOW_FRAMELESS = (1 << 4),
        MAIN_WINDOW_MINIMIZED = (1 << 5),
        MAIN_WINDOW_MAXIMIZED = (1 << 6),
    };

    class PI_EXPORT MainWindow
    {
        friend class Application;

    public:
        MainWindow(PiInt32 x, PiInt32 y, PiUInt32 width, PiUInt32 height, const PiString& title, int flags);
        MainWindow(PiUInt32 width, PiUInt32 height, const PiString& title, int flags);
        MainWindow(const Rect& rect, const PiString& title, int flags);
        MainWindow(const Size& size, const PiString& title, int flags);

        ~MainWindow();

        void CreateNativeWindow();
        [[nodiscard]] PiVoidPtr GetNativeHandle() const;

        void CreateRootCanvas(Skin* skin);
        [[nodiscard]] const std::unique_ptr<Canvas>& GetRootCanvas() const;

        void SetTitle(const PiString& title);
        [[nodiscard]] const PiString& GetTitle() const;

        void Maximize();
        void Minimize();

        void SetSize(const Size& size);
        [[nodiscard]] const Size& GetSize() const;

        [[nodiscard]] PiInt32 GetWidth() const;
        [[nodiscard]] PiInt32 GetHeight() const;

        void SetPosition(const Point& position);
        [[nodiscard]] const Point& GetPosition() const;

        [[nodiscard]] Cursor* GetCursor() const;
        void SetCursorStyle(CursorStyle style);

        bool FileOpen(const PiString& name, const PiString& startPath, const PiString& extension, PiString& filePathOut);
        bool FileSave(const PiString& name, const PiString& startPath, const PiString& extension, PiString& filePathOut);
        bool FolderOpen(const PiString& name, const PiString& startPath, PiString& filePathOut);

    protected:
        virtual void OnResize(const Size& newSize);
        virtual void OnExpose();

    private:
        void Paint(Skin* skin);

        PiVoidPtr _nativeHandle;
        int _flags;

        PiString _title;
        Point _position;
        Size _size;

        Cursor* _cursor;

        std::unique_ptr<Canvas> _rootCanvas;
    };
} // namespace SparkyStudios::UI::Pixel

#endif // PIXEL_UI_MAINWINDOW_H
