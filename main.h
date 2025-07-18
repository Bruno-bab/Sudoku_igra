#pragma once

#include "Sudoku_igra.h"

class main_window : public window {
public:
	LOGFONT lf;
	COLORREF cr;
	main_window();
protected:
	int on_create(CREATESTRUCT* pcs) override;
	void on_paint(HWND hw) override;
	void on_command(int id) override;
	void on_drawitem(LPARAM lp) override;
	void on_destroy() override;
};
