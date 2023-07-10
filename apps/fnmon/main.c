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

// Command structure
typedef struct {
    const char* keyword;
    const char* help;
    enum CommandResult (*function)(char* tokens[], int num_tokens);
} Command;

// Command list
Command command_list[] = {
        {"help", "Display available commands", cmd_help},
        {"dir", "Display file directory", cmd_dir},
        {"dump", "memory dump", cmd_dump},
        {"hosts", "Display list of configured hosts", cmd_hosts},
        {"load", "Load file to memory", cmd_load},
        {"time", "Display current time", cmd_time},
        // Add more commands here
};

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
    enum CommandResult command_result;

    while (!done) {
        // Print the prompt
        printf("> ");

        // Read a line of input
        read_line(command, MAX_COMMAND_LENGTH);

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

