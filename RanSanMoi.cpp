/*
Ho Nguyen Minh Khoa
13/03/2024
Game do hoa Console - Ran san moi
*/

#include <iostream>
#include <conio.h>
#include <windows.h>
#include <ctime>
using namespace std;
#define MAX_SIZE 1000000
/* ----- Struct ----- */
typedef struct {
    int x = 0;
    int y = 0;
} Ran;

/* ----- Khoi tao ----- */
Ran ran[MAX_SIZE];
int kich_thuoc = 4;
int x_qua = -1;
int y_qua = -1;
int score = 0;

/* ----- Dinh nghia ham ----- */
void ve_tuong();
void tao_ran();
void ve_ran();
void di_chuyen(int X, int Y);
void play();
void tao_qua();
boolean game_over();

/* ----- Console.h ----- */
HANDLE hConsoleOutput;
HANDLE hConsoleInput;

void setColor(WORD color);
void gotoXY(SHORT x, SHORT y);
void Cursor(BOOL bVisible, DWORD dwSize);

/* ----- Main ----- */
int main() {
    /* ----- Set up ----- */
    ve_tuong();
    tao_ran();
    Cursor(false, 10);
    srand(time(NULL));
    tao_qua();
    
    /* ----- Play ----- */
    play();
    _getch();
    return 0;
}

void ve_tuong() {
    setColor(11);
    for (int x = 17; x < 100; x++) {
        gotoXY(x, 1);
        cout << "_";
        gotoXY(x, 27);
        cout << "_";
    }
    for (int y = 2; y < 28; y++) {
        gotoXY(17, y);
        cout << "|";
        gotoXY(99, y);
        cout << "|";
    }
}

void tao_ran() {
    int X = 60;
    int Y = 13;
    for (int i = 0; i < kich_thuoc; i++) {
        ran[i].x = X -= 2;
        ran[i].y = Y;
    }
}

void ve_ran() {
    gotoXY(ran[kich_thuoc].x, ran[kich_thuoc].y);
    cout << " ";
    setColor(4);
    gotoXY(ran[0].x, ran[0].y);
    cout << "0";
    gotoXY(ran[1].x, ran[1].y);
    cout << "o";
}

void di_chuyen(int X, int Y) {
    for (int i = kich_thuoc; i > 0; i--) {
        ran[i] = ran[i - 1];
    }
    ran[0].x = X;
    ran[0].y = Y;
}

void play() {
    bool running = true;
    int X = ran[0].x;
    int Y = ran[0].y;
    int move = 2;
    int speed = 150;
    int time_count = 0;
    while (running) {
        ve_ran();
        /* ----- Dieu khien ----- */
        if (_kbhit()) {
            char ch = _getch();
            if (ch == -32) ch = _getch();
            switch (ch) {
                case 72:
                    if (move != 4) move = 3;
                    break;
                case 80:
                    if (move != 3) move = 4;
                    break;
                case 75:
                    if (move != 2) move = 1;
                    break;
                case 77:
                    if (move != 1) move = 2;
                    break;
            }
        }
        /* ----- Chuyen huong ----- */
        if (move == 1) X -= 2;
        if (move == 2) X += 2;
        else if (move == 3) Y--;
        else if (move == 4) Y++;
        /* ----- Xu ly bien ----- */
        if (X == 100) X = 18;
        else if (X == 16) X = 98;
        else if (Y == 27) Y = 2;
        else if (Y == 1) Y = 26;
        /* ----- An qua ----- */
        if (X == x_qua && Y == y_qua) {
            kich_thuoc ++;
            score++;
            time_count++;
            tao_qua();
        }
        /* ----- Di chuyen ----- */
        di_chuyen(X, Y);
        /* ----- Game over ----- */
        if (game_over()) {
            /* ----- Xoa ran voi qua ----- */
            for (int i = 0; i <= kich_thuoc; i++) {
                gotoXY(ran[i].x, ran[i].y);;
                cout << " ";
            }
            gotoXY(x_qua, y_qua);
            cout << " ";
            /* ----- Thong bao ----- */
            gotoXY(55, 5);
            setColor(4);
            cout << "GAME OVER";
            gotoXY(53, 7);
            setColor(14);
            cout << "YOUR SCORE: " << score;
            // gotoXY(45, 9);
            // cout << "GAME CON RAN LAM BOI MINH KHOA";
            break;
        }
        /* ----- Toc do ----- */
        if (time_count == 5) {
            speed --;
            time_count = 0;
        }
        Sleep(speed);
        /* ----- Hien thi diem so ----- */
        gotoXY(55, 0);
        setColor(14);
        cout << "SCORES: " << score;
    }
}
void tao_qua() {
    x_qua = rand() % 81 + 17;
    if (x_qua % 2 == 1) x_qua++;
    y_qua = rand() % 24 + 3;
    for (int i = 0; i < kich_thuoc; i++) {
        if (x_qua == ran[i].x && y_qua == ran[i].y) tao_qua();
    }
    gotoXY(x_qua, y_qua);
    setColor(10);
    cout << "$";
}
boolean game_over() {
    for (int i = 1; i < kich_thuoc; i++) {
        if (ran[i].x == ran[0].x && ran[i].y == ran[0].y) return true;
    }
    return false;
}

/* ----- Console.h ----- */
void setColor(WORD color) {
	hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;
	GetConsoleScreenBufferInfo(hConsoleOutput, &screen_buffer_info);

	WORD wAttributes = screen_buffer_info.wAttributes;
	color &= 0x000f;
	wAttributes &= 0xfff0;
	wAttributes |= color;

	SetConsoleTextAttribute(hConsoleOutput, wAttributes);
}

void gotoXY(SHORT x, SHORT y) {
	COORD Cursor_an_Pos = { x, y };
	hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hConsoleOutput, Cursor_an_Pos);
}

void Cursor(BOOL bVisible, DWORD dwSize) {
	CONSOLE_CURSOR_INFO ConsoleCursorInfo;
	ConsoleCursorInfo.bVisible = bVisible;
	ConsoleCursorInfo.dwSize = dwSize;
	SetConsoleCursorInfo(hConsoleOutput, &ConsoleCursorInfo);
}
