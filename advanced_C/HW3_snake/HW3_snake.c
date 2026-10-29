
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses/ncurses.h>
#include <stdint.h>

#define MIN_Y 2
#define MOVE_INTERVAL (CLOCKS_PER_SEC / 8)

enum {
    LEFT = 1,
    UP,
    RIGHT,
    DOWN,
    STOP_GAME = KEY_F(10)
};

enum {
    MAX_TAIL_SIZE = 100,
    START_TAIL_SIZE = 3
};

struct control_buttons {
    int down;
    int up;
    int left;
    int right;
};

struct control_buttons default_controls = {
    KEY_DOWN, KEY_UP, KEY_LEFT, KEY_RIGHT
};

typedef struct tail_t {
    int x;
    int y;
} tail_t;

typedef struct snake_t {
    int x;
    int y;
    int direction;
    size_t tsize;
    tail_t *tail;
    struct control_buttons controls;
} snake_t;

/* Инициализация змейки */
int initSnake(snake_t *snake, size_t tail_size, int x, int y)
{
    if (tail_size > MAX_TAIL_SIZE || x < (int)tail_size ||
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
    snake->controls = default_controls;

    for (size_t i = 0; i < tail_size; i++) {
        snake->tail[i].x = x - (int)i - 1;
        snake->tail[i].y = y;
    }

    return 1;
}

/* Изменение направления движения */
void changeDirection(snake_t *snake, int key)
{
    int new_direction = snake->direction;

    if (key == snake->controls.left) {
        new_direction = LEFT;
    } else if (key == snake->controls.right) {
        new_direction = RIGHT;
    } else if (key == snake->controls.up) {
        new_direction = UP;
    } else if (key == snake->controls.down) {
        new_direction = DOWN;
    }

    /* Запрещаем разворот прямо в противоположную сторону */
    if ((snake->direction == LEFT && new_direction == RIGHT) ||
        (snake->direction == RIGHT && new_direction == LEFT) ||
        (snake->direction == UP && new_direction == DOWN) ||
        (snake->direction == DOWN && new_direction == UP)) {
        return;
    }

    snake->direction = new_direction;
}

/*
 * Перемещение змейки.
 * Возвращает 1 при успешном перемещении,
 * 0 при столкновении со стеной или собственным хвостом.
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

    /* Проверка выхода за границы игрового поля */
    if (new_x < 0 || new_x >= max_x ||
        new_y < MIN_Y || new_y >= max_y) {
        return 0;
    }

    /* Проверка столкновения головы с хвостом */
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

    /* Обновляем позицию головы */
    snake->x = new_x;
    snake->y = new_y;

    return 1;
}

/* Отрисовка игрового поля и змейки */
void drawSnake(const snake_t *snake)
{
    erase();

    mvprintw(0, 0,
             "Arrows: move | F10: exit | Crash: game over");

    for (size_t i = 0; i < snake->tsize; i++) {
        mvaddch(snake->tail[i].y, snake->tail[i].x, '*');
    }

    mvaddch(snake->y, snake->x, '@');

    refresh();
}

int main(void)
{
    snake_t snake;

    initscr();
    keypad(stdscr, TRUE);
    raw();
    noecho();
    curs_set(FALSE);

    /* getch() не блокирует цикл в ожидании клавиши */
    nodelay(stdscr, TRUE);

    int max_x, max_y;
    getmaxyx(stdscr, max_y, max_x);

    if (max_x < 15 || max_y < MIN_Y + 1) {
        endwin();
        printf("Terminal is too small for the game.\n");
        return 1;
    }

    if (!initSnake(&snake, START_TAIL_SIZE, 10, 10)) {
        endwin();
        printf("Failed to initialize snake.\n");
        return 1;
    }

    /* Проверяем, помещается ли начальная змейка на экране */
    if (snake.y >= max_y || snake.x >= max_x) {
        free(snake.tail);
        endwin();
        printf("Terminal is too small for the initial snake.\n");
        return 1;
    }

    int game_over = 0;
    int key_pressed;
    clock_t last_move = clock();

    drawSnake(&snake);

    while (!game_over) {
        key_pressed = getch();

        if (key_pressed == STOP_GAME) {
            break;
        }

        if (key_pressed != ERR) {
            changeDirection(&snake, key_pressed);
        }

        clock_t current_time = clock();

        if (current_time != (clock_t)-1 &&
            current_time - last_move >= MOVE_INTERVAL) {

            last_move = current_time;

            if (!moveSnake(&snake)) {
                game_over = 1;
                break;
            }

            drawSnake(&snake);
        }
    }

    free(snake.tail);
    endwin();

    if (game_over) {
        printf("Game over! The snake crashed.\n");
    }

    return 0;
}
