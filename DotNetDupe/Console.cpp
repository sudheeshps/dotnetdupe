#include "pch.h"
#include "System/Console.h"
#include <mutex>
#include <iostream>
#include <string>

#ifdef UNICODE
#define TO_TSTRING std::to_wstring
#else
#define TO_TSTRING std::to_string
#endif

using namespace DotNetDupe::System;

namespace {
    static std::vector<String> s_outputs;
    static std::vector<String> s_inputs;
    static String s_accumulator = String(_T(""));
    static std::mutex s_mutex;
    static ConsoleColor s_defaultFore = ConsoleColor::Gray;
    static ConsoleColor s_defaultBack = ConsoleColor::Black;
    static bool s_colorsInitialized = false;

    void EnsureColorsInitialized() {
        if (!s_colorsInitialized) {
            CONSOLE_SCREEN_BUFFER_INFO csbi;
            if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
                s_defaultFore = (ConsoleColor)(csbi.wAttributes & 0x0F);
                s_defaultBack = (ConsoleColor)((csbi.wAttributes & 0xF0) >> 4);
            }
            s_colorsInitialized = true;
        }
    }
}

void InternalWrite(const String& value) {
    std::lock_guard<std::mutex> lk(s_mutex);
    s_accumulator = s_accumulator + value;
#ifdef UNICODE
    std::wcout << value;
#else
    std::cout << value;
#endif
}

void InternalWriteLine(const String& value) {
    std::lock_guard<std::mutex> lk(s_mutex);
    s_accumulator = s_accumulator + value;
    s_outputs.push_back(s_accumulator);
#ifdef UNICODE
    std::wcout << value << std::endl;
#else
    std::cout << value << std::endl;
#endif
    s_accumulator = String(_T(""));
}

void Console::Write(bool value) { InternalWrite(value ? String(_T("True")) : String(_T("False"))); }
void Console::Write(Char value) { TCHAR buf[2] = { value.GetChar(), 0 }; InternalWrite(String(buf)); }
void Console::Write(double value) { InternalWrite(String(TO_TSTRING(value).c_str())); }
void Console::Write(int value) { InternalWrite(String(TO_TSTRING(value).c_str())); }
void Console::Write(long value) { InternalWrite(String(TO_TSTRING(value).c_str())); }
void Console::Write(long long value) { InternalWrite(String(TO_TSTRING(value).c_str())); }
void Console::Write(float value) { InternalWrite(String(TO_TSTRING(value).c_str())); }
void Console::Write(const String& value) { InternalWrite(value); }
void Console::Write(const TCHAR* value) { InternalWrite(String(value)); }

void Console::WriteLine() { InternalWriteLine(String(_T(""))); }
void Console::WriteLine(bool value) { InternalWriteLine(value ? String(_T("True")) : String(_T("False"))); }
void Console::WriteLine(Char value) { TCHAR buf[2] = { value.GetChar(), 0 }; InternalWriteLine(String(buf)); }
void Console::WriteLine(double value) { InternalWriteLine(String(TO_TSTRING(value).c_str())); }
void Console::WriteLine(int value) { InternalWriteLine(String(TO_TSTRING(value).c_str())); }
void Console::WriteLine(long value) { InternalWriteLine(String(TO_TSTRING(value).c_str())); }
void Console::WriteLine(long long value) { InternalWriteLine(String(TO_TSTRING(value).c_str())); }
void Console::WriteLine(float value) { InternalWriteLine(String(TO_TSTRING(value).c_str())); }
void Console::WriteLine(const String& value) { InternalWriteLine(value); }
void Console::WriteLine(const TCHAR* value) { InternalWriteLine(String(value)); }

int Console::Read() {
#ifdef UNICODE
    return std::wcin.get();
#else
    return std::cin.get();
#endif
}

String Console::ReadLine() {
    std::lock_guard<std::mutex> lk(s_mutex);
    if (!s_inputs.empty()) {
        String v = s_inputs.front();
        s_inputs.erase(s_inputs.begin());
        return v;
    }
    
    std::basic_string<TCHAR> line;
#ifdef UNICODE
    std::getline(std::wcin, line);
#else
    std::getline(std::cin, line);
#endif
    return String(line.c_str());
}

int Console::GetCursorLeft() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        return csbi.dwCursorPosition.X;
    }
    return 0;
}

void Console::SetCursorLeft(int left) {
    SetCursorPosition(left, GetCursorTop());
}

int Console::GetCursorTop() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        return csbi.dwCursorPosition.Y;
    }
    return 0;
}

void Console::SetCursorTop(int top) {
    SetCursorPosition(GetCursorLeft(), top);
}

void Console::SetCursorPosition(int left, int top) {
    COORD coord = { (SHORT)left, (SHORT)top };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

bool Console::GetCursorVisible() {
    CONSOLE_CURSOR_INFO cci;
    if (GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci)) {
        return cci.bVisible != FALSE;
    }
    return true;
}

void Console::SetCursorVisible(bool visible) {
    CONSOLE_CURSOR_INFO cci;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (GetConsoleCursorInfo(hConsole, &cci)) {
        cci.bVisible = visible;
        SetConsoleCursorInfo(hConsole, &cci);
    }
}

int Console::GetWindowHeight() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        return csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    }
    return 0;
}

void Console::SetWindowHeight(int height) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        SMALL_RECT rect = csbi.srWindow;
        rect.Bottom = rect.Top + (SHORT)height - 1;
        SetConsoleWindowInfo(hConsole, TRUE, &rect);
    }
}

int Console::GetWindowWidth() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        return csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }
    return 0;
}

void Console::SetWindowWidth(int width) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        SMALL_RECT rect = csbi.srWindow;
        rect.Right = rect.Left + (SHORT)width - 1;
        SetConsoleWindowInfo(hConsole, TRUE, &rect);
    }
}

bool Console::GetCapsLock() {
    return (GetKeyState(VK_CAPITAL) & 0x0001) != 0;
}

bool Console::GetNumberLock() {
    return (GetKeyState(VK_NUMLOCK) & 0x0001) != 0;
}

ConsoleColor Console::GetBackgroundColor() {
    EnsureColorsInitialized();
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        return (ConsoleColor)((csbi.wAttributes & 0xF0) >> 4);
    }
    return s_defaultBack;
}

void Console::SetBackgroundColor(ConsoleColor color) {
    EnsureColorsInitialized();
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        WORD attributes = (csbi.wAttributes & 0xFF0F) | (((WORD)color << 4) & 0x00F0);
        SetConsoleTextAttribute(hConsole, attributes);
    }
}

ConsoleColor Console::GetForegroundColor() {
    EnsureColorsInitialized();
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        return (ConsoleColor)(csbi.wAttributes & 0x0F);
    }
    return s_defaultFore;
}

void Console::SetForegroundColor(ConsoleColor color) {
    EnsureColorsInitialized();
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        WORD attributes = (csbi.wAttributes & 0xFFF0) | ((WORD)color & 0x000F);
        SetConsoleTextAttribute(hConsole, attributes);
    }
}

void Console::ResetColor() {
    EnsureColorsInitialized();
    SetForegroundColor(s_defaultFore);
    SetBackgroundColor(s_defaultBack);
}

String Console::GetTitle() {
    TCHAR title[MAX_PATH];
    if (GetConsoleTitle(title, MAX_PATH)) {
        return String(title);
    }
    return String(_T(""));
}

void Console::SetTitle(const String& title) {
    SetConsoleTitle(title);
}

void Console::Beep() {
    ::Beep(800, 200);
}

void Console::Clear() {
    std::lock_guard<std::mutex> lk(s_mutex);
    s_outputs.clear();
    s_inputs.clear();
    s_accumulator = String(_T(""));

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD count;
    DWORD cellCount;
    COORD homeCoords = { 0, 0 };

    if (GetConsoleScreenBufferInfo(hConsole, &csbi)) {
        cellCount = csbi.dwSize.X * csbi.dwSize.Y;
        if (FillConsoleOutputCharacter(hConsole, (TCHAR) ' ', cellCount, homeCoords, &count)) {
            if (FillConsoleOutputAttribute(hConsole, csbi.wAttributes, cellCount, homeCoords, &count)) {
                SetConsoleCursorPosition(hConsole, homeCoords);
            }
        }
    }
}

void Console::SetIn(const String& value) {
    std::lock_guard<std::mutex> lk(s_mutex);
    s_inputs.push_back(value);
}

std::vector<String> Console::GetOutputs() {
    std::lock_guard<std::mutex> lk(s_mutex);
    return s_outputs;
}
