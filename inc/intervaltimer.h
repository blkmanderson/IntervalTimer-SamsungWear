#ifndef __intervaltimer_H__
#define __intervaltimer_H__

#include <app.h>
#include <glib.h>
#include <Elementary.h>
#include <system_settings.h>
#include <efl_extension.h>
#include <dlog.h>
#include <device/haptic.h>

#define TAG "HelloMessageConsumer"

void     initialize_sap();
gboolean find_peers();
void     mex_send(char *message, int length, gboolean is_secured);
void     update_ui(char *data);

#if !defined(PACKAGE)
#define PACKAGE "org.example.intervaltimer"
#endif

#endif /* __intervaltimer_H__ */
