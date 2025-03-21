#include "stdafx.h"

// Dynamic Link defines
typedef bool (__stdcall * VISELECTAPPTYPE)(int iAppType);
typedef bool (__stdcall * VIOPENRM )();
typedef bool (__stdcall * VIWRITE  )( char* szCmd );
typedef int  (__stdcall * VIREAD   )( char* szRet, int iBufLen );
typedef bool (__stdcall * VICLOSERM)();
typedef int  (__stdcall * VIERRCODE)();
typedef int  (__stdcall * VIGETCOMMANDRESULT)();
