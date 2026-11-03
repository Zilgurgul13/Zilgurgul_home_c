#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses/ncurses.h>
#include <stdint.h>

#define MIN_Y 2
#define MOVE_INTERVAL (CLOCKS_PER_SEC / 8)

/* Направления движения */
enum {
    LEFT = 1,
    UP,
    RIGHT,
    DOWN,
    STOP_GAME = KEY_F(10)
};

/* Размеры змейки */
enum {
    MAX_TAIL_SIZE = 100,
    START_TAIL_SIZE = 3
};

/* Количество клавиш управления */
#define CONTROLS 12

/* Структура клавиши управления */
struct control_buttons {
    int32_t key;
    int direction;
};

/* Структура сегмента хвоста */
typedef struct tail_t {
    int x;
    int y;
} tail_t;

/* Структура змейки */
typedef struct snake_t {
    int x;
    int y;
    int direction;
    size_t tsize;
    tail_t *tail;
} snake_t;

/* Массив клавиш управления */
struct control_buttons default_controls[CONTROLS] = {
    {KEY_DOWN, DOWN},
    {KEY_UP, UP},
    {KEY_LEFT, LEFT},
    {KEY_RIGHT, RIGHT},

    {'w', UP},
    {'W', UP},

    {'s', DOWN},
    {'S', DOWN},

    {'a', LEFT},
    {'A', LEFT},

    {'d', RIGHT},
    {'D', RIGHT}
};

/* Инициализация змейки */
int initSnake(snake_t *snake, size_t tail_size, int x, int y)
{
    if (tail_size > MAX_TAIL_SIZE ||
        x < (int)tail_size ||
        y < MIN_Y) {
        return 0;
    }

    snake->tail = malloc(MAX_TAIL_SIZE * sizeof(tail_t));

    if (snake->tail == NULL) {
        return 0;
    }

    snake->x = x;
    snake->y = y;
    snake->direction = RIGHT;
    snake->tsize = tail_size;

    for (size_t i = 0; i < tail_size; i++) {
        snake->tail[i].x = x - (int)i - 1;
        snake->tail[i].y = y;
    }

    return 1;
}

/*
 * Проверка корректности выбранного направления.
 *
 * Возвращает новое направление, если оно допустимо.
 * Если клавиша не распознана или направление противоположно
 * текущему, возвращает текущее направление.
 */
int checkDirection(snake_t *snake, int32_t key)
{
    int new_direction = snake->direction;

    /* Перебираем все клавиши управления */
    for (int i = 0; i < CONTROLS; i++) {
        if (key == default_controls[i].key) {
            new_direction = default_controls[i].direction;
            break;
        }
    }

    /* Запрещаем разворот на 180 градусов */
    if ((snake->direction == LEFT && new_direction == RIGHT) ||
        (snake->direction == RIGHT && new_direction == LEFT) ||
        (snake->direction == UP && new_direction == DOWN) ||
        (snake->direction == DOWN && new_direction == UP)) {

        return snake->direction;
    }

    return new_direction;
}

/* Изменение направления движения */
void changeDirection(snake_t *snake, int32_t key)
{
    snake->direction = checkDirection(snake, key);
}

/*
 * Перемещение змейки.
 *
 * Возвращает:
 * 1 — перемещение успешно;
 * 0 — столкновение со стеной или хвостом.
 */
int moveSnake(snake_t *snake)
{
    int new_x = snake->x;
    int new_y = snake->y;

    switch (snake->direction) {
        case LEFT:
            new_x--;
            break;

        case RIGHT:
            new_x++;
            break;

        case UP:
            new_y--;
            break;

        case DOWN:
            new_y++;
            break;

        default:
            return 1;
    }

    int max_x, max_y;
    getmaxyx(stdscr, max_y, max_x);

    /* Проверка границ игрового поля */
    if (new_x < 0 || new_x >= max_x ||
        new_y < MIN_Y || new_y >= max_y) {
        return 0;
    }

    /* Проверка столкновения с собственным хвостом */
    for (size_t i = 0; i < snake->tsize; i++) {
        if (new_x == snake->tail[i].x &&
            new_y == snake->tail[i].y) {
            return 0;
        }
    }

    /* Сдвигаем сегменты хвоста */
    for (size_t i = snake->tsize; i > 1; i--) {
        snake->tail[i - 1] = snake->tail[i - 2];
    }

    if (snake->tsize > 0) {
        snake->tail[0].x = snake->x;
        snake->tail[0].y = snake->y;
    }

    /* Обновляем координаты головы */
    snake->x = new_x;
    snake->y = new_y;

    return 1;
}

/*
 * Отрисовка игрового поля и змейки.
 * Отображает код последней нажатой клавиши.
 */
void drawSnake(const snake_t *snake, int last_key)
{
    erase();

    mvprintw(0, 0,
             "Arrows / WASD: move | F10: exit | Crash: game over");

    /* Диагностика клавиатурного ввода */
    if (last_key != ERR) {
        mvprintw(1, 0, "Last key code: %d | Character: %c     ",
                 last_key, last_key);
    } else {
        mvprintw(1, 0, "Last key code: waiting...");
    }

    /* Отрисовка хвоста */
    for (size_t i = 0; i < snake->tsize; i++) {
        mvaddch(snake->tail[i].y, snake->tail[i].x, '*');
    }

    /* Отрисовка головы */
    mvaddch(snake->y, snake->x, '@');

    refresh();
}

int main(void)
{
    snake_t snake;

    /* Инициализация ncurses */
    initscr();
    keypad(stdscr, TRUE);
    raw();
    noecho();
    curs_set(FALSE);

    /* Неблокирующий ввод */
    nodelay(stdscr, TRUE);

    int max_x, max_y;
    getmaxyx(stdscr, max_y, max_x);

    if (max_x < 15 || max_y < MIN_Y + 1) {
        endwin();
        printf("Terminal is too small for the game.\n");
        return 1;
    }

    /* Инициализация змейки */
    if (!initSnake(&snake, START_TAIL_SIZE, 10, 10)) {
        endwin();
        printf("Failed to initialize snake.\n");
        return 1;
    }

    /* Проверка начального положения */
    if (snake.y >= max_y || snake.x >= max_x) {
        free(snake.tail);
        endwin();
        printf("Terminal is too small for the initial snake.\n");
        return 1;
    }

    int game_over = 0;
    int key_pressed;
    int last_key = ERR;

    clock_t last_move = clock();

    /* Первоначальная отрисовка */
    drawSnake(&snake, last_key);

    while (!game_over) {

        /* Получаем нажатую клавишу */
        key_pressed = getch();

        /* Выход из игры */
        if (key_pressed == STOP_GAME) {
            break;
        }

        /* Обработка нажатия клавиши */
        if (key_pressed != ERR) {
            last_key = key_pressed;

            changeDirection(&snake, key_pressed);

            /* Немедленное обновление экрана */
            drawSnake(&snake, last_key);
        }

        /* Проверка времени следующего перемещения */
        clock_t current_time = clock();

        if (current_time != (clock_t)-1 &&
            current_time - last_move >= MOVE_INTERVAL) {

            last_move = current_time;

            /* Перемещение змейки */
            if (!moveSnake(&snake)) {
                game_over = 1;
                break;
            }

            /* Обновление экрана после перемещения */
            drawSnake(&snake, last_key);
        }
    }

    /* Освобождение памяти */
    free(snake.tail);

    /* Завершение работы ncurses */
    endwin();

    if (game_over) {
        printf("Game over! The snake crashed.\n");
    }

    return 0;
}

