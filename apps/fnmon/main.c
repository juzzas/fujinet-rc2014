//
// Created by jskists on 21/06/2023.
//
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "cmd.h"
#include "console.h"

#define MAX_COMMAND_LENGTH 100
#define MAX_TOKENS 10
#define MAX_ERROR_MESSAGE_LENGTH 100

#pragma scanf = "%["
#pragma printf = "%x %s %d"

static bool done = false;

enum CommandResult process_command(const char* command);
void read_line(char* buffer, int max_length);
void tokenize_command(const char* command, char* tokens[], int* num_tokens);
enum CommandResult execute_command(const char* command, char* tokens[], int num_tokens);

// core command functions
enum CommandResult help_command(char* tokens[], int num_tokens);

// VT102 Escape Sequences
#define VT102_ESCAPE "\x1B"
#define VT102_ERASE_LINE (VT102_ESCAPE "[K")

void vt102_cursor_left(int n) {
    if (n == 0)
        return;

    printf(VT102_ESCAPE "[%dD", n);
}

void vt102_cursor_right(int n) {
    if (n == 0)
        return;

    printf(VT102_ESCAPE "[%dC", n);
}

void vt102_erase_line() {
    printf(VT102_ERASE_LINE);
}


// Command structure
typedef struct {
    const char* keyword;
    const char* help;
    enum CommandResult (*function)(char* tokens[], int num_tokens);
} Command;

// Command list
Command command_list[] = {
        {"help", "Display available commands", cmd_help},
        {"time", "Display current time", cmd_time},
        {"dump", "memory dump", cmd_dump},
        // Add more commands here
};

// Command history
#define MAX_HISTORY_COMMANDS 4
char history_commands[MAX_HISTORY_COMMANDS][MAX_COMMAND_LENGTH];
int history_index = 0;

// Command functions
enum CommandResult cmd_help(char* tokens[], int num_tokens) {
    // Print the list of available commands and their help texts
    for (int i = 0; i < sizeof(command_list) / sizeof(Command); i++) {
        printf("%s: %s\n", command_list[i].keyword, command_list[i].help);
    }

    return COMMAND_SUCCESS;
}

int main() {
    char command[MAX_COMMAND_LENGTH];
    char buffer[MAX_COMMAND_LENGTH];
    enum CommandResult command_result = COMMAND_SUCCESS;

    history_index = 0;

    while (!done) {
        // Print the prompt
        printf("> ");

        // Read a line of input
        buffer[0] = '\0';
        read_line(command, MAX_COMMAND_LENGTH);

        // add command to history
        strncpy(history_commands[history_index], command, MAX_COMMAND_LENGTH);
        history_index = (history_index + 1) % MAX_HISTORY_COMMANDS;

        // Process the command and get the result/error code
        command_result = process_command(command);

        // Check if there is an error
        if (command_result != COMMAND_SUCCESS) {
            // Format the error message
            printf("Command failed with error code: %d\n", command_result);
        }
    }

    return 0;
}

enum CommandResult process_command(const char* command) {
    char* tokens[MAX_TOKENS];
    int num_tokens = 0;

    // Tokenize the command
    tokenize_command(command, tokens, &num_tokens);

    // Execute the command and get the result/error code
    return execute_command(tokens[0], tokens, num_tokens);
}

void read_line(char* buffer, int max_length) {
    fflush(stdin);
    scanf("%80[^\n\x1a]", buffer);

#if 0
    int buffer_index = 0;
    int cursor_index = 0;
    char c;

    while (true) {
            c = fgetc(stdin);

            if (c == '\n') {
                break;
            } else if (c == 0x1B) { // ESC key pressed
                // Handle VT102 control sequences
                char seq[3];
                seq[0] = fgetc(stdin);
                seq[1] = fgetc(stdin);
                seq[2] = '\0';

                if (strcmp(seq, "[A") == 0) { // Up arrow key pressed
                    // Move cursor to the beginning of the line
                    vt102_cursor_left(cursor_index);

                    // Erase the current line
                    vt102_erase_line();

                    // Calculate the previous command index in history
                    int prev_index = (history_index + MAX_HISTORY_COMMANDS - 1) % MAX_HISTORY_COMMANDS;

                    // Copy the previous command to the buffer
                    strncpy(buffer, history_commands[prev_index], max_length);

                    // Update the buffer and cursor positions
                    buffer_index = strlen(buffer);
                    cursor_index = buffer_index;

                    // Print the previous command
                    console_put_buffer(buffer, buffer_index);
                } else if (strcmp(seq, "[D") == 0) { // Left arrow key pressed
                    if (buffer_index > 0) {
                        // Move cursor left
                        cursor_index--;

                        vt102_cursor_left(1);
                    }
                } else if (strcmp(seq, "[C") == 0) { // Right arrow key pressed
                    if (cursor_index < buffer_index) {
                        // Move cursor right
                        cursor_index++;

                        vt102_cursor_right(1);
                    }
                } else {
                    // Ignore unrecognized VT102 control sequences
                    fgetc(stdin);
                }
            } else if (c == 0x7F || c == '\b') { // Backspace key pressed
                if (buffer_index > 0) {
                    // Move cursor left
                    vt102_cursor_left(1);

                    // Erase the character at the cursor position
                    vt102_erase_line();

                    // Move characters to the left in the buffer
                    memmove(&buffer[cursor_index - 1], &buffer[cursor_index], buffer_index - cursor_index);

                    buffer_index--;
                    cursor_index--;

                    // Print the updated buffer
                    console_put_buffer(buffer + cursor_index, buffer_index - cursor_index + 1);

                    // Move cursor to the correct position
                    vt102_cursor_left(buffer_index - cursor_index);
                }
            } else if (c >= 0x20 && c <= 0x7E && buffer_index < max_length - 1) { // Printable character
                // Move characters to the right in the buffer
                memmove(&buffer[cursor_index + 1], &buffer[cursor_index], buffer_index - cursor_index);

                // Store the new character in the buffer at the cursor position
                buffer[cursor_index] = c;

                buffer_index++;
                cursor_index++;

                // Print the updated buffer
                console_put_buffer(buffer + cursor_index - 1, buffer_index - cursor_index + 1);

                // Move cursor to the correct position
                vt102_cursor_left(buffer_index - cursor_index);
            }
        }

    // Null-terminate the buffer
    buffer[buffer_index] = '\0';

    // Move cursor to the next line
    console_puts("\n");
#endif
}

void tokenize_command(const char* command, char* tokens[], int* num_tokens) {
    // Make a copy of the command
    char command_copy[MAX_COMMAND_LENGTH];
    strncpy(command_copy, command, sizeof(command_copy));

    // Tokenize the command
    char* token = strtok(command_copy, " \t\n");
    *num_tokens = 0;

    while (token != NULL && *num_tokens < MAX_TOKENS) {
        tokens[*num_tokens] = token;
        (*num_tokens)++;
        token = strtok(NULL, " \t\n");
    }
}

enum CommandResult execute_command(const char* command, char* tokens[], int num_tokens) {
    // Find the command in the command list
    for (int i = 0; i < sizeof(command_list) / sizeof(Command); i++) {
        if (strcmp(command, command_list[i].keyword) == 0) {
            // Call the command's function and return the result/error code
            return command_list[i].function(tokens, num_tokens);
        }
    }

    // Command not found
    return COMMAND_ERROR_UNKNOWN_COMMAND; // Return an error code
}
