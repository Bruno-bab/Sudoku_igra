#pragma once  

#include "Resource.h"
#include "Number_generator.h"  
#include <windows.h>  
#include <string>  
#include <tchar.h>
#include <list>

using tstring = std::basic_string<TCHAR>;

class application
{
public:
	int run();
};

class sudoku_button  
{   
    friend class window;
    friend class main_window;
private:  
   bool entered_number = false;  
   bool clicked_button = false;  
   bool number_highlighted = false;  
   HWND hwnd;  
   int id;  
   std::string text;  
   std::vector<std::string> note_numbers; 
   RECT rect;

   void setText(std::string txt)  
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

   void setRect(RECT r) { rect = r; }

   std::string getText() const { return text; }  

   bool getEnteredNumber() const { return entered_number; }  

   bool getClickedButton() const { return clicked_button; }  

   bool getNumberHighlighted() const { return number_highlighted; }  

   int getId() const { return id; }  

   std::vector<std::string> getNoteNumbers() const { return note_numbers; }  

   HWND getHWND() const { return hwnd; }

   RECT getRect() const { return rect; }

   bool contains(POINT p) { return PtInRect(&rect, p); }

public:
    sudoku_button(int ID, const std::string& txt) : id(ID), text(txt)
    {
        note_numbers = std::vector<std::string>(9, "");
    }
};  

class number_button  
{  
private:  
   HWND hwnd;  
   int id;  
   std::string text;  
public:  
   number_button(HWND hw, int ID, const std::string& txt) : id(ID), text(txt)
   {  
       hwnd = CreateWindow("BUTTON", txt.c_str(), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  
           0, 0, 0, 0, hw, HMENU(ID), 0, 0);  
   }  

   std::string getText() const { return text; }  

   int getId() const { return id; }  
}; 

class window
{
protected:
    std::list<POINT> points;
    HWND hw{ 0 };
    virtual tstring class_name();
    bool register_class(const tstring& name);
    tstring generate_class_name();
    void game_start(HWND hw, int numbers_to_remove);
    void reset_game(HWND hw);
    void create_buttons();
    void paint_sudoku_buttons(HDC hdc);
    void input_number();
    void input_note();
    void number_button_clicked(int id);
    void delete_button_clicked();
    void solve_button_clicked();
    void difficulty_button_clicked(int difficulty);
    void scale_buttons(int w, int h, HFONT scaled_font);
    void scale_sudoku_rectangles(int w, int h, int cell_size);

    std::vector<std::vector<int>> create_solved_sudoku();
    void remove_numbers(std::vector<std::vector<int>>& grid, int k);
    int create_mode(int mode);

    std::vector<std::vector<int>> solved_sudoku;
    std::vector<sudoku_button> sudoku_buttons;
    std::vector<number_button> number_buttons;

    const int game_size = 81;
    int selected_sudoku_id = -1;
    int selected_number_id = -1;
    int mistakes = 0;
    bool notes_on = false;
    bool rect_drawn = false;

    TCHAR w_text[100];

    HWND e_button;
    HWND n_button;
    HWND h_button;
    HWND delete_button;
    HWND solve_button;
    HWND reset_button;
    HWND notes_button;
    HWND mistakes_text;

    HFONT button_font;
    HFONT notes_font;

public:
    bool create(HWND parent, DWORD style, LPCTSTR caption = 0, UINT_PTR id_or_menu = 0, int x = CW_USEDEFAULT,
        int y = CW_USEDEFAULT, int width = CW_USEDEFAULT, int height = CW_USEDEFAULT);

    operator HWND();
    static LRESULT CALLBACK proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
    static std::basic_string<TCHAR> load_text(int id);

protected:
    virtual int on_create(CREATESTRUCT*) { return 0; }
    virtual void on_command(int) {}
    virtual void on_destroy() {}
    virtual void on_size(int w, int h) {}
    virtual void on_paint(HDC hdc) {}
    virtual void on_left_button_down(POINT) {}
};