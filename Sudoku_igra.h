#pragma once

#include <windows.h>
#include <string>

class sudoku_button
{
private:
    bool entered_number = false;
    bool clicked_button = false;
    bool number_highlighted = false;
    bool not_same_number = false;
	HWND hwnd = nullptr;
	int id;
	string text;
public:
    sudoku_button(HWND hw, int x, int y, int w, int h, int ID, const string& txt): id(ID), text(txt)
    {
        hwnd = CreateWindow("BUTTON", "", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
            x, y, 70, 70, hw, HMENU(ID), 0, 0);
    }

    void setText(const string& txt)
    {
        text = txt;
        InvalidateRect(hwnd, nullptr, TRUE);
    }

    void setEnteredNumber(bool en)
    {
        entered_number = en;
        InvalidateRect(hwnd, nullptr, TRUE);
    }

    void setClickedButton(bool cb)
    {
        clicked_button = cb;
        InvalidateRect(hwnd, nullptr, TRUE);
    }

    void setNumberHighlighted(bool nh)
    {
        number_highlighted = nh;
        InvalidateRect(hwnd, nullptr, TRUE);
    }

    void setNotSameNumber(bool sn)
    {
        not_same_number = sn;
        InvalidateRect(hwnd, nullptr, TRUE);
    }

    bool getEnteredNumber() const { return entered_number; }

    bool getClickedButton() const { return clicked_button; }

    bool getNumberHighlighted() const { return number_highlighted; }

    bool getNotSameNumber() const { return not_same_number; }

    string getText() { return text; }

    HWND getHWND() { return hwnd; }

    int getId() const { return id; }

};

class number_button
{
private:
    HWND hwnd = nullptr;
    int id;
    string text;
public:
    number_button(HWND hw, int x, int y, int width, int height, int ID, const string& txt): id(ID), text(txt)
    {
        hwnd = CreateWindow("BUTTON", txt.c_str(), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            x, y, 70, 70, hw, HMENU(ID), 0, 0);
    }

    string getText() { return text; }
    
    HWND getHWND() { return hwnd; }

};

vector<vector<int>> createSolvedSudoku();

void remove_numbers(vector<vector<int>>& grid, int k);

void game_start(HWND hw, int mode);

