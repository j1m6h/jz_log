#ifndef JZ_LOG_H
#define JZ_LOG_H

enum level 
{
	INFO,
	WARNING,
	ERROR
};

struct jz_log
{
	char* filename;
	size_t filesize; /* in bytes */
};

/* useful if you want to break your log apart once it reaches a certain size */
size_t get_filesize(struct jz_log* size);

void log_info(const char* txt, struct jz_log* log);
void log_warn(const char* txt, struct jz_log* log);
void log_err(const char* txt, struct jz_log* log);
#endif
