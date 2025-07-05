#include "Number_generator.h"
#include "Sudoku_igra.h"

vector<vector<int>> solved_sudoku = createSolvedSudoku();
vector<sudoku_button> sudoku_buttons;
vector<number_button> number_buttons;
std::string selected_number = "";

int selected_sudoku_id = -1;
int selected_number_id = -1;
int mistakes = 0;

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

}

void on_create_game(HWND hw)
{
	srand(time(0));
	int numbers_to_remove = 45;

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

	CreateWindow("BUTTON", "Delete", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON ,
		1100, 200, 100, 100, hw, HMENU(110), 0, 0);

	CreateWindow("BUTTON", "Solve", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		1300, 200, 100, 100, hw, HMENU(111), 0, 0);

    HWND hwndStatic = CreateWindow("STATIC", ("Number of mistakes: 0/3"), WS_CHILD | WS_VISIBLE | SS_LEFT,
       1100, 100, 255, 50, hw, HMENU(112), 0, 0);

	HFONT hFont = CreateFont( -20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT("Consolas")
	);
	SendMessage(hwndStatic, WM_SETFONT, (WPARAM)hFont, TRUE);
}

void on_paint(HWND hw)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hw, &ps);
	HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));
	RECT r = { 318, 118, 963, 763 };
	FillRect(hdc, &r, brush);

	DeleteObject(brush);

	EndPaint(hw, &ps);
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

				InvalidateRect(hStatic, NULL, TRUE);
				UpdateWindow(hStatic);
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

			if (sudoku_buttons[i].getClickedButton() && sudoku_buttons[i].getText() == "")
			{
				string solved_number = to_string(solved_sudoku[i / 9][i % 9]);
				sudoku_buttons[i].setText(solved_number);
				sudoku_buttons[i].setEnteredNumber(true);
			}

		}
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
			on_create_game(hw);
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
	const auto class_name = "Sudoku";
	if (!register_class(hi, class_name))
		return 0;
	::CreateWindow(class_name, "Sudoku", WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, hi, 0);
	MSG msg;
	while (::GetMessage(&msg, 0, 0, 0))
		::DispatchMessage(&msg);
	return msg.wParam;
}
