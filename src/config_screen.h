#ifndef CONFIG_H

typedef struct {
    struct {
        const char *file;
        const char *group;
    } background;

    const char *default_user;
} Config;

extern Config *config;

int config_init(void);
void config_shutdown(void);

void config_start(Evas_Object *win);

#endif
