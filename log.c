#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "jz_log.h"

#define INFO    0
#define WARNING 1
#define ERROR   2

/* ANSI color codes */
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define BLUE	"\033[36m"
#define YELLOW  "\033[33m"
#define DEFAULT "\033[0m"

/* print to the console with a variable argument list */
#define VA_ARGS_PRINT(fmt) va_list args; \
	va_start(args, fmt);       	 \
	vprintf(fmt, args);              \
	printf("\n");		         \
	va_end(args);	                 \

struct jz_log
{
	FILE* file;
	char filename[30];
	char filepath[100];
};

static void get_timestamp(char* buf, size_t len)
{
	time_t rawtime;
	struct tm* timeinfo;

	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(buf, len, "%Y-%m-%d %H:%M:%S", timeinfo);
}

static void open_file(FILE* file, const char* path)
{
	file = fopen(path, "w");
	if (file == NULL)
	{
		fprintf(stderr, "Could not open file for writing");
		file_is_open = false;
	}

	file_is_open = true;
}

static char* lvl_to_tag(int lvl)
{
	if (lvl == WARNING)
		return "[WARNING]";
	if (lvl == ERROR)
		return "[ERROR]";
	return "[INFO]"; /* default use INFO */
}

static void set_color(const char* ansi_escape)
{
	printf(ansi_escape);
}

static void print_ts_and_tag(int lvl, const char* col)
{
	char ts[20];
	char* tag;

	get_timestamp(ts, 20);
	tag = lvl_to_tag(lvl);
	printf("%s ", ts);
	printf("%s%s ", col, tag);
	set_color(DEFAULT);
}

static void write_file(const char* txt, int lvl, struct jz_log* log)
{
	char buf[20];
	char* tag;

	get_timestamp(buf, 20);
	tag = lvl_to_tag(lvl);

	fprintf(log->file, "%s ", buf); /* timestamp */
	fprintf(log->file, "%s ", tag); /* level tag */
	fprintf(log->file, "%s\n", txt); /* log text */
}

size_t jz_get_log_size(struct jz_log* log)
{
	size_t prev;
	size_t size;

	prev = ftell(log->file);
	fseek(log->file, 0L, SEEK_END);
	size = ftell(log->file);
	fseek(log->file, prev, SEEK_SET);
	return size;
}

void jz_set_log_name(const char* name, struct jz_log* log)
{
	/* clear the previous filename */
	memset(log->filename, 0, strlen(log->filename));
	strcpy(log->filename, name);
}

void jz_info(struct jz_log* log, char* txt, ...)
{
	print_ts_and_tag(INFO, GREEN);

	VA_ARGS_PRINT(txt)
	write_file(txt, INFO, log);
}

void jz_warn(struct jz_log* log, char* txt, ...)
{
	print_ts_and_tag(WARNING, YELLOW);

	VA_ARGS_PRINT(txt)
	write_file(txt, WARNING, log);
}

void jz_err(struct jz_log* log, char* txt, ...)
{
	print_ts_and_tag(ERROR, RED);

	VA_ARGS_PRINT(txt)
	write_file(txt, ERROR, log);
}

void jz_open_log(const char* path, struct jz_log* log)
{
	char buf[20];

	/* print JZLOG banner */
	set_color(BLUE);
	printf(" ▐▄▄▄·▄▄▄▄•▄▄▌         ▄▄ • \n");
	printf("  ·██▪▀·.█▌██•  ▪     ▐█ ▀ ▪\n");
	printf("▪▄ ██▄█▀▀▀•██▪   ▄█▀▄ ▄█ ▀█▄\n");
	printf("▐▌▐█▌█▌▪▄█▀▐█▌▐▌▐█▌.▐▌▐█▄▪▐█\n");
	printf(" ▀▀▀•·▀▀▀ •.▀▀▀  ▀█▄▀▪·▀▀▀▀ \n");
	set_color(DEFAULT);

	/* filename */
	if (log->filename[0] == '\0')
	{
		get_timestamp(buf, 20); 
		strcpy(log->filename, "jz_log-");
		strcat(log->filename, buf);
	}

	/* full file path including filename */
	strcpy(log->filepath, path);
	strcat(log->filepath, log->filename);

	log->file = fopen(log->filepath, "w");
	if (log->file == NULL)
		fprintf(stderr, "Failed to open log file");
}

void jz_close_log(struct jz_log* log)
{
	fclose(log->file);
	exit(1);
}
