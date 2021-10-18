#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "jz_log.h"

/* ANSI color codes */
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define DEFAULT "\033[0m"

struct jz_log
{
	FILE* file;
	char filename[22];
}

static bool file_is_open = false;

static char* get_timestamp(char* buf, size_t len)
{
	time_t rawtime;
	struct tm* timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(buf, len, "%Y-%m-%d %H:%M:%S", timeinfo);
	return buf;
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

static void print(const char* txt, const char* col, int lvl)
{
	size_t len = 20;
	char buf[len];
	char* ts = get_timestamp(buf, len);
	char* tag = lvl_to_tag(lvl);
	printf("%s%s %s %s\n", col, ts, tag, txt); 
}

static void write_file(const char* txt, int lvl, struct jz_log* log)
{
	char  buf[20];
	char* ts;
	char* tag;

	/* get timestamp and convert the log level to a text tag */
	ts = get_timestamp(buf, 20);
	tag = lvl_to_tag(lvl);

	/* put the filename together */
	strcpy(log->filename, "jz_log-");
	strcat(log->filename, ts);

	if (!file_is_open)
		open_file(log->file);

	fprintf(log->file, "%s ", ts); /* timestamp */
	fprintf(log->file, "%s ", tag); /* level tag */
	fprintf(log->file, "%s\n", txt); /* log text */
}

size_t get_filesize(struct jz_log* log)
{
	size_t prev;
	size_t size;

	if (!file_is_open)
		open_file(log->file);

	prev = ftell(fp);
	fseek(fp, 0L, SEEK_END);
	size = ftell(fp);
	fseek(fp, prev, SEEK_SET);
	return size;
}

void log_info(const char* txt, struct jz_log* log)
{
	print(txt, GREEN, INFO);
	write_file(txt, INFO);
}

void log_warn(const char* txt, struct jz_log* log)
{
	print(txt, YELLOW, WARNING);
	write_file(txt, WARNING);
}

void log_err(const char* txt, struct jz_log* log)
{
	print(txt, RED, ERROR);
	write_file(txt, ERROR);
}

void close_log(struct jz_log* log)
{
	fclose(log->filename);
	exit(1);
}
