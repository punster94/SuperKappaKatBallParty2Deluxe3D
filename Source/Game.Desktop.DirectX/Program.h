#pragma once

void InitializeWindow(HINSTANCE instance, const std::wstring& className, const std::wstring windowTitle, int showCommand);
void InitializeDirectX();
void ShutdownDirectX();
void RenderGame();
LRESULT WINAPI WndProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam);