#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define ROWS 25
#define COLS 80
#define QUIT 'q'

void initialize_board(int board[ROWS][COLS]) {
    // Инициализация игровой доски случайными значениями
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            char ch;
            scanf("%c", &ch);
            board[i][j] = ch - '0';
        }
        char enter;
        scanf("%c", &enter);
    }
}

void print_board(int board[ROWS][COLS]) {
    // Вывод игровой доски на экран с использованием библиотеки ncurses
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (board[i][j]) {
                mvprintw(i, j, "o");
            } else {
                mvprintw(i, j, " ");
            }
        }
    }
    refresh();
}

int count_neighbors(int board[ROWS][COLS], int row, int col) {
    // Подсчет количества соседей клетки с координатами (row, col)
    int count = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) {
                continue;
            }
            int neighbor_row = (row + i + ROWS) % ROWS;
            int neighbor_col = (col + j + COLS) % COLS;
            if (neighbor_row >= 0 && neighbor_row < ROWS && neighbor_col >= 0 && neighbor_col < COLS) {
                if (board[neighbor_row][neighbor_col]) {
                    count++;
                }
            }
        }
    }
    return count;
}

void update_board(int board[ROWS][COLS]) {
    // Обновление состояния игровой доски
    int new_board[ROWS][COLS];
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            int neighbors = count_neighbors(board, i, j);
            if (board[i][j]) {
                if (neighbors < 2 || neighbors > 3) {
                    new_board[i][j] = 0;
                } else {
                    new_board[i][j] = 1;
                }
            } else {
                if (neighbors == 3) {
                    new_board[i][j] = 1;
                } else {
                    new_board[i][j] = 0;
                }
            }
        }
    }
    // Копирование новой доски в исходную
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            board[i][j] = new_board[i][j];
        }
    }
}

int main() {
    int board[ROWS][COLS];
    int speed = 100000;  // Скорость игры (в миллисекундах)

    // Инициализация игровой доски
    initialize_board(board);
    if (!freopen("/dev/tty", "r", stdin)) {
        return 0;
    }

    // Инициализация библиотеки ncurses
    initscr();
    timeout(speed);  // Установить время ожидания ввода
    keypad(stdscr, 1);
    halfdelay(10);

    // Игровой цикл
    int run = 1;
    while (run) {
        clear();
        print_board(board);
        usleep(100000);
        update_board(board);

        int ch = getch();
        if (ch == QUIT) {
            run = 0;
        }
        if (ch == KEY_UP) {
            speed -= 100000;
            if (speed < 0) {
                speed = 0;
            }
            timeout(speed);
        } else if (ch == KEY_DOWN) {
            speed += 100000;
            timeout(speed);
        }
    }

    // Завершение работы библиотеки ncurses
    endwin();

    return 0;
}