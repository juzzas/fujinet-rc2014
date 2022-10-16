/*
 * Copyright (c) 2022 Justin Skists.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
*/

#include <input.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>


/* 
 * |  \/  (_) | |  _ \    / \  / ___| ____|
 * | |\/| | | | | |_) |  / _ \| |   |  _|
 * | |  | | | | |  _ <  / ___ \ |___| |___
 * |_|  |_|_|_|_|_| \_\/_/   \_\____|_____|
 */
#define MAX_MENU_OPTIONS  9
#define MAX_SUBMENU_OPTIONS  5

#define MENU_TOP   12

typedef void (*fnc_test)(void);

struct ui_menu_option {
    const char *name;
    struct ui_submenu_option *suboptions;
};

struct ui_submenu_option {
    const char *name;
    fnc_test fnc;
};

static struct ui_submenu_option submenu_wifi[MAX_SUBMENU_OPTIONS] = {
    { "scan network", NULL },
    { "connect", NULL }
};

static struct ui_menu_option menu[MAX_MENU_OPTIONS] =
{
    { "Wifi Setup", submenu_wifi },
};

static uint16_t tick_count;

static uint8_t current_menu_option = 0;
static uint8_t current_submenu_option = 0;

static uint8_t max_menu_options = 0;
static uint8_t max_submenu_options = 0;

#define BUFFER_SIZE 132
static char output_buffer[BUFFER_SIZE];



static void print_menu_option(uint8_t index, bool hilight)
{
    if (hilight)
        puts("\x1b[7m");

    snprintf(output_buffer, BUFFER_SIZE, "\x1b[%d;%dH %c> %s ", MENU_TOP+index, 8, '1' + index, menu[index].name);
    puts(output_buffer);

    if (hilight)
        puts("\x1b[0m");
}

static void print_submenu_option(struct ui_menu_option *menu, uint8_t index, bool hilight)
{
    if (hilight)
        puts("\x1b[7m");

    snprintf(output_buffer, BUFFER_SIZE, "\x1b[%d;%dH %c> %s ", MENU_TOP+index, 40, 'A' + index, menu->suboptions[index].name);
    puts(output_buffer);

    if (hilight)
        puts("\x1b[0m");
}

void ui_refresh(void)
{
    int n;

    for (n = 0; n < MAX_MENU_OPTIONS; n++)
    {
        snprintf(output_buffer, BUFFER_SIZE, "\x1b[%d;0H\x1b[K", MENU_TOP+n);
        puts(output_buffer);
    }

    max_menu_options = 0;
    for (n = 0; n < MAX_MENU_OPTIONS; n++)
    {
        if (menu[n].name)
        {
            print_menu_option(n, (n == current_menu_option));
            max_menu_options++;
        }
    }

    max_submenu_options = 0;
    for (n = 0; n < MAX_SUBMENU_OPTIONS; n++)
    {
        if (menu[current_menu_option].suboptions[n].name)
        {
            print_submenu_option(&menu[current_menu_option], n, (n == current_submenu_option));
            max_submenu_options++;
        }
    }

    puts("\x1b[22;40H RETURN> EXECUTE ");
}

void ui_setup(void)
{
    puts("\x1b[2J"); // clear screen
    puts("\x1b[H");  // move cursor to top left
    puts("         __  __ _ _ _ ____      _    ____ _____ \n\r");
    puts("        |  \\/  (_) | |  _ \\    / \\  / ___| ____|\n\r"  );
    puts("        | |\\/| | | | | |_) |  / _ \\| |   |  _|\n\r"    );
    puts("        | |  | | | | |  _ <  / ___ \\ |___| |___\n\r"   );
    puts("        |_|  |_|_|_|_|_| \\_\\/_/   \\_\\____|_____|\n\r"  );
    puts("                         #RetroChallenge 2021/10\n\r"   );
    puts("                                        @jskists\n\r"   );

    ui_refresh();
}


void ui_tick(void)
{
    char ch = 0;
    uint8_t new_menu_option = current_menu_option;
    uint8_t new_submenu_option = current_submenu_option;

    if (kbhit())
    {
        ch = getchar();

        if ((ch >= 'a') && (ch < ('a' + max_submenu_options)))
        {
            new_submenu_option = ch - 'a';
        }

        if ((ch >= 'A') && (ch < ('A' + max_submenu_options)))
        {
            new_submenu_option = ch - 'A';
        }

        if ((ch >= '1') && (ch < ('1' + max_menu_options)))
        {
            new_menu_option = ch - '1';
            new_submenu_option = 0;
        }

        if (ch == 13)
        {
            if (menu[current_menu_option].suboptions[current_submenu_option].fnc)
            {
                menu[current_menu_option].suboptions[current_submenu_option].fnc();
                ui_setup();
            }
        }

        if ((new_menu_option != current_menu_option)
            || (new_submenu_option != current_submenu_option))
        {
            current_menu_option = new_menu_option;
            current_submenu_option = new_submenu_option;

            ui_refresh();
        }

    }
}
