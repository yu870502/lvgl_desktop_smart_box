#pragma once

extern void *dm_handle;

int32_t demo_send_property_post(void *dm_handle, char *params);

int32_t demo_send_property_batch_post(void *dm_handle, char *params);

int32_t demo_send_event_post(void *dm_handle, char *event_id, char *params);

int linkkit_main(void);