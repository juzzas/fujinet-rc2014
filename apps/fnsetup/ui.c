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

#include "console.h"

#include "ui.h"


/*
 *     ______        _ _ _   __     __
 *    / ____/_  __  (_|_) | / /__  / /_
 *   / /_  / / / / / / /  |/ / _ \/ __/
 *  / __/ / /_/ / / / / /|  /  __/ /_
 * /_/    \__,_/_/ /_/_/ |_/\___/\__/
 *            /___/
 */

#define MENU_TOP   12

static uint16_t tick_count;

static uint8_t current_menu_option = 0;
static uint8_t current_submenu_option = 0;

static uint8_t max_menu_options = 0;
static uint8_t max_submenu_options = 0;

#define BUFFER_SIZE 132
static char output_buffer[BUFFER_SIZE];

static struct ui_menu_context *menu_context;

static void print_menu_option(uint8_t index, bool hilight)
{
    if (hilight)
        console_puts("\x1b[7m");

    snprintf(output_buffer, BUFFER_SIZE, "\x1b[%d;%dH %c> %s ", MENU_TOP+index, 8, '1' + index, menu_context->items[index].name);
    console_puts(output_buffer);

    if (hilight)
        console_puts("\x1b[0m");
}

static void print_submenu_option(struct ui_menu_option *menu, uint8_t index, bool hilight)
{
    if (hilight)
        console_puts("\x1b[7m");

    snprintf(output_buffer, BUFFER_SIZE, "\x1b[%d;%dH %c> %s ", MENU_TOP+index, 40, 'A' + index, menu->suboptions[index].name);
    console_puts(output_buffer);

    if (hilight)
        console_puts("\x1b[0m");
}

void ui_refresh(void)
{
    int n = 0;

    for (n = 0; n < MAX_MENU_OPTIONS; n++) {
        snprintf(output_buffer, BUFFER_SIZE, "\x1b[%d;0H\x1b[K", MENU_TOP+n);
        console_puts(output_buffer);
    }

    max_menu_options = 0;
    n = 0;
    while (menu_context->items[n].name) {
        print_menu_option(n, (n == current_menu_option));
        max_menu_options++;
        n++;
    }

    max_submenu_options = 0;
    n = 0;
    while (menu_context->items[current_menu_option].suboptions[n].name) {
        print_submenu_option(&menu_context->items[current_menu_option], n, (n == current_submenu_option));
        max_submenu_options++;
        n++;
    }

    snprintf(output_buffer, BUFFER_SIZE, "\x1b[22;40H RETURN> %s ", menu_context->select_title ? menu_context->select_title : "SELECT");
    console_puts(output_buffer);

    snprintf(output_buffer, BUFFER_SIZE, "\x1b[23;40H    ESC> %s ", menu_context->cancel_title ? menu_context->cancel_title : "CANCEL");
    console_puts(output_buffer);
}

void ui_refresh_scr(void)
{
    console_puts("\x1b[2J"); // clear screen
    console_puts("\x1b[H");  // move cursor to top left
    console_puts("    ______        _ _ _   __     __ \n\r"
                 "   / ____/_  __  (_|_) | / /__  / /_\n\r"
                 "  / /_  / / / / / / /  |/ / _ \\/ __/\n\r"
                 " / __/ / /_/ / / / / /|  /  __/ /_  \n\r"
                 "/_/    \\__,_/_/ /_/_/ |_/\\___/\\__/  \n\r"
                 "           /___/                    \n\r");
    console_puts("                             Designed for RC2014\n\r");
    console_puts("                                        @jskists\n\r");

    ui_refresh();
}

void ui_set_menu(struct ui_menu_context *menu)
{
    menu_context = menu;

    ui_refresh_scr();
}

void ui_tick(void)
{
    char ch = 0;
    uint8_t new_menu_option = current_menu_option;
    uint8_t new_submenu_option = current_submenu_option;

    if (console_rx_avail()) {
        ch = console_rx();

        if ((ch >= 'a') && (ch < ('a' + max_submenu_options))) {
            new_submenu_option = ch - 'a';
        }

        if ((ch >= 'A') && (ch < ('A' + max_submenu_options))) {
            new_submenu_option = ch - 'A';
        }

        if ((ch >= '1') && (ch < ('1' + max_menu_options))) {
            new_menu_option = ch - '1';
            new_submenu_option = 0;
        }

        if (ch == 13) {
            if (menu_context->items[current_menu_option].suboptions[current_submenu_option].fnc) {
                menu_context->items[current_menu_option].suboptions[current_submenu_option].fnc();
                ui_refresh_scr();
            }
        }

        if ((new_menu_option != current_menu_option)
            || (new_submenu_option != current_submenu_option)) {
            current_menu_option = new_menu_option;
            current_submenu_option = new_submenu_option;

            ui_refresh();
        }
    }
}
