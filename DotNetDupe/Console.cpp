#include "pch.h"
#include "System/Console.h"
#include <mutex>
#include <iostream>
#include <string>
#include <vector>

#if defined(_WIN32)
#include <windows.h>
#include "Win32Internal.h"
using namespace DotNetDupe::System::Internal;
#else
#include <unistd.h>
#endif

using namespace DotNetDupe::System;

namespace {
    static std::vector<String> s_outputs;
    static std::vector<String> s_inputs;
    static String s_accumulator = String("");
    static std::mutex s_mutex;
    static ConsoleColor s_defaultFore = ConsoleColor::Gray;
    static ConsoleColor s_defaultBack = ConsoleColor::Black;
    static bool s_colorsInitialized = false;

    void EnsureColorsInitialized() {
        if (!s_colorsInitialized) {
#if defined(_WIN32)
            CONSOLE_SCREEN_BUFFER_INFO csbi;
            if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
                s_defaultFore = (ConsoleColor)(csbi.wAttributes & 0x0F);
                s_defaultBack = (ConsoleColor)((csbi.wAttributes & 0xF0) >> 4);
            }
#endif
            s_colorsInitialized = true;
        }
    }
}

void InternalWrite(const String& sValue) {
    std::lock_guard<std::mutex> lk(s_mutex);
    s_accumulator = s_accumulator + sValue;
    std::cout << sValue.GetRawString() << std::flush;
}

void InternalWriteLine(const String& sValue) {
    std::lock_guard<std::mutex> lk(s_mutex);
    s_accumulator = s_accumulator + sValue;
    s_outputs.push_back(s_accumulator);
    std::cout << sValue.GetRawString() << std::endl;
    s_accumulator = String("");
}

void Console::Write(bool value) { InternalWrite(value ? String("True") : String("False")); }
void Console::Write(Char value) { char buf[2] = { value.GetChar(), 0 }; InternalWrite(String(buf)); }
void Console::Write(double value) { InternalWrite(String(std::to_string(value).c_str())); }
void Console::Write(int value) { InternalWrite(String(std::to_string(value).c_str())); }
void Console::Write(long value) { InternalWrite(String(std::to_string(value).c_str())); }
void Console::Write(long long value) { InternalWrite(String(std::to_string(value).c_str())); }
void Console::Write(float value) { InternalWrite(String(std::to_string(value).c_str())); }
void Console::Write(const String& value) { InternalWrite(value); }
void Console::Write(const char* value) { InternalWrite(String(value)); }

void Console::WriteLine() { InternalWriteLine(String("")); }
void Console::WriteLine(bool value) { InternalWriteLine(value ? String("True") : String("False")); }
void Console::WriteLine(Char value) { char buf[2] = { value.GetChar(), 0 }; InternalWriteLine(String(buf)); }
void Console::WriteLine(double value) { InternalWriteLine(String(std::to_string(value).c_str())); }
void Console::WriteLine(int value) { InternalWriteLine(String(std::to_string(value).c_str())); }
void Console::WriteLine(long value) { InternalWriteLine(String(std::to_string(value).c_str())); }
void Console::WriteLine(long long value) { InternalWriteLine(String(std::to_string(value).c_str())); }
void Console::WriteLine(float value) { InternalWriteLine(String(std::to_string(value).c_str())); }
void Console::WriteLine(const String& value) { InternalWriteLine(value); }
void Console::WriteLine(const char* value) { InternalWriteLine(String(value)); }

int Console::Read() {
    return std::cin.get();
}

String Console::ReadLine() {
    std::lock_guard<std::mutex> lk(s_mutex);
    if (!s_inputs.empty()) {
        String v = s_inputs.front();
        s_inputs.erase(s_inputs.begin());
        return v;
    }
    
    std::string line;
    std::getline(std::cin, line);
    return String(line.c_str());
}

int Console::GetCursorLeft() {
#if defined(_WIN32)
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        return csbi.dwCursorPosition.X;
    }
#endif
    return 0;
}

void Console::SetCursorLeft(int left) {
    SetCursorPosition(left, GetCursorTop());
}

int Console::GetCursorTop() {
#if defined(_WIN32)
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        return csbi.dwCursorPosition.Y;
    }
#endif
    return 0;
}

void Console::SetCursorTop(int top) {
    SetCursorPosition(GetCursorLeft(), top);
}

void Console::SetCursorPosition(int left, int top) {
#if defined(_WIN32)
    COORD coord = { (SHORT)left, (SHORT)top };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
#else
    // ANSI escape: \033[{row};{col}H (1-indexed)
    std::cout << "\033[" << (top + 1) << ";" << (left + 1) << "H" << std::flush;
#endif
}

bool Console::GetCursorVisible() {
#if defined(_WIN32)
    CONSOLE_CURSOR_INFO cci;
    if (GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci)) {
        return cci.bVisible != FALSE;
    }
#endif
    return true;
}

void Console::SetCursorVisible(bool visible) {
#if defined(_WIN32)
    CONSOLE_CURSOR_INFO cci;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (GetConsoleCursorInfo(hConsole, &cci)) {
        cci.bVisible = visible;
        SetConsoleCursorInfo(hConsole, &cci);
    }
#else
    if (visible) std::cout << "\033[?25h" << std::flush;
    else std::cout << "\033[?25l" << std::flush;
#endif
}

int Console::GetWindowHeight() {
#if defined(_WIN32)
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        return csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    }
#endif
    return 25; // Standard fallback
}

void Console::SetWindowHeight(int height) {
#if defined(_WIN32)
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        SMALL_RECT rect = csbi.srWindow;
        rect.Bottom = rect.Top + (SHORT)height - 1;
        SetConsoleWindowInfo(hConsole, TRUE, &rect);
    }
#endif
}

int Console::GetWindowWidth() {
#if defined(_WIN32)
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        return csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }
#endif
    return 80; // Standard fallback
}

void Console::SetWindowWidth(int width) {
#if defined(_WIN32)
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        SMALL_RECT rect = csbi.srWindow;
        rect.Right = rect.Left + (SHORT)width - 1;
        SetConsoleWindowInfo(hConsole, TRUE, &rect);
    }
#endif
}

bool Console::GetCapsLock() {
#if defined(_WIN32)
    return (GetKeyState(VK_CAPITAL) & 0x0001) != 0;
#endif
    return false;
}

bool Console::GetNumberLock() {
#if defined(_WIN32)
    return (GetKeyState(VK_NUMLOCK) & 0x0001) != 0;
#endif
    return false;
}

ConsoleColor Console::GetBackgroundColor() {
    EnsureColorsInitialized();
#if defined(_WIN32)
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        return (ConsoleColor)((csbi.wAttributes & 0xF0) >> 4);
    }
#endif
    return s_defaultBack;
}

void Console::SetBackgroundColor(ConsoleColor color) {
    EnsureColorsInitialized();
#if defined(_WIN32)
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        WORD attributes = (csbi.wAttributes & 0xFF0F) | (((WORD)color << 4) & 0x00F0);
        SetConsoleTextAttribute(hConsole, attributes);
    }
#endif
}

ConsoleColor Console::GetForegroundColor() {
    EnsureColorsInitialized();
#if defined(_WIN32)
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        return (ConsoleColor)(csbi.wAttributes & 0x0F);
    }
#endif
    return s_defaultFore;
}

void Console::SetForegroundColor(ConsoleColor color) {
    EnsureColorsInitialized();
#if defined(_WIN32)
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        WORD attributes = (csbi.wAttributes & 0xFFF0) | ((WORD)color & 0x000F);
        SetConsoleTextAttribute(hConsole, attributes);
    }
#endif
}

void Console::ResetColor() {
    EnsureColorsInitialized();
    SetForegroundColor(s_defaultFore);
    SetBackgroundColor(s_defaultBack);
}

String Console::GetTitle() {
#if defined(_WIN32)
    wchar_t title[MAX_PATH];
    if (GetConsoleTitleW(title, MAX_PATH)) {
        return String(WCharToUtf8(title).c_str());
    }
#endif
    return String("");
}

void Console::SetTitle(const String& title) {
#if defined(_WIN32)
    ::SetConsoleTitleW(Utf8ToWChar(title.GetRawString()).c_str());
#else
    std::cout << "\033]0;" << title.GetRawString() << "\007" << std::flush;
#endif
}

void Console::Beep() {
#if defined(_WIN32)
    ::Beep(800, 200);
#else
    std::cout << "\a" << std::flush;
#endif
}

void Console::Clear() {
    std::lock_guard<std::mutex> lk(s_mutex);
    s_outputs.clear();
    s_inputs.clear();
    s_accumulator = String("");

#if defined(_WIN32)
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD count;
    DWORD cellCount;
    COORD homeCoords = { 0, 0 };

    if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        cellCount = csbi.dwSize.X * csbi.dwSize.Y;
        if (FillConsoleOutputCharacterW(hConsole, (wchar_t) ' ', cellCount, homeCoords, &count)) {
            if (FillConsoleOutputAttribute(hConsole, csbi.wAttributes, cellCount, homeCoords, &count)) {
                SetConsoleCursorPosition(hConsole, homeCoords);
            }
        }
    }
#else
    std::cout << "\033[2J\033[1;1H" << std::flush;
#endif
}

void Console::SetIn(const String& value) {
    std::lock_guard<std::mutex> lk(s_mutex);
    s_inputs.push_back(value);
}

void Console::ClearInputs() {
    std::lock_guard<std::mutex> lk(s_mutex);
    s_inputs.clear();
}

Array<String> Console::GetOutputs() {
    std::lock_guard<std::mutex> lock(s_mutex);
    Array<String> result((int)s_outputs.size());
    for (int i = 0; i < (int)s_outputs.size(); i++) result[i] = s_outputs[i];
    return result;
}
