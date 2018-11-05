// IDIS_WatchSDK.h : header file
//

#ifndef _IDIS_WATCHSDK_INTERFACE_
#define _IDIS_WATCHSDK_INTERFACE_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IDIS_Define.h"

#ifdef __cplusplus
extern "C" {
#endif

//////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma pack(push, 1)
#endif

typedef struct
#if defined(__linux__) || defined(__APPLE__) || defined(ANDROID)
__attribute__ ((packed))
#endif
{
	enum _types {
		ONCONNECTED = 0,
		ONDISCONNECTED,
		FRAMELOADED,
		EVENTLOADED,
		STATUSLOADED,
		RECVPTZPRESET,
        RECVG2PTZPRESET,
		RECVPTZPRESETIDR,
		RECVIDRTEXTIN,
		RECVIDRTEXTINDATA,
		IDRTITLECHANGED,
		RECVTEXTIN,
		RECVPTZADVMENU,
		IDRSTATUSLOADED,
        DEVICESTATUSLOADED,   // for new DVR and IP Cameras..
        AUDIOOUTOPENRESULT,   // [audio] response for watch_requestDeviceAudioOutOpen from DVR (contains audio out availability of connected device)
        AUDIOCAPTURED,        // [audio] this callback is from sound device (not DVR).
        AUDIOSTREAMCLOSED,    // [audio] this callback is from sound device (not DVR) when sound streaming is finished.
        AUDIOCAPTURESTARTED,  // [audio] this callback is from sound device (not DVR) when sound capturing is started.
        AUDIOCAPTUREFINISHED, // [audio] this callback is from sound device (not DVR) when sound capturing is finished.
		MAX_LISTENER
	};
    // Note : STATUSLOADED callback is replaced with DEVICESTATUSLOADED.
    // SDK user must implement both callbacks for compatibility.
} CALLBACK_WATCH;

/* Removed
: use watch_getProductInfo to retrieve device information
typedef struct
{
	UINT16 _camcount;		// count of camera
	UINT16 _alarmcnt;		// count of alarm-out
	UINT16 _audiocnt;		// count of audio
	UINT16 _dummy;
} PARAMW_ONCONNECTED, *LPPARAMW_ONCONNECTED;
*/

typedef struct
#if defined(__linux__) || defined(__APPLE__) || defined(ANDROID)
__attribute__ ((packed))
#endif
{
    enum _codec_types {
        CODEC_UNKNOWN    = 0,
        CODEC_ADPCM      = 1,
        CODEC_L16        = 2,
        CODEC_G723       = 3,
        CODEC_G726       = 4,
        CODEC_G711_ULAW  = 5,
        CODEC_G711_ALAW  = 6,
        CODEC_ISP1000    = 7,
        CODEC_ADPCM4BIT  = 8,
        CODEC_ADPCMF     = 9,
        CODEC_G721       = 10,
        CODEC_AAC        = 11,
        CODEC_AAC_INT    = 12,
        CODEC_PCM        = 13,
    };

    UINT16 _codec;
    UINT16 _sampling;       // e.g. 16000 hz
    UINT16 _channel;        // 1(mode), 2(left + right), 3(right + left)
    UINT16 _segment;        // e.g. 6400 (samples)
    UINT32 _bitrate;        // e.g. 64000 bps
    UINT16 _bitsPerSample;  // e.g. 16 bits
    LPBYTE _rawData;
    LPBYTE _encodedData;    // pointer to actual audio data.
    int    _length;         // encoded data length
} PARAMW_AUDIOINFO, *LPPARAMW_AUDIOINFO;

typedef struct
#if defined(__linux__) || defined(__APPLE__) || defined(ANDROID)
__attribute__ ((packed))
#endif
{
	enum _const {
		MAX_LEN_DESCRIPTION = 31,
		MAX_LEN_CAMERA_TITLE= 31
	};

    enum _cameraStatus {
        VIDEOLOSS = -1,
        INACTIVE = 0,
        ACTIVE = 1,
        NOTCONNECTED = 2,
        MULTISTREAM = 3
    };

    enum _covertLevel {
        NORMAL = 0,
        COVERT1 = 1,
        COVERT2 = 2
    };

	DWORD   _dwSize;								// this struct size
	UINT16  _cameraCount;							// dvr camera count that enable connect
	UINT16  _sensorCount;							// dvr sensor count that enable connect
	CHAR    _szSiteDesc[MAX_LEN_DESCRIPTION + 1];	// dvr site name
#if defined(__APPLE__) || defined(ANDROID)
	CHAR    _szCameraDesc[::MAX_SYS_CAMERA32][MAX_LEN_CAMERA_TITLE + 1];	// each camera name
	CHAR    _camStatus[::MAX_SYS_CAMERA32];			// not activated, activated, no video...
	BYTE    _ptzAvailable[::MAX_SYS_CAMERA32];		// whether is ptz camera or not
	BYTE    _audioAvailable[::MAX_SYS_CAMERA32];		// enable audio
	BYTE    _covertLevel[::MAX_SYS_CAMERA32];			// covert level(covert1, covert2)
#else
	CHAR    _szCameraDesc[MAX_SYS_CAMERA32][MAX_LEN_CAMERA_TITLE + 1];	// each camera name
	CHAR    _camStatus[MAX_SYS_CAMERA32];			// not activated, activated, no video...
	BYTE    _ptzAvailable[MAX_SYS_CAMERA32];		// whether is ptz camera or not
	BYTE    _audioAvailable[MAX_SYS_CAMERA32];		// enable audio
	BYTE    _covertLevel[MAX_SYS_CAMERA32];			// covert level(covert1, covert2)
#endif
} PARAMW_NETSTATUS, *LPPARAMW_NETSTATUS;

typedef struct
#if defined(__linux__) || defined(__APPLE__) || defined(ANDROID)
__attribute__ ((packed))
#endif
{
    enum _const {
        MAX_LEN_DESCRIPTION = 31,
        MAX_LEN_CAMERA_TITLE= 31
    };
    enum CommonStatus {
        UNKNOWN = -1,
        INACTIVE = 0,
        ACTIVE = 1,
    };
    enum CameraStatus {
        VIDEOLOSS = -1,
        NOTCONNECTED = 2,
        MULTISTREAM = 3
    };
    enum PTZStatus {
        ADVANCED = 2
    };
    enum CovertLevel {
        NORMAL = 0,
        COVERT1,
        COVERT2
    };
    enum AudioInStatus {
        MICROPHONE = 1,
        LINEIN
    };
    // Changed from [NC, NO] to [STATUS_NC, STATUS_NO]
    // NO is reserved keyword in iPhone, iPad, OSX dev. environments.
    enum AlarmStatus {
        STATUS_NC = 1,
        STATUS_NO
    };
    enum RecordingStatus {
        NO_RECORD = 0,
        RECORD
    };

    DWORD   _dwSize;
    CHAR    _szSiteDescription[MAX_LEN_DESCRIPTION + 1];
    CHAR    _szCameraDescription[MAX_SYS_DEVICE][MAX_LEN_CAMERA_TITLE + 1];	// each camera name
    CHAR    _cameraStatus[MAX_SYS_DEVICE];			// not activated, activated, no video..
    CHAR    _ptzStatus[MAX_SYS_DEVICE];
    CHAR    _cameraCovertLevel[MAX_SYS_DEVICE];
    CHAR    _audioInStatus[MAX_SYS_DEVICE];
    CHAR    _audioOutStatus[MAX_SYS_DEVICE];
    CHAR    _alarmInDescription[MAX_SYS_DEVICE][MAX_LEN_DESCRIPTION + 1];
    CHAR    _alarmInStatus[MAX_SYS_DEVICE];
    CHAR    _alarmOutDescription[MAX_SYS_DEVICE][MAX_LEN_DESCRIPTION + 1];
    CHAR    _alarmOutStatus[MAX_SYS_DEVICE];
    CHAR    _recordingStatus[MAX_SYS_DEVICE];
    UINT32  _multiStreamStatus[MAX_SYS_DEVICE];
    CHAR    _remoteAudioInStatus[MAX_SYS_DEVICE];
} PARAMW_NETDEVICESTATUS, *LPPARAMW_NETDEVICESTATUS;

typedef struct
#if defined(__linux__) || defined(__APPLE__) || defined(ANDROID)
__attribute__ ((packed))
#endif
{
	DWORD	_dwSize;
	BYTE	_countofCamera;							// count of camera
	BYTE	_countofSensor;							// count of sensor
	BYTE	_countofAlarmout;						// count of alarm-out
	/* system check - video loss
	 * 0: STATE_LOSS; 1:STATE_NORMAL; etc: STATE_NONCHECK
	 */
#if defined(__APPLE__) || defined(ANDROID)
	BYTE	_stateCamera[::MAX_SYS_CAMERA32];
#else
	BYTE	_stateCamera[MAX_SYS_CAMERA32];
#endif
	/* status - motion event
	 * 0: STATE_ON; 1: STATE_ON | STATE_MOTION; etc: STATE_OFF
	 */
#if defined(__APPLE__) || defined(ANDROID)
	BYTE	_stateMotion[::MAX_SYS_CAMERA32];
#else
	BYTE	_stateMotion[MAX_SYS_CAMERA32];
#endif
	/* status - alarm-in event
	 * 0: STATE_ON; 1: STATE_ON | STATE_DETECT; etc: STATE_OFF
	 */
#if defined(__APPLE__) || defined(ANDROID)
	BYTE	_stateAlarmin[::MAX_SYS_CAMERA32];
#else
	BYTE	_stateAlarmin[MAX_SYS_CAMERA32];
#endif
	/* status - alarm-out event
	 * 0: STATE_ON; 1: STATE_ON | STATE_OUT; etc: STATE_OFF
	 */
#if defined(__APPLE__) || defined(ANDROID)
	BYTE	_stateAlarmout[::MAX_SYS_CAMERA32];
#else
	BYTE	_stateAlarmout[MAX_SYS_CAMERA32];
#endif
	/*
	 * will modify; may add attributes
	 */
} PARAMW_NETSTATUSIDR, *LPPARAMW_NETSTATUSIDR;

typedef struct
#if defined(__linux__) || defined(__APPLE__) || defined(ANDROID)
__attribute__ ((packed))
#endif
{
	enum _event_type {
		DISK_ALMOST_FULL	= 0U,
		ALARM_IN			= 1U,
		TEXT_IN				= 2U,
		MOTION				= 3U,
		VIDEOLOSS			= 4U,
		ALARM_RESET_IN		= 5U,
		SENSORMANUAL		= 6U,
		RECORDING			= 7U,
		DISK_FULL			= 8U,
		RECORDER_BAD		= 9U,
		ALARM_IN_BAD		= 10U,
		DISK_BAD			= 11U,
		DISK_TEMPERATURE	= 12U,
		DISK_SMART			= 13U,
		SYSTEM_ALIVE		= 14U,
		PANIC_RECORD		= 15U,
		SIPASS_REC_ON		= 16U,
		SIPASS_REC_OFF		= 17U,
		OBJECT_TRACK		= 18U,
		VIDEOBLIND			= 19U,
		PANIC_RECORD_OFF	= 20U,
		ALARM_IN_OFF		= 21U,
		STOP_RECORDING		= 22U,
		FAN_ERROR_ON		= 23U,
		FAN_ERROR_OFF		= 24U,
		VIDEOON				= 28U,	// for INT
		MOTION_OFF			= 29U,	// for INT
		VIDEO_BLIND_OFF		= 30U,	// for INT
		NETWORK_ALARM_ON	= 34U,	// for Network Alarm On
		NETWORK_ALARM_OFF	= 35U, // for Network Alarm Off
        DISK_CONFIG_CHANGE  = 36U,
        SYSTEM_BOOT_UP      = 37U,
        SYSTEM_RESTART      = 38U,
        SYSTEM_SHUTDOWN     = 39U,
        STORAGE_ON          = 40U,
        STORAGE_OFF         = 41U,
        VIDEO_ANALYTICS     = 42U,
        VIDEO_ANALYTICS_OFF = 43U,
        IGNORED_MOTION_ON   = 44U,
        ALARM_IN_BAD_OFF    = 47U,
        OBJECT_TRACKER_OFF  = 48U,
        VIDEO_INIT          = 49U,
        TEXT_IN_OFF         = 50U,
        RECORDER_BAD_OFF    = 51U,
        DISK_FULL_OFF       = 52U,
        DISK_ALMOST_FULL_OFF = 53U,
        DISK_TEMPERATURE_OFF = 54U,
        DISK_SMART_OFF       = 55U,
        CAR_OVERSPEED_ON     = 56U,
        CAR_OVERSPEED_OFF    = 57U,
        CAR_SUDDEN_ACCELERATION = 58U,
        CAR_SUDDEN_STOP      = 59U,
        CAR_STARTING_WITH_DOORS_OPEN = 60U,
        AUDIO_ON             = 61U,
        AUDIO_OFF            = 62U,
        IGNORED_AUDIO_ON     = 63U,
        TRIPZONE_ON          = 64U,
        TRIPZONE_OFF         = 65U,
        IGNORED_TRIPZONE_ON  = 66U,
        TAMPER_ON            = 67U,
        TAMPER_OFF           = 68U,
        IGNORED_VIDEO_ANALYTICS_ON = 69U,
        NO_DISK              = 70U,
        TEXT_IN_BAD_ON       = 71U,
        TEXT_IN_BAD_OFF      = 72U,
        USER_DEFINED_ALARM_ON = 73U,
        USER_DEFINED_ALARM_OFF = 74U,
        GPS_RECEIVE_ERROR_ON     = 75U,
        GPS_RECEIVE_ERROR_OFF    = 76U,
        LOGIN_FAILED_SEVERAL_TIMES = 77U,
        USER_LOGIN           = 78U,
        USER_LOGOUT          = 79U,
        SETUP_CHANGED        = 80U
	};

	enum _const {
		MAX_LEN_LOG_LABEL       = 32,
		MAX_ALARMOUT_BUTTONS_EX = 32
	};

	DWORD   _dwSize;						// this struct size
	UINT32  _seqnum;						// saved log number
	BYTE    _evtType;						// alarm type (Motion, Text-In, Alarm, Sensor)
	BYTE    _evtId;							// alarm device number
	UINT32  _tmTime;						// event occurred time
	UINT32  _tmMsec;						// event occurred msec
	UINT32	_cameraRef;						// relative all cameras
	BYTE    _fisIDR;						// whether idr or not
	CHAR    _evtLabel[MAX_LEN_LOG_LABEL];	// alarm label
	BYTE    _evtLevel;						// event level(emergency, normal)
	BYTE    _netAlarmType;					// network alarm type
} PARAMW_EVENTINFO, *LPPARAMW_EVENTINFO;

typedef struct
#if defined(__linux__) || defined(__APPLE__) || defined(ANDROID)
__attribute__ ((packed))
#endif
{
	enum _setcolor_type {
		CTRL_REVERT = -1,
		CTRL_BRIGHT,
		CTRL_CONTRAST,
		CTRL_SATURATION,
		CTRL_HUE
	};
} PARAMW_SETCOLOR;

typedef struct
#if defined(__linux__) || defined(__APPLE__) || defined(ANDROID)
__attribute__ ((packed))
#endif
{
	UINT16 _camera;
	BYTE   _fstart;
	BYTE   _dummy;
} PARAMW_IDRTEXTIN, *LPPARAMW_IDRTEXTIN;

typedef struct
#if defined(__linux__) || defined(__APPLE__) || defined(ANDROID)
__attribute__ ((packed))
#endif
{
	UINT32 _length;
	PCHAR  _lpData;
} PARAMW_IDRTEXTINDATA, *LPPARAMW_IDRTEXTINDATA;

typedef struct
#if defined(__linux__) || defined(__APPLE__) || defined(ANDROID)
__attribute__ ((packed))
#endif
{
	enum _type_transaction {
		TRANSACTION_BEGIN,
		TRANSACTION_CONTINUE,
		TRANSACTION_END,
		TRANSACTION_COMPLETE,
	};

	UINT32 _cameras;			// cameras of relative
	BYTE   _typeofTransaction;
	UINT32 _length;
	PCHAR  _lpData;
} PARAMW_TEXTINDATA, *LPPARAMW_TEXTINDATA;

typedef struct
#if defined(__linux__) || defined(__APPLE__) || defined(ANDROID)
__attribute__ ((packed))
#endif
{
	enum _const {
		MAX_NUM_PTZ_PRESET = 16,
		MAX_LEN_PTZ_PRESET = 16
	};
	CHAR _szPresetLabel[MAX_NUM_PTZ_PRESET][MAX_LEN_PTZ_PRESET + 1];
} PARAMW_PTZPRESET, *LPPARAMW_PTZPRESET;


typedef struct
#if defined(__linux__) || defined(__APPLE__) || defined(ANDROID)
__attribute__ ((packed))
#endif
{
    UINT32 _camara;
	enum _const {
		MAX_NUM_G2PTZ_PRESET = 256,
		MAX_LEN_G2PTZ_PRESET = 16
	};
	CHAR _szPresetLabel[MAX_NUM_G2PTZ_PRESET][MAX_LEN_G2PTZ_PRESET + 1];
} PARAMW_PTZG2PRESET, *LPPARAMW_PTZG2PRESET;

typedef struct
#if defined(__linux__) || defined(__APPLE__) || defined(ANDROID)
__attribute__ ((packed))
#endif
{
	enum _const {
		MAX_NUM_PTZ_PRESET128 = 128,
		MAX_LEN_PTZ_PRESET    = 16
	};
	DWORD _dwSize;
	INT   _nComPort;
	INT   _nModel;
	INT   _nID;
	INT   _byteHeader;
	CHAR  _szPresetLabel[MAX_NUM_PTZ_PRESET128][MAX_LEN_PTZ_PRESET + 1];
} PARAMW_PTZINFO128, *LPPARAMW_PTZINFO128;

typedef struct
#if defined(__linux__) || defined(__APPLE__) || defined(ANDROID)
__attribute__ ((packed))
#endif
{
	enum _ptz_type {
		TYPE_USE_PTZ_NONE = 0,
		TYPE_USE_PTZ_NORMAL,
		TYPE_USE_PTZ_ADVANCED
	};

	enum _ptzcmd {
		PTZ_MOVE_N = 0,
		PTZ_MOVE_NE,
		PTZ_MOVE_E,
		PTZ_MOVE_SE,
		PTZ_MOVE_S,
		PTZ_MOVE_SW,
		PTZ_MOVE_W,
		PTZ_MOVE_NW,
		PTZ_ZOOM_OUT,
		PTZ_ZOOM_IN,
		PTZ_FOCUS_NEAR,
		PTZ_FOCUS_FAR,
		PTZ_IRIS_CLOSE,
		PTZ_IRIS_OPEN,
		PTZ_MOVE_PRESET,
		PTZ_SET_PRESET,
		PTZ_STOP_MOVE,
		PTZ_STOP_ZOOM,
		PTZ_STOP_FOCUS,
		PTZ_STOP_IRIS,

		PTZ_SET_SPEED,
		PTZ_MOVETO_ORIGIN,
		PTZ_AUTO_PAN_ON,
		PTZ_AUTO_PAN_OFF,
		PTZ_SCAN_ON,
		PTZ_SCAN_OFF,
		PTZ_PATTERN_ON,
		PTZ_PATTERN_OFF,
		PTZ_TOUR_ON,
		PTZ_TOUR_OFF,

		PTZ_MENU_ON,
		PTZ_MENU_OFF,
		PTZ_MENU_UP,
		PTZ_MENU_DOWN,
		PTZ_MENU_RIGHT,
		PTZ_MENU_LEFT,
		PTZ_MENU_ENTER,
		PTZ_MENU_ESC,
		PTZ_NUMBERKEY,
		PTZ_SETCTRL_ON,

		PTZ_LIGHT_TURNON,
		PTZ_LIGHT_TURNOFF,
		PTZ_POWER_TURNON,
		PTZ_POWER_TURNOFF,
		PTZ_PUMP_TURNON,
		PTZ_PUMP_TURNOFF,
		PTZ_WIPER_TURNON,
		PTZ_WIPER_TURNOFF,
		PTZ_AUX_TURNON,
		PTZ_AUX_TURNOFF,
		PTZ_ONE_PUSH    = 52
	};

	enum {
		PTZ_ADVANCED_MENU_OPEN = 0x7F01,	// for ptz_advanced_menu dialog
	};

	enum _ptzarg {
		PTZARG_STEP   = 0,
		PTZARG_REPEAT = 1
	};

	DWORD _dwSize;		// this struct size
	SHORT _ptzcamid;	// camera number
	SHORT _command;		// ptz_command
	SHORT _argument;	// 0: step, 1: continuing & require stop command
	SHORT _reserved;
} PARAMW_PTZCOMMAND, *LPPARAMW_PTZCOMMAND;

typedef struct
#if defined(__linux__) || defined(__APPLE__) || defined(ANDROID)
__attribute__ ((packed))
#endif
{
	enum _ptzadv_mask {
		PTZADV_MASK_SPEED		= 0x00000001,
		PTZADV_MASK_AUTOPAN		= 0x00000002,
		PTZADV_MASK_TOUR		= 0x00000004,
		PTZADV_MASK_PATTERN		= 0x00000008,
		PTZADV_MASK_SCAN		= 0x00000010,
		PTZADV_MASK_OSDMENU		= 0x00000020,
		PTZADV_MASK_LIGHT		= 0x00000040,
		PTZADV_MASK_PUMP		= 0x00000080,
		PTZADV_MASK_WIPER		= 0x00000100,
		PTZADV_MASK_POWER		= 0x00000200,
		PTZADV_MASK_AUX			= 0x00000400,
		PTZADV_MASK_ORIGIN		= 0x00000800,
		PTZADV_MASK_OUTMONITOR	= 0x00001000,
		PTZADV_MASK_CTRL		= 0x00002000
	};

	DWORD	_dwSize;		// this struct size
	UINT	_menuItem;		// bit-mask for supported function
	UINT	_menuItemEx;
	BYTE	_isExpanded;	// 1 : may use _menuItemEx
	BYTE	_isApplyCtrl;	// 1 : CTRL-Key is applied
} PARAMW_PTZADVMENUITEM, *LPPARAMW_PTZADVMENUITEM;

typedef struct
#if defined(__linux__) || defined(__APPLE__) || defined(ANDROID)
__attribute__ ((packed))
#endif
{
	enum _type_support {
		SUPPORT_FULLDUPLEX_AUDIO = 0,		// full-duplex audio
		SUPPORT_DRAW_MOTION,				// draw motion
		SUPPORT_HYBRIDCAMERA,				// hybrid camera
		SUPPORT_NETSTATUS_IDR,				// net status for IDR
		SUPPORT_MULTI_STREAM,				// select multi-stream channel
	};
} IS_SUPPORT_WATCH;

typedef struct
#if defined(__linux__) || defined(__APPLE__) || defined(ANDROID)
__attribute__ ((packed))
#endif
{
	enum _const {
		MAX_LEN_LOG_LABEL = 32,
	};

	DWORD	_dwSize;						// this struct size
	CHAR	_version[2];					// version
	UINT	_type;							// alarm type (fire ...)
	UINT	_id;							// alarm id
	UINT32  _time;							// alarm occurred time
	UINT32  _msec;							// alarm occurred msec
	CHAR	_label[MAX_LEN_LOG_LABEL];		// alarm label
} PARAMW_NETALARMINFO, *LPPARAMW_NETALARMINFO;

typedef struct
#if defined(__linux__) || defined(__APPLE__) || defined(ANDROID)
__attribute__ ((packed))
#endif
{
        LPSTR lpData; 
        DWORD dwBufferLength; 
        DWORD dwBytesRecorded; 
        DWORD_PTR dwUser; 
        DWORD dwFlags; 
        DWORD dwLoops; 
        struct wavehdr_tag *lpNext; 
        DWORD_PTR reserved; 
} PARAMW_WAVEHDR, *LPPARAMW_WAVEHDR;

#if _MSC_VER > 1000
#pragma pack(pop)
#endif
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

// watchsdk_callback_func types
// reference below CALLBACK_WATCH struct for ntypes
IDIS_DLLFUNC void IDISAPI watch_registerCallback(IDISHWATCH hWatch, int ntypes, LPLISTENER_FUNC lpFunc);

//////////////////////////////////////////////////////////////////////////

IDIS_DLLFUNC IDISHWATCH IDISAPI watch_initialize(void);
IDIS_DLLFUNC void IDISAPI watch_finalize(IDISHWATCH hWatch);
IDIS_DLLFUNC void IDISAPI watch_startup(IDISHWATCH hWatch, int nMaxChannels);
IDIS_DLLFUNC void IDISAPI watch_cleanup(IDISHWATCH hWatch);

// watchSDK_connection //////////////////////////

IDIS_DLLFUNC int  IDISAPI watch_connect(IDISHWATCH hWatch, LPCTSTR siteName, LPCTSTR siteAddress, LPCTSTR userName, LPCTSTR userPassword, int port, bool isPasswordEncrypted = false, void* attachment = NULL);
IDIS_DLLFUNC bool IDISAPI watch_reconnect(IDISHWATCH hWatch, LPCTSTR siteName, LPCTSTR siteAddress, LPCTSTR userName, LPCTSTR userPassword, int port, short channel, void* attachment = NULL);
IDIS_DLLFUNC int  IDISAPI watch_connect_dvrns(IDISHWATCH hWatch, LPCTSTR siteName, LPCTSTR dvrnsAddress, LPCTSTR dvrName, LPCTSTR userName, LPCTSTR userPassword, int dvrnsPort, void* attachment = NULL);
IDIS_DLLFUNC bool IDISAPI watch_reconnect_dvrns(IDISHWATCH hWatch, LPCTSTR siteName, LPCTSTR dvrnsAddress, LPCTSTR dvrName, LPCTSTR userName, LPCTSTR userPassword, int dvrnsPort, short channel, void* attachment = NULL);
IDIS_DLLFUNC void IDISAPI watch_disconnect(IDISHWATCH hWatch, int channel);
IDIS_DLLFUNC bool IDISAPI watch_isConnecting(IDISHWATCH hWatch, int channel);
IDIS_DLLFUNC bool IDISAPI watch_isConnected(IDISHWATCH hWatch, int channel);
IDIS_DLLFUNC bool IDISAPI watch_isDisconnecting(IDISHWATCH hWatch, int channel);
IDIS_DLLFUNC bool IDISAPI watch_isDisconnected(IDISHWATCH hWatch, int channel);
IDIS_DLLFUNC bool IDISAPI watch_isDisconnectable(IDISHWATCH hWatch, int channel);
IDIS_DLLFUNC bool IDISAPI watch_setCameraList(IDISHWATCH hWatch, int channel, const LPBYTE cameras, int length);
IDIS_DLLFUNC bool IDISAPI watch_setCameraList_v2(IDISHWATCH hWatch, int channel, const LPBYTE cameras, int length);
IDIS_DLLFUNC void IDISAPI watch_sendRequestCheck(IDISHWATCH hWatch, int channel);

// connectService
IDIS_DLLFUNC int IDISAPI watch_connect_connectService(IDISHWATCH hWatch, LPCTSTR siteName, LPCTSTR dvrName, LPCTSTR gatewayAddress, int gatewayPort, int proxyPort, LPCTSTR userName, LPCTSTR userPassword, int natType, void* attachment);
// watchSDK_get function ////////////////////////

IDIS_DLLFUNC bool IDISAPI watch_getUsePTZ(IDISHWATCH hWatch, int channel, int camera);
IDIS_DLLFUNC int  IDISAPI watch_getUsePTZEx(IDISHWATCH hWatch, int channel, int camera);
IDIS_DLLFUNC bool IDISAPI watch_needPtzAdvancedMenu(IDISHWATCH hWatch, int channel, int camera);
IDIS_DLLFUNC bool IDISAPI watch_needPtzPreset(IDISHWATCH hWatch, int channel, int camera);
IDIS_DLLFUNC bool IDISAPI watch_getPtzPreset(IDISHWATCH hWatch, int channel, LPBYTE lpPreset);
IDIS_DLLFUNC bool IDISAPI watch_getPtzPresetIDR(IDISHWATCH hWatch, int channel, LPBYTE lpPreset);
IDIS_DLLFUNC bool IDISAPI watch_getStatus(IDISHWATCH hWatch, int channel, LPBYTE lpStatus);
IDIS_DLLFUNC bool IDISAPI watch_sendReqStatusIDR(IDISHWATCH hWatch, int channel);
IDIS_DLLFUNC const char* IDISAPI watch_getCameraTitleIDR(IDISHWATCH hWatch, int channel, int camera);

// watchSDK_set function ////////////////////////

IDIS_DLLFUNC bool IDISAPI watch_setColor(IDISHWATCH hWatch, int channel, int camera, int type, int value);
IDIS_DLLFUNC bool IDISAPI watch_setPtz(IDISHWATCH hWatch, int channel, LPBYTE lpCommand);
IDIS_DLLFUNC bool IDISAPI watch_setPtzPreset(IDISHWATCH hWatch, int channel, int camera, LPBYTE presets);
IDIS_DLLFUNC bool IDISAPI watch_setPtzPresetIDR(IDISHWATCH hWatch, int channel, int camera, LPBYTE presets);
IDIS_DLLFUNC bool IDISAPI watch_setAlarmOut(IDISHWATCH hWatch, int channel, int alarmid, bool fdevon);

// watchSDK_chk function ////////////////////////

IDIS_DLLFUNC int  IDISAPI watch_getCountAlarmOut(IDISHWATCH hWatch, int channel);
IDIS_DLLFUNC int  IDISAPI watch_checkAuthority(IDISHWATCH hWatch, int channel);
IDIS_DLLFUNC bool IDISAPI watch_isAuthorityCovertView(IDISHWATCH hWatch, int channel);
IDIS_DLLFUNC bool IDISAPI watch_isConnectedIDR(IDISHWATCH hWatch, int channel);
IDIS_DLLFUNC bool IDISAPI watch_isSupport(IDISHWATCH hWatch, int channel, int supportType);

// watchSDK_liveAudio function : WIN32 only//////////////////
#if defined(WIN32)
IDIS_DLLFUNC bool IDISAPI watch_startupAudio(int maxChannel, HWND hwnd);
IDIS_DLLFUNC void IDISAPI watch_cleanupAudio();
IDIS_DLLFUNC bool IDISAPI watch_decodeAudioFrame(IDISHWATCH hWatch, int channel, const LPPARAMWS_FRAMEINFO frameInfo, LPBYTE decoded, int bufferSize, int* decodedLen, LPPARAMW_AUDIOINFO audioInfo = NULL);
IDIS_DLLFUNC bool IDISAPI watch_playAudioFrame(IDISHWATCH hWatch, int channel, const LPPARAMWS_FRAMEINFO frameInfo);
IDIS_DLLFUNC bool IDISAPI watch_setAudioList(IDISHWATCH hWatch, int channel, const LPBYTE audios, int length);
IDIS_DLLFUNC bool IDISAPI watch_enableMic(IDISHWATCH hWatch, int channel, bool enable, int camera = -1); // ignore camera
IDIS_DLLFUNC bool IDISAPI watch_disableSpeaker(IDISHWATCH hWatch, int channel, int camera);
IDIS_DLLFUNC bool IDISAPI watch_sendCapturedAudio(IDISHWATCH hWatch, int channel, const LPPARAMW_AUDIOINFO capturedAudio);
IDIS_DLLFUNC bool IDISAPI watch_requestDeviceAudioOutOpen(IDISHWATCH hWatch, int channel, int deviceId);
IDIS_DLLFUNC bool IDISAPI watch_requestDeviceAudioOutClose(IDISHWATCH hWatch, int channel, int deviceId);
IDIS_DLLFUNC bool IDISAPI watch_sendWaveAudio(IDISHWATCH hWatch, int channel,const LPBYTE waveData, int waveLength);
#endif

// watchSDK_etc function ////////////////////////
IDIS_DLLFUNC bool IDISAPI watch_sendNetworkAlarmOn(IDISHWATCH hWatch, int channel, LPBYTE pNetAlarmInfo);
IDIS_DLLFUNC bool IDISAPI watch_sendNetworkAlarmOff(IDISHWATCH hWatch, int channel, LPBYTE pNetAlarmInfo);
IDIS_DLLFUNC bool IDISAPI watch_requestStreamChannelControl(IDISHWATCH hWatch, int channel, int camera, int seqNumber, int bufflength, char* buffer);
IDIS_DLLFUNC int  IDISAPI watch_getFrameDataOffset(IDISHWATCH hWatch, int channel, void* videoData);
IDIS_DLLFUNC void IDISAPI watch_setLoggerProperties(unsigned char logType, unsigned char logLevel);
IDIS_DLLFUNC int IDISAPI watch_getCountAvailableHandle(void);
IDIS_DLLFUNC int IDISAPI watch_getCountAvailableChannel(IDISHWATCH hWatch);

// User-specific data
IDIS_DLLFUNC bool  IDISAPI watch_setUserParamByHandle(IDISHWATCH hWatch, void* pUserParam);
IDIS_DLLFUNC bool  IDISAPI watch_setUserParamByChannel(IDISHWATCH hWatch, int channel, void* pUserParam);
IDIS_DLLFUNC void* IDISAPI watch_getUserParamByHandle(IDISHWATCH hWatch);
IDIS_DLLFUNC void* IDISAPI watch_getUserParamByChannel(IDISHWATCH hWatch, int channel);
// returns fail if 
//  : target buffer size mismatch
//      or unsupported query type.
IDIS_DLLFUNC bool IDISAPI watch_getProductInfo(IDISHWATCH hWatch, int channel, int queryType, void* target, unsigned int targetSize);


////////////////////////////////////////////////////////////////////////////////

typedef void (IDISAPI *pwatch_registerCallback)(IDISHWATCH hWatch, int ntypes, LPLISTENER_FUNC lpFunc);

//////////////////////////////////////////////////////////////////////////

typedef IDISHWATCH (IDISAPI *pwatch_initialize)(void);
typedef void (IDISAPI *pwatch_finalize)(IDISHWATCH hWatch);
typedef void (IDISAPI *pwatch_startup)(IDISHWATCH hWatch, int nMaxChannels);
typedef void (IDISAPI *pwatch_cleanup)(IDISHWATCH hWatch);

// watchSDK_connection //////////////////////////

typedef int  (IDISAPI *pwatch_connect)(IDISHWATCH hWatch, LPCTSTR siteName, LPCTSTR siteAddress, LPCTSTR userName, LPCTSTR userPassword, int port, bool isPasswordEncrypted /*= false*/, void* attachment /*= 0*/);
typedef bool (IDISAPI *pwatch_reconnect)(IDISHWATCH hWatch, LPCTSTR siteName, LPCTSTR siteAddress, LPCTSTR userName, LPCTSTR userPassword, int port, short channel, void* attachment /*= 0*/);
typedef int  (IDISAPI *pwatch_connect_dvrns)(IDISHWATCH hWatch, LPCTSTR siteName, LPCTSTR dvrnsAddress, LPCTSTR dvrName, LPCTSTR userName, LPCTSTR userPassword, int dvrnsPort, void* attachment /*= 0*/);
typedef bool (IDISAPI *pwatch_reconnect_dvrns)(IDISHWATCH hWatch, LPCTSTR siteName, LPCTSTR dvrnsAddress, LPCTSTR dvrName, LPCTSTR userName, LPCTSTR userPassword, int dvrnsPort, short channel, void* attachment /*= 0*/);
typedef void (IDISAPI *pwatch_disconnect)(IDISHWATCH hWatch, int channel);
typedef bool (IDISAPI *pwatch_isConnecting)(IDISHWATCH hWatch, int channel);
typedef bool (IDISAPI *pwatch_isConnected)(IDISHWATCH hWatch, int channel);
typedef bool (IDISAPI *pwatch_isDisconnecting)(IDISHWATCH hWatch, int channel);
typedef bool (IDISAPI *pwatch_isDisconnected)(IDISHWATCH hWatch, int channel);
typedef bool (IDISAPI *pwatch_isDisconnectable)(IDISHWATCH hWatch, int channel);
typedef bool (IDISAPI *pwatch_setCameraList)(IDISHWATCH hWatch, int channel, const LPBYTE cameras, int length);
typedef bool (IDISAPI *pwatch_setCameraList_v2)(IDISHWATCH hWatch, int channel, const LPBYTE cameras, int length);
typedef void (IDISAPI *pwatch_sendRequestCheck)(IDISHWATCH hWatch, int channel);

// connectService
typedef int (IDISAPI *pwatch_connect_connectService)(IDISHWATCH hWatch, LPCTSTR siteName, LPCTSTR dvrName, LPCTSTR gatewayAddress, int gatewayPort, int proxyPort, LPCTSTR userName, LPCTSTR userPassword, int natType, void* attachment);
// watchSDK_get function ////////////////////////

typedef bool (IDISAPI *pwatch_getUsePTZ)(IDISHWATCH hWatch, int channel, int camera);
typedef int  (IDISAPI *pwatch_getUsePTZEx)(IDISHWATCH hWatch, int channel, int camera);
typedef bool (IDISAPI *pwatch_needPtzAdvancedMenu)(IDISHWATCH hWatch, int channel, int camera);
typedef bool (IDISAPI *pwatch_needPtzPreset)(IDISHWATCH hWatch, int channel, int camera);
typedef bool (IDISAPI *pwatch_getPtzPreset)(IDISHWATCH hWatch, int channel, LPBYTE lpPreset);
typedef bool (IDISAPI *pwatch_getPtzPresetIDR)(IDISHWATCH hWatch, int channel, LPBYTE lpPreset);
typedef bool (IDISAPI *pwatch_getStatus)(IDISHWATCH hWatch, int channel, LPBYTE lpStatus);
typedef bool (IDISAPI *pwatch_sendReqStatusIDR)(IDISHWATCH hWatch, int channel);
typedef const char* (IDISAPI *pwatch_getCameraTitleIDR)(IDISHWATCH hWatch, int channel, int camera);

// watchSDK_set function ////////////////////////

typedef bool (IDISAPI *pwatch_setColor)(IDISHWATCH hWatch, int channel, int camera, int type, int value);
typedef bool (IDISAPI *pwatch_setPtz)(IDISHWATCH hWatch, int channel, LPBYTE lpCommand);
typedef bool (IDISAPI *pwatch_setPtzPreset)(IDISHWATCH hWatch, int channel, int camera, LPBYTE presets);
typedef bool (IDISAPI *pwatch_setPtzPresetIDR)(IDISHWATCH hWatch, int channel, int camera, LPBYTE presets);
typedef bool (IDISAPI *pwatch_setAlarmOut)(IDISHWATCH hWatch, int channel, int alarmid, bool fdevon);

// watchSDK_chk function ////////////////////////

typedef int  (IDISAPI *pwatch_getCountAlarmOut)(IDISHWATCH hWatch, int channel);
typedef int  (IDISAPI *pwatch_checkAuthority)(IDISHWATCH hWatch, int channel);
typedef bool (IDISAPI *pwatch_isAuthorityCovertView)(IDISHWATCH hWatch, int channel);
typedef bool (IDISAPI *pwatch_isConnectedIDR)(IDISHWATCH hWatch, int channel);
typedef bool (IDISAPI *pwatch_isSupport)(IDISHWATCH hWatch, int channel, int supportType);

// watchSDK_liveAudio function : WIN32 only//////////////////
#if defined(WIN32)
typedef bool (IDISAPI *pwatch_startupAudio)(int maxChannel, HWND hwnd);
typedef void (IDISAPI *pwatch_cleanupAudio)();
typedef bool (IDISAPI *pwatch_decodeAudioFrame)(IDISHWATCH hWatch, int channel, const LPPARAMWS_FRAMEINFO frameInfo, LPBYTE decoded, int bufferSize, int* decodedLen, LPPARAMW_AUDIOINFO audioInfo /*= 0*/);
typedef bool (IDISAPI *pwatch_playAudioFrame)(IDISHWATCH hWatch, int channel, const LPPARAMWS_FRAMEINFO frameInfo);
typedef bool (IDISAPI *pwatch_setAudioList)(IDISHWATCH hWatch, int channel, const LPBYTE audios, int length);
typedef bool (IDISAPI *pwatch_enableMic)(IDISHWATCH hWatch, int channel, bool enable, int camera/* = -1*/); // ignore camera
typedef bool (IDISAPI *pwatch_disableSpeaker)(IDISHWATCH hWatch, int channel, int camera);
typedef bool (IDISAPI *pwatch_sendCapturedAudio)(IDISHWATCH hWatch, int channel, const LPPARAMW_AUDIOINFO capturedAudio);
typedef bool (IDISAPI *pwatch_requestDeviceAudioOutOpen)(IDISHWATCH hWatch, int channel, int deviceId);
typedef bool (IDISAPI *pwatch_requestDeviceAudioOutClose)(IDISHWATCH hWatch, int channel, int deviceId);
typedef bool (IDISAPI *pwatch_sendWaveAudio)(IDISHWATCH hWatch, int channel,const LPBYTE waveData, int waveLength);
#endif

// watchSDK_etc function ////////////////////////
typedef bool (IDISAPI *pwatch_sendNetworkAlarmOn)(IDISHWATCH hWatch, int channel, LPBYTE pNetAlarmInfo);
typedef bool (IDISAPI *pwatch_sendNetworkAlarmOff)(IDISHWATCH hWatch, int channel, LPBYTE pNetAlarmInfo);
typedef bool (IDISAPI *pwatch_requestStreamChannelControl)(IDISHWATCH hWatch, int channel, int camera, int seqNumber, int bufflength, char* buffer);
typedef int  (IDISAPI *pwatch_getFrameDataOffset)(IDISHWATCH hWatch, int channel, void* videoData);
typedef void (IDISAPI *pwatch_setLoggerProperties)(unsigned char logType, unsigned char logLevel);
typedef int  (IDISAPI *pwatch_getCountAvailableHandle)(void);
typedef int  (IDISAPI *pwatch_getCountAvailableChannel)(IDISHWATCH hWatch);

// User-specific data
typedef bool  (IDISAPI *pwatch_setUserParamByHandle)(IDISHWATCH hWatch, void* pUserParam);
typedef bool  (IDISAPI *pwatch_setUserParamByChannel)(IDISHWATCH hWatch, int channel, void* pUserParam);
typedef void* (IDISAPI *pwatch_getUserParamByHandle)(IDISHWATCH hWatch);
typedef void* (IDISAPI *pwatch_getUserParamByChannel)(IDISHWATCH hWatch, int channel);
// returns fail if 
//  : target buffer size mismatch
//      or unsupported query type.
typedef bool (IDISAPI *pwatch_getProductInfo)(IDISHWATCH hWatch, int channel, int queryType, void* target, unsigned int targetSize);


#ifdef __cplusplus
}
#endif

//////////////////////////////////////////////////////////////////////////

#endif	// !_IDIS_WATCHSDK_INTERFACE_
