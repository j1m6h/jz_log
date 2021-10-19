#ifndef JZ_LOG_H 
#define JZ_LOG_H

/* we don't need the contents exposed */
struct jz_log;

size_t jz_get_log_size(struct jz_log* log);
void jz_set_log_name(const char* name, struct jz_log* log);

/* logging funcs */
void jz_info(struct jz_log* log, char* txt, ...);
void jz_warn(struct jz_log* log, char* txt, ...);
void jz_err(struct jz_log* log, char* txt, ...);

void jz_open_log(const char* path, struct jz_log* log);
void jz_close_log(struct jz_log* log);
#endif
