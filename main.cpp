#include "main.h"

main_window::main_window() {
	ZeroMemory(&lf, sizeof(lf));
	_tcscpy_s(lf.lfFaceName, _T("Arial"));
	HDC hdc = ::GetDC(0);
	lf.lfHeight = -15 * ::GetDeviceCaps(hdc, LOGPIXELSY) / 72;
	cr = RGB(0, 0, 0);
	::ReleaseDC(0, hdc);
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

void main_window::on_paint(HDC hdc)
{	
	//crtanje okvira sudoku polja
	if (rect_drawn)
	{
		RECT r;
		GetClientRect(*this, &r);
		RECT topLeft = sudoku_buttons[0].getRect();
		RECT bottomRight = sudoku_buttons[80].getRect();

		RECT border;
		border.left = topLeft.left - 8;
		border.top = topLeft.top - 8;
		border.right = bottomRight.right + 8;
		border.bottom = bottomRight.bottom + 8;

		HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));
		FillRect(hdc, &border, brush);
		DeleteObject(brush);
	}

	paint_sudoku_buttons(hdc);
}

void main_window::on_command(int id)
{
	if (id >= 101 && id <= 109)
		number_button_clicked(id);
	
	selected_number_id = -1;
	if (id == IDS_BTN_DELETE)
		delete_button_clicked();
	
	if (id == IDS_BTN_SOLVE)
		solve_button_clicked();
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

	if (id == IDS_BTN_EASY)
		difficulty_button_clicked(0);

	if (id == IDS_BTN_NORMAL)
		difficulty_button_clicked(1);

	if (id == IDS_BTN_HARD)
		difficulty_button_clicked(2);
}

//ako je kliknuto sudoku polje, oznacava se i isticu se druga polja sa istim brojevima
void main_window::on_left_button_down(POINT p)
{
	points.push_back(p);

	for (int i = 0; i < sudoku_buttons.size(); i++)
	{
		sudoku_buttons[i].setNumberHighlighted(false);
		sudoku_buttons[i].setClickedButton(false);
	}

	for (int i = 0; i < sudoku_buttons.size(); i++)
	{
		if (sudoku_buttons[i].contains(p))
		{
			selected_sudoku_id = i;
			sudoku_buttons[i].setClickedButton(true);
			for (int j = 0; j < sudoku_buttons.size(); j++)
			{
				if (!sudoku_buttons[i].getText().empty() && sudoku_buttons[j].getText() == sudoku_buttons[i].getText())
					sudoku_buttons[j].setNumberHighlighted(true);
			}
			InvalidateRect(hw, 0, true);
			break;
		}
	}
}

//postavljanje koordinata i velicina gumba, te fontova ovisno o velicini prozora 
void main_window::on_size(int w, int h)
{
	int font_h = -MulDiv(h / 25, GetDeviceCaps(GetDC(hw), LOGPIXELSY), 72) / 1.5;
	int font_w = -MulDiv(w / 25, GetDeviceCaps(GetDC(hw), LOGPIXELSY), 72) / 6;

	HFONT scaled_font = CreateFont(font_h, font_w, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Arial"));

	scale_buttons(w, h, scaled_font);

	int cell_w = w / 22;
	int cell_h = h / 11;
	int cell_size = min(cell_w, cell_h);

	int num_x = (w - cell_size) / 1.25;
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

	scale_sudoku_rectangles(w, h, cell_size);
}

void main_window::on_destroy()
{
	::PostQuitMessage(0);
}

int WINAPI WinMain(HINSTANCE hi, HINSTANCE, LPSTR, int)
{
	application app;
	main_window wnd;
	wnd.create(0, WS_OVERLAPPEDWINDOW | WS_VISIBLE, window::load_text(IDS_APP_TITLE).c_str());
	return app.run();
}
