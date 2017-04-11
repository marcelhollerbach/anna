#include "main.h"

#define KEY "anna_greeter_0_1"

Config *config;
static Eet_Data_Descriptor *desc;
static Eet_File *file;
static Evas_Object *default_user, *background_image;

static void
_init_read(void)
{
   Eet_Data_Descriptor_Class c;
   Eina_Strbuf *config_file;

   config_file = eina_strbuf_new();
   eina_strbuf_append(config_file, efreet_config_home_get());
   eina_strbuf_append(config_file, "spawny_config.eet");

   file = eet_open(eina_strbuf_string_get(config_file), EET_FILE_MODE_READ_WRITE);
   eina_strbuf_free(config_file);

   EET_EINA_FILE_DATA_DESCRIPTOR_CLASS_SET(&c, Config);
   desc = eet_data_descriptor_file_new(&c);

   EET_DATA_DESCRIPTOR_ADD_BASIC(desc, Config, "background_file", background.file, EET_T_STRING);
   EET_DATA_DESCRIPTOR_ADD_BASIC(desc, Config, "default_user", default_user, EET_T_STRING);
}

static void
_config_read(void)
{
   config = eet_data_read(file, desc, KEY);
}

static void
_config_write(void)
{
   eet_data_write(file, desc, KEY, config, 0);
   eet_sync(file);
}

static void
_close(void *data, Evas_Object *obj, void *event_info)
{
    evas_object_del(data);
}

static void
_apply(void *data, Evas_Object *obj, void *event_info)
{
   const char *tmp_background, *tmp_user;

   tmp_background = elm_fileselector_path_get(background_image);
   tmp_user = elm_object_text_get(default_user);

   if (evas_object_image_extension_can_load_get(tmp_background))
     config->background.file = strdup(tmp_background);

   if (tmp_user && getpwnam(tmp_user))
     config->default_user = strdup(tmp_user);

   _config_write();
}

static void
_background_selection(Evas_Object *box)
{
    Evas_Object *frame, *o;

    o = frame = elm_frame_add(box);
    evas_object_size_hint_align_set(o, EVAS_HINT_FILL, 0.0);
    evas_object_size_hint_weight_set(o, EVAS_HINT_EXPAND, 0.0);
    elm_object_text_set(o, "Background settings");
    evas_object_show(o);

    background_image = o = elm_fileselector_entry_add(frame);
    elm_fileselector_path_set(o, config->background.file);
    elm_object_text_set(o, "Background");
    elm_fileselector_entry_inwin_mode_set(o, EINA_TRUE);
    evas_object_size_hint_align_set(o, EVAS_HINT_FILL, 0.0);
    evas_object_size_hint_weight_set(o, EVAS_HINT_EXPAND, 0.0);
    elm_object_content_set(frame, o);
    evas_object_show(o);

    elm_box_pack_end(box, frame);
}

static void
_standard_user(Evas_Object *box)
{
    Evas_Object *frame, *o;

    o = frame = elm_frame_add(box);
    evas_object_size_hint_align_set(o, EVAS_HINT_FILL, 0.0);
    evas_object_size_hint_weight_set(o, EVAS_HINT_EXPAND, 0.0);
    elm_object_text_set(o, "Default user");
    evas_object_show(o);

    default_user = o = elm_entry_add(frame);
    evas_object_size_hint_align_set(o, EVAS_HINT_FILL, 0.0);
    evas_object_size_hint_weight_set(o, EVAS_HINT_EXPAND, 0.0);
    elm_object_part_text_set(o, "guide", "Default user");
    elm_object_text_set(o, config->default_user);
    elm_object_content_set(frame, o);
    evas_object_show(o);

    elm_box_pack_end(box, frame);
}

void
config_start(Evas_Object *win)
{
    Evas_Object *inwin, *scroller, *table, *o, *box;

    inwin = o = elm_win_inwin_add(win);

    table = o = elm_table_add(inwin);
    evas_object_size_hint_align_set(o, EVAS_HINT_FILL, EVAS_HINT_FILL);
    evas_object_size_hint_weight_set(o, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_show(o);

    scroller = o = elm_scroller_add(inwin);
    evas_object_size_hint_align_set(o, EVAS_HINT_FILL, EVAS_HINT_FILL);
    evas_object_size_hint_weight_set(o, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    elm_table_pack(table, o, 0, 0, 2, 1);
    evas_object_show(scroller);

    box = o = elm_box_add(scroller);
    evas_object_size_hint_align_set(o, EVAS_HINT_FILL, EVAS_HINT_FILL);
    evas_object_size_hint_weight_set(o, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    elm_object_content_set(scroller, o);
    evas_object_show(o);

    _background_selection(box);
    _standard_user(box);

    o = elm_button_add(inwin);
    evas_object_size_hint_align_set(o, EVAS_HINT_FILL, 0.0);
    evas_object_size_hint_weight_set(o, EVAS_HINT_EXPAND, 0.0);
    evas_object_smart_callback_add(o, "clicked", _close, inwin);
    evas_object_smart_callback_add(o, "clicked", _apply, NULL);
    elm_object_text_set(o, "Save & Close");
    elm_table_pack(table, o, 0, 1, 1, 1);
    evas_object_show(o);

    o = elm_button_add(inwin);
    evas_object_size_hint_align_set(o, EVAS_HINT_FILL, 0.0);
    evas_object_size_hint_weight_set(o, EVAS_HINT_EXPAND, 0.0);
    evas_object_smart_callback_add(o, "clicked", _close, inwin);
    elm_object_text_set(o, "Close");
    elm_table_pack(table, o, 1, 1, 1, 1);
    evas_object_show(o);

    elm_win_inwin_content_set(inwin, table);
    evas_object_show(inwin);
}

int
config_init(void)
{
    _init_read();

    _config_read();

    if (!config)
      {
         config = calloc(1, sizeof(Config));

         config->background.file = THEME_INSTALL_DIR"/entrance_background.jpg";
         config->default_user = "";
      }

    return EXIT_SUCCESS;
}

void
config_shutdown(void)
{
   eet_close(file);
}
