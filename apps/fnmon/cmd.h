//
// Created by justin on 31/12/2022.
//


#ifndef FNMON_CMD_H
#define FNMON_CMD_H

// Function prototypes
enum CommandResult {
    COMMAND_SUCCESS,
    COMMAND_ERROR_INVALID_ARGUMENTS,
    COMMAND_ERROR_UNKNOWN_COMMAND,
    COMMAND_ERROR_INVALID_INPUT,
    COMMAND_ERROR_FUJINET
};

// command functions
enum CommandResult cmd_time(char* tokens[], int num_tokens);
enum CommandResult cmd_dump(char* tokens[], int num_tokens);
enum CommandResult cmd_edit(char* tokens[], int num_tokens);
enum CommandResult cmd_dir(char* tokens[], int num_tokens);
enum CommandResult cmd_hexload(char* tokens[], int num_tokens);
enum CommandResult cmd_hosts(char* tokens[], int num_tokens);
enum CommandResult cmd_load(char* tokens[], int num_tokens);
enum CommandResult cmd_reset(char* tokens[], int num_tokens);


#endif //FNMON_CMD_H
