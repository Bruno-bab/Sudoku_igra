#include "Sudoku_igra.h"

std::vector<std::vector<int>> solved_sudoku;
std::vector<sudoku_button> sudoku_buttons;
std::vector<number_button> number_buttons;

int selected_sudoku_id = -1;
int selected_number_id = -1;
int mistakes = 0;
bool notes_on = false;
bool rect_drawn = false;

HWND e_button;
HWND n_button;
HWND h_button;

HFONT button_font = CreateFont(-30, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET,
	OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT("Times New Roman"));

HFONT notes_font = CreateFont(14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET,
	OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT("Times New Roman"));

//generira rijesenu sudoku igru
std::vector<std::vector<int>> create_solved_sudoku()
{
	srand(time(0));
	std::vector<std::vector<int>> solved_sudoku = sudokuGenerator(0);
	return solved_sudoku;
}
//uklanja k brojeva iz sudoku igre
void remove_numbers(std::vector<std::vector<int>>& grid, int k)
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

void game_start(HWND hw, int mode)
{
	srand(time(0));
	//uklanja brojeve iz sudoku igre ovisno o odabranoj težini
	int numbers_to_remove;
	if (mode == 0)
		numbers_to_remove = rand() % 10 + 29;
	else if (mode == 1)
		numbers_to_remove = rand() % 10 + 40;
	else if (mode == 2)
		numbers_to_remove = rand() % 10 + 51;

	solved_sudoku = create_solved_sudoku();

	std::vector<std::vector<int>> unsolved_sudoku = solved_sudoku;
	remove_numbers(unsolved_sudoku, numbers_to_remove);
	//kreiranje sudoku gumba
	int y = 10;
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
				sudoku_buttons.emplace_back(hw, x, y, 70, 70, id, std::to_string(unsolved_sudoku[i][j]));
			else
				sudoku_buttons.emplace_back(hw, x, y, 70, 70, id, "");
			id++;
		}
	}
	//kreiranje gumba za brojeve
	int y2 = 270;
	int counter = 1;
	for (int i = 0; i < 3; i++)
	{
		int x2 = 1140;
		y2 += 73;
		for (int j = 0; j < 3; j++)
		{
			number_buttons.emplace_back(hw, x2, y2, 70, 70, 100 + counter, std::to_string(counter));
			x2 += 73;
			counter++;
		}
	}
	//kreiranje ostalih gumba i statica
	HWND delete_button = CreateWindow("BUTTON", "Delete", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		1050, 200, 100, 100, hw, HMENU(110), 0, 0);

	SendMessage(delete_button, WM_SETFONT, (WPARAM)button_font, true);

	HWND solve_button = CreateWindow("BUTTON", "Solve", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		1350, 200, 100, 100, hw, HMENU(111), 0, 0);

	SendMessage(solve_button, WM_SETFONT, (WPARAM)button_font, true);

	HWND reset_button = CreateWindow("BUTTON", "New game", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		100, 150, 150, 100, hw, HMENU(113), 0, 0);

	SendMessage(reset_button, WM_SETFONT, (WPARAM)button_font, true);

	HWND notes_button = CreateWindow("BUTTON", "Notes: OFF", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		1175, 200, 150, 100, hw, HMENU(114), 0, 0);

	SendMessage(notes_button, WM_SETFONT, (WPARAM)button_font, true);

	HWND mistakes_text = CreateWindow("STATIC", ("Number of mistakes: 0/3"), WS_CHILD | WS_VISIBLE | SS_LEFT,
		1060, 100, 371, 30, hw, HMENU(112), 0, 0);

	HFONT static_font = CreateFont(-25, -15, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT("Times New Roman")
	);
	SendMessage(mistakes_text, WM_SETFONT, (WPARAM)static_font, true);
}
//brise sve gumbe, stavlja vrijednosti na pocetno stanje i resetira igru
void reset_game(HWND hw)
{
	for (int i = 0; i < 81; i++)
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