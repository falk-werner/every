/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

struct context
{
    unsigned int interval;
    char * command;
    char * * args;
    bool show_help;
    int exit_code;
};

static void parse_interval(struct context * ctx, char const * value)
{
    ctx->interval = 0;

    while ('\0' != *value)
    {
        if (('0' <= *value) && (*value <= '9'))
        {
            ctx->interval *= 10;
            ctx->interval += (*value) - '0';
        }
        else
        {
            int const c = tolower(*value);
            switch (c)
            {
                case 's':
                    break;
                case 'm':
                    ctx->interval *= 60;
                    break;
                case 'h':
                    ctx->interval *= 60 * 60;
                    break;
                case 'd':
                    ctx->interval *= 60 * 60 * 24;
                    break;
                default:
                    ctx->interval = 0;
                    break;
            }

            if ('\0' != value[1])
            {
                ctx->interval = 0;
                break;
            }
        }

        value++;
    }


    if (ctx->interval == 0)
    {
        fprintf(stderr, "error: invalid interval");
        ctx->show_help = true;
        ctx->exit_code = EXIT_FAILURE;
    }
}

static void context_init(struct context * ctx, int argc, char * argv[])
{
    ctx->show_help = false;
    ctx->exit_code = EXIT_SUCCESS;

    if (argc > 2)
    {
        parse_interval(ctx, argv[1]);
        ctx->command = argv[2];
        ctx->args = &argv[2];
    }
    else
    {
        ctx->show_help = true;
        ctx->exit_code = ((argc > 1) && ((0 == strcmp(argv[1], "-h")) || (0 == strcmp(argv[1], "--help")))) ? EXIT_SUCCESS : EXIT_FAILURE;
    }
}

static void print_usage()
{
    printf(
        "every, Copyright (c) 2023 by Falk Werner\n"
        "run command periodically\n"
        "\n"
        "Usage:\n"
        "    every <interval> <command> [<arg> ...]\n"
        "\n"
        "Arguments:\n"
        "    interval - interval to run command\n"
        "    command  - command to run\n"
    );
}

int main(int argc, char * argv[])
{

    struct context ctx;
    context_init(&ctx, argc, argv);

    if (!ctx.show_help)
    {
        while (true)
        {
            pid_t const pid = fork();
            if (0 == pid)
            {
                int rc = execvp(ctx.command, ctx.args);
                if (0 != rc)
                {
                    exit(EXIT_FAILURE);
                }
            }
            else if (0 < pid)
            {
                int status = 0;
                waitpid(pid, &status, 0);
            }
            else
            {
                // fork failed
                // do nothing
            }

            unsigned int remaining = sleep(ctx.interval);
            while (0 != remaining)
            {
                remaining = sleep(remaining);
            }
        }
    }
    else
    {
        print_usage();
    }

    return ctx.exit_code;
}
