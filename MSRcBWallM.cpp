#include "StdAfx.h"
#include "MSRcBWallM.h"
#include "MSMainBar.h"
#include "GMLib/MSVersionInfo.h"

IMPLEMENT_SERIAL(MSRcBWallM, MDMember, VERSIONABLE_SCHEMA | 1)
MSRcBWallM::MSRcBWallM(void)
{
	CString sBar = _T("");
	
	sBar = MSDeformedBar::GetStirrupBarStr(300);
	m_HBars.InitData(sBar);
	m_VBars.InitData(sBar);
	sBar = MSDeformedBar::GetMainBarStr(4);
	m_MainVBar.InitData(sBar);
	m_MainHBar.InitData(sBar);

	sBar = MSDeformedBar::GetStirrupBarStr(300);
	//전단 보강근
	m_TopShearHBars.InitData(sBar);
	m_TopShearVBars.InitData(sBar);
	m_BotShearHBars.InitData(sBar);
	m_BotShearVBars.InitData(sBar);

	m_bTopShearAddBar = FALSE;					//상부근 유무
	m_bBotShearAddBar = FALSE;					//하부근 유무

	m_nTopShearAddHBarLength = 1000;	
	m_nBotShearAddVBarLength = 1000;  

	m_bIsSinglePlaceBar = FALSE;			
	m_bIsNumPlaceVBar = FALSE;				
	m_bIsNumPlaceHBar = FALSE;

	m_dCvrConc = 20;

	m_bStartShearAddBar = FALSE;
	m_bEndShearAddBar = FALSE;
	m_StartShearHBars.InitData(sBar);
	m_StartShearVBars.InitData(sBar);
	m_EndShearHBars.InitData(sBar);
	m_EndShearVBars.InitData(sBar);
	m_nStartShearAddBarLength = 1000;
	m_nEndShearAddBarLength = 1000;

	m_bAllShearAddBar = FALSE;
	m_AllShearHBars.InitData(sBar);
	m_AllShearVBars.InitData(sBar);

	CString sDefDiaName = MSDeformedBar::GetDefaultRebarName();
	m_nShearBarPlaceType = 0;
	m_dShearBarRateDiaIndex = MSDeformedBar::FindBarDiameterIndexByName(sDefDiaName);

	m_nHorShearBarRateSpacing = 150;
	m_nHorShearBarRate = 2;

	m_nVerShearBarRateSpacing = 150;
	m_nVerShearBarRate = 1;


	// 특수 경계 요소
	m_IsSpecialBoundary = false;
	// 경계 길이(유형) : 전체(0) / 구간(1)
	m_nSpecialBoundaryType = 1;
	// 경계 길이
	m_nBoundaryLength = 1600;

	// 단부 수직근
	// 갯수배근(true) / 간격 배근(false)
	m_IsNumPlaceEndVerBar = true;
	sBar = MSDeformedBar::GetMainBarStr(30, 25);
	m_EndVerMainBar.InitData(sBar);
	sBar = MSDeformedBar::GetStirrupBarStr(300, 25);
	m_EndVerBar.InitData(sBar);

	// 대근
	sBar = MSDeformedBar::GetStirrupBarStr(300, 13);
	m_HoopBar.InitData(sBar);

	// 보조대근 Lx
	m_IsNumPlaceTieBarLx = false;
	sBar = MSDeformedBar::GetStirrupBarStr(100, 13);
	m_TieBarLx.InitData(sBar);
	m_nTieBarLxNum = 1;

	// 보조대근 Ly 
	m_IsNumPlaceTieBarLy = false;
	sBar = MSDeformedBar::GetStirrupBarStr(100, 13);
	m_TieBarLy.InitData(sBar);
	m_nTieBarLyNum = 1;
}

MSRcBWallM::~MSRcBWallM(void)
{
}

void MSRcBWallM::CopyFromMe(MDMember* pSource)
{
	if(pSource==NULL)	return;
	MSRcBWallM* pSourceWallM = (MSRcBWallM*)pSource;
	m_HBars.CopyFromMe(&pSourceWallM->m_HBars);
	m_VBars.CopyFromMe(&pSourceWallM->m_VBars);
	m_MainVBar.CopyFromMe(&pSourceWallM->m_MainVBar);
	m_MainHBar.CopyFromMe(&pSourceWallM->m_MainHBar);

	m_TopShearHBars.CopyFromMe(&pSourceWallM->m_TopShearHBars);
	m_TopShearVBars.CopyFromMe(&pSourceWallM->m_TopShearVBars);
	m_BotShearHBars.CopyFromMe(&pSourceWallM->m_BotShearHBars);
	m_BotShearVBars.CopyFromMe(&pSourceWallM->m_BotShearVBars);

	m_bTopShearAddBar = pSourceWallM->m_bTopShearAddBar;		
	m_bBotShearAddBar = pSourceWallM->m_bBotShearAddBar;				

	m_nTopShearAddHBarLength = pSourceWallM->m_nTopShearAddHBarLength;	
	m_nBotShearAddVBarLength = pSourceWallM->m_nBotShearAddVBarLength;  

	m_bIsSinglePlaceBar = pSourceWallM->m_bIsSinglePlaceBar;			
	m_bIsNumPlaceVBar = pSourceWallM->m_bIsNumPlaceVBar;				
	m_bIsNumPlaceHBar = pSourceWallM->m_bIsNumPlaceHBar;

	m_bStartShearAddBar = pSourceWallM->m_bStartShearAddBar;
	m_bEndShearAddBar = pSourceWallM->m_bEndShearAddBar;
	m_StartShearHBars.CopyFromMe(&pSourceWallM->m_StartShearHBars);
	m_StartShearVBars.CopyFromMe(&pSourceWallM->m_StartShearVBars);
	m_EndShearHBars.CopyFromMe(&pSourceWallM->m_EndShearHBars);
	m_EndShearVBars.CopyFromMe(&pSourceWallM->m_EndShearVBars);
	m_nStartShearAddBarLength = pSourceWallM->m_nStartShearAddBarLength;
	m_nEndShearAddBarLength = pSourceWallM->m_nEndShearAddBarLength;

	m_bAllShearAddBar = pSourceWallM->m_bAllShearAddBar;
	m_AllShearHBars.CopyFromMe(&pSourceWallM->m_AllShearHBars);
	m_AllShearVBars.CopyFromMe(&pSourceWallM->m_AllShearVBars);

	m_nShearBarPlaceType = pSourceWallM->m_nShearBarPlaceType;
	m_dShearBarRateDiaIndex = pSourceWallM->m_dShearBarRateDiaIndex;

	m_nHorShearBarRateSpacing = pSourceWallM->m_nHorShearBarRateSpacing;
	m_nHorShearBarRate = pSourceWallM->m_nHorShearBarRate;

	m_nVerShearBarRateSpacing = pSourceWallM->m_nVerShearBarRateSpacing;
	m_nVerShearBarRate = pSourceWallM->m_nVerShearBarRate;

	// 특수 경계 요소
	m_IsSpecialBoundary = pSourceWallM->GetIsSpecialBoundary();
	m_nSpecialBoundaryType = pSourceWallM->GetSpecialBoundaryType();
	m_nBoundaryLength = pSourceWallM->GetBoundaryLength();

	// 단부 수직근
	m_IsNumPlaceEndVerBar = pSourceWallM->GetIsNumPlaceEndVerBar();
	m_EndVerMainBar.CopyFromMe(pSourceWallM->GetEndVerMainBar());
	m_EndVerBar.CopyFromMe(pSourceWallM->GetEndVerBar());

	// 대근
	m_HoopBar.CopyFromMe(pSourceWallM->GetHoopBar());

	// 보조대근 Lx
	m_IsNumPlaceTieBarLx = pSourceWallM->GetIsNumPlaceTieBarLx();
	m_TieBarLx.CopyFromMe(pSourceWallM->GetTieBarLx());
	m_nTieBarLxNum = pSourceWallM->GetTieBarLxNum();

	// 보조대근 Ly 
	m_IsNumPlaceTieBarLy = pSourceWallM->GetIsNumPlaceTieBarLy();
	m_TieBarLy.CopyFromMe(pSourceWallM->GetTieBarLy());
	m_nTieBarLyNum = pSourceWallM->GetTieBarLyNum();

	MDMember::CopyFromMe(pSource);
}

void MSRcBWallM::Dump(EFS::EFS_Buffer& buffer)
{
	MDMember::Dump(buffer);
	buffer << m_bTopShearAddBar << m_bBotShearAddBar << m_nTopShearAddHBarLength << m_nBotShearAddVBarLength;
	buffer << m_bIsSinglePlaceBar << m_bIsNumPlaceVBar << m_bIsNumPlaceHBar;
	buffer << m_bStartShearAddBar << m_bEndShearAddBar;
	buffer << m_nStartShearAddBarLength << m_nEndShearAddBarLength;
	buffer << m_bAllShearAddBar;
	buffer << m_nShearBarPlaceType << m_dShearBarRateDiaIndex;
	buffer << m_nHorShearBarRateSpacing << m_nHorShearBarRate;
	buffer << m_nVerShearBarRateSpacing << m_nVerShearBarRate;

	m_HBars.Dump(buffer);
	m_VBars.Dump(buffer);
	m_MainVBar.Dump(buffer);
	m_MainHBar.Dump(buffer);
	m_TopShearHBars.Dump(buffer);
	m_TopShearVBars.Dump(buffer);
	m_BotShearHBars.Dump(buffer);
	m_BotShearVBars.Dump(buffer);
	m_StartShearHBars.Dump(buffer);
	m_StartShearVBars.Dump(buffer);
	m_EndShearHBars.Dump(buffer);
	m_EndShearVBars.Dump(buffer);
	m_AllShearHBars.Dump(buffer);
	m_AllShearVBars.Dump(buffer);

	// 특수 경계 요소
	buffer << m_IsSpecialBoundary;
	buffer << m_nSpecialBoundaryType;
	buffer << m_nBoundaryLength;

	// 단부 수직근
	buffer << m_IsNumPlaceEndVerBar;
	m_EndVerMainBar.Dump(buffer);
	m_EndVerBar.Dump(buffer);

	// 대근
	m_HoopBar.Dump(buffer);

	// 보조대근 Lx
	buffer << m_IsNumPlaceTieBarLx;
	m_TieBarLx.Dump(buffer);
	buffer << m_nTieBarLxNum;

	// 보조대근 Ly 
	buffer << m_IsNumPlaceTieBarLy;
	m_TieBarLy.Dump(buffer);
	buffer << m_nTieBarLyNum;
}

void MSRcBWallM::Recovery(EFS::EFS_Buffer& buffer)
{
	MDMember::Recovery(buffer);
	buffer >> m_bTopShearAddBar >> m_bBotShearAddBar >> m_nTopShearAddHBarLength >> m_nBotShearAddVBarLength;
	buffer >> m_bIsSinglePlaceBar >> m_bIsNumPlaceVBar >> m_bIsNumPlaceHBar;
	buffer >> m_bStartShearAddBar >> m_bEndShearAddBar;
	buffer >> m_nStartShearAddBarLength >> m_nEndShearAddBarLength;
	buffer >> m_bAllShearAddBar;
	buffer >> m_nShearBarPlaceType >> m_dShearBarRateDiaIndex;
	buffer >> m_nHorShearBarRateSpacing >> m_nHorShearBarRate;
	buffer >> m_nVerShearBarRateSpacing >> m_nVerShearBarRate;

	m_HBars.Recovery(buffer);
	m_VBars.Recovery(buffer);
	m_MainVBar.Recovery(buffer);
	m_MainHBar.Recovery(buffer);
	m_TopShearHBars.Recovery(buffer);
	m_TopShearVBars.Recovery(buffer);
	m_BotShearHBars.Recovery(buffer);
	m_BotShearVBars.Recovery(buffer);
	m_StartShearHBars.Recovery(buffer);
	m_StartShearVBars.Recovery(buffer);
	m_EndShearHBars.Recovery(buffer);
	m_EndShearVBars.Recovery(buffer);
	m_AllShearHBars.Recovery(buffer);
	m_AllShearVBars.Recovery(buffer);

	// 특수 경계 요소
	buffer >> m_IsSpecialBoundary;
	buffer >> m_nSpecialBoundaryType;
	buffer >> m_nBoundaryLength;

	// 단부 수직근
	buffer >> m_IsNumPlaceEndVerBar;
	m_EndVerMainBar.Recovery(buffer);
	m_EndVerBar.Recovery(buffer);

	// 대근
	m_HoopBar.Recovery(buffer);

	// 보조대근 Lx
	buffer >> m_IsNumPlaceTieBarLx;
	m_TieBarLx.Recovery(buffer);
	buffer >> m_nTieBarLxNum;

	// 보조대근 Ly 
	buffer >> m_IsNumPlaceTieBarLy;
	m_TieBarLy.Recovery(buffer);
	buffer >> m_nTieBarLyNum;
}

void MSRcBWallM::Serialize(CArchive &ar)
{
	MDMember::Serialize(ar);

	MSMainBar oldMainHBar;
	BOOL oldIsNumPlaceHBar = FALSE;
	if(ar.IsStoring())
	{
		m_HBars.Serialize(ar);
		m_VBars.Serialize(ar);

		oldMainHBar.Serialize(ar);
		m_MainVBar.Serialize(ar);

		m_TopShearHBars.Serialize(ar);
		m_TopShearVBars.Serialize(ar);
		m_BotShearHBars.Serialize(ar);
		m_BotShearVBars.Serialize(ar);

		ar << m_bTopShearAddBar;				
		ar << m_bBotShearAddBar;				

		ar << m_nTopShearAddHBarLength;
		ar << m_nBotShearAddVBarLength;

		ar << m_bIsSinglePlaceBar;
		ar << m_bIsNumPlaceVBar;
		ar << oldIsNumPlaceHBar;

		// 2013.09.09
		ar << m_bStartShearAddBar;
		ar << m_bEndShearAddBar;
		m_StartShearHBars.Serialize(ar);
		m_StartShearVBars.Serialize(ar);
		m_EndShearHBars.Serialize(ar);
		m_EndShearVBars.Serialize(ar);
		ar << m_nStartShearAddBarLength;
		ar << m_nEndShearAddBarLength;

		// BH3354
		m_MainHBar.Serialize(ar);
		ar << m_bIsNumPlaceHBar;

		ar << m_bAllShearAddBar;
		m_AllShearHBars.Serialize(ar);
		m_AllShearVBars.Serialize(ar);

		ar << m_nShearBarPlaceType;
		ar << m_dShearBarRateDiaIndex;

		ar << m_nHorShearBarRateSpacing;
		ar << m_nHorShearBarRate;

		ar << m_nVerShearBarRateSpacing;
		ar << m_nVerShearBarRate;

#pragma region 특수 전단벽 DB
		ar << m_IsSpecialBoundary;
		ar << m_nSpecialBoundaryType;
		ar << m_nBoundaryLength;

		// 단부 수직근
		ar << m_IsNumPlaceEndVerBar;
		m_EndVerMainBar.Serialize(ar);
		m_EndVerBar.Serialize(ar);

		// 대근
		m_HoopBar.Serialize(ar);

		// 보조대근 Lx
		ar << m_IsNumPlaceTieBarLx;
		m_TieBarLx.Serialize(ar);
		ar << m_nTieBarLxNum;

		// 보조대근 Ly 
		ar << m_IsNumPlaceTieBarLy;
		m_TieBarLy.Serialize(ar);
		ar << m_nTieBarLyNum;
#pragma endregion

	}
	else
	{
		m_HBars.Serialize(ar);
		m_VBars.Serialize(ar);

		oldMainHBar.Serialize(ar);
		m_MainVBar.Serialize(ar);

		m_TopShearHBars.Serialize(ar);
		m_TopShearVBars.Serialize(ar);
		m_BotShearHBars.Serialize(ar);
		m_BotShearVBars.Serialize(ar);

		ar >> m_bTopShearAddBar;				
		ar >> m_bBotShearAddBar;				

		ar >> m_nTopShearAddHBarLength;
		ar >> m_nBotShearAddVBarLength;

		ar >> m_bIsSinglePlaceBar;
		ar >> m_bIsNumPlaceVBar;
		ar >> oldIsNumPlaceHBar;
		ar >> m_bStartShearAddBar;
		ar >> m_bEndShearAddBar;
		m_StartShearHBars.Serialize(ar);
		m_StartShearVBars.Serialize(ar);
		m_EndShearHBars.Serialize(ar);
		m_EndShearVBars.Serialize(ar);
		ar >> m_nStartShearAddBarLength;
		ar >> m_nEndShearAddBarLength;

		if (MSVersionInfo::GetCurrentVersion() >= 20171110)
		{
			m_MainHBar.Serialize(ar);
			ar >> m_bIsNumPlaceHBar;
		}

		if (IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() >= 20180718)
		{
			ar >> m_bAllShearAddBar;
			m_AllShearHBars.Serialize(ar);
			m_AllShearVBars.Serialize(ar);
		}

		if (IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() >= 20180920)
		{
			ar >> m_nShearBarPlaceType;
			ar >> m_dShearBarRateDiaIndex;

			ar >> m_nHorShearBarRateSpacing;
			ar >> m_nHorShearBarRate;

			ar >> m_nVerShearBarRateSpacing;
			ar >> m_nVerShearBarRate;
		}


#pragma region 특수 전단벽 DB
		if (IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() >= 20231010)
		{
			ar >> m_IsSpecialBoundary;
			ar >> m_nSpecialBoundaryType;
			ar >> m_nBoundaryLength;

			// 단부 수직근
			ar >> m_IsNumPlaceEndVerBar;
			m_EndVerMainBar.Serialize(ar);
			m_EndVerBar.Serialize(ar);

			// 대근
			m_HoopBar.Serialize(ar);

			// 보조대근 Lx
			ar >> m_IsNumPlaceTieBarLx;
			m_TieBarLx.Serialize(ar);
			ar >> m_nTieBarLxNum;

			// 보조대근 Ly 
			ar >> m_IsNumPlaceTieBarLy;
			m_TieBarLy.Serialize(ar);
			ar >> m_nTieBarLyNum;
		}
#pragma endregion
	}
}

vector<double> MSRcBWallM::GetMainBarDiaAllLayer()
{
	vector<double> DiaArr;

	vector<double> HBarDiaArr = m_HBars.GetBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), HBarDiaArr.begin(), HBarDiaArr.end());

	if(m_bIsNumPlaceVBar == 0)						//배근 방식 // 디폴트 = 1 // 0 = 간격 1 = 갯수	
	{
		vector<double> VBarDiaArr = m_VBars.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), VBarDiaArr.begin(), VBarDiaArr.end());
	}
	else if(m_bIsNumPlaceVBar == 1)
	{
		vector<double> MainVBarDiaArr = m_MainVBar.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), MainVBarDiaArr.begin(), MainVBarDiaArr.end());
	}
	else
		ASSERT(0);

	sort(DiaArr.begin(), DiaArr.end(), 
		[](const long& a, const long& b)->bool {return a < b;}
	);

	vector<double>::iterator new_end;
	new_end = unique(DiaArr.begin(), DiaArr.end());
	DiaArr.erase(new_end, DiaArr.end());

	return DiaArr;
}

vector<double> MSRcBWallM::GetVerBarDiaAllLayer()
{
	vector<double> DiaArr;

	if (m_bIsNumPlaceVBar == 0)						//배근 방식 // 디폴트 = 1 // 0 = 간격 1 = 갯수	
	{
		vector<double> VBarDiaArr = m_VBars.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), VBarDiaArr.begin(), VBarDiaArr.end());
	}
	else if (m_bIsNumPlaceVBar == 1)
	{
		vector<double> MainVBarDiaArr = m_MainVBar.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), MainVBarDiaArr.begin(), MainVBarDiaArr.end());
	}
	else
		ASSERT(0);

	sort(DiaArr.begin(), DiaArr.end(),
		[](const long& a, const long& b)->bool {return a < b; }
	);

	vector<double>::iterator new_end;
	new_end = unique(DiaArr.begin(), DiaArr.end());
	DiaArr.erase(new_end, DiaArr.end());

	return DiaArr;
}

vector<double> MSRcBWallM::GetHorBarDiaAllLayer()
{
	vector<double> DiaArr;

	vector<double> HBarDiaArr = m_HBars.GetBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), HBarDiaArr.begin(), HBarDiaArr.end());

	sort(DiaArr.begin(), DiaArr.end(),
		[](const long& a, const long& b)->bool {return a < b; }
	);

	vector<double>::iterator new_end;
	new_end = unique(DiaArr.begin(), DiaArr.end());
	DiaArr.erase(new_end, DiaArr.end());

	return DiaArr;
}

vector<double> MSRcBWallM::GetStirrupBarDiaAllLayer()
{
	vector<double> DiaArr;

	if (m_bAllShearAddBar)
	{
		vector<double> AllShearHBarDiaArr = m_AllShearHBars.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), AllShearHBarDiaArr.begin(), AllShearHBarDiaArr.end());

		vector<double> AllShearVBarDiaArr = m_AllShearVBars.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), AllShearVBarDiaArr.begin(), AllShearVBarDiaArr.end());
	}

	if(m_bTopShearAddBar)
	{
		vector<double> TopShearHBarDiaArr = m_TopShearHBars.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), TopShearHBarDiaArr.begin(), TopShearHBarDiaArr.end());

		vector<double> TopShearVBarDiaArr = m_TopShearVBars.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), TopShearVBarDiaArr.begin(), TopShearVBarDiaArr.end());
	}

	if(m_bBotShearAddBar)
	{
		vector<double> BotShearHBarDiaArr = m_BotShearHBars.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), BotShearHBarDiaArr.begin(), BotShearHBarDiaArr.end());

		vector<double> BotShearVBarDiaArr = m_BotShearVBars.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), BotShearVBarDiaArr.begin(), BotShearVBarDiaArr.end());
	}

	if(m_bStartShearAddBar)					//시작근 유무
	{
		vector<double> StartShearHBarDiaArr = m_StartShearHBars.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), StartShearHBarDiaArr.begin(), StartShearHBarDiaArr.end());

		vector<double> StartShearVBarDiaArr = m_StartShearVBars.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), StartShearVBarDiaArr.begin(), StartShearVBarDiaArr.end());
	}

	if(m_bEndShearAddBar)					//끝근 유무
	{
		vector<double> EndShearHBarDiaArr = m_EndShearHBars.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), EndShearHBarDiaArr.begin(), EndShearHBarDiaArr.end());

		vector<double> EndShearVBarDiaArr = m_EndShearVBars.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), EndShearVBarDiaArr.begin(), EndShearVBarDiaArr.end());
	}

	sort(DiaArr.begin(), DiaArr.end(), 
		[](const long& a, const long& b)->bool {return a < b;}
	);

	vector<double>::iterator new_end;
	new_end = unique(DiaArr.begin(), DiaArr.end());
	DiaArr.erase(new_end, DiaArr.end());

	return DiaArr;
}
double MSRcBWallM::getVBarMinDeformedBarDiameter()
{
	vector<double> DiaArr;

	if (m_bIsNumPlaceVBar == 0)						//배근 방식 // 디폴트 = 1 // 0 = 간격 1 = 갯수	
	{
		vector<double> VBarDiaArr = m_VBars.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), VBarDiaArr.begin(), VBarDiaArr.end());
	}
	else if (m_bIsNumPlaceVBar == 1)
	{
		vector<double> MainVBarDiaArr = m_MainVBar.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), MainVBarDiaArr.begin(), MainVBarDiaArr.end());
	}
	else
		ASSERT(0);

	sort(DiaArr.begin(), DiaArr.end(),
		[](const long& a, const long& b)->bool {return a > b; }
	);

	vector<double>::iterator new_end;
	new_end = unique(DiaArr.begin(), DiaArr.end());
	DiaArr.erase(new_end, DiaArr.end());

	long nDiaSize = DiaArr.size();
	if (nDiaSize == 0) return 0;

	return DiaArr[nDiaSize-1];
}
double MSRcBWallM::getVBarMaxDeformedBarDiameter()
{
	vector<double> DiaArr;

	if (m_bIsNumPlaceVBar == 0)						//배근 방식 // 디폴트 = 1 // 0 = 간격 1 = 갯수	
	{
		vector<double> VBarDiaArr = m_VBars.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), VBarDiaArr.begin(), VBarDiaArr.end());
	}
	else if (m_bIsNumPlaceVBar == 1)
	{
		vector<double> MainVBarDiaArr = m_MainVBar.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), MainVBarDiaArr.begin(), MainVBarDiaArr.end());
	}
	else
		ASSERT(0);

	sort(DiaArr.begin(), DiaArr.end(),
		[](const long& a, const long& b)->bool {return a < b; }
	);

	vector<double>::iterator new_end;
	new_end = unique(DiaArr.begin(), DiaArr.end());
	DiaArr.erase(new_end, DiaArr.end());

	long nDiaSize = DiaArr.size();
	if (nDiaSize == 0) return 0;

	return DiaArr[nDiaSize-1];
}
double MSRcBWallM::getHBarMaxDeformedBarDiameter()
{
	vector<double> DiaArr;

	if (m_bIsNumPlaceHBar == 0)						//배근 방식 // 디폴트 = 1 // 0 = 간격 1 = 갯수	
	{
		vector<double> HBarDiaArr = m_HBars.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), HBarDiaArr.begin(), HBarDiaArr.end());
	}
	else if (m_bIsNumPlaceHBar == 1)
	{
		vector<double> MainHBarDiaArr = m_MainHBar.GetBarDiaAllLayer();
		DiaArr.insert(DiaArr.end(), MainHBarDiaArr.begin(), MainHBarDiaArr.end());
	}
	else
		ASSERT(0);

	sort(DiaArr.begin(), DiaArr.end(),
		[](const long& a, const long& b)->bool {return a < b; }
	);

	vector<double>::iterator new_end;
	new_end = unique(DiaArr.begin(), DiaArr.end());
	DiaArr.erase(new_end, DiaArr.end());

	long nDiaSize = DiaArr.size();
	if (nDiaSize == 0) return 0;

	return DiaArr[nDiaSize - 1];
}
double MSRcBWallM::GetMaxDia()
{
	return max(GetMaxDia_Ver(), GetMaxDia_Hor());
}
double MSRcBWallM::GetMaxDia_Ver()
{
	double dMaxDia = 0;
	if(m_bIsNumPlaceVBar)
		dMaxDia = max(dMaxDia,m_MainVBar.GetDeformedBarDiameter());
	else
	{
		dMaxDia = max(dMaxDia,m_VBars.GetDeformedBarDiameter(0));
		if(m_VBars.GetNumLayers() >= 2)
			dMaxDia = max(dMaxDia,m_VBars.GetDeformedBarDiameter(1));
	}
	return dMaxDia;
}
bool MSRcBWallM::GetVerBarUserSteelGradeS()
{
	if(m_bIsNumPlaceVBar)
		return m_MainVBar.GetUserSteelGradeS();
	else
		return m_VBars.GetUserSteelGradeS();
}
double MSRcBWallM::GetMaxDia_EndVerBar()
{
	double dMaxDia = 0;
	if(m_IsNumPlaceEndVerBar)
		dMaxDia = max(dMaxDia,m_EndVerMainBar.GetDeformedBarDiameter());
	else
	{
		dMaxDia = max(dMaxDia,m_EndVerBar.GetDeformedBarDiameter(0));
		if(m_VBars.GetNumLayers() >= 2)
			dMaxDia = max(dMaxDia,m_EndVerBar.GetDeformedBarDiameter(1));
	}
	return dMaxDia;
}
bool MSRcBWallM::GetEndVerBarUserSteelGradeS()
{
	if(m_IsNumPlaceEndVerBar)
		return m_EndVerMainBar.GetUserSteelGradeS();
	else
		return m_EndVerBar.GetUserSteelGradeS();	
}
double MSRcBWallM::GetMaxDia_Hor()
{
	return getHBarMaxDeformedBarDiameter();
}