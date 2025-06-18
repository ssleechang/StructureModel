// StructureModel.h : StructureModel DLL의 기본 헤더 파일입니다.
//

#pragma once

#ifndef __AFXWIN_H__
#error "PCH에 대해 이 파일을 포함하기 전에 'stdafx.h'를 포함합니다."
#endif

// Modefied  04/21/96

#ifndef __StructureModel_StructureModel_H
#define __StructureModel_StructureModel_H

#if defined(_StructureModelSTATIC) 
#define StructureModel_CLASS
#define StructureModel_API   __stdcall
#define StructureModel_DATA
#elif defined(_StructureModelDLL) 
#define StructureModel_CLASS __declspec(dllimport)
#define StructureModel_API   __declspec(dllimport)
#define StructureModel_DATA  __declspec(dllimport)
#else 
#define StructureModel_CLASS __declspec(dllexport)
#define StructureModel_API   __declspec(dllexport)
#define StructureModel_DATA  __declspec(dllexport)
#endif 

// StructureModel_CDECL is used for rare functions taking variable arguments
// such printf(" ", ...)
#ifndef StructureModel_CDECL   
#define StructureModel_CDECL __cdecl
#endif

#ifdef _StructureModelDLL
#endif

#endif