#include "StdAfx.h"
#include "MSRcStrip.h"

#include "GMLib/MSVersionInfo.h"
#include "MSDeformedBar.h"

IMPLEMENT_SERIAL(MSRcStrip, MSRcObject, VERSIONABLE_SCHEMA | 1)
MSRcStrip::MSRcStrip(void)
{
	m_nUseColumnStrip = 0;
	m_StripType = TYPE_A;
	m_Direction = -1;
	m_FromRatio = 0;
	m_ToRatio = 0;
	m_IsMiddle = -1;
// 	m_Name = "";
	m_IsEndBar = TRUE;

	m_CenBarType = 2;
	m_CenBarTypeBot = 2;

	CString sBar = MSDeformedBar::GetStirrupBarStr(300);	
	m_Bar1.InitData(sBar);
	m_Bar2.InitData(sBar);
	m_Bar3.InitData(sBar);
	m_Bar4.InitData(sBar);
	m_Bar5.InitData(sBar);
	m_Bar6.InitData(sBar);
	m_Bar7.InitData(sBar);
	m_Bar8.InitData(sBar);
	m_Bar9.InitData(sBar);
	m_Bar10.InitData(sBar);

	m_IsNumPlaceBar4 = false;
	CString sMainBar = MSDeformedBar::GetMainBarStr(4);
	m_MainBar3.InitData(sMainBar);
	m_MainBar4.InitData(sMainBar);
}

MSRcStrip::~MSRcStrip(void)
{
}
void MSRcStrip::CopyFromMe(MSRcStrip* pSource)
{
	if(pSource==NULL)	return; 
	m_Direction = pSource->m_Direction;
	m_IsMiddle = pSource->m_IsMiddle;
	m_FromRatio = pSource->m_FromRatio;
	m_ToRatio = pSource->m_ToRatio;
	m_CenBarType = pSource->m_CenBarType;
	
	m_Bar1.CopyFromMe(&pSource->m_Bar1);
	m_Bar2.CopyFromMe(&pSource->m_Bar2);
	m_Bar3.CopyFromMe(&pSource->m_Bar3);
	m_Bar4.CopyFromMe(&pSource->m_Bar4);
	m_Bar5.CopyFromMe(&pSource->m_Bar5);
	m_Bar6.CopyFromMe(&pSource->m_Bar6);
	m_Bar7.CopyFromMe(&pSource->m_Bar7);
	m_Bar8.CopyFromMe(&pSource->m_Bar8);
	m_Bar9.CopyFromMe(&pSource->m_Bar9);
	m_Bar10.CopyFromMe(&pSource->m_Bar10);
	m_nUseColumnStrip = pSource->m_nUseColumnStrip;     
	m_StripType = pSource->m_StripType;     

	m_IsNumPlaceBar4 = pSource->m_IsNumPlaceBar4;
	m_MainBar3.CopyFromMe(&pSource->m_MainBar3);
	m_MainBar4.CopyFromMe(&pSource->m_MainBar4);
}

void MSRcStrip::Dump(EFS::EFS_Buffer& buffer)
{
	MSObject::Dump(buffer);

	int StripType = (int)m_StripType;
	buffer << m_Direction << m_IsMiddle << m_FromRatio << m_ToRatio << m_CenBarType << m_CenBarTypeBot; 
	buffer << m_nUseColumnStrip << StripType;

	m_Bar6.Dump(buffer);
	m_Bar1.Dump(buffer);
	m_Bar2.Dump(buffer);
	m_Bar4.Dump(buffer);
	m_Bar5.Dump(buffer);
	m_Bar3.Dump(buffer);
	m_Bar7.Dump(buffer);
	m_Bar8.Dump(buffer);
	m_Bar9.Dump(buffer);
	m_Bar10.Dump(buffer);
	
	buffer << m_IsNumPlaceBar4;
	m_MainBar3.Dump(buffer);
	m_MainBar4.Dump(buffer);
}
void MSRcStrip::Recovery(EFS::EFS_Buffer& buffer)
{
	MSObject::Recovery(buffer);

	int StripType = 0;
	buffer >> m_Direction >> m_IsMiddle >> m_FromRatio >> m_ToRatio >> m_CenBarType >> m_CenBarTypeBot; 
	buffer >> m_nUseColumnStrip >> StripType;
	m_StripType = (STRIP_TYPE)StripType;

	m_Bar6.Recovery(buffer);
	m_Bar1.Recovery(buffer);
	m_Bar2.Recovery(buffer);
	m_Bar4.Recovery(buffer);
	m_Bar5.Recovery(buffer);
	m_Bar3.Recovery(buffer);
	m_Bar7.Recovery(buffer);
	m_Bar8.Recovery(buffer);
	m_Bar9.Recovery(buffer);
	m_Bar10.Recovery(buffer);

	buffer >> m_IsNumPlaceBar4;
	m_MainBar3.Recovery(buffer);
	m_MainBar4.Recovery(buffer);
}

void MSRcStrip::Serialize(CArchive &ar)
{
	MSRcObject::Serialize(ar);
	int StripType = 0;
	if(ar.IsStoring())
	{
		ar << m_Direction;
		ar << m_IsMiddle;
		ar << m_FromRatio;
		ar << m_ToRatio;
		ar << m_CenBarType; 
		ar << m_CenBarTypeBot; 

		m_Bar6.Serialize(ar);
		m_Bar1.Serialize(ar);
		m_Bar2.Serialize(ar);
		m_Bar4.Serialize(ar);
		m_Bar5.Serialize(ar);

		m_Bar3.Serialize(ar);

		ar << m_nUseColumnStrip;
		StripType = (STRIP_TYPE)m_StripType;
		ar << StripType;

		m_Bar7.Serialize(ar);
		m_Bar8.Serialize(ar);
		m_Bar9.Serialize(ar);
		m_Bar10.Serialize(ar);

		ar << m_IsNumPlaceBar4;
		m_MainBar3.Serialize(ar);
		m_MainBar4.Serialize(ar);
	}
	else
	{
		ar >> m_Direction;
		ar >> m_IsMiddle;
		ar >> m_FromRatio;
		ar >> m_ToRatio;
		ar >> m_CenBarType; 
		ar >> m_CenBarTypeBot; 

		m_Bar6.Serialize(ar);
		m_Bar1.Serialize(ar);
		m_Bar2.Serialize(ar);
		m_Bar4.Serialize(ar);
		m_Bar5.Serialize(ar);
		m_Bar3.Serialize(ar);

		ar >> m_nUseColumnStrip;
		ar >> StripType;
		m_StripType = (STRIP_TYPE)StripType;

		if(MSVersionInfo::GetCurrentVersion() >= 20150501)
		{
			m_Bar7.Serialize(ar);
			m_Bar8.Serialize(ar);
			m_Bar9.Serialize(ar);
			m_Bar10.Serialize(ar);
		}
		if(IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() >= 20221011)
		{
			ar >> m_IsNumPlaceBar4;
			m_MainBar3.Serialize(ar);
			m_MainBar4.Serialize(ar);
		}
	}
}

vector<long> MSRcStrip::GetBarDiaAllLayer()
{
	vector<long> DiaArr;

	//if(m_StripType == TYPE_A || m_StripType == TYPE_D || m_StripType == TYPE_E || m_StripType == TYPE_G || m_StripType == TYPE_I)
	//{
	//	vector<long> TopBar1DiaArr = m_TopBar1.GetBarDiaAllLayer();
	//	DiaArr.insert(DiaArr.end(), TopBar1DiaArr.begin(), TopBar1DiaArr.end());
	//}
	//if(m_StripType == TYPE_B || m_StripType == TYPE_C || m_StripType == TYPE_D || m_StripType == TYPE_E || m_StripType == TYPE_H)
	//{
	//	vector<long> TopBar2DiaArr = m_TopBar2.GetBarDiaAllLayer();
	//	DiaArr.insert(DiaArr.end(), TopBar2DiaArr.begin(), TopBar2DiaArr.end());
	//}
	//if(m_StripType == TYPE_C || m_StripType == TYPE_D || m_StripType == TYPE_F || m_StripType == TYPE_G || m_StripType == TYPE_H || m_StripType == TYPE_I)
	//{
	//	vector<long> BotBar3DiaArr = m_BotBar3.GetBarDiaAllLayer();
	//	DiaArr.insert(DiaArr.end(), BotBar3DiaArr.begin(), BotBar3DiaArr.end());
	//}
	//if(m_StripType == TYPE_A || m_StripType == TYPE_B || m_StripType == TYPE_C || m_StripType == TYPE_D || m_StripType == TYPE_E || m_StripType == TYPE_F || m_StripType == TYPE_G || m_StripType == TYPE_H || m_StripType == TYPE_I)
	//{
	//	vector<long> BotBar4DiaArr = m_BotBar4.GetBarDiaAllLayer();
	//	DiaArr.insert(DiaArr.end(), BotBar4DiaArr.begin(), BotBar4DiaArr.end());
	//}
	//if(m_StripType == TYPE_A || m_StripType == TYPE_B || m_StripType == TYPE_C || m_StripType == TYPE_D || m_StripType == TYPE_E || m_StripType == TYPE_F || m_StripType == TYPE_G || m_StripType == TYPE_H || m_StripType == TYPE_I)
	//{
	//	vector<long> TopBar5DiaArr = m_TopBar5.GetBarDiaAllLayer();
	//	DiaArr.insert(DiaArr.end(), TopBar5DiaArr.begin(), TopBar5DiaArr.end());
	//}
	//if(m_StripType == TYPE_A || m_StripType == TYPE_B || m_StripType == TYPE_C || m_StripType == TYPE_D || m_StripType == TYPE_E || m_StripType == TYPE_F || m_StripType == TYPE_G || m_StripType == TYPE_H || m_StripType == TYPE_I)
	//{
	//	vector<long> BotBar6DiaArr = m_BotBar6.GetBarDiaAllLayer();
	//	DiaArr.insert(DiaArr.end(), BotBar6DiaArr.begin(), BotBar6DiaArr.end());
	//}

	sort(DiaArr.begin(), DiaArr.end(), 
		[](const long& a, const long& b)->bool {return a < b;}
	);

	vector<long>::iterator new_end;
	new_end = unique(DiaArr.begin(), DiaArr.end());
	DiaArr.erase(new_end, DiaArr.end());

	return DiaArr;
}

bool MSRcStrip::GetRebarInfobyFlatSlabType(msBarSetType eBarSetType, vector<double>& DiaArr, double& dSpacing)
{
	//       TOP			  BOT
	// all: 1-2-3-5-8-9		4-6-7-10 
	// 	a : 1-5				4-6
	// 	b : 2-5				4-6
	//  c : 2-6				4-6-7
	//  d : 1-2-5			4-6-7
	//  e : 1-2-5			4-6
	//	f : 3-5				4-6
	//	g : 1-3-5			4-6
	//  H : 2-3-5			4-6
	//	I : 1-5				4-6-7

	// 1 : all,a,d,e,g,i
	// 2 : all,b,c,d,e,h
	// 3 : all,f,g,h
	// 4 : all,a,b,c,d,e,f,g,h,i
	// 5 : all,a,b,d,e,f,g,h,i
	// 6 : all,a,b,c,d,e,f,g,h,i
	// 7 : all, c, d, I
	// 8,9,10 : 

	bool isTop = false;
	switch (eBarSetType)
	{
	case SLAB_TOPLX:
	case SLAB_TOPLY:
		isTop = true;
		break;
	
	case SLAB_BOTLX:
	case SLAB_BOTLY:
		isTop = false;
		break;
	default:
		return false;
	}

	vector<double> TopDiaArr;
	vector<double> BotDiaArr;
	double dTopSpacing = 0;
	double dBotSpacing = 0;

	if (m_StripType == TYPE_ALL || m_StripType == TYPE_A || m_StripType == TYPE_D || m_StripType == TYPE_E || m_StripType == TYPE_G || m_StripType == TYPE_I)
	{
		vector<double> TopBar1DiaArr = m_Bar1.GetBarDiaAllLayer();
		TopDiaArr.insert(TopDiaArr.end(), TopBar1DiaArr.begin(), TopBar1DiaArr.end());
		dTopSpacing = __max(dTopSpacing, m_Bar1.GetSpacing());
	}
	if (m_StripType == TYPE_ALL || m_StripType == TYPE_B || m_StripType == TYPE_C || m_StripType == TYPE_D || m_StripType == TYPE_E || m_StripType == TYPE_H)
	{
		vector<double> TopBar2DiaArr = m_Bar2.GetBarDiaAllLayer();
		TopDiaArr.insert(TopDiaArr.end(), TopBar2DiaArr.begin(), TopBar2DiaArr.end());
		dTopSpacing = __max(dTopSpacing, m_Bar2.GetSpacing());
	}
	if (m_StripType == TYPE_ALL || m_StripType == TYPE_F || m_StripType == TYPE_G || m_StripType == TYPE_H)
	{
		vector<double> TopBar3DiaArr = m_Bar3.GetBarDiaAllLayer();
		TopDiaArr.insert(TopDiaArr.end(), TopBar3DiaArr.begin(), TopBar3DiaArr.end());
		dTopSpacing = __max(dTopSpacing, m_Bar3.GetSpacing());
	}
	if (m_StripType == TYPE_ALL || m_StripType == TYPE_A || m_StripType == TYPE_B || m_StripType == TYPE_C || m_StripType == TYPE_D || m_StripType == TYPE_E || m_StripType == TYPE_F || m_StripType == TYPE_G || m_StripType == TYPE_H || m_StripType == TYPE_I)
	{
		vector<double> BotBar4DiaArr = m_Bar4.GetBarDiaAllLayer();
		BotDiaArr.insert(BotDiaArr.end(), BotBar4DiaArr.begin(), BotBar4DiaArr.end());
		dBotSpacing = __max(dBotSpacing, m_Bar4.GetSpacing());
	}
	if (m_StripType == TYPE_ALL || m_StripType == TYPE_A || m_StripType == TYPE_B || m_StripType == TYPE_C || m_StripType == TYPE_D || m_StripType == TYPE_E || m_StripType == TYPE_F || m_StripType == TYPE_G || m_StripType == TYPE_H || m_StripType == TYPE_I)
	{
		vector<double> TopBar5DiaArr = m_Bar5.GetBarDiaAllLayer();
		TopDiaArr.insert(TopDiaArr.end(), TopBar5DiaArr.begin(), TopBar5DiaArr.end());
		dTopSpacing = __max(dTopSpacing, m_Bar5.GetSpacing());
	}
	if (m_StripType == TYPE_ALL || m_StripType == TYPE_A || m_StripType == TYPE_B || m_StripType == TYPE_C || m_StripType == TYPE_D || m_StripType == TYPE_E || m_StripType == TYPE_F || m_StripType == TYPE_G || m_StripType == TYPE_H || m_StripType == TYPE_I)
	{
		vector<double> BotBar6DiaArr = m_Bar6.GetBarDiaAllLayer();
		BotDiaArr.insert(BotDiaArr.end(), BotBar6DiaArr.begin(), BotBar6DiaArr.end());
		dBotSpacing = __max(dBotSpacing, m_Bar6.GetSpacing());
	}

	if (m_StripType == TYPE_ALL || m_StripType == TYPE_A || m_StripType == TYPE_B || m_StripType == TYPE_C || m_StripType == TYPE_D || m_StripType == TYPE_E || m_StripType == TYPE_F || m_StripType == TYPE_G || m_StripType == TYPE_H || m_StripType == TYPE_I)
	{
		vector<double> BotBar7DiaArr = m_Bar7.GetBarDiaAllLayer();
		BotDiaArr.insert(BotDiaArr.end(), BotBar7DiaArr.begin(), BotBar7DiaArr.end());
		dBotSpacing = __max(dBotSpacing, m_Bar7.GetSpacing());
	}

	if (m_StripType == TYPE_ALL || m_StripType == TYPE_A || m_StripType == TYPE_B || m_StripType == TYPE_C || m_StripType == TYPE_D || m_StripType == TYPE_E || m_StripType == TYPE_F || m_StripType == TYPE_G || m_StripType == TYPE_H || m_StripType == TYPE_I)
	{
		vector<double> TopBar8DiaArr = m_Bar8.GetBarDiaAllLayer();
		TopDiaArr.insert(TopDiaArr.end(), TopBar8DiaArr.begin(), TopBar8DiaArr.end());
		dTopSpacing = __max(dTopSpacing, m_Bar8.GetSpacing());
	}

	if (m_StripType == TYPE_ALL || m_StripType == TYPE_A || m_StripType == TYPE_B || m_StripType == TYPE_C || m_StripType == TYPE_D || m_StripType == TYPE_E || m_StripType == TYPE_F || m_StripType == TYPE_G || m_StripType == TYPE_H || m_StripType == TYPE_I)
	{
		vector<double> TopBar9DiaArr = m_Bar9.GetBarDiaAllLayer();
		TopDiaArr.insert(TopDiaArr.end(), TopBar9DiaArr.begin(), TopBar9DiaArr.end());
		dTopSpacing = __max(dTopSpacing, m_Bar9.GetSpacing());
	}

	if (m_StripType == TYPE_ALL || m_StripType == TYPE_A || m_StripType == TYPE_B || m_StripType == TYPE_C || m_StripType == TYPE_D || m_StripType == TYPE_E || m_StripType == TYPE_F || m_StripType == TYPE_G || m_StripType == TYPE_H || m_StripType == TYPE_I)
	{
		vector<double> BotBar10DiaArr = m_Bar10.GetBarDiaAllLayer();
		BotDiaArr.insert(BotDiaArr.end(), BotBar10DiaArr.begin(), BotBar10DiaArr.end());
		dBotSpacing = __max(dBotSpacing, m_Bar10.GetSpacing());
	}

	sort(TopDiaArr.begin(), TopDiaArr.end(),
		[](const double& a, const double& b)->bool {return a > b; }
	);

	vector<double>::iterator new_end;
	new_end = unique(TopDiaArr.begin(), TopDiaArr.end());
	TopDiaArr.erase(new_end, TopDiaArr.end());

	sort(BotDiaArr.begin(), BotDiaArr.end(),
		[](const double& a, const double& b)->bool {return a > b; }
	);

	new_end = unique(BotDiaArr.begin(), BotDiaArr.end());
	BotDiaArr.erase(new_end, BotDiaArr.end());

	if (isTop)
	{
		DiaArr = TopDiaArr;
		dSpacing = dTopSpacing;
	}
	else
	{
		DiaArr = BotDiaArr;
		dSpacing = dBotSpacing;

	}

	if (DiaArr.size() > 1)
		dSpacing = dSpacing / 2;
	return isTop;
}

