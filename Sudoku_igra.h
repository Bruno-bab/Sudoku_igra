#pragma once

#include <windows.h>
#include <string>

class sudoku_button
{
private:
    bool entered_number = false;
    bool clicked_button = false;
    bool number_highlighted = false;
	HWND hwnd;
	int id;
	string text;
    vector<string> note_numbers;
public:
    sudoku_button(HWND hw, int x, int y, int w, int h, int ID, const string& txt): id(ID), text(txt)
    {
        hwnd = CreateWindow("BUTTON", "", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
            x, y, 70, 70, hw, HMENU(ID), 0, 0);

        note_numbers = std::vector<std::string>(9, "");
    }

    void setText(const string& txt)
    {
        text = txt;
        InvalidateRect(hwnd, 0, true);
    }

    void setEnteredNumber(bool en)
    {
        entered_number = en;
        InvalidateRect(hwnd, 0, true);
    }

    void setClickedButton(bool cb)
    {
        clicked_button = cb;
        InvalidateRect(hwnd, 0, true);
    }

    void setNumberHighlighted(bool nh)
    {
        number_highlighted = nh;
        InvalidateRect(hwnd, 0, true);
    }

    void setNoteNumbers (vector<string> nn)
    {
        note_numbers = nn;
        InvalidateRect(hwnd, 0, true);
    }

    string getText() const { return text; }

    bool getEnteredNumber() const { return entered_number; }

    bool getClickedButton() const { return clicked_button; }

    bool getNumberHighlighted() const { return number_highlighted; }

    int getId() const { return id; }

    vector<string> getNoteNumbers() const { return note_numbers; }
};

class number_button
{
private:
    HWND hwnd;
    int id;
    string text;
public:
    number_button(HWND hw, int x, int y, int width, int height, int ID, const string& txt): id(ID), text(txt)
    {
        hwnd = CreateWindow("BUTTON", txt.c_str(), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            x, y, 70, 70, hw, HMENU(ID), 0, 0);
    }

    string getText() const { return text; }
    
    int getId() const { return id; }
};

vector<vector<int>> createSolvedSudoku();

void remove_numbers(vector<vector<int>>& grid, int k);

void game_start(HWND hw, int mode);

void reset_game(HWND hw);

