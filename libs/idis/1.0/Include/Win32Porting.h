#if defined(__linux__) || defined(__APPLE__) || defined(ANDROID)

#ifndef _WIN32_PORTING_H
#define  _WIN32_PORTING_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include <wchar.h>
#include <stdint.h>
#include <assert.h>

#if !defined(UNICODE) && !defined(_UNICODE)
#ifndef _NOT_UNICODE
#define UNICODE
#endif
#endif

/******************************************
 * MACRO
 ******************************************/

#define  MAX_PATH FILENAME_MAX
#define _MAX_PATH FILENAME_MAX

// DEBUG
#if defined(_DEBUG)
    #define TRACE _tprintf
#else
    #define TRACE(...)
#endif

#define ASSERT(a)   assert(a)
#define VERIFY(f)   ((void)(f))

// BOOL
#if defined(__APPLE__) && defined(__IPHONE_OS_VERSION_MIN_REQUIRED)
    #ifndef TYPE_BOOL
    #define TYPE_BOOL 1
    #endif
#endif

#ifndef FALSE
#define FALSE				0
#endif

#ifndef TRUE
#define TRUE				1
#endif

// BYTE MANIPULATION
#define MAKEWORD(a, b)      ((WORD)(((BYTE)(((DWORD)(a)) & 0xff)) | ((WORD)((BYTE)(((DWORD)(b)) & 0xff))) << 8))
#define MAKELONG(a, b)      ((LONG)(((WORD)(((DWORD)(a)) & 0xffff)) | ((DWORD)((WORD)(((DWORD)(b)) & 0xffff))) << 16))
#define LOWORD(l)           ((WORD)(((DWORD)(l)) & 0xffff))
#define HIWORD(l)           ((WORD)((((DWORD)(l)) >> 16) & 0xffff))
#define LOBYTE(w)           ((BYTE)(((DWORD)(w)) & 0xff))
#define HIBYTE(w)           ((BYTE)((((DWORD)(w)) >> 8) & 0xff))

#ifndef min
//#define min(a,b) ((a)<(b)?(a):(b))
#endif

#ifndef __max
#define __max(a,b)  (((a) > (b)) ? (a) : (b))
#endif

#ifndef __min
#define __min(a,b)  (((a) < (b)) ? (a) : (b))
#endif

// _countof
#ifndef _countof
#define _countof(x) (sizeof(x) / sizeof((x)[0]))
#endif

#ifndef RGB
#define RGB(r,g,b)          ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))
#endif

/******************************************
 * WIN32_API and compiler
 ******************************************/

#define _stat stat
#define __stdcall
#define _stdcall
#define CALLBACK

/******************************************
 * WIN32 Data type and Pointer
 ******************************************/

typedef char 	 	CHAR;
typedef uint32_t   	DWORD;

#if defined(__APPLE__) && !defined(__OBJC__)
typedef signed char     BOOL;
#elif defined(__linux__) || defined(ANDROID)
typedef int           	BOOL;
#endif
typedef uint8_t	      	BYTE;
typedef uint8_t       	UINT8;
typedef uint16_t      	WORD;
typedef uint16_t      	UINT16;
typedef uint32_t	ULONG;
typedef uint32_t	UINT32;
typedef int16_t		SHORT;
typedef int32_t 		LONG;
typedef int32_t      	INT;
typedef uint32_t    UINT;
typedef uint64_t  ULONGLONG;
typedef uint8_t*	LPBYTE;
typedef char*         	LPSTR;
typedef const char*	LPCSTR;
typedef char*         	PCHAR;

#if UINTPTR_MAX == 0xffffffff // 32bit
typedef int32_t		INT_PTR, *PINT_PTR;
typedef uint32_t    UINT_PTR, *PUINT_PTR;
typedef int32_t 	LONG_PTR, *PLONG_PTR;
typedef uint32_t 	ULONG_PTR, *PULONG_PTR;
typedef uint16_t	UHALF_PTR, *PUHALF_PTR;
typedef int16_t		HALF_PTR, *PHALF_PTR;
typedef uint32_t 	HANDLE_PTR;
#elif UINTPTR_MAX == 0xffffffffffffffff // 64bit
typedef int64_t		INT_PTR, *PINT_PTR;
typedef uint64_t    UINT_PTR, *PUINT_PTR;
typedef int64_t 	LONG_PTR, *PLONG_PTR;
typedef uint64_t 	ULONG_PTR, *PULONG_PTR;
typedef uint32_t	UHALF_PTR, *PUHALF_PTR;
typedef int32_t		HALF_PTR, *PHALF_PTR;
typedef uint64_t 	HANDLE_PTR;
#else
typedef int32_t		INT_PTR, *PINT_PTR;
typedef uint32_t    UINT_PTR, *PUINT_PTR;
typedef int32_t 	LONG_PTR, *PLONG_PTR;
typedef uint32_t 	ULONG_PTR, *PULONG_PTR;
typedef uint16_t	UHALF_PTR, *PUHALF_PTR;
typedef int16_t		HALF_PTR, *PHALF_PTR;
typedef uint32_t 	HANDLE_PTR;
#endif

typedef ULONG_PTR	DWORD_PTR, *PDWORD_PTR;
typedef DWORD 		COLORREF;

/******************************************
 * WIN32 char and string stuff
 ******************************************/

#define USES_CONVERSION

#if defined(UNICODE) || defined(_UNICODE)
    #ifndef _TCHAR_DEFINED
//        typedef wchar_t TCHAR;
        typedef wchar_t _TCHAR;
        typedef const wchar_t* LPCTSTR;
        typedef wchar_t* LPTSTR;
        #define _TCHAR_DEFINED
    #endif

    #define _TEOF WEOF
    #define __TEXT(q)   L##q

    #define _tprintf    wprintf
    #define _ftprintf   fwprintf
    #define _stprintf   swprintf
    #define _stprintf_s swprintf
    #define _sntprintf  _snwprintf
    #define _vtprintf   vwprintf
    #define _vftprintf  vfwprintf
    #define _vstprintf  vswprintf
    #define _vsntprintf _vsnwprintf
    #define _tscanf     wscanf
    #define _ftscanf    fwscanf
    #define _stscanf    swscanf
    #define _fgettc     fgetwc
    #define _fgettchar  _fgetwchar
    #define _fgetts     fgetws
    #define _fputtc     fputwc
    #define _fputtchar  _fputwchar
    #define _fputts     fputws
    #define _gettc      getwc
    #define _getts      _getws
    #define _puttc      putwc
    #define _puttchar       putwchar
    #define _putts      _putws
    #define _ungettc    ungetwc
    #define _tstof(a)   wcstof(a, NULL)
    #define _tcstod     wcstod
    #define _tcstol     wcstol
    #define _tcstoul    wcstoul
    #define _itot       _itow
    #define _ltot       _ltow
    #define _ultot      _ultow
    #define _ttoi       _wtoi
    #define _ttol       _wtol
    #define _tcscat     wcscat
    #define _tcschr     wcschr
    #define _tcscmp     wcscmp
    #define _tcscpy     wcscpy
    #define _tcscspn    wcscspn
    #define _tcslen     wcslen
    #define _tcsncat    wcsncat
    #define _tcsncmp    wcsncmp
    #define _tcsncpy    wcsncpy
    #define _tcspbrk    wcspbrk
    #define _tcsrchr    wcsrchr
    #define _tcsspn     wcsspn
    #define _tcsstr     wcsstr
    #define _tcstok_s   wcstok
    #define _tcsdup     _wcsdup
    #define _tcsicmp    wcscasecmp
    #define _tcsnicmp   wcsncasecmp
    #define _tcsnset    _wcsnset
    #define _tcsrev     _wcsrev
    #define _tcsset     _wcsset
    #define _tcslwr     _wcslwr
    #define _tcsupr     _wcsupr
    #define _tcsxfrm    wcsxfrm
    #define _tcscoll    wcscoll
    #define _tcsicoll   _wcsicoll
    #define _istalpha   iswalpha
    #define _istupper   iswupper
    #define _istlower   iswlower
    #define _istdigit   iswdigit
    #define _istxdigit  iswxdigit
    #define _istspace   iswspace
    #define _istpunct   iswpunct
    #define _istalnum   iswalnum
    #define _istprint   iswprint
    #define _istgraph   iswgraph
    #define _istcntrl   iswcntrl
    #define _istascii   iswascii
    #define _totupper   towupper
    #define _totlower   towlower
    #define _tcsftime   wcsftime

    #define _tcsdec     _wcsdec
    #define _tcsinc     _wcsinc
    #define _tcsnbcnt   _wcsncnt
    #define _tcsnccnt   _wcsncnt
    #define _tcsnextc   _wcsnextc
    #define _tcsninc    _wcsninc
    #define _tcsspnp    _wcsspnp
    #define _wcsdec(_wcs1, _wcs2) ((_wcs1)>=(_wcs2) ? NULL : (_wcs2)-1)
    #define _wcsinc(_wcs)  ((_wcs)+1)
    #define _wcsnextc(_wcs) ((unsigned int) *(_wcs))
    #define _wcsninc(_wcs, _inc) (((_wcs)+(_inc)))
    #define _wcsncnt(_wcs, _cnt) ((wcslen(_wcs)>_cnt) ? _count : wcslen(_wcs))
    #define _wcsspnp(_wcs1, _wcs2) ((*((_wcs1)+wcsspn(_wcs1,_wcs2))) ? ((_wcs1)+wcsspn(_wcs1,_wcs2)) : NULL)
#else	// defined(UNICODE) || defined(_UNICODE)
    #ifndef _TCHAR_DEFINED
        typedef char    TCHAR;
        typedef char    _TCHAR;
        typedef const char* LPCTSTR;
        typedef char* LPTSTR;
        #define _TCHAR_DEFINED
    #endif

    #define _TEOF EOF
    #define __TEXT(q)   q

    #define _tprintf    printf
    #define _ftprintf   fprintf
    #define _stprintf   sprintf
    #define _stprintf_s  snprintf
    #define _sntprintf  _snprintf
    #define _vtprintf   vprintf
    #define _vftprintf  vfprintf
    #define _vstprintf  vsprintf
    #define _vsntprintf _vsnprintf
    #define _tscanf     scanf
    #define _ftscanf    fscanf
    #define _stscanf    sscanf
    #define _fgettc     fgetc
    #define _fgettchar  _fgetchar
    #define _fgetts     fgets
    #define _fputtc     fputc
    #define _fputtchar  _fputchar
    #define _fputts     fputs
    #define _tfdopen    _fdopen
    #define _tfopen     fopen
    #define _tfreopen   freopen
    #define _tfsopen    _fsopen
    #define _tgetenv    getenv
    #define _tputenv    _putenv
    #define _tsearchenv _searchenv
    #define  _tsystem       system
    #define _tmakepath  _makepath
    #define _tsplitpath _splitpath
    #define _tfullpath  _fullpath
    #define _gettc      getc
    #define _getts      gets
    #define _puttc      putc
    #define _puttchar       putchar
    #define _putts      puts
    #define _ungettc    ungetc
    #define _tstof      atof
    #define _tcstod     strtod
    #define _tcstol     strtol
    #define _tcstoul    strtoul
    #define _itot       _itoa
    #define _ltot       _ltoa
    #define _ultot      _ultoa
    #define _ttoi       atoi
    #define _ttol       atol
    #define _tcscat     strcat
    #define _tcschr     strchr
    #define _tcscmp     strcmp
    #define _tcsftime   strftime

    #define _tcsdec     _strdec
    #define _tcsinc     _strinc
    #define _tcsnbcnt   _strncnt
    #define _tcsnccnt   _strncnt
    #define _tcsnextc   _strnextc
    #define _tcsninc    _strninc
    #define _tcsspnp    _strspnp
    #define _strdec(_str1, _str2) ((_str1)>=(_str2) ? NULL : (_str2)-1)
    #define _strinc(_str)  ((_str)+1)
    #define _strnextc(_str) ((unsigned int) *(_str))
    #define _strninc(_str, _inc) (((_str)+(_inc)))
    #define _strncnt(_str, _cnt) ((strlen(_str)>_cnt) ? _count : strlen(_str))
    #define _strspnp(_str1, _str2) ((*((_str1)+strspn(_str1,_str2))) ? ((_str1)+strspn(_str1,_str2)) : NULL)
    #define _tcscpy     strcpy
    #define _tcscspn    strcspn
    #define _tcslen     strlen
    #define _tcsncat    strncat
    #define _tcsncmp    strncmp
    #define _tcsncpy    strncpy
    #define _tcspbrk    strpbrk
    #define _tcsrchr    strrchr
    #define _tcsspn     strspn
    #define _tcsstr     strstr
    #define _tcstok_s   strtok_r
    #define _tcsdup     _strdup
    #define _tcsicmp    strcasecmp
    #define _tcsnicmp   strncasecmp
    #define _tcsnset    _strnset
    #define _tcsrev     _strrev
    #define _tcsset     _strset
    #define _tcslwr     _strlwr
    #define _tcsupr     _strupr
    #define _tcsxfrm    strxfrm
    #define _tcscoll    strcoll
    #define _tcsicoll   _stricoll
    #define _istalpha   isalpha
    #define _istupper   isupper
    #define _istlower   islower
    #define _istdigit   isdigit
    #define _istxdigit  isxdigit
    #define _istspace   isspace
    #define _istpunct   ispunct
    #define _istalnum   isalnum
    #define _istprint   isprint
    #define _istgraph   isgraph
    #define _istcntrl   iscntrl
    #define _istascii   isascii
    #define _totupper   toupper
    #define _totlower   tolower
    #define _tasctime   asctime
    #define _tctime     ctime
    #define _tstrdate   _strdate
	#define _tstrtime   _strtime
	#define _tutime     _utime
#endif // UNICODE

#ifndef _TEXT
#define _TEXT(x)    __TEXT(x)
#endif
#ifndef _T
#define _T(x)       __TEXT(x)
#endif
#define _UI16_MAX UINT16_MAX

/******************************************
 * windef.h
 ******************************************/

typedef struct tagPOINT
{
    LONG x;
    LONG y;
} POINT, *PPOINT;

typedef struct tagRECT
{
    LONG    left;
    LONG    top;
    LONG    right;
    LONG    bottom;
} RECT, *PRECT;

typedef struct tagSIZE
{
    LONG    cx;
    LONG    cy;
} SIZE, *PSIZE;

#if defined(__OBJC__)
#define BOOL signed char
#endif
inline BOOL SetRectEmpty(RECT* rp)
{
    rp->left = rp->top = rp->right = rp->bottom = 0;
    return TRUE;
}

inline BOOL SetRect(RECT* rp, int left, int top, int right, int bottom)
{
    rp->left = left;
	rp->top = top;
	rp->right = right;
	rp->bottom = bottom;
    return TRUE;
}

inline BOOL PtInRect(const RECT* rp, POINT& point)
{
    LONG x0, x1, y0, y1;
    if (rp->left <= rp->right) {
        x0 = rp->left;
        x1 = rp->right;
    }
    else {
        x0 = rp->right;
        x1 = rp->left;
    }
    if (rp->top <= rp->bottom) {
        y0 = rp->top;
        y1 = rp->bottom;
    }
    else {
        y0 = rp->bottom;
        y1 = rp->top;
    }

    if ( x0 <= point.x && point.x <= x1 &&
         y0 <= point.y && point.y <= y1 )
        return TRUE;

    return FALSE;
}

/******************************************
 * Helper
 ******************************************/

inline int DecodeBase64Char(unsigned int ch) throw()
{
	// returns -1 if the character is invalid
	// or should be skipped
	// otherwise, returns the 6-bit code for the character
	// from the encoding table
	if (ch >= 'A' && ch <= 'Z') {
		return ch - 'A' + 0;	// 0 range starts at 'A'
    }

	if (ch >= 'a' && ch <= 'z') {
		return ch - 'a' + 26;	// 26 range starts at 'a'
    }

	if (ch >= '0' && ch <= '9') {
		return ch - '0' + 52;	// 52 range starts at '0'
    }

	if (ch == '+') {
		return 62;
    }

	if (ch == '/') {
		return 63;
    }

	return -1;
}

inline BOOL Base64Decode(LPCSTR szSrc, int nSrcLen, BYTE *pbDest, int *pnDestLen) throw()
{
	// walk the source buffer
	// each four character sequence is converted to 3 bytes
	// CRLFs and =, and any characters not in the encoding table
	// are skiped

	if (szSrc == NULL || pnDestLen == NULL) {
        assert(FALSE);
		return FALSE;
	}

	LPCSTR szSrcEnd = szSrc + nSrcLen;
	int nWritten = 0;

	BOOL bOverflow = (pbDest == NULL) ? TRUE : FALSE;

	while (szSrc < szSrcEnd &&(*szSrc) != 0) {
		DWORD dwCurr = 0;
		int i;
		int nBits = 0;
		for (i=0; i<4; i++) {
			if (szSrc >= szSrcEnd) {
				break;
            }

			int nCh = DecodeBase64Char(*szSrc);
			szSrc++;
			if (nCh == -1) {
				// skip this char
				i--;
				continue;
			}
			dwCurr <<= 6;
			dwCurr |= nCh;
			nBits += 6;
		}

		if (!bOverflow && nWritten + (nBits/8) > (*pnDestLen)) {
			bOverflow = TRUE;
        }

		// dwCurr has the 3 bytes to write to the output buffer
		// left to right
		dwCurr <<= 24-nBits;
		for (i=0; i<nBits/8; i++) {
			if (!bOverflow) {
				*pbDest = (BYTE) ((dwCurr & 0x00ff0000) >> 16);
				pbDest++;
			}
			dwCurr <<= 8;
			nWritten++;
		}

	}

	*pnDestLen = nWritten;

	if (bOverflow) {
		if (pbDest != NULL) {
            assert(FALSE);
		}

		return FALSE;
	}

	return TRUE;
}
#if defined(__OBJC__)
#ifdef BOOL
#undef BOOL
#endif
#endif


#endif // _WIN32_PORTING_H

#endif // __linux__ || __APPLE_ || ANDROID

