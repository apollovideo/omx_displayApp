// ***************************************************************************
//  ... Copyright (c) 2018 LTG
//  ... All rights reserved.
//  ... Any use without the prior written consent of LTG
//      is strictly prohibited.
// ***************************************************************************
// ***************************************************************************
//
//  Filename:       video.c
//
//  Description:    A simple proof of concept app using idis SDK and OpenMax IL
//
//  Revision History:
//  Date        Name            Ver     Remarks
//  10-30-2018  Steve Mellon    0       Original
//
//  Notes:  Uses a very simple buffering technique.
//
//  usage: ./vid <layout>
//     where layout can be:   0 - fullscreen       (1 camera)
//                            1 - vertical split   (2 cameras)
//                            2 - horizontal split (2 cameras)
//                            3 - 2x2 grid         (4 cameras)
//                            4 - 2x3 grid         (6 cameras)
//                            5 - 3x3 grid         (9 cameras)
//                            6 - 3x4 grid         (12 cameras)
//                            7 - 3x5 grid         (15 cameras)
//                            8 - 4x4 grid         (16 cameras)
//
//   issue SIGUSR1 to transition to next camera
//         SIGUSR2 to dump buffer stats
//
// ***************************************************************************

/*
Copyright (c) 2012, Broadcom Europe Ltd
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the copyright holder nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

// Video deocode demo using OpenMAX IL though the ilcient helper library

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h> 
#include <signal.h> 

extern "C" {
#include "bcm_host.h"
#include "ilclient.h"
#include <IL/OMX_Core.h>
#include <IL/OMX_Component.h>
#include <IL/OMX_Video.h>
#include <IL/OMX_Broadcom.h>
}


#include "IDIS_WatchSDK.h"


#define OMX_INIT_STRUCTURE(a) \
    memset(&(a), 0, sizeof(a)); \
    (a).nSize = sizeof(a); \
    (a).nVersion.nVersion = OMX_VERSION; \
    (a).nVersion.s.nVersionMajor = OMX_VERSION_MAJOR; \
    (a).nVersion.s.nVersionMinor = OMX_VERSION_MINOR; \
    (a).nVersion.s.nRevision = OMX_VERSION_REVISION; \
(a).nVersion.s.nStep = OMX_VERSION_STEP


#define NUM_CAMS                16
#define CAMERA_BUFFER_SIZE      20000           // bytes

typedef enum
{
    LAYOUT_TYPE_FULLSCREEN,
    LAYOUT_TYPE_VERTICAL_SPLIT,
    LAYOUT_TYPE_HORIZONTAL_SPLIT,
    LAYOUT_TYPE_2X2_MATRIX,
    LAYOUT_TYPE_2X3_MATRIX,
    LAYOUT_TYPE_3X3_MATRIX,
    LAYOUT_TYPE_3X4_MATRIX,
    LAYOUT_TYPE_3X5_MATRIX,
    LAYOUT_TYPE_4X4_MATRIX,
    
    LAYOUT_TYPE_MAX    
} LAYOUT_TYPE;


typedef struct buffData 
{
    uint8_t     *ptr;
    int         size;
    int         loc;
    int         end;
    int         maxSize;
    sem_t       mutex;
    int         reader;
    int         skippedPFrames;
    int         skippedIFrames;
} BUFF_DATA;


typedef struct winRect
{
    int         x;
    int         y;
    int         w;
    int         h;
} WIN_RECT;

typedef struct layout
{
    int         numHoriz;
    int         numVert;
    WIN_RECT    *rects;
} LAYOUT;


static LAYOUT windowLayout [LAYOUT_TYPE_MAX] =
// calculate these runtime based on the screen dimensions 
{
    {1,1, NULL},  // LAYOUT_TYPE_FULLSCREEN,       1 streams
    {1,2, NULL},  // LAYOUT_TYPE_VERTICAL_SPLIT,   2 streams
    {2,1, NULL},  // LAYOUT_TYPE_HORIZONTAL_SPLIT, 2 streams
    {2,2, NULL},  // LAYOUT_TYPE_2X2_MATRIX,       4 streams
    {2,3, NULL},  // LAYOUT_TYPE_2X3_MATRIX        6 streams
    {3,3, NULL},  // LAYOUT_TYPE_3X3_MATRIX,       9 streams
    {3,4, NULL},  // LAYOUT_TYPE_3X4_MATRIX,       12 streams
    {3,5, NULL},  // LAYOUT_TYPE_3X5_MATRIX,       15 streams
    {4,4, NULL}   // LAYOUT_TYPE_4X4_MATRIX,       16 streams
};



typedef struct winData
{
    int         cameraNumber;       // active camera stream in window
    WIN_RECT    winDim;             // window dimensions
    pthread_t   vidThread; 
    int         id;
} WIN_DATA;




// define a work area data structure
typedef struct work
{
    IDISHWATCH                  watch;
    int                         channel;
    BUFF_DATA                   bd [NUM_CAMS];
    int                         numWindows;
    WIN_DATA                    windows[NUM_CAMS];
    int                         stopVid;
    int                         resize;
    int                         layout;
} WORK;


// work area 
static WORK             work              = {0};
pthread_mutex_t         reconfigure_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t          reconfigure_cond  = PTHREAD_COND_INITIALIZER;


/*  -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//  Method:  readPacket ()
//      called by the omx il to get data from the camera stream
//
//  Author: S.S. Mellon
//  Date: Oct 30, 2018
//  -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */

static int readPacket(void *opaque, uint8_t *buf, int buf_size)
{
    BUFF_DATA       *buff = (BUFF_DATA*)opaque;

    buff->reader = 1;
    
    // wait if the buffer is empty
    if (buff->end == 0)
        sem_wait(&buff->mutex); 

    if (buf_size > buff->end)
        buf_size = buff->end;

    memcpy (buf, &buff->ptr[buff->loc], buf_size);
    buff->loc = buf_size;
    
    // reset the buffer when we finish reading
    // could be implemented in a ring buffer
    if (buff->loc == buff->end)
    {
        buff->loc = 0;
        buff->end = 0;
    }

    return buf_size;
}


/*  -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//  Method:  frameLoaded ()
//      called by the idis library when new data is available from a camera.
//  
//  write as much frame data into the buffer as we can
//  if we fill the buffer, it is probably because the reader stopped reading.
//  if it is still full when we have more data, we will reset the buffer with the
//  next i-frame
//
//
//  keep some stats on thre number of frames lost, max buffer used.  they can be
//  viewed by signaling the program with sigusr2
//
//  Author: S.S. Mellon
//  Date: Oct 30, 2018
//  -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */

void IDISCALLBACK frameLoaded (IDISHWATCH hWatch, IDISWPARAM wParam, IDISLPARAM lParam)
{
    PARAMWS_FRAMEINFO   *frame  = (PARAMWS_FRAMEINFO*)lParam;
    
    if (frame == NULL)
    {
        printf ("%s: NULL frame ptr!\n", __func__);
        return;
    }
    
    int                 val     = 0;
    int                 wr_size = frame->_szBuffer;    
    
    if (frame->_hostcamera < 0 || frame->_hostcamera >= NUM_CAMS)
    {
        printf ("%s: invalid camera number: %d!\n", __func__, frame->_hostcamera);
        return;
    }
    
    BUFF_DATA           *buff   = &work.bd[frame->_hostcamera];

    // if the buffer is full, count some stats and reset on the next i-frame
    if (buff->end == buff->size)
    {
        if (frame->_frameType == IDIS_FRAMETYPE::I_FRAME)
        {
            if (buff->reader)
            {
                buff->skippedIFrames++;
                printf ("%s: skipped i-frame for camera %d: %d\n", __func__, 
                        frame->_hostcamera, buff->skippedIFrames);
            }
            
            // reset the buffer
            buff->end = 0;
        }
        else if (frame->_frameType == IDIS_FRAMETYPE::P_FRAME)
        {
            if (buff->reader)
            {
                buff->skippedPFrames++;
//                printf ("%s: skipped p-frame for camera %d: %d\n", __func__, 
//                        frame->_hostcamera, buff->skippedPFrames);
            }
        }
    }
         
    // write as much data into the buffer as we can
    if ((buff->end + (int)frame->_szBuffer) >= buff->size)
        wr_size = buff->size - buff->end;
        
    memcpy(&buff->ptr[buff->end], frame->_lpBuffer, wr_size);
    buff->end += wr_size;
    
    // update the max used value for the buffer, if necessary
    if (buff->end > buff->maxSize)
    {
        buff->maxSize = buff->end;
//        printf ("max buff size exceeded: %d\n", buff->maxSize); 
    }   

    // post the semaphore if it is not already posted.  this will wake the video thread
    sem_getvalue (&buff->mutex, &val);
    if (val == 0)
        sem_post(&buff->mutex); 

}


/*  -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//  Method:  watchConnected ()
//      called by idis sdk when connected
//
//      As a worst case experiment, all camera streams are enabled
//
//  Author: S.S. Mellon
//  Date: Oct 30, 2018
//  -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */

void IDISCALLBACK watchConnected (IDISHWATCH hWatch, IDISWPARAM wParam, IDISLPARAM lParam)
{
    BYTE     cameraList[NUM_CAMS] = {0};
       
    // enable all the camera streams
    for (int num = 0; num < NUM_CAMS; num++)
        cameraList[num] = 1;

    if (watch_setCameraList (hWatch, wParam, cameraList, sizeof(cameraList)) == false)
    {
        printf("%s: watch_setCameraList failed\n", __func__);
    }
    else
    {
        printf( "%s: watch_setCameraList success\n", __func__);
    }    
}


/*  -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//  Method:  initWatcher ()
//      Startup the idis watcher SDK and connect.  Setup the connected and
//      frameloaded callbacks 
//
//  Author: S.S. Mellon
//  Date: Oct 30, 2018
//  -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */

int initWatcher (WORK *workPtr)
{
    workPtr->watch = watch_initialize ();

    if (workPtr->watch != IDISHNULL)
    {
        
        watch_registerCallback (workPtr->watch, CALLBACK_WATCH::FRAMELOADED, frameLoaded);
        watch_registerCallback (workPtr->watch, CALLBACK_WATCH::ONCONNECTED, watchConnected);
        
        watch_startup (workPtr->watch, NUM_CAMS);
        
        workPtr->channel = watch_connect(workPtr->watch, _T("jon's dvr"), _T("192.168.125.25"), _T("admin"), _T("AVT7100"), 8200, 0, NULL);
        
        if (workPtr->channel >= 0)
            printf ("%s: watch_connect success: channel: %d\n", __func__, workPtr->channel);
        else
            printf ("%s: watch_connect failed\n", __func__);
    }
    else
    {
        printf ("%s: watch_initiailize failed\n", __func__);
        return -1;
    }   
   
    return 0;
}


/*  -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//  Method:  playVideo ()
//      Lifted from github/raspberrypi/userland/hello_pi/hello_video
//
//  Author: Broadcomm, Europe
//  Date: 2012
//      revision:
//      S.S. Mellon     10/31/2018
//      tweaked to make an example program uses idis streams, change window size, reformatted
//  -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */

static void *playVideo (void *userData)
{
    OMX_VIDEO_PARAM_PORTFORMATTYPE   format;
    OMX_TIME_CONFIG_CLOCKSTATETYPE   cstate;
    COMPONENT_T                      *video_decode    = NULL, 
                                     *video_scheduler = NULL, 
                                     *video_render    = NULL, 
                                     *clock           = NULL;
    COMPONENT_T                      *list[5];
    TUNNEL_T                         tunnel[4];
    ILCLIENT_T                       *client;
    int                              status           = 0;
    unsigned int                     data_len         = 0;
    OMX_ERRORTYPE                    r;
    WIN_DATA                         *data            = (WIN_DATA*)userData;
    
    memset(list,   0, sizeof(list));
    memset(tunnel, 0, sizeof(tunnel));
    
    if((client = ilclient_init()) == NULL)
    {
        return NULL;
    }

    // create video_decode
    if(ilclient_create_component(client, &video_decode, (char*)"video_decode", (ILCLIENT_CREATE_FLAGS_T)(ILCLIENT_DISABLE_ALL_PORTS | ILCLIENT_ENABLE_INPUT_BUFFERS)) != 0)
        status = -14;
    list[0] = video_decode;
    
    // create video_render
    if(status == 0 && ilclient_create_component(client, &video_render, (char*)"video_render", ILCLIENT_DISABLE_ALL_PORTS) != 0)
        status = -14;
    list[1] = video_render;
    
    // create clock
    if(status == 0 && ilclient_create_component(client, &clock, (char*)"clock", ILCLIENT_DISABLE_ALL_PORTS) != 0)
        status = -14;
    list[2] = clock;

    memset(&cstate, 0, sizeof(cstate));
    cstate.nSize             = sizeof(cstate);
    cstate.nVersion.nVersion = OMX_VERSION;
    cstate.eState            = OMX_TIME_ClockStateWaitingForStartTime;
    cstate.nWaitMask         = 1;
    if(clock != NULL && OMX_SetParameter(ILC_GET_HANDLE(clock), OMX_IndexConfigTimeClockState, &cstate) != OMX_ErrorNone)
        status = -13;

    // create video_scheduler
    if(status == 0 && ilclient_create_component(client, &video_scheduler, (char*)"video_scheduler", ILCLIENT_DISABLE_ALL_PORTS) != 0)
        status = -14;
    list[3] = video_scheduler;

    // change the window dimenions
    OMX_CONFIG_DISPLAYREGIONTYPE        display_region;
    
    OMX_INIT_STRUCTURE(display_region);
    
    display_region.nPortIndex         = 90;
    display_region.set                = (OMX_DISPLAYSETTYPE)(OMX_DISPLAY_SET_NUM | OMX_DISPLAY_SET_FULLSCREEN | OMX_DISPLAY_SET_MODE | OMX_DISPLAY_SET_DEST_RECT);
    display_region.num                = 0;
    display_region.fullscreen         = OMX_FALSE;
    display_region.mode               = OMX_DISPLAY_MODE_LETTERBOX; // OMX_DISPLAY_MODE_FILL;
    display_region.dest_rect.width    = data->winDim.w;
    display_region.dest_rect.height   = data->winDim.h;
    display_region.dest_rect.x_offset = data->winDim.x;
    display_region.dest_rect.y_offset = data->winDim.y;
//    printf ("%s: setting screen size: %d %d %d %d, camera: %d\n", __func__, data->winDim.x, data->winDim.y, data->winDim.w, data->winDim.h, data->cameraNumber);
    
    if((r = OMX_SetConfig(ilclient_get_handle(video_render), OMX_IndexConfigDisplayRegion, &display_region)) != OMX_ErrorNone) 
    {
        printf("%s: Failed to set display region for render output port 90", __func__);
    }   

    set_tunnel(tunnel,      video_decode, 131, video_scheduler, 10);
    set_tunnel(tunnel+1, video_scheduler, 11,     video_render, 90);
    set_tunnel(tunnel+2,           clock, 80,  video_scheduler, 12);
    
    // setup clock tunnel first
    if(status == 0 && ilclient_setup_tunnel(tunnel+2, 0, 0) != 0)
        status = -15;
    else
        ilclient_change_component_state(clock, OMX_StateExecuting);
    
    if(status == 0)
        ilclient_change_component_state(video_decode, OMX_StateIdle);

    memset(&format, 0, sizeof(OMX_VIDEO_PARAM_PORTFORMATTYPE));
    format.nSize              = sizeof(OMX_VIDEO_PARAM_PORTFORMATTYPE);
    format.nVersion.nVersion  = OMX_VERSION;
    format.nPortIndex         = 130;
    format.eCompressionFormat = OMX_VIDEO_CodingAVC;

    
    if(status == 0 &&
        OMX_SetParameter(ILC_GET_HANDLE(video_decode), OMX_IndexParamVideoPortFormat, &format) == OMX_ErrorNone &&
        ilclient_enable_port_buffers(video_decode, 130, NULL, NULL, NULL) == 0)
    {
        OMX_BUFFERHEADERTYPE    *buf;
        int                     port_settings_changed = 0;
        int                     first_packet          = 1;
        
        ilclient_change_component_state(video_decode, OMX_StateExecuting);
        
        while((buf = ilclient_get_input_buffer(video_decode, 130, 1)) != NULL)
        {
            // feed data and wait until we get port settings changed
            unsigned char   *dest     = buf->pBuffer;

            // get everyone to stop briefly to reconfirgure
            if (work.stopVid)
            {
                pthread_mutex_lock(&reconfigure_mutex);
                pthread_cond_wait( &reconfigure_cond, &reconfigure_mutex );
                pthread_mutex_unlock(&reconfigure_mutex);
                
                display_region.dest_rect.width    = data->winDim.w;
                display_region.dest_rect.height   = data->winDim.h;
                display_region.dest_rect.x_offset = data->winDim.x;
                display_region.dest_rect.y_offset = data->winDim.y;
//                    printf ("%s: setting screen size: %d %d %d %d, camera: %d\n", __func__, data->winDim->x, data->winDim->y, data->winDim->w, data->winDim->h, data->cameraNumber);
                
                if((r = OMX_SetConfig(ilclient_get_handle(video_render), OMX_IndexConfigDisplayRegion, &display_region)) != OMX_ErrorNone) 
                {
                    printf("%s: Failed to set display region for render output port 90", __func__);
                }
            }


            data_len += readPacket((void*)&work.bd[data->cameraNumber],  dest,  buf->nAllocLen-data_len);
            
            if(port_settings_changed == 0 &&
                ((data_len > 0 && ilclient_remove_event(video_decode, OMX_EventPortSettingsChanged, 131, 0, 0, 1) == 0) ||
                (data_len == 0 && ilclient_wait_for_event(video_decode, OMX_EventPortSettingsChanged, 131, 0, 0, 1,
                ILCLIENT_EVENT_ERROR | ILCLIENT_PARAMETER_CHANGED, 10) == 0)))
            {
                port_settings_changed = 1;
                
                if(ilclient_setup_tunnel(tunnel, 0, 0) != 0)
                {
                    status = -7;
                    break;
                }
                
                ilclient_change_component_state(video_scheduler, OMX_StateExecuting);
                
                // now setup tunnel to video_render
                if(ilclient_setup_tunnel(tunnel+1, 0, 1000) != 0)
                {
                    status = -12;
                    break;
                }
                
                ilclient_change_component_state(video_render, OMX_StateExecuting);
            }
        
            buf->nFilledLen = data_len;
            data_len        = 0;
            
            buf->nOffset    = 0;
            if(first_packet)
            {
                buf->nFlags  = OMX_BUFFERFLAG_STARTTIME;
                first_packet = 0;
            }
            else
                buf->nFlags  = OMX_BUFFERFLAG_TIME_UNKNOWN;
            
            if(OMX_EmptyThisBuffer(ILC_GET_HANDLE(video_decode), buf) != OMX_ErrorNone)
            {
                status = -6;
                break;
            }
        }
        
        buf->nFilledLen = 0;
        buf->nFlags     = OMX_BUFFERFLAG_TIME_UNKNOWN | OMX_BUFFERFLAG_EOS;
        
        if(OMX_EmptyThisBuffer(ILC_GET_HANDLE(video_decode), buf) != OMX_ErrorNone)
            status = -20;
        
        // wait for EOS from render
        ilclient_wait_for_event(video_render, OMX_EventBufferFlag, 90, 0, OMX_BUFFERFLAG_EOS, 0,
                                ILCLIENT_BUFFER_FLAG_EOS, 10000);
        
        // need to flush the renderer to allow video_decode to disable its input port
        ilclient_flush_tunnels(tunnel, 0);
    }

    ilclient_disable_tunnel(tunnel);
    ilclient_disable_tunnel(tunnel+1);
    ilclient_disable_tunnel(tunnel+2);
    ilclient_disable_port_buffers(video_decode, 130, NULL, NULL, NULL);
    ilclient_teardown_tunnels(tunnel);
    
    ilclient_state_transition(list, OMX_StateIdle);
    ilclient_state_transition(list, OMX_StateLoaded);
    
    ilclient_cleanup_components(list);
    
    ilclient_destroy(client);
    return NULL;
}


/*  -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//  Method:  handleSigUser1 ()
//      Simple debug method.  When receiving a SIGUSR1, the program will change
//      the camera number for each of the active windows.  This is to experiment
//      with camera switching
//
//  Author: S.S. Mellon
//  Date: Oct 30, 2018
//  -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */

void handleSigUser1 (int sig) 
{ 
    int         num =0;
    
    work.stopVid = 1;
    usleep (100000);
    if (++work.layout >= LAYOUT_TYPE_3X5_MATRIX)
        work.layout = LAYOUT_TYPE_FULLSCREEN;
    
    work.numWindows = windowLayout[work.layout].numVert * windowLayout[work.layout].numHoriz;
    
    pthread_mutex_lock( &reconfigure_mutex );
    
    // resize the visable windows
    for (num = 0; num < work.numWindows; num++)
    {
        work.windows[num].winDim       = windowLayout[work.layout].rects[num];
    }
    
    // and resize the ones we don't want to see anymore
    for (; num < NUM_CAMS; num++)
    {
        work.bd[num].reader = 0;
        work.windows[num].winDim       = {0,0,0,0};
    }
    
     work.stopVid = 0;
    
     pthread_cond_broadcast( &reconfigure_cond );
     pthread_mutex_unlock( &reconfigure_mutex );
}   


/*  -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//  Method:  handleSigUser2 ()
//      Simple debug method.  When receiving a SIGUSR2, the program will print
//      the collected statistics for each of the camera streams
//
//  Author: S.S. Mellon
//  Date: Oct 30, 2018
//  -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */

void handleSigUser2 (int sig) 
{ 
    printf ("camera stats:\n");
    // print out the stats for the windows
    for (int cam = 0; cam < NUM_CAMS; cam++)
    {
        int val = 0;

        printf ("    camera %2d:", cam);
        if (work.bd[cam].reader == 0)
        {
            printf ("    no reader\n\n");
            continue;
        }
        
        sem_getvalue (&work.bd[cam].mutex, &val);  
        
        printf ("\n         rd loc: %d\n", work.bd[cam].loc);
        printf ("         wr loc: %d\n", work.bd[cam].end);
        printf ("      max usage: %d\n", work.bd[cam].maxSize);
        printf ("    mutex state: %d\n", val);
        printf ("  i-frame skips: %d\n", work.bd[cam].skippedIFrames);
        printf ("  p-frame skips: %d\n", work.bd[cam].skippedPFrames);
        printf ("\n");            
    }
}   


/*  -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
//  Method:  main ()
//      
//
//  Author: S.S. Mellon
//  Date: Oct 30, 2018
//  -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */

int main (int argc, char **argv)
{
    OMX_U32             screen_width  = 1920, 
                        screen_height = 1080;
    int                 err;
    int                 cam;

    memset (&work, 0, sizeof(WORK));    

    if (argc < 2 || (strcmp (argv[1], "-h") == 0))
    {
        printf ("usage: %s <layout>\n", argv[0]);
        printf ("     where layout can be:   0 - fullscreen       (1 camera)\n");
        printf ("                            1 - vertical split   (2 cameras)\n");
        printf ("                            2 - horizontal split (2 cameras)\n");
        printf ("                            3 - 2x2 grid         (4 cameras)\n");
        printf ("                            4 - 2x3 grid         (6 cameras)\n");
        printf ("                            5 - 3x3 grid         (9 cameras)\n");
        printf ("                            6 - 3x4 grid         (12 cameras)\n");
        printf ("                            7 - 3x5 grid         (15 cameras)\n");
        printf ("                            8 - 4x4 grid         (16 cameras)\n\n");
        
        printf ("   issue SIGUSR1 to transition to next camera\n");
        printf ("         SIGUSR2 to dump buffer stats\n\n");
        
        if (argc >= 2)
            return 0;
    }
    else
    {
        // get the layout type from the command line
        work.layout = atoi (argv[argc-1]);
    }
    
    // assign signal handlers for debug/test functions
    signal(SIGUSR1, handleSigUser1); 
    signal(SIGUSR2, handleSigUser2); 
    
    bcm_host_init();
    
    if(OMX_Init() != OMX_ErrorNone)
    {
        printf ("%s: OMX_Init call failed\n", __func__);
        return 0;
    }

    
    // calculate all the window sizes based on the current screen dimensions
    if(graphics_get_display_size(0, &screen_width, &screen_height) < 0) 
    {
        printf("%s: Failed to get display size", __func__);
    }   
    
   printf ("%s: screen size: %d %d\n", __func__, screen_width, screen_height);
   
    for (int type = LAYOUT_TYPE_FULLSCREEN; type < LAYOUT_TYPE_MAX; type++)
    {
        int      numRects   = windowLayout[type].numHoriz * windowLayout[type].numVert;
        int      rectWidth  = screen_width / windowLayout[type].numVert;
        int      rectHeight = screen_height / windowLayout[type].numHoriz;
        int      rectNum    = 0;
    
        windowLayout[type].rects = (WIN_RECT*) malloc (numRects * sizeof (WIN_RECT));
        printf ("layout type %d,  v:%d  h:%d  \n", type, windowLayout[type].numVert,windowLayout[type].numHoriz);        
    
        for (int vert = 0; vert < windowLayout[type].numVert; vert++)
        {
    
            for (int horiz = 0; horiz < windowLayout[type].numHoriz; horiz++)
            {
                windowLayout[type].rects[rectNum].x = 0 + (vert * rectWidth);
                windowLayout[type].rects[rectNum].y = 0 + (horiz * rectHeight);
                windowLayout[type].rects[rectNum].w = rectWidth;
                windowLayout[type].rects[rectNum].h = rectHeight;
//                printf ("    rect %d %d       %d,%d    %d,%d\n", vert, horiz, 
//                    windowLayout[type].rects[rectNum].x,         
//                    windowLayout[type].rects[rectNum].y,       
//                    windowLayout[type].rects[rectNum].w,       
//                    windowLayout[type].rects[rectNum].h);
                rectNum++;
            }
        }
    }
   
    // initialize all the camera stream buffers
    for (cam = 0; cam < NUM_CAMS; cam++)
    {
        work.bd[cam].size    = CAMERA_BUFFER_SIZE;
        work.bd[cam].ptr     = (uint8_t*)malloc (work.bd[cam].size);
        work.bd[cam].end     = 0;
        work.bd[cam].loc     = 0;
        work.bd[cam].maxSize = 0;
        sem_init (&work.bd[cam].mutex, 0, 0); 
        
        work.windows[cam].cameraNumber = cam;  //4 + num;
        work.windows[cam].winDim       = {0,0,0,0};
        work.windows[cam].id           = cam;
            
        err = pthread_create (&work.windows[cam].vidThread, 0, &playVideo, (void*)&work.windows[cam]);
        if (err != 0)
        {
            printf ("%s: create video thread failed\n", __func__);
        }
        
    }
               
    // calculate the number of windows in the layout
    work.numWindows = windowLayout[work.layout].numVert * windowLayout[work.layout].numHoriz;
        
    pthread_mutex_lock( &reconfigure_mutex );

    for (int num = 0; num < work.numWindows; num++)
    {
        work.windows[num].cameraNumber = num;  //4 + num;
        work.windows[num].winDim       = windowLayout[work.layout].rects[num];
    }
    
     pthread_cond_broadcast( &reconfigure_cond );
     pthread_mutex_unlock( &reconfigure_mutex );
            
    // initialize the watcher interaction
    if (initWatcher (&work) < 0)
    {
        printf ("%s: idis watcher initialization failed\n", __func__);
        return 0;
    }
    
    // wait for all the threads to complete (shouldn't happen in current approach)
    for (int num = 0; num < work.numWindows; num++)
    {
        pthread_join (work.windows[num].vidThread, NULL);
    }
    
    printf ("%s: all threads completed.  exitting\n", __func__);
    
    OMX_Deinit();

    return 0;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  ...End of file */

