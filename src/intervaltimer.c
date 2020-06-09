#include "intervaltimer.h"

typedef struct appdata {
	Evas_Object *win;
	Evas_Object *conform;
	Evas_Object *bg;
	Evas_Object *label;
} appdata_s;

static appdata_s *ad;

static void
win_delete_request_cb(void *data, Evas_Object *obj, void *event_info)
{
	ui_app_exit();
}

static void
win_back_cb(void *data, Evas_Object *obj, void *event_info)
{
	appdata_s *ad = data;
	/* Let window go to hide state. */
	elm_win_lower(ad->win);
}

static void
create_base_gui(appdata_s *ad)
{
	/* Window */
	/* Create and initialize elm_win.
	   elm_win is mandatory to manipulate window. */
	ad->win = elm_win_util_standard_add(PACKAGE, PACKAGE);
	elm_win_autodel_set(ad->win, EINA_TRUE);

	if (elm_win_wm_rotation_supported_get(ad->win)) {
		int rots[4] = { 0, 90, 180, 270 };
		elm_win_wm_rotation_available_rotations_set(ad->win, (const int *)(&rots), 4);
	}

	evas_object_smart_callback_add(ad->win, "delete,request", win_delete_request_cb, NULL);
	eext_object_event_callback_add(ad->win, EEXT_CALLBACK_BACK, win_back_cb, ad);

	/* Conformant */
	/* Create and initialize elm_conformant.
	   elm_conformant is mandatory for base gui to have proper size
	   when indicator or virtual keypad is visible. */
	ad->conform = elm_conformant_add(ad->win);
	elm_win_indicator_mode_set(ad->win, ELM_WIN_INDICATOR_SHOW);
	elm_win_indicator_opacity_set(ad->win, ELM_WIN_INDICATOR_OPAQUE);
	evas_object_size_hint_weight_set(ad->conform, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_win_resize_object_add(ad->win, ad->conform);
	evas_object_show(ad->conform);

	ad->bg = elm_bg_add(ad->conform);
	elm_bg_color_set(ad->bg, 0x00, 0x00, 0x00);
	elm_object_content_set(ad->conform, ad->bg);

	/* Label */
	/* Create an actual view of the base gui.
	   Modify this part to change the view. */

	ad->label = elm_label_add(ad->conform);
	elm_object_text_set(ad->label, "<font align=center color=#FFFFFF>Waiting...</font>");
	Evas_Coord w = 0;
	Evas_Coord h = 0;
	evas_object_size_hint_max_get(ad->label, &w, &h);
	evas_object_size_hint_max_set(ad->label, w, 80);
	elm_object_content_set(ad->bg, ad->label);
	evas_object_show(ad->label);

	/* Show window after base gui is set up */
    evas_object_show(ad->win);
}

static bool
app_create(void *data)
{
	/* Hook to take necessary actions before main event loop starts
		Initialize UI resources and application's data
		If this function returns true, the main loop of application starts
		If this function returns false, the application is terminated */
	ad = data;

	create_base_gui(ad);
	initialize_sap();

	return true;
}

static void
app_control(app_control_h app_control, void *data)
{
	/* Handle the launch request. */
}

static void
app_pause(void *data)
{
	/* Take necessary actions when application becomes invisible. */
}

static void
app_resume(void *data)
{
	/* Take necessary actions when application becomes visible. */
}

static void
app_terminate(void *data)
{
	/* Release all resources. */
}

static void
ui_app_lang_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LANGUAGE_CHANGED*/
	char *locale = NULL;
	system_settings_get_value_string(SYSTEM_SETTINGS_KEY_LOCALE_LANGUAGE, &locale);
	elm_language_set(locale);
	free(locale);
	return;
}

static void
ui_app_orient_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_DEVICE_ORIENTATION_CHANGED*/
	return;
}

static void
ui_app_region_changed(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_REGION_FORMAT_CHANGED*/
}

static void
ui_app_low_battery(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LOW_BATTERY*/
}

static void
ui_app_low_memory(app_event_info_h event_info, void *user_data)
{
	/*APP_EVENT_LOW_MEMORY*/
}

void update_ui(char *msg)
{

	dlog_print(DLOG_INFO, TAG, "Updating UI with data %s", msg);

	if(strncmp(msg, "Green", 5) == 0) {
		evas_object_hide(ad->label);
		elm_bg_color_set(ad->bg, 0x60, 0xA5, 0x61);
	} else if(strncmp(msg, "Black", 5) == 0) {
		evas_object_hide(ad->label);
		elm_bg_color_set(ad->bg, 0x00, 0x00, 0x00);
	} else if(strncmp(msg, "Fin", 3) == 0) {
		elm_object_text_set(ad->label, "<font align=center color=#FFFFFF>Finished</font>");
		Evas_Coord w = 0;
		Evas_Coord h = 0;
		evas_object_size_hint_max_get(ad->label, &w, &h);
		evas_object_size_hint_max_set(ad->label, w, 80);
		evas_object_show(ad->label);
	} else if(strncmp(msg, "Ready", 5) == 0) {
		elm_object_text_set(ad->label, "<font align=center color=#FFFFFF>Ready</font>");
		Evas_Coord w = 0;
		Evas_Coord h = 0;
		evas_object_size_hint_max_get(ad->label, &w, &h);
		evas_object_size_hint_max_set(ad->label, w, 80);
		evas_object_show(ad->label);
		elm_bg_color_set(ad->bg, 0x00, 0x00, 0x00);
	} else if(strncmp(msg, "Pause", 5) == 0) {
		elm_object_text_set(ad->label, "<font align=center color=#FFFFFF>Paused</font>");
		Evas_Coord w = 0;
		Evas_Coord h = 0;
		evas_object_size_hint_max_get(ad->label, &w, &h);
		evas_object_size_hint_max_set(ad->label, w, 80);
		evas_object_show(ad->label);
	}  else if(strncmp(msg, "Vibe", 4) == 0) {
		haptic_device_h haptic_handle;
		haptic_effect_h effect_handle;

		int result = device_haptic_open(0, &haptic_handle);
		if(result == DEVICE_ERROR_NONE) {
			dlog_print(DLOG_INFO, TAG, "Open Success %d",result);
		}
		result = device_haptic_vibrate(haptic_handle, 1000, 60, &effect_handle);
		if(result == DEVICE_ERROR_NONE) {
			dlog_print(DLOG_INFO, TAG, "Vibrating Success %d",result);
		}
	} else if(strncmp(msg, "Wait", 4) == 0) {
		elm_object_text_set(ad->label, "<font align=center color=#FFFFFF>Waiting...</font>");
		Evas_Coord w = 0;
		Evas_Coord h = 0;
		evas_object_size_hint_max_get(ad->label, &w, &h);
		evas_object_size_hint_max_set(ad->label, w, 80);
	}
}


int
main(int argc, char *argv[])
{
	//dlog_print(DLOG_INFO, TAG, "Starting app.");

	appdata_s ad = {0,};
	int ret = 0;

	ui_app_lifecycle_callback_s event_callback = {0,};
	app_event_handler_h handlers[5] = {NULL, };

	event_callback.create = app_create;
	event_callback.terminate = app_terminate;
	event_callback.pause = app_pause;
	event_callback.resume = app_resume;
	event_callback.app_control = app_control;

	ui_app_add_event_handler(&handlers[APP_EVENT_LOW_BATTERY], APP_EVENT_LOW_BATTERY, ui_app_low_battery, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_LOW_MEMORY], APP_EVENT_LOW_MEMORY, ui_app_low_memory, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_DEVICE_ORIENTATION_CHANGED], APP_EVENT_DEVICE_ORIENTATION_CHANGED, ui_app_orient_changed, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_LANGUAGE_CHANGED], APP_EVENT_LANGUAGE_CHANGED, ui_app_lang_changed, &ad);
	ui_app_add_event_handler(&handlers[APP_EVENT_REGION_FORMAT_CHANGED], APP_EVENT_REGION_FORMAT_CHANGED, ui_app_region_changed, &ad);

	ret = ui_app_main(argc, argv, &event_callback, &ad);
	if (ret != APP_ERROR_NONE) {
		dlog_print(DLOG_ERROR, LOG_TAG, "app_main() is failed. err = %d", ret);
	}

	return ret;
}
