#include "Sudoku_igra.h"

void on_create(HWND hw)
{
	ShowWindow(hw, SW_MAXIMIZE);

	RECT r;
	GetClientRect(hw, &r);
	int x = (r.left + r.right) / 2 - 75;

	e_button = CreateWindow(TEXT("BUTTON"), TEXT("Easy"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		x, 150, 150, 100,
		hw, (HMENU)200, 0, 0);

	SendMessage(e_button, WM_SETFONT, (WPARAM)button_font, true);

	n_button = CreateWindow(TEXT("BUTTON"), TEXT("Normal"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		x, 300, 150, 100,
		hw, (HMENU)201, 0, 0);

	SendMessage(n_button, WM_SETFONT, (WPARAM)button_font, true);

	h_button = CreateWindow(TEXT("BUTTON"), TEXT("Hard"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		x, 450, 150, 100,
		hw, (HMENU)202, 0, 0);

	SendMessage(h_button, WM_SETFONT, (WPARAM)button_font, true);
}
//crta crnu pozadinu za sudoku polja
void on_paint(HWND hw)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hw, &ps);

	if (rect_drawn)
	{
		HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));
		RECT r = { 318, 78, 963, 723 };
		FillRect(hdc, &r, brush);
		DeleteObject(brush);
		EndPaint(hw, &ps);
	}
}

void on_command(HWND hw, int id)
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
					string number = number_buttons[selected_number_id - 101].getText();
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
						to_string(solved_sudoku[selected_sudoku_id / 9][selected_sudoku_id % 9]) &&
						sudoku_buttons[selected_sudoku_id].getText() != "")
					{
						mistakes++;
						HWND hStatic = GetDlgItem(hw, 112);
						string stext = "Number of mistakes: " + to_string(mistakes) + "/3";
						SetWindowText(hStatic, stext.c_str());

						InvalidateRect(hStatic, 0, true);
						UpdateWindow(hStatic);

						if (mistakes >= 3)
						{
							if (MessageBox(hw, "Game Over! You have made 3 mistakes. Play again?", "Game over", MB_YESNO | MB_ICONERROR) == IDYES)
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
						if (to_string(solved_sudoku[i / 9][i % 9]) == sudoku_buttons[i].getText())
							correct_counter++;
					}

					if (correct_counter == 81)
					{
						if (MessageBox(hw, "Sudoku solved! Play again?", "You win", MB_YESNO | MB_ICONINFORMATION) == IDYES)
							reset_game(hw);
						else
							::PostQuitMessage(0);
					}
				}
			}
			else
			{
				vector<string> note_numbers = sudoku_buttons[selected_sudoku_id].getNoteNumbers();
				string selected_note_number = number_buttons[selected_number_id - 101].getText();

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
	if (id == 110)
	{
		for (int i = 0; i < 81; i++)
		{
			sudoku_buttons[i].setNumberHighlighted(false);

			if (sudoku_buttons[i].getClickedButton() && sudoku_buttons[i].getEnteredNumber())
				sudoku_buttons[i].setText("");

			if (sudoku_buttons[i].getClickedButton() && sudoku_buttons[i].getText().size() > 1)
			{
				vector<string> empty_notes(9, "");
				sudoku_buttons[i].setNoteNumbers(empty_notes);
				sudoku_buttons[i].setText("");
			}
		}
	}
	/* ako je kliknut gumb "solve", prazno polje se ispunjava tocnim brojem, te ako je odabrano zadnje polje bilo
	prazno salje se poruka za kraj igre*/
	if (id == 111)
	{
		int correct_counter = 0;
		string inserted_value = "";

		for (int i = 0; i < 81; i++)
		{
			sudoku_buttons[i].setNumberHighlighted(false);

			if (sudoku_buttons[i].getClickedButton() && sudoku_buttons[i].getText() == "")
			{
				inserted_value = to_string(solved_sudoku[i / 9][i % 9]);
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
			if (to_string(solved_sudoku[i / 9][i % 9]) == sudoku_buttons[i].getText())
				correct_counter++;
		}

		if (correct_counter == 81)
		{
			if (MessageBox(hw, "Sudoku solved! Play again?", "You win", MB_YESNO | MB_ICONINFORMATION) == IDYES)
				reset_game(hw);
			else
				::PostQuitMessage(0);
		}
	}
	//ako je kliknut gumb "new game", salje se poruka za potvrdu i ako je odabrano "yes" igra se resetira
	if (id == 113)
	{
		if (MessageBox(hw, "Are you sure you want to start a new game?", "New game", MB_YESNO | MB_ICONINFORMATION) == IDYES)
			reset_game(hw);
	}
	//ako je kliknut gumb "notes", ukljucuje ili iskljucuje mogucnost unosa biljeski
	if (id == 114)
	{
		notes_on = !notes_on;

		if (notes_on)
			SetWindowText(GetDlgItem(hw, 114), "Notes: ON");
		else
			SetWindowText(GetDlgItem(hw, 114), "Notes: OFF");
	}

	HWND e_button = GetDlgItem(hw, 200);
	HWND n_button = GetDlgItem(hw, 201);
	HWND h_button = GetDlgItem(hw, 202);

	if (id == 200)
	{
		rect_drawn = true;
		InvalidateRect(hw, 0, true);
		game_start(hw, 0);

		ShowWindow(e_button, SW_HIDE);
		ShowWindow(n_button, SW_HIDE);
		ShowWindow(h_button, SW_HIDE);

	}
	if (id == 201)
	{
		rect_drawn = true;
		InvalidateRect(hw, 0, true);
		game_start(hw, 1);

		ShowWindow(e_button, SW_HIDE);
		ShowWindow(n_button, SW_HIDE);
		ShowWindow(h_button, SW_HIDE);
	}
	if (id == 202)
	{
		rect_drawn = true;
		InvalidateRect(hw, 0, true);
		game_start(hw, 2);

		ShowWindow(e_button, SW_HIDE);
		ShowWindow(n_button, SW_HIDE);
		ShowWindow(h_button, SW_HIDE);
	}
}

void on_drawitem(LPARAM lp)
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

				if (sudoku_buttons[i].getText() != to_string(solved_sudoku[i / 9][i % 9]))
					SetTextColor(dis->hDC, RGB(255, 0, 0));

				else if (sudoku_buttons[i].getEnteredNumber())
					SetTextColor(dis->hDC, RGB(0, 0, 255));
			}
			else
				HFONT font2 = (HFONT)SelectObject(dis->hDC, notes_font);
			//tekst se centrira 
			string text = sudoku_buttons[i].getText();
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

void on_destroy()
{
	::PostQuitMessage(0);
}

LRESULT CALLBACK window_proc(HWND hw, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_CREATE:
		on_create(hw);
		return 0;
	case WM_PAINT:
		on_paint(hw);
		return 0;
	case WM_COMMAND:
		on_command(hw, LOWORD(wp));
		return 0;
	case WM_DRAWITEM:
		on_drawitem(lp);
		return 0;
	case WM_DESTROY:
		on_destroy();
		return 0;
	}
	return ::DefWindowProc(hw, msg, wp, lp);
}

int register_class(HINSTANCE hi, const char* name)
{
	WNDCLASS wc{};
	wc.lpfnWndProc = window_proc;
	wc.lpszClassName = name;
	wc.hInstance = hi;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hCursor = ::LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));
	return ::RegisterClass(&wc);
}

int WINAPI WinMain(HINSTANCE hi, HINSTANCE, LPSTR cmd_line, int show_flag)
{
	const auto window = "Sudoku";

	if (!register_class(hi, window))
		return 0;
	::CreateWindow(window, "Sudoku", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, 0, 0, hi, 0);
	MSG msg;
	while (::GetMessage(&msg, 0, 0, 0))
		::DispatchMessage(&msg);
	return msg.wParam;
}