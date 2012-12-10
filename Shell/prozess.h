#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "utils.h"
#include "listen.h"

typedef struct _PROZESS {
	pid_t pid;
	char* state;
	int signal;
	char* prog;
} Prozess;

Prozess neuerProzess(pid_t, char* state, int signal, char* prog);
