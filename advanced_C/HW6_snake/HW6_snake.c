
#define _XOPEN_SOURCE_EXTENDED 1

#include <locale.h>
#include <wchar.h>
#include <ncurses/curses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_SNAKE 500
#define FOOD_COUNT 3
#define TICK_MS 120

typedef enum {
    LEFT = 0,
    UP,
    RIGHT,
    DOWN
} Direction;

typedef struct {
    int x;
    int y;
} Point;

typedef struct {
    Point body[MAX_SNAKE];
    int length;
    Direction dir;
    int color;
    int score;
    int controls[4];
} Snake;

typedef struct {
    Point p;
} Food;

/* Глобальные переменные */

static Snake snakes[2];
static Food foods[FOOD_COUNT];

static int player_count = 1;
static int field_w;
static int field_h;

/* Названия цветов */

static const wchar_t *color_names[] = {
    L"Зелёный",
    L"Голубой",
    L"Жёлтый",
    L"Пурпурный"
};

/* Проверка совпадения координат */

static bool same_point(Point a, Point b)
{
    return a.x == b.x && a.y == b.y;
}

/* Проверка противоположного направления */

static bool opposite(Direction a, Direction b)
{
    return
        (a == LEFT && b == RIGHT) ||
        (a == RIGHT && b == LEFT) ||
        (a == UP && b == DOWN) ||
        (a == DOWN && b == UP);
}

/* Вывод текста по центру экрана */

static void draw_center(int y, const wchar_t *s)
{
    int width = (int)wcslen(s);
    mvaddwstr(y, (COLS - width) / 2, s);
}

/* Инициализация цветов */

static void init_colors(void)
{
    if (!has_colors())
        return;

    start_color();

    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_CYAN, COLOR_BLACK);
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(6, COLOR_WHITE, COLOR_BLACK);
}

/* Стартовое меню */

static void startMenu(void)
{
    int choice = 0;
    int color1 = 0;
    int color2 = 1;

    keypad(stdscr, TRUE);
    nodelay(stdscr, FALSE);
    curs_set(FALSE);

    /* Выбор количества игроков */

    while (1)
    {
        erase();

        attron(A_BOLD);
        draw_center(2, L"ИГРА «ЗМЕЙКА»");
        attroff(A_BOLD);

        draw_center(5, L"Выберите режим:");

        draw_center(
            7,
            choice == 0 ? L"> Один игрок" : L"  Один игрок"
        );

        draw_center(
            8,
            choice == 1 ? L"> Два игрока" : L"  Два игрока"
        );

        draw_center(
            11,
            L"Управление: WASD — игрок 1, стрелки — игрок 2"
        );

        draw_center(
            13,
            L"Enter — продолжить, Esc — выход"
        );

        refresh();

        wint_t ch;
        int result = get_wch(&ch);

        if (result == KEY_CODE_YES &&
            (ch == KEY_UP || ch == KEY_DOWN))
        {
            choice = 1 - choice;
        }
        else if (result == OK && ch == 27)
        {
            endwin();
            exit(0);
        }
        else if ((result == OK && ch == L'\n') ||
                 (result == KEY_CODE_YES && ch == KEY_ENTER))
        {
            break;
        }
    }

    player_count = choice + 1;

    /* Выбор цвета каждой змейки */

    for (int p = 0; p < player_count; ++p)
    {
        int selected = (p == 0) ? color1 : color2;

        while (1)
        {
            erase();

            wchar_t title[80];

            swprintf(
                title,
                80,
                L"Цвет змейки игрока %d",
                p + 1
            );

            draw_center(3, title);

            for (int i = 0; i < 4; ++i)
            {
                int y = 6 + i;

                if (has_colors())
                    attron(COLOR_PAIR(i + 2) | A_BOLD);

                mvaddwstr(
                    y,
                    (COLS - 20) / 2,
                    i == selected ? L"> " : L"  "
                );

                mvaddwstr(
                    y,
                    (COLS - 20) / 2 + 2,
                    color_names[i]
                );

                if (has_colors())
                    attroff(COLOR_PAIR(i + 2) | A_BOLD);
            }

            draw_center(
                12,
                L"Стрелки — выбор, Enter — подтвердить"
            );

            refresh();

            wint_t ch;
            int result = get_wch(&ch);

            if (result == KEY_CODE_YES && ch == KEY_UP)
                selected = (selected + 3) % 4;

            else if (result == KEY_CODE_YES && ch == KEY_DOWN)
                selected = (selected + 1) % 4;

            else if ((result == OK && ch == L'\n') ||
                     (result == KEY_CODE_YES && ch == KEY_ENTER))
                break;
        }

        if (p == 0)
            color1 = selected;
        else
            color2 = selected;
    }

    /* Сохраняем выбранные цвета */

    snakes[0].color = color1 + 2;
    snakes[1].color = color2 + 2;
}

/* Инициализация змейки */

static void init_snake(
    Snake *s,
    int x,
    int y,
    int color,
    bool second
)
{
    s->length = 4;
    s->dir = RIGHT;
    s->color = color;
    s->score = 0;

    for (int i = 0; i < s->length; ++i)
    {
        s->body[i].x = x - i;
        s->body[i].y = y;
    }

    /* Управление первого игрока */

    if (!second)
    {
        s->controls[LEFT] = 'a';
        s->controls[UP] = 'w';
        s->controls[RIGHT] = 'd';
        s->controls[DOWN] = 's';
    }
    /* Управление второго игрока */

    else
    {
        s->controls[LEFT] = KEY_LEFT;
        s->controls[UP] = KEY_UP;
        s->controls[RIGHT] = KEY_RIGHT;
        s->controls[DOWN] = KEY_DOWN;
    }
}

/* Проверка занятости координат змейками */

static bool occupied(Point p)
{
    for (int s = 0; s < player_count; ++s)
    {
        for (int i = 0; i < snakes[s].length; ++i)
        {
            if (same_point(p, snakes[s].body[i]))
                return true;
        }
    }

    return false;
}

/* Проверка, занята ли точка другой едой */

static bool food_occupied(Point p, int current)
{
    for (int i = 0; i < FOOD_COUNT; ++i)
    {
        if (i == current)
            continue;

        if (same_point(p, foods[i].p))
            return true;
    }

    return false;
}

/* Генерация свободной точки */

static Point random_empty_point(int current_food)
{
    Point p = {1, 2};

    for (int attempts = 0; attempts < 10000; ++attempts)
    {
        p.x = 1 + rand() % (field_w - 2);
        p.y = 2 + rand() % (field_h - 3);

        if (!occupied(p) && !food_occupied(p, current_food))
            return p;
    }

    return p;
}

/* Создание еды */

static void init_food(void)
{
    for (int i = 0; i < FOOD_COUNT; ++i)
    {
        foods[i].p = random_empty_point(i);
    }
}

/* Отрисовка рамки */

static void draw_border(void)
{
    if (has_colors())
        attron(COLOR_PAIR(6));

    for (int x = 0; x < field_w; ++x)
    {
        mvaddch(1, x, ACS_HLINE);
        mvaddch(field_h - 1, x, ACS_HLINE);
    }

    for (int y = 1; y < field_h; ++y)
    {
        mvaddch(y, 0, ACS_VLINE);
        mvaddch(y, field_w - 1, ACS_VLINE);
    }

    mvaddch(1, 0, ACS_ULCORNER);
    mvaddch(1, field_w - 1, ACS_URCORNER);

    mvaddch(field_h - 1, 0, ACS_LLCORNER);
    mvaddch(field_h - 1, field_w - 1, ACS_LRCORNER);

    if (has_colors())
        attroff(COLOR_PAIR(6));
}

/* Отрисовка еды */

static void draw_food(void)
{
    for (int i = 0; i < FOOD_COUNT; ++i)
    {
        if (has_colors())
            attrset(COLOR_PAIR(1) | A_BOLD);

        mvaddch(
            foods[i].p.y,
            foods[i].p.x,
            '$'
        );
    }

    attrset(A_NORMAL);
}

/* Отрисовка змеек */

static void draw_snakes(void)
{
    for (int s = 0; s < player_count; ++s)
    {
        int pair = snakes[s].color;

        for (int i = snakes[s].length - 1; i >= 0; --i)
        {
            if (has_colors())
                attrset(COLOR_PAIR(pair) | A_BOLD);
            else
                attrset(A_BOLD);

            mvaddch(
                snakes[s].body[i].y,
                snakes[s].body[i].x,
                i == 0 ? '@' : '*'
            );
        }
    }

    attrset(A_NORMAL);
}

/* Отрисовка игрового поля */

static void draw_game(void)
{
    erase();

    draw_border();
    draw_food();
    draw_snakes();

    wchar_t status[160];

    if (player_count == 1)
    {
        swprintf(
            status,
            160,
            L"Счёт: %d   F10 — выход",
            snakes[0].score
        );
    }
    else
    {
        swprintf(
            status,
            160,
            L"Игрок 1: %d     Игрок 2: %d     F10 — выход",
            snakes[0].score,
            snakes[1].score
        );
    }

    attrset(A_NORMAL);
    mvaddwstr(0, 1, status);

    refresh();
}

/* Изменение направления движения */

static void change_direction(Snake *s, wint_t key)
{
    /*
       Преобразуем русские буквы в соответствующие
       латинские клавиши WASD.

       Английская раскладка:
       A — влево, W — вверх,
       D — вправо, S — вниз.

       Русская раскладка:
       Ф — влево, Ц — вверх,
       В — вправо, Ы — вниз.
    */

    if (key == L'ф' || key == L'Ф')
        key = 'a';

    else if (key == L'ц' || key == L'Ц')
        key = 'w';

    else if (key == L'в' || key == L'В')
        key = 'd';

    else if (key == L'ы' || key == L'Ы')
        key = 's';

    /* Поддержка заглавных латинских WASD */

    if (key == L'A') key = 'a';
    if (key == L'W') key = 'w';
    if (key == L'D') key = 'd';
    if (key == L'S') key = 's';

    for (int d = 0; d < 4; ++d)
    {
        if (key == (wint_t)s->controls[d])
        {
            if (!opposite((Direction)d, s->dir))
                s->dir = (Direction)d;

            return;
        }
    }
}

/* Движение змейки */

static void move_snake(Snake *s)
{
    Point next = s->body[0];

    if (s->dir == LEFT)
        --next.x;

    else if (s->dir == RIGHT)
        ++next.x;

    else if (s->dir == UP)
        --next.y;

    else if (s->dir == DOWN)
        ++next.y;

    /* Проход через границы поля */

    if (next.x < 1)
        next.x = field_w - 2;

    if (next.x >= field_w - 1)
        next.x = 1;

    if (next.y < 2)
        next.y = field_h - 2;

    if (next.y >= field_h - 1)
        next.y = 2;

    /* Проверка поедания еды */

    int eaten_food = -1;

    for (int f = 0; f < FOOD_COUNT; ++f)
    {
        if (same_point(next, foods[f].p))
        {
            eaten_food = f;
            ++s->score;
            break;
        }
    }

    /* Увеличение длины змейки */

    int new_length = s->length;

    if (eaten_food >= 0 && s->length < MAX_SNAKE - 1)
        ++new_length;

    /* Перемещение тела */

    for (int i = new_length - 1; i > 0; --i)
    {
        s->body[i] = s->body[i - 1];
    }

    s->body[0] = next;
    s->length = new_length;

    /* Перемещаем съеденную еду после движения */

    if (eaten_food >= 0)
    {
        foods[eaten_food].p = random_empty_point(eaten_food);
    }
}

/* Экран завершения игры */

static void show_game_over(void)
{
    nodelay(stdscr, FALSE);

    erase();

    draw_center(
        LINES / 2 - 2,
        L"ИГРА ОКОНЧЕНА"
    );

    wchar_t result[120];

    if (player_count == 1)
    {
        swprintf(
            result,
            120,
            L"Ваш счёт: %d",
            snakes[0].score
        );
    }
    else
    {
        swprintf(
            result,
            120,
            L"Игрок 1: %d     Игрок 2: %d",
            snakes[0].score,
            snakes[1].score
        );
    }

    draw_center(LINES / 2, result);

    draw_center(
        LINES / 2 + 2,
        L"Нажмите любую клавишу для выхода"
    );

    refresh();

    wint_t ch;
    get_wch(&ch);
}

/* Главная функция */

int main(void)
{
    setlocale(LC_ALL, "");

    initscr();

    init_colors();

    if (LINES < 15 || COLS < 40)
    {
        endwin();

        fprintf(
            stderr,
            "Увеличьте размер окна терминала минимум до 40x15.\n"
        );

        return 1;
    }

    /* Стартовое меню */

    startMenu();

    field_w = COLS;
    field_h = LINES;

    srand((unsigned)time(NULL));

    /* Создание первой змейки */

    init_snake(
        &snakes[0],
        field_w / 3,
        field_h / 2,
        snakes[0].color,
        false
    );

    /* Создание второй змейки */

    if (player_count == 2)
    {
        init_snake(
            &snakes[1],
            (field_w * 2) / 3,
            field_h / 2,
            snakes[1].color,
            true
        );
    }

    init_food();

    /* Настройка клавиатуры */

    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);

    noecho();
    curs_set(FALSE);

    /* Основной игровой цикл */

    while (1)
    {
        wint_t key;
        int result = get_wch(&key);

        /* Обрабатываем все доступные нажатия */

        while (result != ERR)
        {
            if (result == KEY_CODE_YES && key == KEY_F(10))
                goto exit_game;

            for (int s = 0; s < player_count; ++s)
            {
                change_direction(&snakes[s], key);
            }

            result = get_wch(&key);
        }

        /* Движение всех змеек */

        for (int s = 0; s < player_count; ++s)
        {
            move_snake(&snakes[s]);
        }

        /* Отрисовка */

        draw_game();

        /* Пауза между игровыми тактами */

        napms(TICK_MS);
    }

exit_game:

    show_game_over();

    endwin();

    return 0;
}
