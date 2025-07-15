#pragma once  

#include "Number_generator.h"  
#include <windows.h>  
#include <string>  
#include <vector> 

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
   number_button(HWND hw, int x, int y, int width, int height, int ID, const std::string& txt) : id(ID), text(txt) // Fixed "string" to "std::string"  
   {  
       hwnd = CreateWindow("BUTTON", txt.c_str(), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  
           x, y, 70, 70, hw, HMENU(ID), 0, 0);  
   }  

   std::string getText() const { return text; }  

   int getId() const { return id; }  
};  

LRESULT CALLBACK window_proc(HWND hw, UINT msg, WPARAM wp, LPARAM lp);  

int register_class(HINSTANCE hi, const char* name);  

std::vector<std::vector<int>> create_solved_sudoku();  

void remove_numbers(std::vector<std::vector<int>>& grid, int k);  

void game_start(HWND hw, int mode);  

void reset_game(HWND hw);  

extern std::vector<std::vector<int>> solved_sudoku;  
extern std::vector<sudoku_button> sudoku_buttons;  
extern std::vector<number_button> number_buttons;  

extern int selected_sudoku_id;  
extern int selected_number_id;  
extern int mistakes;  
extern bool notes_on;  
extern bool rect_drawn;  

extern HWND e_button;  
extern HWND n_button;  
extern HWND h_button;  

extern HFONT button_font;  
extern HFONT notes_font;
