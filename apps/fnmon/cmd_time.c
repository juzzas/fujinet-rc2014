//
// Created by jskists on 03/07/2023.
//
#include <stdio.h>

#include "cmd.h"
#include "console.h"
#include "console_utils.h"

enum CommandResult cmd_time(char* tokens[], int num_tokens) {
    // TODO: Implement the time command logic here
    console_puts("Current time: ...\n");

    return COMMAND_SUCCESS;
}

