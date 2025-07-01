#include "Number_generator.h"
#include "Sudoku_igra.h"

vector<sudoku_button> sudoku_buttons;
vector<number_button> number_buttons;
std::string selected_number = "";
bool same_numbers = false;

int selected_sudoku_id = -1;
int selected_number_id = -1;

void on_create(HWND hw) 
{

}

void on_create_game(HWND hw)
{
	srand(time(0));
	int numbers_to_remove = 30;
	vector<vector<int>> sudoku = sudokuGenerator(numbers_to_remove);
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

		for (int i = 0; i < 81; i++)
		{
			sudoku_buttons[i].setSameNumber(false);
			if (sudoku_buttons[i].getText() == sudoku_buttons[id].getText() && sudoku_buttons[id].getText() != "" && id != i)
			{
				sudoku_buttons[i].setSameNumber(true);
				HWND button = GetDlgItem(hw, sudoku_buttons[i].getId());
				InvalidateRect(button, 0, true);
			}
		}

		selected_sudoku_id = id;
		sudoku_buttons[selected_sudoku_id].setClickedButton(true);

		HWND button = GetDlgItem(hw, sudoku_buttons[selected_sudoku_id].getId());
		InvalidateRect(button, 0, true);
	}
	
	if (id >= 101 && id <= 109)
	{
		selected_number_id = id;

		if (selected_sudoku_id != -1 && sudoku_buttons[selected_sudoku_id].getText() == "")
		{
			std::string val = number_buttons[selected_number_id - 101].getText();
			sudoku_buttons[selected_sudoku_id].setText(val);
			sudoku_buttons[selected_sudoku_id].setEnteredNumber(true);

			HWND button = GetDlgItem(hw, sudoku_buttons[selected_sudoku_id].getId());
			InvalidateRect(button, 0, true);
		}
	}
	selected_number_id = -1;
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

			if (sudoku_buttons[i].getSameNumber())
				button_color = RGB(220, 220, 235);

			HBRUSH brush = CreateSolidBrush(button_color);
			FillRect(dis->hDC, &dis->rcItem, brush);

			DrawEdge(dis->hDC, &dis->rcItem, EDGE_RAISED, BF_RECT);

			if (sudoku_buttons[i].getEnteredNumber())
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
