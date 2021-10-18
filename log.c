#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "jz_log.h"

/* ascii color codes */
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define DEFAULT "\033[0m"

static char* get_timestamp(char* buf, size_t len)
{
	time_t rawtime;
	struct tm* timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	strftime(buf, len, "%Y-%m-%d %I:%M:%S", timeinfo);
	return buf;
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

static void write_file(const char* txt, int lvl)
{
	FILE* fp;
	char* filename;
	char  buf[20];
	char* ts;
	char* tag;

	/* get timestamp and convert the log level to a text tag */
	ts = get_timestamp(buf, 20);
	tag = lvl_to_tag(lvl);

	/* allocate mem for the filename string */
	filename = (char*)malloc((8 + strlen(ts) + 1) * sizeof(char));
	strcpy(filename, "jz_log-");
	strcat(filename, ts);

	fp = fopen(filename, "w");
	if (fp == NULL) 
	{
		print("**Failed to open log file for writing**", RED, ERROR); /* use our print func */ 
		exit(1);
	}

	fprintf(fp, "%s ", ts); /* timestamp */
	fprintf(fp, "%s ", tag); /* level tag */
	fprintf(fp, "%s\n", txt); /* log text */
	
	free(filename);
	fclose(fp);
}

size_t get_filesize(struct jz_log* log)
{
	FILE* fp;
	size_t prev;
	size_t size;

	fp = fopen(log->filename, "r");
	if (fp == NULL)
		exit(1);

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
