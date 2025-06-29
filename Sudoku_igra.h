#pragma once

#include <windows.h>
#include <string>

class sudoku_button
{
private:
    bool is_highlighted = false;
	HWND hwnd = nullptr;
	int id;
	string text;
public:
    sudoku_button(HWND hw, int x, int y, int w, int h, int controlId, const string& txt)
        : id(controlId), text(txt)
    {
        hwnd = CreateWindow("BUTTON", txt.c_str(), WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_PUSHLIKE, x, y, 70, 70, hw, HMENU(controlId), 0, 0);
    }

    void setText(const string& txt) {
        text = txt;
        SetWindowText(hwnd, text.c_str());
    }

    void setHighlighted(bool state) { is_highlighted = state; }

    bool isHighlighted() const { return is_highlighted; }

    string getText() { return text; }

    HWND getHWND() { return hwnd; }

    void Enable(bool enable = true) { EnableWindow(hwnd, enable); }

    void Show(bool show = true) { ShowWindow(hwnd, show ? SW_SHOW : SW_HIDE); }

};

class number_button
{
private:
    HWND hwnd = nullptr;
    int id;
    string label;
public:
    number_button(HWND hw, int x, int y, int width, int height, int controlId, const string& text): id(controlId), label(text)
    {
        hwnd = CreateWindow("BUTTON", text.c_str(), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            x, y, 70, 70, hw, HMENU(controlId), 0, 0);
    }

    string getText() { return label; }
    
    HWND getHWND() { return hwnd; }

};

