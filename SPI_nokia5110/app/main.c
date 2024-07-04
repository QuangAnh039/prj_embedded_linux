#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "lcd_16x2.h"

int main()
{
	char c;

	while(1) {


		printf("0: Exit\n");
		printf("1: Quang Anh\n");
		printf("2: QuangAnh0 Linux\n");
		printf("3; Linux Embedded\n");

		scanf("%d", &c);

		switch(c) {
			case 0:
				return 0;
			case 1:
				lcd_gotoxy(0, 0);
				draw_string("Quang Anh", Pixel_Set, FontSize_5x7);
				break;
			case 2:
				lcd_gotoxy(15, 10);
				draw_string("QuangAnh039 Linux", Pixel_Set, FontSize_5x7);
				break;
			case 3:
				lcd_gotoxy(20, 24);
				draw_string("Linux Embedded", Pixel_Set, FontSize_5x7);
				break;
			default:
				lcd_gotoxy(0, 0);
				lcd_clear_screen();
				draw_string("Default !", Pixel_Set, FontSize_5x7);
				break;
		}
		lcd_send_buff();

		do {
			c = getchar();
		}while(c != '\n' && c != EOF);

	}
	return 0;
}