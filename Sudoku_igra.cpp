#include "Sudoku_igra.h"
#include <windowsx.h>

int application::run()
{
	MSG msg;
	while (::GetMessage(&msg, NULL, 0, 0)) {
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}

tstring window::class_name()
{
	return {};
}

bool window::register_class(const tstring& name)
{
	WNDCLASS wc{};
	wc.lpfnWndProc = proc;
	wc.lpszClassName = name.c_str();
	wc.cbWndExtra = sizeof(window*);

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);

	return ::RegisterClass(&wc) != 0;
}

tstring window::generate_class_name()
{
	static int n = 1;
	return load_text(IDS_APP_TITLE).c_str() + std::to_string(n++);
}

POINT get_point(LPARAM lp)
{
	POINT p;
	p.x = GET_X_LPARAM(lp);
	p.y = GET_Y_LPARAM(lp);
	return p;
}

bool window::create(HWND parent, DWORD style, LPCTSTR caption, UINT_PTR id_or_menu,
	int x, int y, int width, int height)
{
	tstring cn = class_name();
	if (cn.empty())
		register_class(cn = generate_class_name());
	hw = ::CreateWindow(cn.c_str(), caption, style, x, y, width, height, parent, (HMENU)id_or_menu, 0, this);
	return hw != 0;
}

window::operator HWND()
{
	return hw;
}

LRESULT CALLBACK window::proc(HWND hw, UINT msg, WPARAM wp, LPARAM lp)  
{  
   if (msg == WM_CREATE) {  
       CREATESTRUCT* pcs = reinterpret_cast<CREATESTRUCT*>(lp);  
       window* pw = reinterpret_cast<window*>(pcs->lpCreateParams);  
       ::SetWindowLongPtr(hw, 0, reinterpret_cast<LONG_PTR>(pw));  
       pw->hw = hw;  
       return pw->on_create(pcs);  
   }  

   window* pw = reinterpret_cast<window*>(::GetWindowLongPtr(hw, 0));  
   switch (msg)  
   {  
   case WM_CREATE:  
       pw->on_create(reinterpret_cast<CREATESTRUCT*>(lp));  
       return 0;
   case WM_PAINT:
   {
	   PAINTSTRUCT ps;
	   HDC hdc = ::BeginPaint(hw, &ps);
	   pw->on_paint(hdc);
	   ::EndPaint(hw, &ps);
	   return 0;
   }
   case WM_COMMAND:  
       pw->on_command(LOWORD(wp));  
       return 0;
   case WM_SIZE:
	   pw->on_size(LOWORD(lp), HIWORD(lp));
	   return 0;
   case WM_LBUTTONDOWN:
	   pw->on_left_button_down(get_point(lp));
	   return 0;
   case WM_DESTROY:
	   pw->on_destroy();
	   return 0;
   }  
   return ::DefWindowProc(hw, msg, wp, lp);  
}

//generira rijesenu sudoku igru
std::vector<std::vector<int>> window:: create_solved_sudoku()
{
	srand(time(0));
	std::vector<std::vector<int>> solved_sudoku = sudokuGenerator(0);
	return solved_sudoku;
}
//uklanja k brojeva iz sudoku igre
void window::remove_numbers(std::vector<std::vector<int>>& grid, int k)
{
	int count = 0;
	while (count < k)
	{
		int row = rand() % 9;
		int col = rand() % 9;
		if (grid[row][col] != 0)
		{
			grid[row][col] = 0;
			count++;
		}
	}
}

void window::game_start(HWND hw, int numbers_to_remove)
{
	solved_sudoku = create_solved_sudoku();
	std::vector<std::vector<int>> unsolved_sudoku = solved_sudoku;
	remove_numbers(unsolved_sudoku, numbers_to_remove);

	sudoku_buttons.reserve(game_size);
	
	for (int i = 0; i < game_size; i++)
	{
		int row = i / 9;
		int col = i % 9;
		if (unsolved_sudoku[row][col] != 0)
			sudoku_buttons.emplace_back(i, std::to_string(unsolved_sudoku[row][col]));
		else
			sudoku_buttons.emplace_back(i, "");
	}

	//kreiranje gumba za brojeve
	for (int i = 0; i < 9; i++)
		number_buttons.emplace_back(hw, IDC_BTN_NUMBER + i, std::to_string(i + 1));

	create_buttons();

	RECT r;
	GetClientRect(hw, &r);
	int w = r.right - r.left;
	int h = r.bottom - r.top;
	on_size(w, h);
}
//brise sve gumbe, stavlja vrijednosti na pocetno stanje i resetira igru
void window::reset_game(HWND hw)
{
	for (int i = 0; i < sudoku_buttons.size(); i++)
		DestroyWindow(GetDlgItem(hw, sudoku_buttons[i].getId()));
	sudoku_buttons.clear();

	for (int i = 0; i < 9; i++)
		DestroyWindow(GetDlgItem(hw, number_buttons[i].getId()));
	number_buttons.clear();

	for (int i = 110; i < 115; i++)
		DestroyWindow(GetDlgItem(hw, i));

	selected_sudoku_id = -1;
	selected_number_id = -1;
	mistakes = 0;
	notes_on = false;
	rect_drawn = false;

	ShowWindow(e_button, SW_SHOW);
	ShowWindow(n_button, SW_SHOW);
	ShowWindow(h_button, SW_SHOW);

	InvalidateRect(hw, 0, true);
	UpdateWindow(hw);
}

std::basic_string<TCHAR> window::load_text(int id)
{
    TCHAR text[100];
    LoadString(GetModuleHandle(NULL), id, text, sizeof(text)/sizeof(TCHAR));
    return text;  
}

//uklanja brojeve iz sudoku igre ovisno o odabranoj težini
int window::create_mode(int mode)
{
	srand(time(0));
	int numbers_to_remove;
	if (mode == 0)
		numbers_to_remove = rand() % 10 + 29;
	else if (mode == 1)
		numbers_to_remove = rand() % 10 + 40;
	else if (mode == 2)
		numbers_to_remove = rand() % 10 + 51;

	return numbers_to_remove;
}

void window::create_buttons()
{
	delete_button = CreateWindow("BUTTON", load_text(IDS_BTN_DELETE).c_str(), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		0, 0, 0, 0, hw, HMENU(IDS_BTN_DELETE), 0, 0);

	solve_button = CreateWindow("BUTTON", load_text(IDS_BTN_SOLVE).c_str(), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		0, 0, 0, 0, hw, HMENU(IDS_BTN_SOLVE), 0, 0);

	reset_button = CreateWindow("BUTTON", load_text(IDS_BTN_RESET).c_str(), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		0, 0, 0, 0, hw, HMENU(IDS_BTN_RESET), 0, 0);

	notes_button = CreateWindow("BUTTON", load_text(IDS_BTN_NOTES_OFF).c_str(), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		0, 0, 0, 0, hw, HMENU(IDS_BTN_NOTES_OFF), 0, 0);

	mistakes_text = CreateWindow("STATIC", load_text(IDS_TXT_MISTAKES).c_str(), WS_CHILD | WS_VISIBLE | SS_LEFT,
		0, 0, 0, 0, hw, HMENU(IDS_TXT_MISTAKES), 0, 0);
}

void window::paint_sudoku_buttons(HDC hdc)
{
	for (int i = 0; i < sudoku_buttons.size(); ++i)
	{
		RECT r = sudoku_buttons[i].getRect();

		COLORREF button_color = RGB(240, 240, 240);
		if (sudoku_buttons[i].getClickedButton())
			button_color = RGB(180, 180, 240);
		else if (sudoku_buttons[i].getNumberHighlighted())
			button_color = RGB(210, 210, 240);

		HBRUSH brush = CreateSolidBrush(button_color);
		FillRect(hdc, &r, brush);
		DeleteObject(brush);
		FrameRect(hdc, &r, (HBRUSH)GetStockObject(BLACK_BRUSH));
		SetBkMode(hdc, TRANSPARENT);

		//mijenjanje boje teksta ovisno da li je upisan tocan broj
		HFONT font;
		if (sudoku_buttons[i].getText().size() == 1)
		{
			font = (HFONT)SelectObject(hdc, button_font);

			if (sudoku_buttons[i].getText() != std::to_string(solved_sudoku[i / 9][i % 9]))
				SetTextColor(hdc, RGB(255, 0, 0));
			else if (sudoku_buttons[i].getEnteredNumber())
				SetTextColor(hdc, RGB(0, 0, 255));
			else
				SetTextColor(hdc, RGB(0, 0, 0));
		}
		else
		{
			font = (HFONT)SelectObject(hdc, notes_font);
			SetTextColor(hdc, RGB(0, 0, 0));
		}
		//centriranje teksta
		RECT cen = r;
		DrawText(hdc, sudoku_buttons[i].getText().c_str(), -1, &cen,
			DT_CENTER | DT_WORDBREAK | DT_CALCRECT | DT_NOPREFIX);

		int vertical_offset = (r.bottom - r.top - (cen.bottom - cen.top)) / 2;
		r.top += vertical_offset;

		DrawText(hdc, sudoku_buttons[i].getText().c_str(), -1, &r,
			DT_CENTER | DT_WORDBREAK | DT_NOPREFIX);

		SelectObject(hdc, font);
	}
}
/*upisuje odabrani broj u sudoku polje, provjerava da li je broj tocan, ako je upisan treci krivi broj
	ispisuje se poruka o kraju igre, a ako je upisan dobar broj, isticu se sva polja sa istim brojem*/
void window::input_number()
{
	std::string number = number_buttons[selected_number_id - 101].getText();
	sudoku_buttons[selected_sudoku_id].setText(number);
	sudoku_buttons[selected_sudoku_id].setEnteredNumber(true);

	for (auto& btn : sudoku_buttons)
		btn.setNumberHighlighted(false);

	for (int i = 0; i < sudoku_buttons.size(); i++)
	{
		if (sudoku_buttons[i].getText() == number && i != selected_sudoku_id)
		{
			sudoku_buttons[i].setNumberHighlighted(true);
		}
	}

	if (sudoku_buttons[selected_sudoku_id].getText() !=
		std::to_string(solved_sudoku[selected_sudoku_id / 9][selected_sudoku_id % 9]))
	{
		mistakes++;

		HWND hStatic = GetDlgItem(hw, 112);
		std::string stext = "Number of mistakes: " + std::to_string(mistakes) + "/3";
		SetWindowText(hStatic, stext.c_str());

		if (mistakes >= 3)
		{
			if (MessageBox(hw,
				load_text(IDS_GAME_OVER_MESSAGE).c_str(),
				load_text(IDS_GAME_OVER_TITLE).c_str(),
				MB_YESNO | MB_ICONERROR) == IDYES)
			{
				reset_game(hw);
				return;
			}
			else
			{
				::PostQuitMessage(0);
				return;
			}
		}
	}
}
//upisuje brojeve u obliku biljeske
void window::input_note()
{
	std::vector<std::string> note_numbers = sudoku_buttons[selected_sudoku_id].getNoteNumbers();
	std::string selected_note_number = number_buttons[selected_number_id - 101].getText();

	int idx = stoi(selected_note_number) - 1;
	if (note_numbers[idx] == "")
		note_numbers[idx] = selected_note_number;

	sudoku_buttons[selected_sudoku_id].setNoteNumbers(note_numbers);
	sudoku_buttons[selected_sudoku_id].setText(note_numbers[0] + "      " + note_numbers[1] + "      " + note_numbers[2] + "\n\n"
											+ note_numbers[3] + "      " + note_numbers[4] + "      " + note_numbers[5] + "\n\n"
											+ note_numbers[6] + "      " + note_numbers[7] + "      " + note_numbers[8]);
}
/*ako je odabran broj dok su biljeske iskljucene, upisuje se u sudoku polje u obliku broja, a ako su biljeske ukljucene,
upisuje se u obliku biljeske, takoder, provjerava se da li je korisnik pobjedio*/
void window::number_button_clicked(int id)
{
	selected_number_id = id;
	if (selected_sudoku_id != -1)
	{
		if (!notes_on && sudoku_buttons[selected_sudoku_id].getText() == "")
		{
			input_number();
			int correct_counter = 0;
			for (int i = 0; i < sudoku_buttons.size(); i++)
			{
				if (std::to_string(solved_sudoku[i / 9][i % 9]) == sudoku_buttons[i].getText())
					correct_counter++;
			}

			if (correct_counter == 81)
			{
				if (MessageBox(hw, load_text(IDS_YOU_WIN_MESSAGE).c_str(), load_text(IDS_YOU_WIN_TITLE).c_str(),
					MB_YESNO | MB_ICONINFORMATION) == IDYES)
					reset_game(hw);
				else
					::PostQuitMessage(0);
			}
		}
		else
			input_note();
		InvalidateRect(hw, 0, true);
	}
}

//brisanje unesenog broja ili biljeske i isti brojevi vise nisu oznaceni
void window::delete_button_clicked()
{
	for (int i = 0; i < sudoku_buttons.size(); i++)
	{
		sudoku_buttons[i].setNumberHighlighted(false);

		if (sudoku_buttons[i].getClickedButton() && sudoku_buttons[i].getEnteredNumber())
			sudoku_buttons[i].setText("");

		if (sudoku_buttons[i].getClickedButton() && sudoku_buttons[i].getText().size() > 1)
		{
			std::vector<std::string> empty_notes(9, "");
			sudoku_buttons[i].setNoteNumbers(empty_notes);
			sudoku_buttons[i].setText("");
		}
	}
	InvalidateRect(hw, 0, true);
}

/* ako je kliknut gumb "solve", prazno polje se ispunjava tocnim brojem, te ako je odabrano zadnje polje bilo
	prazno salje se poruka za kraj igre*/
void window::solve_button_clicked()
{
	int correct_counter = 0;
	std::string inserted_value = "";

	for (int i = 0; i < sudoku_buttons.size(); i++)
	{
		sudoku_buttons[i].setNumberHighlighted(false);

		if (sudoku_buttons[i].getClickedButton() && sudoku_buttons[i].getText() == "")
		{
			inserted_value = std::to_string(solved_sudoku[i / 9][i % 9]);
			sudoku_buttons[i].setText(inserted_value);
			sudoku_buttons[i].setEnteredNumber(true);
		}
	}

	if (!inserted_value.empty())
	{
		for (int i = 0; i < sudoku_buttons.size(); i++)
		{
			if (sudoku_buttons[i].getText() == inserted_value && !sudoku_buttons[i].getClickedButton())
			{
				sudoku_buttons[i].setNumberHighlighted(true);
				HWND button = GetDlgItem(hw, sudoku_buttons[i].getId());
				InvalidateRect(button, 0, true);
			}
		}
	}

	for (int i = 0; i < sudoku_buttons.size(); i++)
	{
		if (std::to_string(solved_sudoku[i / 9][i % 9]) == sudoku_buttons[i].getText())
			correct_counter++;
	}

	if (correct_counter == 81)
	{
		if (MessageBox(hw, load_text(IDS_YOU_WIN_MESSAGE).c_str(), load_text(IDS_YOU_WIN_TITLE).c_str(),
			MB_YESNO | MB_ICONINFORMATION) == IDYES)
			reset_game(hw);
		else
			::PostQuitMessage(0);
	}
	InvalidateRect(hw, 0, true);
}

void window::difficulty_button_clicked(int difficulty)
{
	rect_drawn = true;
	InvalidateRect(hw, 0, true);
	game_start(hw, create_mode(difficulty));

	ShowWindow(e_button, SW_HIDE);
	ShowWindow(n_button, SW_HIDE);
	ShowWindow(h_button, SW_HIDE);
}

//mijenja velicinu, poziciju i font gumba ovisno o velicini prozora
void window::scale_buttons(int w, int h, HFONT scaled_font)
{
	int dif_w = w / 7;
	int dif_h = h / 11;
	int dif_x = (w - dif_w) / 2;
	int dif_y = (h - dif_h) / 2;

	MoveWindow(e_button, dif_x, dif_y * 0.4, dif_w, dif_h, true);
	MoveWindow(n_button, dif_x, dif_y * 0.7, dif_w, dif_h, true);
	MoveWindow(h_button, dif_x, dif_y, dif_w, dif_h, true);

	SendMessage(e_button, WM_SETFONT, (WPARAM)scaled_font, true);
	SendMessage(n_button, WM_SETFONT, (WPARAM)scaled_font, true);
	SendMessage(h_button, WM_SETFONT, (WPARAM)scaled_font, true);

	int btn_w = w / 10;
	int btn_h = h / 10;

	int delete_x = w * 0.7;
	int delete_y = h * 0.25;

	int solve_x = w * 0.9;
	int solve_y = delete_y;

	int reset_x = w * 0.05;
	int reset_y = h * 0.15;

	int notes_x = w * 0.78;
	int notes_y = delete_y;

	int mistakes_x = w * 0.73;
	int mistakes_y = w * 0.07;

	MoveWindow(delete_button, delete_x, delete_y, btn_w / 1.6, btn_h, true);
	MoveWindow(solve_button, solve_x, solve_y, btn_w / 1.6, btn_h, true);
	MoveWindow(reset_button, reset_x, reset_y, btn_w, btn_h, true);
	MoveWindow(notes_button, notes_x, notes_y, btn_w, btn_h, true);
	MoveWindow(mistakes_text, mistakes_x, mistakes_y, btn_w * 2.1, btn_h * 0.4, true);

	SendMessage(delete_button, WM_SETFONT, (WPARAM)scaled_font, true);
	SendMessage(solve_button, WM_SETFONT, (WPARAM)scaled_font, true);
	SendMessage(reset_button, WM_SETFONT, (WPARAM)scaled_font, true);
	SendMessage(notes_button, WM_SETFONT, (WPARAM)scaled_font, true);
	SendMessage(mistakes_text, WM_SETFONT, (WPARAM)scaled_font, true);
}
//mijenja velicinu, poziciju i font sudoku polja ovisno o velicini prozora
void window::scale_sudoku_rectangles(int w, int h, int cell_size)
{
	if (button_font)
		DeleteObject(button_font);
	if (notes_font)
		DeleteObject(notes_font);

	int font_size = cell_size * 0.5;
	button_font = CreateFont(-font_size, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, TEXT("Arial"));
	notes_font = CreateFont(-font_size * 0.35, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, TEXT("Arial"));

	int x_start = (w - cell_size) / 4.45;
	int y_start = (h - cell_size) / 8.7;

	for (int i = 0; i < sudoku_buttons.size(); ++i)
	{
		int row = i / 9;
		int col = i % 9;

		int row_space = 0;
		if (row >= 3) row_space += 3;
		if (row >= 6) row_space += 3;
		int col_space = 0;
		if (col >= 3) col_space += 3;
		if (col >= 6) col_space += 3;

		int x = x_start + col * cell_size + col_space;
		int y = y_start + row * cell_size + row_space;

		RECT r;
		r.left = x;
		r.top = y;
		r.right = x + cell_size;
		r.bottom = y + cell_size;
		sudoku_buttons[i].setRect(r);
	}
	InvalidateRect(hw, 0, true);
}