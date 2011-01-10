/*
 * @video_stage.c
 * @author marc-olivier.dzeukou@parrot.com
 * @date 2007/07/27
 *
 * ihm vision thread implementation
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <termios.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

#include <gtk/gtk.h>
#include <gtk/gtkcontainer.h>
#include <sys/time.h>
#include <time.h>

#include <VP_Api/vp_api.h>
#include <VP_Api/vp_api_error.h>
#include <VP_Api/vp_api_stage.h>
#include <VP_Api/vp_api_picture.h>
#include <VP_Stages/vp_stages_io_file.h>
#include <VP_Stages/vp_stages_i_camif.h>

#include <config.h>
#include <VP_Os/vp_os_print.h>
#include <VP_Os/vp_os_malloc.h>
#include <VP_Os/vp_os_delay.h>
#include <VP_Stages/vp_stages_yuv2rgb.h>
#include <VP_Stages/vp_stages_buffer_to_picture.h>
#include <VLIB/Stages/vlib_stage_decode.h>

#include <ardrone_tool/ardrone_tool.h>
#include <ardrone_tool/Com/config_com.h>

#ifndef RECORD_VIDEO
#define RECORD_VIDEO
#endif
#ifdef RECORD_VIDEO
#    include <ardrone_tool/Video/video_stage_recorder.h>
#endif

#include <ardrone_tool/Video/video_com_stage.h>

#include "Video/video_stage.h"

#define NB_STAGES 10

#define FPS2TIME  0.10

#define CAMIF_V_CAMERA_USED CAMIF_CAMERA_CRESYN
#define CAMIF_H_CAMERA_USED CAMIF_CAMERA_OVTRULY


PIPELINE_HANDLE pipeline_handle;


static GtkImage *image = NULL;
static GdkPixbuf *pixbuf = NULL;

static int32_t   pixbuf_width      = 0;
static int32_t   pixbuf_height     = 0;
static int32_t   pixbuf_rowstride  = 0;
static uint8_t*  pixbuf_data       = NULL;


static vp_os_mutex_t  video_update_lock = PTHREAD_MUTEX_INITIALIZER;

//C_RESULT output_gtk_stage_open( void *cfg, vp_api_io_data_t *in, vp_api_io_data_t *out)
C_RESULT output_gtk_stage_open( vp_stages_gtk_config_t *cfg, vp_api_io_data_t *in, vp_api_io_data_t *out)
{
  pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB,
                          FALSE             ,
                          8                 ,
                          cfg->width        ,
                          cfg->height       );






  return (SUCCESS);
}


//C_RESULT output_gtk_stage_transform( void *cfg, vp_api_io_data_t *in, vp_api_io_data_t *out)
C_RESULT output_gtk_stage_transform( vp_stages_gtk_config_t *cfg, vp_api_io_data_t *in, vp_api_io_data_t *out)
{
  vp_os_mutex_lock(&video_update_lock);
 
  /* Get a reference to the last decoded picture */
  pixbuf_data = (uint8_t*)in->buffers[0];

  pixbuf_width     = cfg->width;
  pixbuf_height    = cfg->height;
  pixbuf_rowstride = cfg->rowstride;



  vp_os_mutex_unlock(&video_update_lock);

  return (SUCCESS);
}

//C_RESULT output_gtk_stage_close( void *cfg, vp_api_io_data_t *in, vp_api_io_data_t *out)
C_RESULT output_gtk_stage_close( vp_stages_gtk_config_t *cfg, vp_api_io_data_t *in, vp_api_io_data_t *out)
{
  return (SUCCESS);
}


const vp_api_stage_funcs_t vp_stages_output_gtk_funcs =
{
  NULL,
  (vp_api_stage_open_t)output_gtk_stage_open,
  (vp_api_stage_transform_t)output_gtk_stage_transform,
  (vp_api_stage_close_t)output_gtk_stage_close
};

DEFINE_THREAD_ROUTINE(video_stage, data)
{
  C_RESULT res;

     // SEEMS TO BE LISTING 5.7, the start 
  vp_api_io_pipeline_t    pipeline;
  vp_api_io_data_t        out;
  vp_api_io_stage_t       stages[NB_STAGES];

  vp_api_picture_t picture;

  video_com_config_t              icc;
  vlib_stage_decoding_config_t    vec;
  vp_stages_yuv2rgb_config_t      yuv2rgbconf;
#ifdef RECORD_VIDEO
  video_stage_recorder_config_t   vrc;
#endif


  /// Picture configuration
  picture.format        = PIX_FMT_YUV420P;

  picture.width         = QVGA_WIDTH;
  picture.height        = QVGA_HEIGHT;
  picture.framerate     = 30;

  picture.y_buf   = vp_os_malloc( QVGA_WIDTH * QVGA_HEIGHT     );
  picture.cr_buf  = vp_os_malloc( QVGA_WIDTH * QVGA_HEIGHT / 4 );
  picture.cb_buf  = vp_os_malloc( QVGA_WIDTH * QVGA_HEIGHT / 4 );

  picture.y_line_size   = QVGA_WIDTH;
  picture.cb_line_size  = QVGA_WIDTH / 2;
  picture.cr_line_size  = QVGA_WIDTH / 2;

  vp_os_memset(&icc,          0, sizeof( icc ));
  vp_os_memset(&vec,          0, sizeof( vec ));
  vp_os_memset(&yuv2rgbconf,  0, sizeof( yuv2rgbconf ));

  icc.com                 = COM_VIDEO();
  icc.buffer_size         = 100000;
  icc.protocol            = VP_COM_UDP;
  COM_CONFIG_SOCKET_VIDEO(&icc.socket, VP_COM_CLIENT, VIDEO_PORT, wifi_ardrone_ip);

  vec.width               = QVGA_WIDTH;
  vec.height              = QVGA_HEIGHT;
  vec.picture             = &picture;
  vec.block_mode_enable   = TRUE;
  vec.luma_only           = FALSE;

  yuv2rgbconf.rgb_format = VP_STAGES_RGB_FORMAT_RGB24;
#ifdef RECORD_VIDEO
  vrc.fp = NULL;
#endif

  pipeline.nb_stages = 0;

  stages[pipeline.nb_stages].type    = VP_API_INPUT_SOCKET;
  stages[pipeline.nb_stages].cfg     = (void *)&icc;
  stages[pipeline.nb_stages].funcs   = video_com_funcs;

  pipeline.nb_stages++;

#ifdef RECORD_VIDEO
  stages[pipeline.nb_stages].type    = VP_API_FILTER_DECODER;
  stages[pipeline.nb_stages].cfg     = (void*)&vrc;
  stages[pipeline.nb_stages].funcs   = video_recorder_funcs;

  pipeline.nb_stages++;
#endif // RECORD_VIDEO
  stages[pipeline.nb_stages].type    = VP_API_FILTER_DECODER;
  stages[pipeline.nb_stages].cfg     = (void*)&vec;
  stages[pipeline.nb_stages].funcs   = vlib_decoding_funcs;

  pipeline.nb_stages++;

  stages[pipeline.nb_stages].type    = VP_API_FILTER_YUV2RGB;
  stages[pipeline.nb_stages].cfg     = (void*)&yuv2rgbconf;
  stages[pipeline.nb_stages].funcs   = vp_stages_yuv2rgb_funcs;

  pipeline.nb_stages++;

  stages[pipeline.nb_stages].type    = VP_API_OUTPUT_SDL;
  stages[pipeline.nb_stages].cfg     = NULL;
  stages[pipeline.nb_stages].funcs   = vp_stages_output_gtk_funcs;

  pipeline.nb_stages++;

  pipeline.stages = &stages[0];
 
  /* Processing of a pipeline */
  if( !ardrone_tool_exit() )
  {
    PRINT("\n   Video stage thread initialisation\n\n");

    res = vp_api_open(&pipeline, &pipeline_handle);

    if( SUCCEED(res) )
    {
      int loop = SUCCESS;
      out.status = VP_API_STATUS_PROCESSING;

     // LISTING 5.8
      while( !ardrone_tool_exit() && (loop == SUCCESS) )
      {
          if( SUCCEED(vp_api_run(&pipeline, &out)) ) {
            if( (out.status == VP_API_STATUS_PROCESSING || out.status == VP_API_STATUS_STILL_RUNNING) ) {
              loop = SUCCESS;
            }
          }
          else loop = -1; // Finish this thread
      }

      vp_api_close(&pipeline, &pipeline_handle);
    }
  }

  PRINT("   Video stage thread ended\n\n");

  return (THREAD_RET)0;
}


void update_vision( void )
{

//  if( image_vision_window_view == WINDOW_VISIBLE) {

    vp_os_mutex_lock(&video_update_lock);

    // Vision state refresh
    //gtk_label_set_label(label_vision_values, label_vision_state_value);

    if( pixbuf_data != NULL )
    {
         pixbuf = gdk_pixbuf_new_from_data(pixbuf_data,
                                        GDK_COLORSPACE_RGB,
                                        FALSE,
                                        8,
                                        pixbuf_width,
                                        pixbuf_height,
                                        pixbuf_rowstride,
                                        NULL,
                                        NULL);

         if( image == NULL )
         {
            image  = (GtkImage*) gtk_image_new_from_pixbuf( pixbuf );
            //gtk_container_add( GTK_CONTAINER( ihm_Image_VBox ), (GtkWidget*)image );
         }
         else
         {
             gtk_image_set_from_pixbuf(image, pixbuf);
         }
         g_object_unref(pixbuf);
   // }

    vp_os_mutex_unlock(&video_update_lock);

    //gtk_widget_show_all( ihm_ImageWin );
  }
  else
  {
    if( image != NULL )
    {
         printf("\tImage is not null in update_vision\n");
         //if( GTK_IS_CONTAINER( ihm_Image_VBox ) && GTK_IS_WIDGET(image) )
         //    gtk_container_remove( GTK_CONTAINER( ihm_Image_VBox ), (GtkWidget*)image );
    }
    image = NULL;
  }
//}
}


