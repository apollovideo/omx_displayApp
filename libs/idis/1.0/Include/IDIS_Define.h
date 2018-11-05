// IDIS_Define.h : header file
//

#ifndef _IDIS_DEFINE_H_
#define _IDIS_DEFINE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef __cplusplus
#define EXTERN_C_BEGIN  extern "C" {
#define EXTERN_C_END    }
#else
#define EXTERN_C_BEGIN
#define EXTERN_C_END
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__linux__) || defined(__APPLE__) || defined(ANDROID)
    #ifdef SDK_BUILDER
        #include <Win32Porting/Win32Porting.h>
    #else
        #include "Win32Porting.h"
    #endif
#elif defined (__MINGW32__)
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    #include <time.h>
#endif

//////////////////////////////////////////////////////////////////////////
#if defined(_WIN32)
    #ifdef _IDIS_STATIC_BUILD
        #define IDIS_DLLFUNC
    #else
        #ifdef _IDIS_DLL_EXPORT
            #define IDIS_DLLFUNC __declspec(dllexport)
        #else
            #define IDIS_DLLFUNC __declspec(dllimport)
        #endif
    #endif
#elif defined(__GNUC__)
    #ifdef _IDIS_DLL_EXPORT
        #define IDIS_DLLFUNC __attribute__ ((visibility ("default")))
    #else
        #define IDIS_DLLFUNC __attribute__ ((visibility ("default")))
    #endif
#else
    #define IDIS_DLLFUNC
#endif

#ifndef IDISAPI
#define IDISAPI      __stdcall
#endif

#ifndef IDISCALLBACK
#define IDISCALLBACK __stdcall
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(LPFAR)	     { if (LPFAR) { delete (LPFAR);     (LPFAR) = NULL; } }
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(LPFAR) { if (LPFAR) { delete[] (LPFAR);   (LPFAR) = NULL; } }
#endif

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(LPFAR)      { if (LPFAR) { (LPFAR)->Release(); (LPFAR) = NULL; } }
#endif

#if defined (__MINGW32__)
    typedef unsigned char       UINT8, *PUINT8;
    typedef unsigned short      UINT16, *PUINT16;
#endif

typedef LONG         IDISHANDLE;
typedef LONG		 IDISHADMIN;
typedef LONG         IDISHWATCH;
typedef LONG         IDISHSEARCH;
typedef LONG         IDISHUPGRADE;
typedef LONG         IDISHDECODER;
typedef LONG         IDISHAUDIO;
typedef LONG         IDISHSTORAGE;

#ifndef IDISHNULL
#define IDISHNULL    -1L
#endif

#ifndef IDISTRUE
#define IDISTRUE     1
#endif

#ifndef IDISFALSE
#define IDISFALSE    0
#endif

typedef UINT_PTR     IDISWPARAM;
typedef LONG_PTR     IDISLPARAM;
typedef LONG_PTR     IDISRESULT;

#if defined(_WIN32)
#define SDK_TIME_T __time32_t
#else
#define SDK_TIME_T int
#endif
typedef void(IDISCALLBACK *LPLISTENER_FUNC)(IDISHANDLE hModule, IDISWPARAM wParam, IDISLPARAM lParam);

typedef struct _taglistener {
	ULONG           _nTypes;
	LPLISTENER_FUNC _lpFunc;
} LISTENERINFO, *LPLISTENERINFO;

//////////////////////////////////////////////////////////////////////////

typedef struct {
    enum _decoder {
        MLJPEG = 0,
        MPEG4,
        MJPEG,
        H264,
        BITMAP,
        MXPEG,
        HEVC,
        DEC_COUNT
    };
} IDIS_DECODERTYPE;

    typedef struct {
        enum _frame {
            I_FRAME = 0,
            P_FRAME,
            AUDIO_FRAME,
            X_FRAME,
            B_FRAME,
            X2_FRAME,
        };
        enum _max_size {
            MAX_SIZE_CX = 4096,
            MAX_SIZE_CY = 2592
        };
    } IDIS_FRAMETYPE;

typedef struct {
    enum _audio {
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
} IDIS_AUDIOTYPE;

typedef struct {
    enum _event_type {
        EVENT_UNKNOWN = -1,
        EVENT_DISK_ALMOST_FULL_ON = 0,
        EVENT_ALARM_IN = 1,
        EVENT_TEXT_IN = 2,
        EVENT_MOTION = 3,
        EVENT_VLOSS = 4,
        EVENT_ALARM_RESET_IN = 5,
        EVENT_SENSORMANUAL = 6,
        EVENT_RECORDING = 7,
        EVENT_DISK_FULL = 8,
        EVENT_RECORDER_BAD = 9,
        EVENT_ALARM_IN_BAD = 10,
        EVENT_DISK_BAD = 11,
        EVENT_DISK_TEMPERATURE = 12,
        EVENT_DISK_SMART = 13,
        EVENT_SYSTEM_ALIVE = 14,
        EVENT_PANIC_RECORD_ON = 15,
        EVENT_SIPASS_REC_ON = 16,
        EVENT_SIPASS_REC_OFF = 17,
        EVENT_OBJECT_TRACKER_ON = 18,
        EVENT_VIDEO_BLIND_ON = 19,
        EVENT_PANIC_RECORD_OFF = 20,
        EVENT_ALARM_IN_OFF = 21,
        EVENT_RECORD_STOP = 22,
        EVENT_FAN_ERROR_ON = 23,
        EVENT_FAN_ERROR_OFF = 24,
        EVENT_VIDEOON = 28,
        EVENT_MOTION_OFF = 29,
        EVENT_VIDEO_BLIND_OFF = 30,
        EVENT_NETWORK_ALARM_ON = 34,
        EVENT_NETWORK_ALARM_OFF = 35,
        EVENT_DISK_CONFIG_CHANGE = 36,
        EVENT_SYSTEM_BOOT_UP = 37,
        EVENT_SYSTEM_RESTART = 38,
        EVENT_SYSTEM_SHUTDOWN = 39,
        EVENT_STORAGE_ON = 40,
        EVENT_STORAGE_OFF = 41,
        EVENT_VIDEO_ANALYTICS = 42,
        EVENT_VIDEO_ANALYTICS_OFF = 43,
        EVENT_IGNORED_MOTION_ON  = 44,
        EVENT_COVER_OPEN = 45,
        EVENT_COVER_CLOSE = 46,
        EVENT_ALARM_IN_BAD_OFF = 47,
        EVENT_OBJECT_TRACKER_OFF  = 48,
        EVENT_VIDEO_INIT = 49,
        EVENT_TEXT_IN_OFF = 50,
        EVENT_RECORDER_BAD_OFF = 51,
        EVENT_DISK_FULL_OFF = 52,
        EVENT_DISK_ALMOST_FULL_OFF = 53,
        EVENT_DISK_TEMPERATURE_OFF = 54,
        EVENT_DISK_SMART_OFF     = 55,
        EVENT_CAR_OVERSPEED_ON = 56,
        EVENT_CAR_OVERSPEED_OFF = 57,
        EVENT_CAR_SUDDEN_ACCELERATION = 58,
        EVENT_CAR_SUDDEN_STOP = 59,
        EVENT_CAR_STARTING_WITH_DOORS_OPEN = 60,
        EVENT_AUDIO_ON = 61,
        EVENT_AUDIO_OFF = 62,
        EVENT_IGNORED_AUDIO_ON = 63,
        EVENT_TRIPZONE_ON = 64,
        EVENT_TRIPZONE_OFF = 65,
        EVENT_IGNORED_TRIPZONE_ON = 66,
        EVENT_TAMPER_ON = 67,
        EVENT_TAMPER_OFF = 68,
        EVENT_IGNORED_VIDEO_ANALYTICS_ON = 69,
        EVENT_NO_DISK = 70,
        EVENT_TEXT_IN_BAD_ON = 71,
        EVENT_TEXT_IN_BAD_OFF = 72,
        EVENT_USER_DEFINED_ALARM_ON = 73,
        EVENT_USER_DEFINED_ALARM_OFF = 74,
        EVENT_GPS_RECEIVE_ERROR_ON     = 75,
        EVENT_GPS_RECEIVE_ERROR_OFF    = 76,
        EVENT_LOGIN_FAILED_SEVERAL_TIMES = 77,
        EVENT_USER_LOGIN = 78,
        EVENT_USER_LOGOUT = 79,
        EVENT_SETUP_CHANGED = 80,
        EVENT_SECOM_FS_MACHINE = 81,
        EVENT_FACE_DETECTION_ON = 82,
        EVENT_IGNORED_FACE_DETECTION_ON = 83,
        EVENT_FACE_DETECTION_OFF = 84,
        EVENT_CAMERA_RECORD_BAD_ON = 85,
        EVENT_CAMERA_RECORD_BAD_OFF = 86,
        EVENT_CAMERA_FAN_ERROR_ON = 87,
        EVENT_CAMERA_FAN_ERROR_OFF = 88,
        EVENT_TYPE_PT_STEP_OUT                          = 89,
        EVENT_TYPE_ONLINE_UPGRADE_FOUND                 = 90,
        EVENT_TYPE_ONLINE_UPGRADE_SUCCEEDED             = 91,
        EVENT_TYPE_ONLINE_UPGRADE_FAILED                = 92,
        EVENT_VA_PEOPLE_COUNT_SETUP_CHANGED = 93,
        EVENT_VA_PEOPLE_COUNT_IN = 94,
        EVENT_VA_PEOPLE_COUNT_OUT = 95,
        EVENT_VA_QUEUE_MAX_OBJECT_EXCEED_ON = 96,
        EVENT_VA_QUEUE_MAX_OBJECT_EXCEED_OFF = 97,
        EVENT_VA_QUEUE_MAX_WAIT_TIME_EXCEED_ON = 98,
        EVENT_VA_QUEUE_MAX_WAIT_TIME_EXCEED_OFF = 99,
        EVENT_RAID_BROKEN_ON = 104,
        EVENT_RAID_BROKEN_OFF = 105,
        EVENT_TANGO_PARTIALLY_FULL_ON = 106,
        EVENT_TANGO_PARTIALLY_FULL_OFF = 107,
        EVENT_TANGO_PARTIALLY_ALMOST_FULL_ON = 108,
        EVENT_TANGO_PARTIALLY_ALMOST_FULL_OFF = 109,
        EVENT_REDUNDANT_POWER_FAILURE_ON = 110,
        EVENT_REDUNDANT_POWER_FAILURE_OFF = 111,
    };
} IDIS_EVENT_TYPES;

typedef struct {
	enum _authority {
		AUTHORITY_COLOR_CONTROL      = 0x00000040,
		AUTHORITY_PTZ_CONTROL        = 0x00000080,
		AUTHORITY_ALARM_OUT_CONTROL  = 0x00000100,
		AUTHORITY_COVERT_CAMERA_VIEW = 0x00000200,
		AUTHORITY_SEARCH             = 0x00001000,
		AUTHORITY_ALL                = 0x3FFFFFFF,
		MAX_AUTHORITY_COUNT          = 30
	};
} IDIS_USERAUTHORITY;

typedef struct {
    enum _disk_type {
        DISK_UNKNOWN,
        IDE_HDD,
        SCSI_HDD,
        USB_HDD,
        IDE_CDRW,
        IDE_DVDRW,
        SW_RAID_DISK,
        FLASH_MEMORY,
        ESATA_HDD,
        ISCSI_HDD,
    };
} DISK_TYPE;

typedef struct {
	enum _because {
		UNKNOWN          =  0,    // unknown case
		LOGOUT           =  1,    // normally logout (base->post)
		FULL_CHANNEL     =  2,    // deny connection because all of server channels are used (base<-post)
		INVALID_VERSION  =  3,    // invalid product version (base->post)
		LOGIN_FAIL       =  4,    // invalid user or password (base<-post)
		ADMIN_CLOSE      =  5,    // admin close the current connection forcibly (base<-post)
		ADMIN_TIMEOUT    =  6,    // timeout (base<-post)
		SYS_SHUTDOWN     =  7,    // post system shutdown (base<-post)
		NO_CHANNEL       =  8,    // cann't connect - all of my network channels are used
		NO_SERVER        = 10,    // cann't connect - no server module (sock. err=10061)
		NET_DOWN         = 11,    // network is down (sock. err=10050)
		NET_UNREACHABLE  = 12,    // network is unreachable (sock. err=10051)
		CONN_TIMEOUT     = 13,    // connection time out (sock. err=10060)
		CONN_RESET       = 14,    // connection reset by peer (sock. err=10054)
		HOST_DOWN        = 15,    // host is down (sock. err=10064)
		HOST_UNREACHABLE = 16,    // no route th host (sock. err=10065)
		CONN_ABORTED     = 17,    // connection aborted (sock. err=10053)
		CONN_CANCEL      = 20,    // connection has been canceled by user.
		NET_NORESPONSE   = 21,    // the peer host does not respond.
		NET_NOISY        = 22,    // network is too noisy.
		SEND_OVERFLOW    = 23,    // sending queue overflow.
        INVALID_OEM      = 24,    // invalid oem number (base->post)
        NO_AUTHORITY     = 25,    // You have no authority for search.
        PORT_USED        = 26,       // the port is alreay in use.
        SSL_CONNECTION_FAILED   = 27,       // SSL connection failed.
        NET_TIMEOUT      = 28,       // network is timed out
        HOST_TIMEOUT     = 29,       // host is timed out
        NOT_SUPPORT_RTP_TCP = 30     // host cannot support RTP over TCP
	};
} IDIS_WHYDISCONNECTED;

typedef struct {
    enum _because_dvrns {
        DVRNS_NETWORK_ERROR           = -101,   // network error
        DVRNS_CONNECTION_FAILED       = -102,   // can't connect to dvrns server
        DVRNS_PROTOCOL_ERROR          = -103,   // invalid protocol
        DVRNS_INVALID_PACKET          = -104,   // invalid packet
        DVRNS_INVALID_PACKETS          = -105,  // invalid packet version
        DVRNS_NO_DVR                   = -301,  // no such dvr
        DVRNS_SERVER_ERROR            = -302,   // server failed to search dvr
    };
} IDIS_DVRNS_WHYDISCONNECTED;

typedef struct {
    enum {
        LOG_NOT_USE = 0,
        LOG_LEVEL_1,
        LOG_LEVEL_2,
        LOG_LEVEL_3,
    };
} IDIS_API_LOG_LEVEL;

typedef struct {
    enum {
        DAILY_CHANGE,
        HOURLY_CHANGE,
    };
} IDIS_API_LOG_TYPE;

typedef struct {
    enum Id {
        Eniwetok_Kwajalein = 0,
        MidwayIsland_Samoa,
        Hawaii,
        Alaska,
        PacificTime_Tijuana,
        MountainTime,
        Arizona,
        Chihuahua_Mazatlan,
        MexicoCity_Tegucigalpa,
        Saskatchewan,
        CentralTime,
        CentralAmerica,
        EasternTime,
        Bogota_Lima_Quito,
        Indiana,
        AtlanticTime,
        Santiago,
        Caracas_LaPaz,
        Newfoundland,
        Greenland,
        BuenosAires_Georgetown,
        Brasilia,
        MidAtlantic,
        CapeVerdeIs,
        Azores,
        Greenwich_Dublin_Edinburgh_Lisbon_London,
        Casablanca_Monrovia,
        Belgrade_Bratislava_Budapest_Ljubljana_Prague,
        Brussels_Copenhagen_Madrid_Paris_Vilnius,
        Sarajevo_Skopje_Sofija_Warsaw_Zagreb,
        WestCentralAfrica,
        Amsterdam_Berlin_Bern_Rome_Stockholm_Vienna,
        Bucharest,
        Athens_Istanbul_Minsk,
        Jerusalem,
        Cairo,
        Harare_Pretoria,
        Helsinki_Riga_Tallinn,
        Nairobi,
        Moscow_StPetersburg_Volgograd,
        Baghdad_Kuwait_Riyadh,
        Kuwait_Riyadh,
        Tehran,
        Baku_Tbilisi,
        AbuDhabi_Muscat,
        Kabul,
        Ekaterinburg,
        Islamabad_Karachi_Tashkent,
        Bombay_Calcutta_Madras_NewDelhi,
        Kathmandu,
        SriJayawardenepura,
        Astana_Almaty_Dhaka,
        Almaty_Novosiirsk,
        Rangoon,
        Bangkok_Hanoi_Jakarta,
        Krasnoyarsk,
        Beijing_Chongqing_HongKong_Urumqi,
        Irkutsk_UlaanBataar,
        Singapore,
        Taipei,
        Perth,
        Seoul,
        Yakutsk,
        Osaka_Sapporo_Tokyo,
        Darwin,
        Adelaide,
        Guam_PortMoresby,
        Brisbane,
        Vladivostok,
        Canberra_Melbourne_Sydney,
        Hobart,
        Magadan_SolomonIs_NewCaledonia,
        Auckland_Wellington,
        Fiji_Kamchatka_MarshallIs,
        Nukualofa,
        PacificTime,

        MAX_TIME_ZONE_COUNT
    };
    int id;
    int dst;
    unsigned int time;
} DATE_TIME;

//////////////////////////////////////////////////////////////////////////

const int MAX_LEN_LOG_LABEL             = 32;
const int MAX_SYS_CAMERA   = 16;
const int MAX_SYS_CAMERA32 = 32;
const int MAX_SYS_DEVICE   = 70;
const int MAX_SYS_AUDIO    = 16;
const int MAX_FRAME_LENGTH = (IDIS_FRAMETYPE::MAX_SIZE_CX *
							 (IDIS_FRAMETYPE::MAX_SIZE_CY + (IDIS_FRAMETYPE::MAX_SIZE_CY >> 1)));
const int MAX_TITLE_LENGTH = 31;
const int MAX_PASSWD_ENC   = 64;

//////////////////////////////////////////////////////////////////////////
#if _MSC_VER > 1000
#pragma pack(push, 1)
#endif

typedef struct
#if defined(__linux__) || defined(__APPLE__) || defined(ANDROID)
__attribute__ ((packed))
#endif
{
	DWORD	_dwSize;         // this struct size
	LPBYTE	_lpBuffer;       // compressed image data
	UINT32	_szBuffer;       // image data size(byte)
	UINT16	_cxImage;        // image resolution width
	UINT16	_cyImage;        // image resolution height
	UINT8   _frameType;      // i,p, or x frame / audio frame
	UINT8	_decoder;        // a kind of decoder
	UINT8	_hostcamera;     // dvr_hostcamera number
	UINT32	_tmTime;         // time of image
	UINT32	_tmTick;         // tick of image
	BYTE	_isValid;        // whether valid frame or not
	BYTE	_fDisplay;       // whether display frame or not
	BYTE	_byField;        // don't consider
	BYTE	_fvolHeader;     // don't consider
    UINT32  _segmentId;      // segment ID
    BYTE    _title[MAX_TITLE_LENGTH + 1]; // camera title
    UINT32  _reserved[8];    // reserved
} PARAMWS_FRAMEINFO, *LPPARAMWS_FRAMEINFO;

typedef struct
#if defined(__linux__) || defined(__APPLE__) || defined(ANDROID)
__attribute__ ((packed))
#endif
{
    DWORD   _reason;
    void*   _attachment;
} PARAM_DISCONNECT, *LPPARAM_DISCONNECT;

typedef struct {
#if defined(__linux__) || defined(__APPLE__) || defined(ANDROID)
    __attribute__ ((packed))
#endif
    SDK_TIME_T _time;
    unsigned int _segmentId;    // identifier for duplicated record range.
    unsigned int _tick;
} PARAMS_SPOT, *LPPARAMS_SPOT;

// for product capability & information
const int QUERY_CAMERA_COUNT                    = 1;    // unsigned char
const int QUERY_ALARMIN_COUNT                   = 2;    // unsigned char
const int QUERY_ALARMOUT_COUNT                  = 3;    // unsigned char
const int QUERY_TEXTIN_COUNT                    = 4;    // unsigned char
const int QUERY_AUDIO_IN_COUNT                  = 5;    // unsigned char
const int QUERY_AUDIO_OUT_COUNT                 = 6;    // unsigned char
const int QUERY_SUPPORT_EVENT_LOG               = 7;    // bool
const int QUERY_SUPPORT_PARTIAL_SETUP           = 8;    // bool
const int QUERY_MAC_ADDRESS                     = 21;    // 18 bytes including NULL char
const int QUERY_SW_BUILD                        = 22;    // at least 8 bytes including NULL char
const int QUERY_SW_VERSION                      = 23;   // at least 13 bytes including NULL char
const int QUERY_ADMIN_CONNECTABLE               = 24;   // bool
const int QUERY_WATCH_CONNECTABLE               = 25;   // bool
const int QUERY_SEARCH_CONNECTABLE              = 26;   // bool
const int QUERY_AUDIO_CONNECTABLE               = 27;   // bool
const int QUERY_ADMIN_PORT                      = 28;   // unsigned short
const int QUERY_WATCH_PORT                      = 29;   // unsigned short
const int QUERY_SEARCH_PORT                     = 30;   // unsigned short
const int QUERY_AUDIO_PORT                      = 31;   // unsigned short
const int QUERY_PRODUCT_ID                      = 32;   // unsigned int
const int QUERY_SUPPORT_EVENT_LOG_FILTER        = 33;   // unsigned char
const int QUERY_NAME                            = 34;   // at least 16 bytes including NULL char
const int QUERY_SEARCH_TICK_TYPE                = 35;   // unsigned char
const int QUERY_WATCH_AUDIO                     = 36;   // bool
const int QUERY_NETWORK_CAMERA_COUNT            = 37;   // unsigned char

const unsigned char EVENT_LOG_FILTER_NOT_SUPPORT                  = 0;    // does not support log filtering
const unsigned char EVENT_LOG_FILTER_SUPPORT_EVENT_LOG_FILTER     = 1;    // support event log filtering

// tick type of DVRs : for implementing regular-speed playback
const BYTE TICK_TYPE_TICK          = 0;    ///< tick base
const BYTE TICK_TYPE_SYSTEM_MSEC   = 1;    ///< system milisec base
const BYTE TICK_TYPE_LEGACY_MSEC   = 2;    ///< legacy milisec base

#if defined(_WIN32)
    typedef FARPROC (*SDK_FN_GET_PROC_ADDR) (HMODULE, LPCSTR);
    typedef HMODULE (*SDK_FN_GET_MODULE_HANDLE) (LPCTSTR);
    typedef HMODULE (*SDK_FN_LOAD_LIBRARY) (LPCTSTR);
    typedef BOOL    (*SDK_FN_FREE_LIBRARY) (HMODULE);
#endif

#if _MSC_VER > 1000
#pragma pack(pop)
#endif
//////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif

//////////////////////////////////////////////////////////////////////////

#endif	// !_IDIS_DEFINE_H_
