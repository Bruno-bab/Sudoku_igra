#include "Number_generator.h"
#include "Sudoku_igra.h"

std::vector<sudoku_button> sudoku_buttons;
std::vector<number_button> number_buttons;
std::string selected_number = "";

int selected_sudoku_id = -1;
int selected_number_id = -1;

void on_create(HWND hw) 
{

}

void on_create_game(HWND hw)
{
	srand(time(0));
	int k = 30;
	vector<vector<int>> sudoku = sudokuGenerator(k);
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
			if (sudoku[i][j] != 0)
				sudoku_buttons.emplace_back(hw, x, y, 70, 70, id, std::to_string(sudoku[i][j]));
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
			number_buttons.emplace_back(hw, x2, y2, 70, 70, 100 + counter, std::to_string(counter));
			x2 += 73;
			counter++;
		}
	}
}

void on_command(HWND hw, int id)
{
	if (id >= 0 && id < 81)
	{
		if (selected_sudoku_id != -1)
		{
			SendMessage(sudoku_buttons[selected_sudoku_id].getHWND(), BM_SETCHECK, BST_UNCHECKED, 0);
		}

		selected_sudoku_id = id;
	}
	if (id >= 101 && id <= 109)
	{
		selected_number_id = id;
	}

	if (selected_sudoku_id != -1 && selected_number_id != -1 && sudoku_buttons[selected_sudoku_id].getText() == "")
	{
		std::string val = number_buttons[selected_number_id - 101].getText();
		sudoku_buttons[selected_sudoku_id].setText(val);
		sudoku_buttons[selected_sudoku_id].setHighlighted(true);
		InvalidateRect(sudoku_buttons[selected_sudoku_id].getHWND(), 0, true);
		SendMessage(sudoku_buttons[selected_sudoku_id].getHWND(), BM_SETCHECK, BST_UNCHECKED, 0);
	}
	selected_number_id = -1;
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
	{
		on_create_game(hw);
		return 0;
	}
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hw, &ps);
			HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
			RECT rect = { 318, 118, 963, 763 }; 
			FillRect(hdc, &rect, hBrush);

			DeleteObject(hBrush);
			EndPaint(hw, &ps);
			return 0;
		}
		case WM_COMMAND:
			on_command(hw, LOWORD(wp));
			return 0;
		break;
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
