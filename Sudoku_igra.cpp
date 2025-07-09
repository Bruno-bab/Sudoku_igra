#include "Number_generator.h"
#include "Sudoku_igra.h"

vector<vector<int>> solved_sudoku;
vector<sudoku_button> sudoku_buttons;
vector<number_button> number_buttons;
std::string selected_number = "";

int selected_sudoku_id = -1;
int selected_number_id = -1;
int mistakes = 0;

HWND e_button;
HWND n_button;
HWND h_button;

const auto class_name2 = "Mode";
bool rect_drawn = false;

HFONT button_font = CreateFont(-30, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET,
	OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT("Times New Roman"));

vector<vector<int>> createSolvedSudoku()
{
	srand(time(0));
	vector<vector<int>> solved_sudoku = sudokuGenerator(0);
	return solved_sudoku;
}

void remove_numbers(vector<vector<int>>& grid, int k)
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

void on_create(HWND hw)
{
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

void game_start(HWND hw, int mode)
{
	srand(time(0));

	int numbers_to_remove;
	if (mode == 0)
		numbers_to_remove = 1;//rand() % 10 + 25; 
	else if (mode == 1)
		numbers_to_remove = rand() % 10 + 35; 
	else if (mode == 2)
		numbers_to_remove = rand() % 10 + 45;

	solved_sudoku = createSolvedSudoku();

	vector<vector<int>> unsolved_sudoku = solved_sudoku;
	remove_numbers(unsolved_sudoku, numbers_to_remove);

	int y = 50;
	int id = 0;
	for (int i = 0; i < 9; i++)
	{
		if (i % 3 == 0)
			y += 73;
		else
			y += 70;
		int x = 250;
		for (int j = 0; j < 9; j++)
		{
			if (j % 3 == 0)
				x += 73;
			else
				x += 70;
			if (unsolved_sudoku[i][j] != 0)
				sudoku_buttons.emplace_back(hw, x, y, 70, 70, id, to_string(unsolved_sudoku[i][j]));
			else
				sudoku_buttons.emplace_back(hw, x, y, 70, 70, id, "");
			id++;
		}
	}

	int y2 = 270;
	int counter = 1;
	for (int i = 0; i < 3; i++)
	{
		int x2 = 1100;
		y2 += 73;
		for (int j = 0; j < 3; j++)
		{
			number_buttons.emplace_back(hw, x2, y2, 70, 70, 100 + counter, to_string(counter));
			x2 += 73;
			counter++;
		}
	}

	HWND d_button = CreateWindow("BUTTON", "Delete", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON ,
		1100, 200, 100, 100, hw, HMENU(110), 0, 0);

	SendMessage(d_button, WM_SETFONT, (WPARAM)button_font, true);

	HWND s_button = CreateWindow("BUTTON", "Solve", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		1300, 200, 100, 100, hw, HMENU(111), 0, 0);

	SendMessage(s_button, WM_SETFONT, (WPARAM)button_font, true);

    HWND mistakes_text = CreateWindow("STATIC", ("Number of mistakes: 0/3"), WS_CHILD | WS_VISIBLE | SS_LEFT,
       1100, 100, 245, 25, hw, HMENU(112), 0, 0);

	HFONT static_font = CreateFont( -20, -10, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT("Times New Roman")
	);
	SendMessage(mistakes_text, WM_SETFONT, (WPARAM)static_font, true);
}

void reset_game(HWND hw)
{
	for (int i = 0; i < 81; i++)
		DestroyWindow(GetDlgItem(hw, sudoku_buttons[i].getId()));
	
	sudoku_buttons.clear();

	for (int i = 0; i < 9; i++)
		DestroyWindow(GetDlgItem(hw, number_buttons[i].getId()));

	number_buttons.clear();

	DestroyWindow(GetDlgItem(hw, 110));
	DestroyWindow(GetDlgItem(hw, 111));
	DestroyWindow(GetDlgItem(hw, 112));

	mistakes = 0;
	rect_drawn = false;

	ShowWindow(e_button, SW_SHOW);
	ShowWindow(n_button, SW_SHOW);
	ShowWindow(h_button, SW_SHOW);

	InvalidateRect(hw, nullptr, TRUE);
	UpdateWindow(hw);
}

void on_paint(HWND hw)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hw, &ps);

	if (rect_drawn)
	{
		HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));
		RECT r = { 318, 118, 963, 763 };
		FillRect(hdc, &r, brush);

		DeleteObject(brush);

		EndPaint(hw, &ps);
	}
}

void on_command(HWND hw, int id)
{
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
	
	if (id >= 101 && id <= 109)
	{
		selected_number_id = id;

		if (selected_sudoku_id != -1 && sudoku_buttons[selected_sudoku_id].getText() == "")
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
						reset_game(hw);
					else
						::PostQuitMessage(0);
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
	selected_number_id = -1;
	
	if (id == 110)
	{
		for (int i = 0; i < 81; i++)
		{
			sudoku_buttons[i].setNumberHighlighted(false);
			if (sudoku_buttons[i].getClickedButton() && sudoku_buttons[i].getEnteredNumber())
				sudoku_buttons[i].setText("");
		}
	}

	if (id == 111)
	{
		int correct_counter = 0;

		for (int i = 0; i < 81; i++)
		{
			sudoku_buttons[i].setNumberHighlighted(false);
			if (sudoku_buttons[i].getClickedButton() && sudoku_buttons[i].getText() == sudoku_buttons[selected_sudoku_id].getText() &&
				sudoku_buttons[selected_sudoku_id].getText() != "" && selected_sudoku_id != i)
			{
				sudoku_buttons[i].setNumberHighlighted(true);
				HWND button = GetDlgItem(hw, sudoku_buttons[i].getId());
				InvalidateRect(button, 0, true);
			}

			if (sudoku_buttons[i].getClickedButton() && sudoku_buttons[i].getText() == "")
			{
				string solved_number = to_string(solved_sudoku[i / 9][i % 9]);
				sudoku_buttons[i].setText(solved_number);
				sudoku_buttons[i].setEnteredNumber(true);
			}

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

			if (sudoku_buttons[i].getClickedButton())
				button_color = RGB(190, 190, 230);

			if (sudoku_buttons[i].getNumberHighlighted())
				button_color = RGB(220, 220, 235);

			HBRUSH brush = CreateSolidBrush(button_color);
			FillRect(dis->hDC, &dis->rcItem, brush);

			DrawEdge(dis->hDC, &dis->rcItem, EDGE_RAISED, BF_RECT);

			if (sudoku_buttons[i].getText() != to_string(solved_sudoku[i / 9][i % 9]))
				SetTextColor(dis->hDC, RGB(255, 0, 0));
			
			else if (sudoku_buttons[i].getEnteredNumber())
				SetTextColor(dis->hDC, RGB(0, 0, 255));
			
			SetBkMode(dis->hDC, TRANSPARENT);

			RECT button_text = dis->rcItem;
			if (isPressed)
				OffsetRect(&button_text, 1, 1);

			DrawText(dis->hDC, sudoku_buttons[i].getText().c_str(), -1, &button_text,
				DT_CENTER | DT_VCENTER | DT_SINGLELINE);
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
	const auto class_name1 = "TheGame";
	if (!register_class(hi, class_name1) || !register_class(hi, class_name2))
		return 0;
	int screen_w = GetSystemMetrics(SM_CXSCREEN);
	int screen_h = GetSystemMetrics(SM_CYSCREEN);
	::CreateWindow(class_name1, "Sudoku", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, screen_w, screen_h, 0, 0, hi, 0);
	MSG msg;
	while (::GetMessage(&msg, 0, 0, 0))
		::DispatchMessage(&msg);
	return msg.wParam;
}
