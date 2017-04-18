#include "main.h"

static Sp_Client_Context *ctx;
static Ecore_Fd_Handler *fd;
static Numbered_Array username;
static Numbered_Array templates;
static Numbered_Array sessions;

static void _data_cb(void);
static void _login_cb(int success, char *msg);

static Sp_Client_Interface interface = {
    _data_cb,
    _login_cb
};

static void
_ui_login_cb(void)
{
    const char *usr, *pw;
    Template *template;

    usr = ui_get_user();
    pw = ui_get_password();
    template = ui_get_session();

    if (!sp_client_login(ctx, usr, pw, template->id))
      ui_error_message("Login failed!", "Thats a deeper issue, grab the output of the greeter and report it upstream.\n");

    ui_mode_set(UI_MODES_WAITING);
}

static void
_ui_username_changed(const char *name)
{
   for (int i = 0; i < username.length ; ++i) {
      User u = USER_ARRAY(&username, i);

      if (!!strcmp(name, u.name)) continue;

      Template t = TEMPLATE_ARRAY(&templates, u.prefered_session);
      ui_select_session(t.name);

      ui_display_icon(u.icon);

      return;
   }
   ui_display_icon(NULL);
   ui_select_session(NULL);
}

static void
_data_cb(void) {
    sp_client_data_get(ctx, &sessions, &templates, &username);

    /* fill the sessions to the ui */
    for(int i = 0; i < templates.length; i++) {
        Template *t = &TEMPLATE_ARRAY(&templates, i);
        ui_add_session(t->name, t->icon, t);
    }

    /* fill the active session to the ui */
    for (int i = 0; i < sessions.length; i++) {
       Session *s = &SESSION_ARRAY(&sessions, i);
       ui_add_active_session(s->user, s->icon, s);
    }

    /* set to prompting mode */
    ui_mode_set(UI_MODES_PROMPTING);
    _ui_username_changed(ui_get_user());
}

static void
_login_cb(int success, char *msg) {
    if (!success) {
      ui_mode_set(UI_MODES_PROMPTING);
      ui_error_message("Login error", msg);
    } else {
      ui_mode_set(UI_MODES_WAITING); /* lets wait until the session gets acitvated */
    }
}

static Eina_Bool
_read_com(void *data, Ecore_Fd_Handler *handler)
{
   Sp_Client_Read_Result res;

   res = sp_client_read(ctx, &interface);

   if (ecore_main_fd_handler_active_get(handler, ECORE_FD_ERROR))
     {
        //there is a error in the main fd, emergancy exit!!!
        elm_exit();
        return ECORE_CALLBACK_CANCEL;
     }

   switch(res){
    case SP_CLIENT_READ_RESULT_FAILURE:
    case SP_CLIENT_READ_RESULT_EXIT:

        elm_exit();

        sp_client_free(ctx);
        ctx = NULL;

        ecore_main_fd_handler_del(fd);
        fd = NULL;

        return ECORE_CALLBACK_CANCEL;
    default:
    break;
   }

   return ECORE_CALLBACK_RENEW;
}

static int
_com_init(int argc, char *argv[])
{
   int server_fd;

   ctx = sp_client_init(argc, argv, SP_CLIENT_LOGIN_PURPOSE_GREETER_JOB);
   if (!ctx) return EXIT_FAILURE;

   server_fd = sp_client_fd_get(ctx);

   fd = ecore_main_fd_handler_add(server_fd, ECORE_FD_READ | ECORE_FD_ERROR, _read_com,  NULL, NULL, NULL);

   return EXIT_SUCCESS;

}

static void
_com_shutdown(void)
{
   if (ctx)
     {
        sp_client_free(ctx);
        ctx = NULL;
     }

   if (fd)
     {
        ecore_main_fd_handler_del(fd);
        fd = NULL;
     }
}

static void
_sys_op_shutdown(void)
{
   system("poweroff");
}

static void
_sys_op_reboot(void)
{
  system("reboot");
}

static int
_sys_ops_init(void)
{
   ui_add_system_op(_sys_op_shutdown, "Shutdown", "shutdown");
   ui_add_system_op(_sys_op_reboot, "Reboot", "reboot");

   return EXIT_SUCCESS;
}

#define STARTUP(call) if (call == EXIT_FAILURE) return EXIT_FAILURE;

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   STARTUP(_com_init(argc, argv))
   STARTUP(config_init())
   STARTUP(ui_init(_ui_login_cb, _ui_username_changed));
   STARTUP(_sys_ops_init())

   ui_mode_set(UI_MODES_PROMPTING);

   elm_run();

   _com_shutdown();
   config_shutdown();

   return 0;
}

ELM_MAIN()
