#pragma once

#include "Sudoku_igra.h"

class main_window : public window {
public:
	LOGFONT lf;
	COLORREF cr;
	main_window();
protected:
	int on_create(CREATESTRUCT* pcs) override;
	void on_paint(HDC hdc) override;
	void on_command(int id) override;
	void on_size(int w, int h) override;
	void on_left_button_down(POINT) override;
	void on_destroy() override;
};
