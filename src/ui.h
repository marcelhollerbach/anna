#ifndef UI_H
#define UI_H

typedef enum {
    UI_MODES_WAITING,
    UI_MODES_PROMPTING
} Ui_Modes;

typedef void (*System_Operation)(void);


int ui_init(void (*login_cb)(void), void (*user_changed)(const char *username));
void ui_mode_set(Ui_Modes modes);

const char* ui_get_user(void);
const char* ui_get_password(void);
void* ui_get_session(void);

void ui_add_session(const char *name, const char *icon, void *data);
void ui_add_active_session(const char *name, const char *icon, void *data, Evas_Smart_Cb cb);
void ui_add_system_op(System_Operation system_operation, const char *name, const char *icon);
void ui_select_session(const char *name);
void ui_display_icon(const char *something);
void ui_error_message(const char *title, const char *message);

#endif
