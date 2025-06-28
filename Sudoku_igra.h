#pragma once

#include <windows.h>
#include <string>

class number_button
{
private:
	bool is_revealed;
	int revealed_number;
	int unrevealed_number;
	HWND hwnd = nullptr;
	int id;
	std::string label;
public:
    number_button(HWND hw, int x, int y, int width, int height, int controlId, const std::string& text)
        : id(controlId), label(text)
    {
        hwnd = CreateWindow("BUTTON", text.c_str(), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, x, y, 70, 70, hw, HMENU(controlId), 0, 0);
    }

    void SetText(const std::string& text) {
        label = text;
        SetWindowText(hwnd, text.c_str());
    }

    bool getIs_reveald() { return is_revealed; }

    void Enable(bool enable = true) { EnableWindow(hwnd, enable); }

    void Show(bool show = true) { ShowWindow(hwnd, show ? SW_SHOW : SW_HIDE); }

};

