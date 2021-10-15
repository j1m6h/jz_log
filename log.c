#include <stdio.h>
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
	return "[INFO]";
}

static void print(const char* txt, const char* col, int lvl)
{
	size_t len = 20;
	char buf[len];
	char* ts = get_timestamp(buf, len);
	char* tag = lvl_to_tag(lvl);
	printf("%s%s %s %s\n", col, ts, tag, txt); 
}

void log_info(const char* txt)
{
	print(txt, GREEN, INFO);
}

void log_warn(const char* txt)
{
	print(txt, YELLOW, WARNING);
}

void log_err(const char* txt)
{
	print(txt, RED, ERROR);
}
