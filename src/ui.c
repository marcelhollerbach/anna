#include "main.h"

#define GROUND "public.background"
#define WAITING "public.waiting"
#define LOGIN "public.login"

typedef struct {
    struct {
        Evas_Object *user;
        Evas_Object *password;
        Evas_Object *sessions;
        Evas_Object *button;
        Evas_Object *image;
    } login;
    struct {
        Evas_Object *waiting;
    } waiting;

    Evas_Object *background;
    Evas_Object *base;
    Evas_Object *active_sessions;
    Evas_Object *system_operations;
    Evas_Object *settings;

    Ui_Modes current_mode;
} Ui_Elements;

typedef struct {
    System_Operation system_operation;
    const char *name;
    const char *icon;
} System_Operation_Entry;

typedef struct {
    const char *name;
    const char *icon;
    void *data;
} Element;

static Ui_Elements elements;
static Evas_Object *win;

typedef struct {
    const char *part_name;
    Evas_Object **content;
} Element_Content;

Element_Content content[] = {
    {"public.user", &elements.login.user},
    {"public.password", &elements.login.password},
    {"public.login_btn", &elements.login.button},
    {"public.icon", &elements.login.image},
    {"public.waiting", &elements.waiting.waiting},
    {"public.session", &elements.login.sessions},
    {"public.background", &elements.background},
    {"public.system.ops", &elements.system_operations},
    {"public.system.activation", &elements.active_sessions},
    {"public.settings", &elements.settings},
    {NULL, NULL}
};

static void
_clear_cb(void)
{
    if (elements.login.user) elm_object_text_set(elements.login.user, config->default_user);
    if (elements.login.password) elm_object_text_set(elements.login.password, NULL);
}

static void
_config_flush(Config *c) {

}

static void
_theme_unfill(Evas_Object *obj)
{
    Edje_Object *edje;

    edje = elm_layout_edje_get(obj);

    _clear_cb();

    for(int i = 0; content[i].part_name; i++){
        if (!edje_object_part_exists(edje, content[i].part_name)) continue;

        elm_layout_content_unset(obj, content[i].part_name);
        evas_object_hide(*content[i].content);
    }
}

static void
_theme_fill(Evas_Object *obj)
{
    Edje_Object *edje;

    edje = elm_layout_edje_get(obj);

    _clear_cb();

    for(int i = 0; content[i].part_name; i++){
        if (!edje_object_part_exists(edje, content[i].part_name)) continue;

        elm_layout_content_set(obj, content[i].part_name, *content[i].content);
        evas_object_show(*content[i].content);
    }
}

static void
_give_icon(Evas_Object *obj, const char *name)
{
    Evas_Object *ic;

    ic = elm_icon_add(obj);
    elm_icon_standard_set(ic, name);
    evas_object_show(ic);

    elm_object_part_content_set(obj, "icon", ic);
}

static void
settings_cb(void *data EINA_UNUSED, Evas_Object *object EINA_UNUSED, void *event EINA_UNUSED)
{
    config_start(win);
}

static void
_enter_down(void *data EINA_UNUSED, Evas_Object *obj EINA_UNUSED, Evas *e EINA_UNUSED, void *event_info)
{
   Evas_Event_Key_Down *down = event_info;
   void (*func)(void) = data;

   if (!strcmp(down->keyname, "Return") || !strcmp(down->keyname, "Enter") )
     {
        func();
     }
}

static void
_login_clicked(void *data, Evas_Object *obj EINA_UNUSED, void *event_info EINA_UNUSED)
{
   void (*func)(void) = data;
   func();
}

static void
_user_changed(void *data, Evas_Object *obj, void *event_info EINA_UNUSED)
{
   void (*user_changed)(const char *username) = data;

   user_changed(elm_object_text_get(obj));
}

int
ui_init(void (*login_cb)(void), void (*user_changed)(const char *username))
{
    Evas_Object *o;

    elements.current_mode = -1;
    win = elm_win_util_standard_add("Anna", "Anna-greeter");

    evas_object_event_callback_add(win, EVAS_CALLBACK_KEY_DOWN, _enter_down, login_cb);

    if (!win) return EXIT_FAILURE;

    elements.background = o = elm_image_add(win);
    elm_image_file_set(o, config->background.file, NULL);
    evas_object_size_hint_aspect_set(o, EVAS_ASPECT_CONTROL_NONE, 1, 1);
    evas_object_show(o);

    elements.login.user = o = elm_entry_add(win);
    evas_object_smart_callback_add(o, "changed", _user_changed, user_changed);
    evas_object_size_hint_align_set(o, EVAS_HINT_FILL, EVAS_HINT_FILL);
    evas_object_size_hint_weight_set(o, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    elm_object_text_set(o, config->default_user);
    elm_entry_single_line_set(o, EINA_TRUE);
    elm_entry_icon_visible_set(o, EINA_TRUE);
    elm_object_part_text_set(o, "guide", "username");

    if (!config->default_user)
      elm_object_focus_set(o, EINA_TRUE);

    elements.login.password = o = elm_entry_add(win);
    evas_object_size_hint_align_set(o, EVAS_HINT_FILL, EVAS_HINT_FILL);
    evas_object_size_hint_weight_set(o, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    elm_entry_password_set(o, EINA_TRUE);
    elm_entry_single_line_set(o, EINA_TRUE);
    elm_object_part_text_set(o, "guide", "password");

    if (config->default_user)
      elm_object_focus_set(o, EINA_TRUE);

    elements.login.image = o = elm_icon_add(win);
    evas_object_size_hint_align_set(o, EVAS_HINT_FILL, EVAS_HINT_FILL);
    evas_object_size_hint_weight_set(o, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

    elements.login.button = o = elm_button_add(win);
    evas_object_size_hint_align_set(o, EVAS_HINT_FILL, EVAS_HINT_FILL);
    evas_object_size_hint_weight_set(o, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_smart_callback_add(o, "clicked", _login_clicked, login_cb);
    elm_object_text_set(o, "Login");

    elements.login.sessions = o = elm_hoversel_add(win);
    evas_object_size_hint_align_set(o, EVAS_HINT_FILL, EVAS_HINT_FILL);
    evas_object_size_hint_weight_set(o, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    _give_icon(o, "start-here");

    elements.waiting.waiting = o = elm_progressbar_add(win);
    elm_object_style_set(o, "wheel");
    elm_progressbar_pulse_set(o, EINA_TRUE);
    elm_progressbar_pulse(o, EINA_TRUE);

    elements.system_operations = o = elm_hoversel_add(win);
    evas_object_size_hint_align_set(o, EVAS_HINT_FILL, EVAS_HINT_FILL);
    evas_object_size_hint_weight_set(o, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    elm_object_text_set(o, "System Operation");
    _give_icon(o, "applications-system");

    elements.active_sessions = o = elm_hoversel_add(win);
    evas_object_size_hint_align_set(o, EVAS_HINT_FILL, EVAS_HINT_FILL);
    evas_object_size_hint_weight_set(o, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    _give_icon(o, "system-lock-screen");

    elements.settings = o = elm_button_add(win);
    evas_object_size_hint_align_set(o, EVAS_HINT_FILL, EVAS_HINT_FILL);
    evas_object_size_hint_weight_set(o, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    elm_object_text_set(o, "Settings");
    evas_object_smart_callback_add(o, "clicked", settings_cb, NULL);
    _give_icon(o, "emblem-system");

    elements.base = o = elm_layout_add(win);

    if (!elm_layout_file_set(o, THEME_FILE, GROUND))
      return EXIT_FAILURE;

    _theme_fill(elements.base);
    evas_object_size_hint_align_set(o, EVAS_HINT_FILL, EVAS_HINT_FILL);
    evas_object_size_hint_weight_set(o, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_show(o);

    elm_win_resize_object_add(win, elements.base);
    evas_object_show(win);
    evas_object_resize(win, 1920, 1080);

    return EXIT_SUCCESS;
}

void
ui_mode_set(Ui_Modes names)
{
    Evas_Object *layout;

    if (elements.current_mode == names) return;

    layout = elm_layout_content_unset(elements.base, "public.content");

    if (!layout)
      layout = elm_layout_add(win);
    else
      _theme_unfill(layout);

    switch(names){
        case UI_MODES_WAITING:
            elm_layout_file_set(layout, THEME_FILE, WAITING);
        break;
        case UI_MODES_PROMPTING:
            elm_layout_file_set(layout, THEME_FILE , LOGIN);
        break;
    }

    elm_layout_content_set(elements.base, "public.content", layout);
    evas_object_show(layout);
    _theme_fill(layout);

    elements.current_mode = names;
}

static void
_sel(void *data, Evas_Object *obj, void *event_info EINA_UNUSED)
{
    Element *e = data;
    evas_object_data_set(obj, "__selection", e);

    elm_object_text_set(obj, e->name);
}

void
ui_add_session(const char *name, const char *icon, void *data)
{
    Element *e;

    e = calloc(1, sizeof(Element));

    e->data = data;
    e->name = strdup(name);
    e->icon = strdup(icon);

    elm_hoversel_item_add(elements.login.sessions, name, icon, ELM_ICON_STANDARD, _sel, e);

    if (!elm_object_text_get(elements.login.sessions))
      {
         elm_object_text_set(elements.login.sessions, name);
         evas_object_data_set(elements.login.sessions, "__selection", e);
      }
}
void
ui_select_session(const char *name)
{
   const Eina_List *items, *n;
   Elm_Object_Item *item;

   items = elm_hoversel_items_get(elements.login.sessions);

   EINA_LIST_FOREACH(items, n, item)
     {
        Element *e;

        e = elm_object_item_data_get(item);

        if (!name || !strcmp(e->name, name)) {
            evas_object_data_set(elements.login.sessions, "__selection", e);

            elm_object_text_set(elements.login.sessions, e->name);
            return;
        }
     }
}

void
ui_add_active_session(const char *name, const char *icon, void *data, Evas_Smart_Cb cb)
{
    Element *e;

    e = calloc(1, sizeof(Element));

    e->data = data;
    e->name = strdup(name);
    e->icon = strdup(icon);

    elm_hoversel_item_add(elements.active_sessions, name, icon, ELM_ICON_STANDARD, cb, data);

    if (!elm_object_text_get(elements.active_sessions))
      {
         elm_object_text_set(elements.active_sessions, name);
         evas_object_data_set(elements.active_sessions, "__selection", e);
      }
}

void
ui_error_message(const char *title, const char *message)
{
    Evas_Object *popup;

    popup = elm_popup_add(win);

    elm_object_style_set(popup, "transparent");
    elm_object_part_text_set(popup, "title,text", title);
    elm_object_text_set(popup, message);
    elm_popup_timeout_set(popup, 5);
    elm_popup_orient_set(popup, ELM_POPUP_ORIENT_TOP_RIGHT);
    elm_popup_allow_events_set(popup, EINA_TRUE);

    evas_object_show(popup);
}

static void
_selection_cb(void *data, Evas_Object *obj EINA_UNUSED, void *event_info EINA_UNUSED)
{
    System_Operation_Entry *entry = data;

    entry->system_operation();
}

void
ui_add_system_op(System_Operation system_operation, const char *name, const char *icon)
{
    System_Operation_Entry *entry;

    entry = calloc(1, sizeof(System_Operation_Entry));

    entry->system_operation = system_operation;
    entry->icon = strdup(icon);
    entry->name = strdup(name);

    elm_hoversel_item_add(elements.system_operations, entry->name, entry->icon, ELM_ICON_STANDARD, _selection_cb, entry);
}

const char*
ui_get_user(void)
{
    return elm_object_text_get(elements.login.user);
}

const char*
ui_get_password(void)
{
    return elm_object_text_get(elements.login.password);
}

void*
ui_get_session(void)
{
    Element *e;

    e = evas_object_data_get(elements.login.sessions, "__selection");

    return e->data;
}

void
ui_display_icon(const char *something)
{
   Evas_Object *image;

   image = elements.login.image;

   if (!something) goto fallback;

   if (ecore_file_exists(something))
     {
        if (!elm_image_file_set(image, something, NULL))
          goto err;
     }
   else
     {
        if (!elm_icon_standard_set(image, something))
          goto err;
     }
   return;
fallback:
    elm_icon_standard_set(image, "start-here");
    return;
err:
    printf("%s could not be set\n", something);
    if (!elm_icon_standard_set(image, "face-embarrassed"))
      printf("And even the fallback failed\n");
}
