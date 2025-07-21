#include "main.h"

main_window::main_window() {
	ZeroMemory(&lf, sizeof(lf));
	_tcscpy_s(lf.lfFaceName, _T("Arial"));
	HDC hdc = ::GetDC(0);
	lf.lfHeight = -15 * ::GetDeviceCaps(hdc, LOGPIXELSY) / 72;
	cr = RGB(0, 0, 0);
	::ReleaseDC(0, hdc);
}

void main_window::on_size(int w, int h)
{
	int font_h = -MulDiv(h / 25, GetDeviceCaps(GetDC(hw), LOGPIXELSY), 72) / 1.5;
	int font_w = -MulDiv(w / 25, GetDeviceCaps(GetDC(hw), LOGPIXELSY), 72) / 6;

	HFONT scaled_font = CreateFont(font_h, font_w, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"));

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

	int cell_w = w / 22;
	int cell_h = h / 11;
	int cell_size = min(cell_w, cell_h);
	int space = cell_size / 15;

	int sud_x = (w - (9 * cell_size + space * 2)) / 2.75;
	int sud_y = (h - (9 * cell_size + space * 2)) / 2;

	for (int i = 0; i < sudoku_buttons.size(); i++)
	{
		int row = i / 9;
		int col = i % 9;

		int extra_x = (col / 3) * space;
		int extra_y = (row / 3) * space;

		int x = sud_x + col * cell_size + extra_x;
		int y = sud_y + row * cell_size + extra_y;

		HWND button = GetDlgItem(hw, sudoku_buttons[i].getId());
		MoveWindow(button, x, y, cell_size, cell_size, true);
		SendMessage(button, WM_SETFONT, (WPARAM)scaled_font, true);
	}

	int num_x = (w - cell_size) / 1.3;
	int num_y = (h - cell_size) / 2;

	for (int i = 0; i < number_buttons.size(); i++)
	{
		int row = i / 3;
		int col = i % 3;

		int x = num_x + col * cell_size;
		int y = num_y + row * cell_size;

		HWND button = GetDlgItem(hw, number_buttons[i].getId());
		MoveWindow(button, x, y, cell_size, cell_size, true);
	}

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

int main_window::on_create(CREATESTRUCT* pcs)
{
	ShowWindow(hw, SW_MAXIMIZE);

	RECT r;
	GetClientRect(hw, &r);
	int x = (r.left + r.right) / 2 - 75;

	e_button = CreateWindow("BUTTON", load_text(IDS_BTN_EASY).c_str(),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		x, 150, 150, 100,
		hw, (HMENU)IDS_BTN_EASY, 0, 0);

	n_button = CreateWindow("BUTTON", load_text(IDS_BTN_NORMAL).c_str(),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		x, 300, 150, 100,
		hw, (HMENU)IDS_BTN_NORMAL, 0, 0);

	h_button = CreateWindow("BUTTON", load_text(IDS_BTN_HARD).c_str(),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		x, 450, 150, 100,
		hw, (HMENU)IDS_BTN_HARD, 0, 0);

	return 0;
}

//crta crnu pozadinu za sudoku polja
void main_window::on_paint(HWND hw)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hw, &ps);

	if (rect_drawn)
	{
		RECT r;
		GetClientRect(*this, &r);
		int width = r.right - r.left;
		int height = r.bottom - r.top;

		int cell_w =  width/ 22;
		int cell_h = height / 11;
		int cell_size = min(cell_w, cell_h);

		int block_spacing = cell_size / 5;
		int total_spacing = block_spacing * 2;

		int grid_width = 9 * cell_size + total_spacing;
		int grid_height = 9 * cell_size + total_spacing;

		int sud_x = (width - grid_width) / 2.78;
		int sud_y = (height - grid_height) / 1.98;

		RECT border = {sud_x,
			sud_y,
			sud_x + grid_width,
			sud_y + grid_height
		};

		HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));
		FillRect(hdc, &border, brush);
		DeleteObject(brush);
	}
}

void main_window::on_command(int id)
{
	//ako je kliknut sudoku gumb oznacava ga i oznacava sve gumbe s istim brojem
	if (id >= 0 && id < 81)
	{
		if (selected_sudoku_id != -1)
			sudoku_buttons[selected_sudoku_id].setClickedButton(false);

		selected_sudoku_id = id;
		sudoku_buttons[selected_sudoku_id].setClickedButton(true);

		for (int i = 0; i < 81; i++)
		{
			sudoku_buttons[i].setNumberHighlighted(false);
			if (sudoku_buttons[i].getText() == sudoku_buttons[id].getText() && sudoku_buttons[id].getText() != "" && id != i)
			{
				sudoku_buttons[i].setNumberHighlighted(true);
				HWND button = GetDlgItem(hw, sudoku_buttons[i].getId());
				InvalidateRect(button, 0, true);
			}
		}
		HWND button = GetDlgItem(hw, sudoku_buttons[selected_sudoku_id].getId());
		InvalidateRect(button, 0, true);
	}
	/*ako je kliknut gumb za broj, ako je notes iskljucen upisuje se u sudoku polje, a ako je ukljucen,
	upisuje se u obliku biljeske, takoder, provjerava se da li je upisan dobar broj, broji greske i vraca poruku
	ovisno da li je korisnik pobjedio (ispunio sva polja) ili izgubio (upisao 3 kriva broja)*/
	if (id >= 101 && id <= 109)
	{
		selected_number_id = id;
		if (selected_sudoku_id != -1)
		{
			if (!notes_on)
			{
				if (sudoku_buttons[selected_sudoku_id].getText() == "")
				{
					std::string number = number_buttons[selected_number_id - 101].getText();
					sudoku_buttons[selected_sudoku_id].setText(number);
					sudoku_buttons[selected_sudoku_id].setEnteredNumber(true);

					for (int i = 0; i < 81; i++)
					{
						sudoku_buttons[i].setNumberHighlighted(false);
						if (sudoku_buttons[i].getText() == sudoku_buttons[selected_sudoku_id].getText() &&
							sudoku_buttons[selected_sudoku_id].getText() != "" && selected_sudoku_id != i)
						{
							sudoku_buttons[i].setNumberHighlighted(true);
							HWND button = GetDlgItem(hw, sudoku_buttons[i].getId());
							InvalidateRect(button, 0, true);
						}
					}

					HWND button = GetDlgItem(hw, sudoku_buttons[selected_sudoku_id].getId());
					InvalidateRect(button, 0, true);

					if (sudoku_buttons[selected_sudoku_id].getText() !=
						std::to_string(solved_sudoku[selected_sudoku_id / 9][selected_sudoku_id % 9]) &&
						sudoku_buttons[selected_sudoku_id].getText() != "")
					{
						mistakes++;
						HWND hStatic = GetDlgItem(hw, 112);
						std::string stext = "Number of mistakes: " + std::to_string(mistakes) + "/3";
						SetWindowText(hStatic, stext.c_str());

						InvalidateRect(hStatic, 0, true);
						UpdateWindow(hStatic);

						if (mistakes >= 3)
						{
							if (MessageBox(hw, load_text(IDS_GAME_OVER_MESSAGE).c_str(), load_text(IDS_GAME_OVER_TITLE).c_str(),
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

					int correct_counter = 0;

					for (int i = 0; i < 81; i++)
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
			}
			else
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
		}
	}
	selected_number_id = -1;
	//ako je kliknut gumb "delete", brise se uneseni broj ili biljeska i isti brojevi vise nisu oznaceni
	if (id == IDS_BTN_DELETE)
	{
		for (int i = 0; i < 81; i++)
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
	}
	/* ako je kliknut gumb "solve", prazno polje se ispunjava tocnim brojem, te ako je odabrano zadnje polje bilo
	prazno salje se poruka za kraj igre*/
	if (id == IDS_BTN_SOLVE)
	{
		int correct_counter = 0;
		std::string inserted_value = "";

		for (int i = 0; i < 81; i++)
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
			for (int i = 0; i < 81; i++)
			{
				if (sudoku_buttons[i].getText() == inserted_value && !sudoku_buttons[i].getClickedButton())
				{
					sudoku_buttons[i].setNumberHighlighted(true);
					HWND button = GetDlgItem(hw, sudoku_buttons[i].getId());
					InvalidateRect(button, 0, true);
				}
			}
		}

		for (int i = 0; i < 81; i++)
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
	//ako je kliknut gumb "new game", salje se poruka za potvrdu i ako je odabrano "yes" igra se resetira
	if (id == IDS_BTN_RESET)
	{
		if (MessageBox(hw, load_text(IDS_NEW_GAME_MESSAGE).c_str(), load_text(IDS_NEW_GAME_TITLE).c_str(),
			MB_YESNO | MB_ICONINFORMATION) == IDYES)
			reset_game(hw);
	}
	//ako je kliknut gumb "notes", ukljucuje ili iskljucuje mogucnost unosa biljeski
	if (id == IDS_BTN_NOTES_OFF)
	{
		notes_on = !notes_on;

		if (notes_on)
			SetWindowText(GetDlgItem(hw, IDS_BTN_NOTES_OFF), load_text(IDS_BTN_NOTES_ON).c_str());
		else
			SetWindowText(GetDlgItem(hw, IDS_BTN_NOTES_OFF), load_text(IDS_BTN_NOTES_OFF).c_str());
	}

	HWND e_button = GetDlgItem(hw, IDS_BTN_EASY);
	HWND n_button = GetDlgItem(hw, IDS_BTN_NORMAL);
	HWND h_button = GetDlgItem(hw, IDS_BTN_HARD);

	if (id == IDS_BTN_EASY)
	{
		rect_drawn = true;
		InvalidateRect(hw, 0, true);
		game_start(hw, 0);

		ShowWindow(e_button, SW_HIDE);
		ShowWindow(n_button, SW_HIDE);
		ShowWindow(h_button, SW_HIDE);

	}
	if (id == IDS_BTN_NORMAL)
	{
		rect_drawn = true;
		InvalidateRect(hw, 0, true);
		game_start(hw, 1);

		ShowWindow(e_button, SW_HIDE);
		ShowWindow(n_button, SW_HIDE);
		ShowWindow(h_button, SW_HIDE);
	}
	if (id == IDS_BTN_HARD)
	{
		rect_drawn = true;
		InvalidateRect(hw, 0, true);
		game_start(hw, 2);

		ShowWindow(e_button, SW_HIDE);
		ShowWindow(n_button, SW_HIDE);
		ShowWindow(h_button, SW_HIDE);
	}
}

void main_window::on_drawitem(LPARAM lp)
{
	LPDRAWITEMSTRUCT dis = (LPDRAWITEMSTRUCT)lp;

	for (int i = 0; i < 81; i++)
	{
		if (dis->CtlID == sudoku_buttons[i].getId())
		{

			bool isPressed = (dis->itemState & ODS_SELECTED);
			bool isFocused = (dis->itemState & ODS_FOCUS);

			COLORREF button_color = RGB(240, 240, 240);
			//ako je kliknut sudoku gumb mijenja boju
			if (sudoku_buttons[i].getClickedButton())
				button_color = RGB(190, 190, 230);
			//mijenja boju gumba ako imaju isti broj kliknutog gumba
			if (sudoku_buttons[i].getNumberHighlighted())
				button_color = RGB(220, 220, 235);

			HBRUSH brush = CreateSolidBrush(button_color);
			FillRect(dis->hDC, &dis->rcItem, brush);
			DrawEdge(dis->hDC, &dis->rcItem, EDGE_RAISED, BF_RECT);
			SetBkMode(dis->hDC, TRANSPARENT);
			/*ako je uneseni broj krivi boja teksa je crvena, ako je tocan boja teksta je plava,
			font se mijenja ovisno o tome da li je unesena biljeska*/
			if (sudoku_buttons[i].getText().size() == 1)
			{
				HFONT font1 = (HFONT)SelectObject(dis->hDC, button_font);

				if (sudoku_buttons[i].getText() != std::to_string(solved_sudoku[i / 9][i % 9]))
					SetTextColor(dis->hDC, RGB(255, 0, 0));

				else if (sudoku_buttons[i].getEnteredNumber())
					SetTextColor(dis->hDC, RGB(0, 0, 255));
			}
			else
				HFONT font2 = (HFONT)SelectObject(dis->hDC, notes_font);
			//centriranje teksta
			std::string text = sudoku_buttons[i].getText();
			RECT r_text = dis->rcItem;
			RECT measure = r_text;
			DrawText(dis->hDC, text.c_str(), -1, &measure, DT_CENTER | DT_WORDBREAK | DT_CALCRECT | DT_NOPREFIX);

			int text_height = measure.bottom - measure.top;
			int c_height = r_text.bottom - r_text.top;
			int vertical = (c_height - text_height) / 2;

			r_text.top += vertical;
			DrawText(dis->hDC, text.c_str(), -1, &r_text, DT_CENTER | DT_WORDBREAK | DT_NOPREFIX);

			DeleteObject(brush);
		}
	}
}


void main_window::on_destroy()
{
	::PostQuitMessage(0);
}

int WINAPI WinMain(HINSTANCE hi, HINSTANCE, LPSTR, int)
{
	application app;
	main_window wnd;
	wnd.create(0, WS_OVERLAPPEDWINDOW | WS_VISIBLE, load_text(IDS_APP_TITLE).c_str());
	return app.run();
}
