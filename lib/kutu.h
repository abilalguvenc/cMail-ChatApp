#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <locale.h>

/*
	FONKSÝYONLAR
	
	KonsolBoyutu
	gotoxy
	KutuCiz
	GolgeCiz
*/

void KonsolBoyutu (int X, int Y)
{
    SMALL_RECT rect;
    COORD coord;
    coord.X = X; // Defining our X and
    coord.Y = Y;  // Y size for buffer.

    rect.Top = 0;
    rect.Left = 0;
    rect.Bottom = coord.Y-1; // height for window
    rect.Right = coord.X-1;  // width for window

    HANDLE hwnd = GetStdHandle(STD_OUTPUT_HANDLE); // get handle
    SetConsoleScreenBufferSize(hwnd, coord);       // set buffer size
    SetConsoleWindowInfo(hwnd, TRUE, &rect);       // set window size
}

void gotoxy(short x, short y) 
{
     COORD pos = {x, y};
     SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void KutuCiz (int su, int sa, int x, int y) 
{
	int i, j;
	
	for (j=0;j<y;j++) 
	{
		gotoxy (su,sa+j);
		for (i=0;i<x;i++) 
		{
			printf(" ");
		}
	}
}

void GolgeCiz (int su, int sa, int x, int y) 
{
	int i;
	
	for (i=1;i<y;i++) 
	{
		gotoxy (su+x,sa+i);
		printf(" ");
	}
	gotoxy (su+1,sa+y);
	for (i=1;i<x;i++)
	{
        printf(" ");
    }
    printf(" ");
}
