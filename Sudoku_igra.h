#pragma once  

#include "Number_generator.h"  
#include <windows.h>  
#include <string>  
#include <tchar.h>

using tstring = std::basic_string<TCHAR>;

class application
{
public:
	int run();
};

class sudoku_button  
{  
private:  
   bool entered_number = false;  
   bool clicked_button = false;  
   bool number_highlighted = false;  
   HWND hwnd;  
   int id;  
   std::string text;  
   std::vector<std::string> note_numbers;  
public:  
   sudoku_button(HWND hw, int x, int y, int w, int h, int ID, const std::string& txt) : id(ID), text(txt) // Fixed "string" to "std::string"  
   {  
       hwnd = CreateWindow("BUTTON", "", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,  
           x, y, 70, 70, hw, HMENU(ID), 0, 0);  

       note_numbers = std::vector<std::string>(9, "");  
   }  

   void setText(const std::string& txt)  
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

   void setNoteNumbers(std::vector<std::string> nn)  
   {  
       note_numbers = nn;  
       InvalidateRect(hwnd, 0, true);  
   }  

   std::string getText() const { return text; }  

   bool getEnteredNumber() const { return entered_number; }  

   bool getClickedButton() const { return clicked_button; }  

   bool getNumberHighlighted() const { return number_highlighted; }  

   int getId() const { return id; }  

   std::vector<std::string> getNoteNumbers() const { return note_numbers; }  
};  

class number_button  
{  
private:  
   HWND hwnd;  
   int id;  
   std::string text;  
public:  
   number_button(HWND hw, int x, int y, int width, int height, int ID, const std::string& txt) : id(ID), text(txt)
   {  
       hwnd = CreateWindow("BUTTON", txt.c_str(), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  
           x, y, 70, 70, hw, HMENU(ID), 0, 0);  
   }  

   std::string getText() const { return text; }  

   int getId() const { return id; }  
}; 

class window
{
protected:
    HWND hw{ 0 };
    virtual tstring class_name();
    bool register_class(const tstring& name);
    tstring generate_class_name();
    void game_start(HWND hw, int mode);
    void reset_game(HWND hw);

    std::vector<std::vector<int>> solved_sudoku;
    std::vector<sudoku_button> sudoku_buttons;
    std::vector<number_button> number_buttons;

    int selected_sudoku_id = -1;
    int selected_number_id = -1;
    int mistakes = 0;
    bool notes_on = false;
    bool rect_drawn = false;

    HWND e_button;
    HWND n_button;
    HWND h_button;

    HFONT button_font = CreateFont(-30, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT("Times New Roman"));
    HFONT notes_font = CreateFont(14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT("Times New Roman"));

public:
    bool create(HWND parent, DWORD style, LPCTSTR caption = 0, UINT_PTR id_or_menu = 0,
        int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int width = CW_USEDEFAULT, int height = CW_USEDEFAULT);

    operator HWND();
    static LRESULT CALLBACK proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

protected:
    virtual int on_create(CREATESTRUCT*) { return 0; }
    virtual void on_command(int) {}
    virtual void on_drawitem(LPARAM lp) {}
    virtual void on_destroy() {}
    virtual void on_paint(HWND hw) {}
};

std::vector<std::vector<int>> create_solved_sudoku();  

void remove_numbers(std::vector<std::vector<int>>& grid, int k);  