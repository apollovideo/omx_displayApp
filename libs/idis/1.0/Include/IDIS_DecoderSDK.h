// IDIS_Decoder.h : header file
//

#ifndef _IDIS_DECODER_INTERFACE_H
#define _IDIS_DECODER_INTERFACE_H

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif //_MSC_VER

////////////////////////////////////////////////////////////////////////////////

#include "IDIS_Define.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push, 1)

typedef struct {
	DWORD   _dwSize;				// this struct size
	LPBYTE  _lpCompData;			// compressed image data
	UINT32  _lpCompSize;			// compressed image buffer size(bytes)
	UINT16  _camId;					// camera unique index for decompress
	UINT16  _imgFormat;				// video_format(RGB, YUV)
	LPBYTE  _lpDecompData;			// image buffer for save decompressed data
	UINT16  _imgWidth;				// image width
	UINT16  _imgHeight;				// image height
	BYTE    _decoderType;
	BYTE	_byFrameType;
	BYTE    _byField;
	BYTE    _byVolHeader;
	BYTE	_reserved[4];
} PARAMD_DECODERS, *LPPARAMD_DECODERS;

typedef struct {
    enum Const {
        NUM_DATA_POINTERS = 4
    };
    LPBYTE  data[NUM_DATA_POINTERS];
    UINT32  linesize[NUM_DATA_POINTERS];
} PARAMD_PICTURE, *LPPARAMD_PICTURE;

typedef struct {
    DWORD   _dwSize;                // this struct size
    LPBYTE  _lpCompData;            // compressed image data
    UINT32  _lpCompSize;            // compressed image buffer size(bytes)
    UINT16  _camId;                 // camera unique index for decompress
    LPBYTE  _lpDecompData;          // image buffer for save decompressed data
    UINT16  _imgWidth;              // image width
    UINT16  _imgHeight;             // image height
    BYTE    _decoderType;
    BYTE    _byFrameType;
    BYTE    _byField;
    BYTE    _byVolHeader;
    BYTE    _threadsCount;          // thread count of decoder
    BYTE    _useHW;                 // use DXVA or not
    BYTE    _hwDeviceId;            // display apapter ID (D3DADAPTER_DEFAULT = 0)
} PARAMD_DECODERS_V2, *LPPARAMD_DECODERS_V2;

typedef struct {
    enum SDK_RESULT_TYPE_V2 {
        UNKNOWN = -1,
        DECODING_FAILED = 0,
        SUCCESS = 1,
        DECODER_NOT_FOUND,
        DECODER_NOT_READY,
        DECODER_RESETED,
        INVALID_ARGUMENT,
        INVALID_FRAME,
        DISCARD_FRAME,
        SKIPPED_FRAME,
        IS_NOT_IFRAME,
        BROKEN_FRAME,
        REPEAT_FRAME,
        NOT_ENOUGH_DATA,
        NOT_ENOUGH_BUF,
        OUT_OF_MEMORY,
        OUT_OF_DECODER,
        NEEDS_REINIT,
        ERR_DIVA_SESSION_POOL,
        ERR_MEMORY_ALLOC,
        ERR_UNSUPPORTED,
        ERR_UNSUPPORTED_RES,
        ERR_SCALER
    };
    PARAMD_PICTURE _outPicture;
    INT _resWidth;
    INT _resHeight;
    INT _result;
    INT _colorSpace;
    INT _colorRange;
    INT _threads;
} PARAMD_DECODERS_RESULT, *LPPARAMD_DECODERS_RESULT;

typedef struct {
    DWORD  _dwSize;                // this struct size
    LPBYTE _lpCompData;            // compressed audio data
    UINT32 _lpCompSize;            // compressed audio buffer size(bytes)
    LPBYTE _lpDecompData;          // audio buffer for save decompressed data
    UINT32 _lpDecompSize;          // decompressed size [out]
    BYTE   _decoderType;           // decoder
    BYTE   _byFrameType;           // frame type (must be IDIF_FRAMETYPE::AUDIO_FRAME)
    BYTE   _bytePerSample;         // byte per sample [out]
} PARAMD_AUDIO_DECODERS, *LPPARAMD_AUDIO_DECODERS;

typedef struct {
    DWORD  _dwSize;                // this struct size
    LPBYTE _lpCompData;            // compressed audio data
    UINT32 _lpCompSize;            // compressed audio buffer size(bytes)
    LPBYTE _lpDecompData;          // audio buffer for save decompressed data
    UINT32 _lpDecompSize;          // decompressed size [out]
    BYTE   _decoderType;           // decoder
    BYTE   _byFrameType;           // frame type (must be IDIF_FRAMETYPE::AUDIO_FRAME)
    BYTE   _bytePerSample;         // byte per sample [out]
    UINT32 _sampling;              // sample rate
} PARAMD_AUDIO_DECODERS_V2, *LPPARAMD_AUDIO_DECODERS_V2;

struct SDK_COLOR_SPACE {
    enum TYPE {
        RGB         = 0,
        BT709       = 1,
        UNSPECIFIED = 2,
        RESERVED    = 3,
        FCC         = 4,
        BT470BG     = 5,
        SMPTE170M   = 6,
        SMPTE240M   = 7,
        YCOCG       = 8,
        BT2020_NCL  = 9,
        BT2020_CL   = 10
    };
};

struct SDK_COLOR_RANGE {
    enum TYPE {
        UNSPECIFIED = 0,
        MPEG        = 1,
        FULL        = 2
    };
};

typedef struct {
    enum TYPE {
        BILINEAR_FAST = 1,
        BILINEAR      = 2,
        BICUBIC	      = 4,
        POINT	      = 0x10,
        AREA	      = 0x20,
        BICUBLIN      = 0x40,
        GAUSS	      = 0x80,
        SINC		  = 0x100,
        LANCZOS	      = 0x200,
        SPLINE		  = 0x400
    };
} SDK_FILTER_TYPE;

typedef struct {
    enum {
        IDIS_YUV420 = 0,
        IDIS_YUY2,
        IDIS_UYVY,
        IDIS_RGB32,
        IDIS_RGB24,
        IDIS_RGB565,
        IDIS_CLRFMT_MAX_DUMMY		// for sentinel
    };
} IDIS_COLORFORMAT;

typedef struct {
    enum {
        IDIS_DEINTERLACE_NONE = 0,
        IDIS_DEINTERLACE_SOFTEN,
        IDIS_DEINTERLACE_SHARPEN,
        IDIS_DEINTERLACE_EDGEDETECT,
        IDIS_DEINTERLACE_MAX_DUMMY	// for sentinel
    };
} IDIS_DEINTERLACE_FILTER;

#define DECODEMODE_SEARCH   0
#define DECODEMODE_WATCH    1

////////////////////////////////////////////////////////////////////////////////

IDIS_DLLFUNC IDISHDECODER IDISAPI decoder_initialize(int maxCamera);
IDIS_DLLFUNC void IDISAPI decoder_finalize(IDISHDECODER hDecoder);
IDIS_DLLFUNC IDISHDECODER IDISAPI decoder_initialize_v2();
IDIS_DLLFUNC bool IDISAPI decoder_startup(IDISHDECODER hdecoder, int cameraCount);
IDIS_DLLFUNC void IDISAPI decoder_cleanup(IDISHDECODER hDecoder);
IDIS_DLLFUNC bool IDISAPI decoder_doDecompress(IDISHDECODER hDecoder, LPBYTE lpvdParam, UINT decodeMode);
IDIS_DLLFUNC bool IDISAPI decoder_doDecompressAudio(IDISHDECODER hDecoder, LPBYTE lpDecParam);
IDIS_DLLFUNC bool IDISAPI decoder_doDecompressAudio_v2(IDISHDECODER hDecoder, LPBYTE lpDecParam);
IDIS_DLLFUNC void IDISAPI decoder_setdeinterlacefilter(IDISHDECODER hDecoder, int typeofFilter);
IDIS_DLLFUNC bool IDISAPI decoder_doDecompress_v2(IDISHDECODER hDecoder, PARAMD_DECODERS_V2* lpvdParam, PARAMD_DECODERS_RESULT* lpResultParam);
IDIS_DLLFUNC bool IDISAPI decoder_pictureScale(PARAMD_PICTURE* dstPicture, int dstPixFormat, int dstCx, int dstCy, unsigned char* buf, int filter,
									      const PARAMD_PICTURE* srcPicture, int srcPixFormat, int srcCx, int srcCy, int colorSpace, int colorRange);
IDIS_DLLFUNC int IDISAPI  decoder_pictureBufLen(int width, int height, int pixFormat);

#if defined (WIN32)
IDIS_DLLFUNC void IDISAPI decoder_replaceLoaderFns(SDK_FN_GET_PROC_ADDR procGetProcAddr, SDK_FN_GET_MODULE_HANDLE procGetModuleHandle, SDK_FN_LOAD_LIBRARY procLoadLibrary, SDK_FN_FREE_LIBRARY procFreeLibrary);
#endif

typedef IDISHDECODER (IDISAPI *pdecoder_initialize)(int max_camera);
typedef void (IDISAPI *pdecoder_finalize)(IDISHDECODER hDecoder);
typedef IDISHDECODER (IDISAPI *pdecoder_initialize_v2)();
typedef bool (IDISAPI *pdecoder_startup)(IDISHDECODER hdecoder, int maxCamera);
typedef void (IDISAPI *pdecoder_cleanup)(IDISHDECODER hdecoder);
typedef bool (IDISAPI *pdecoder_doDecompress)(IDISHDECODER hDecoder, LPBYTE lpvdParam, UINT decodeMode);
typedef bool (IDISAPI *pdecoder_doDecompressAudio)(IDISHDECODER hDecoder, LPBYTE lpDecParam);
typedef bool (IDISAPI *pdecoder_doDecompressAudio_v2)(IDISHDECODER hDecoder, LPBYTE lpDecParam);
typedef void (IDISAPI *pdecoder_setdeinterlacefilter)(IDISHDECODER hDecoder, int typeofFilter);
typedef bool (IDISAPI *pdecoder_doDecompress_v2)(IDISHDECODER hDecoder, PARAMD_DECODERS_V2* lpvdParam, PARAMD_DECODERS_RESULT* lpResultParam);
typedef bool (IDISAPI *pdecoder_pictureScale)(PARAMD_PICTURE* dstPicture, int dstPixFormat, int dstCx, int dstCy, unsigned char* buf, int filter,
								        const PARAMD_PICTURE* srcPicture, int srcPixFormat, int srcCx, int srcCy, int colorSpace, int colorRange);
typedef int  (IDISAPI *pdecoder_pictureBufLen)(int width, int height, int pixFormat);
#if defined (WIN32)
typedef void (IDISAPI *pdecoder_replaceLoaderFns)(SDK_FN_GET_PROC_ADDR procGetProcAddr, SDK_FN_GET_MODULE_HANDLE procGetModuleHandle, SDK_FN_LOAD_LIBRARY procLoadLibrary, SDK_FN_FREE_LIBRARY procFreeLibrary);
#endif
////////////////////////////////////////////////////////////////////////////////

#pragma pack(pop)

#ifdef __cplusplus
}
#endif

////////////////////////////////////////////////////////////////////////////////

#endif	// !_IDIS_DECODER_INTERFACE_H

// <EOF>
