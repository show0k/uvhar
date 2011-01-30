#ifndef _IHM_STAGES_O_GTK_H
#define _IHM_STAGES_O_GTK_H

#include <config.h>
#include <VP_Api/vp_api_thread_helper.h>


PROTO_THREAD_ROUTINE(video_stage, data);

typedef  struct _vp_stages_gtk_config_ {
  int width;
  int height;
  int rowstride;
}  vp_stages_gtk_config_t;

void update_vision(void);


#endif // _IHM_STAGES_O_GTK_H
