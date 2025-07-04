﻿// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.

#pragma once

#pragma warning(disable	:	4996) 
#pragma warning(disable : 4995)
#pragma warning(disable : 4267)// size_t -> long,int   in x64
#pragma warning(disable : 4244)// INT_PTR -> long, int  in x64

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 일부 CString 생성자는 명시적으로 선언됩니다.

#include <afxwin.h>         // MFC 핵심 및 표준 구성 요소입니다.
#include <afxext.h>         // MFC 확장입니다.

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE 클래스입니다.
#include <afxodlgs.h>       // MFC OLE 대화 상자 클래스입니다.
#include <afxdisp.h>        // MFC 자동화 클래스입니다.
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>                      // MFC ODBC 데이터베이스 클래스입니다.
#endif // _AFX_NO_DB_SUPPORT

// #ifndef _AFX_NO_DAO_SUPPORT
// #include <afxdao.h>                     // MFC DAO 데이터베이스 클래스입니다.
// #endif // _AFX_NO_DAO_SUPPORT

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // Internet Explorer 4 공용 컨트롤에 대한 MFC 지원입니다.
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // Windows 공용 컨트롤에 대한 MFC 지원입니다.
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxtempl.h>
#include <afxcontrolbars.h>     // MFC support for ribbons and control bars

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// start of addition by ironbell
//#include "IFCLib\IFCLib.h"
// end of addition by ironbell
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// #include "MSObject.h"
// #include "MSCompoM.h"
// #include "MSCompoG.h"
#include <math.h>
#include <vector>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <numeric>
#include <math.h>
#include <memory>
#include <functional>

using namespace std;

#include "GMLib\GMLib.h"
#include "GMLib\GMVector.h"
#include "GMLib/Global.h"
#include "GMLib/Msg.h"

#include "GMLib\XErrorContainer.h"//by LSS
#include "GMLib\XErrorObject.h"//by LSS

#include "StructureModel.h"
#include "StructureModelEnum.h"
#include "RADFCore_CLI\RADFCore_CLI.h"
#include "RADFCore_CLI/LoggerWrapper.h"
#include "RADFCore_CLI/StatusReporterWrapper.h"
#include "ActivateLib_CLI\ActivateLib_CLI.h"
#include "resource.h"
#include "RADFCore_CLI/LocalDic.h"
//#include <vld.h>