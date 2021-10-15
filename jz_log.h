#ifndef JZ_LOG_H
#define JZ_LOG_H

enum level 
{
	INFO,
	WARNING,
	ERROR,
	CRITICAL
};

void log_info(const char* txt);
void log_warn(const char* txt);
void log_err(const char* txt);
#endif
