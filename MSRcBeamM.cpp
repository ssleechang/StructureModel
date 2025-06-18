#include "StdAfx.h"
#include "MSRcBeamM.h"
#include "GMLib/MSVersionInfo.h"

IMPLEMENT_SERIAL(MSRcBeamM, MDMember, VERSIONABLE_SCHEMA | 1)
MSRcBeamM::MSRcBeamM(void)
{
	me_SecCombi = ALL;

	mp_SmallSec = NULL;
	mp_MidSec = NULL;
	mp_LargeSec = NULL;		
	m_bCrossBar = FALSE;
	m_IsPC = FALSE;

	m_IsSubBeam = false;
	m_IsCornerPlacing = false;
	CreateSections();

	CString sBar = _T("");
	m_IsDiagonalAddBar = false;
	m_nDiagonalAddBarType = 0;
	sBar = MSDeformedBar::GetMainBarStr(4, 25);
	m_DiagonalBar.InitData(sBar);
	// 스터럽
	sBar = MSDeformedBar::GetStirrupBarStr(300, 13);
	m_DiagonalStirrupBar.InitData(sBar);
	m_nDiagonalStirrupBarNum = 1;
	// STR폭
	m_nStrHor = 100;
	// STR춤
	m_nStrVer = 100;

	m_nBeamMType = BeamM_TYPE::General;
}

MSRcBeamM::~MSRcBeamM(void)
{
	if(mp_LargeSec)	delete mp_LargeSec;
	if(mp_MidSec)		delete mp_MidSec;
	if(mp_SmallSec)	delete mp_SmallSec;
}

void MSRcBeamM::CopyFromMe(MDMember* pSource)
{
	if(pSource==NULL)	return;
	MSRcBeamM * pSourceBeamM = (MSRcBeamM*)pSource;
	me_SecCombi = pSourceBeamM->me_SecCombi;
	
	CreateSections();
	
	mp_SmallSec->CopyFromMe(pSourceBeamM->mp_SmallSec);
	mp_MidSec->CopyFromMe(pSourceBeamM->mp_MidSec);
	mp_LargeSec->CopyFromMe(pSourceBeamM->mp_LargeSec);

	m_bCrossBar = pSourceBeamM->m_bCrossBar;	
	m_IsPC = pSourceBeamM->m_IsPC;
	m_IsSubBeam = pSourceBeamM->GetIsSubBeam();
	m_IsCornerPlacing = pSourceBeamM->m_IsCornerPlacing;

	ma_LinkedExterior.resize((int)pSourceBeamM->ma_LinkedExterior.size());
	std::copy(pSourceBeamM->ma_LinkedExterior.begin(), pSourceBeamM->ma_LinkedExterior.end(), ma_LinkedExterior.begin());

	ma_LinkedInterior.resize((int)pSourceBeamM->ma_LinkedInterior.size());
	std::copy(pSourceBeamM->ma_LinkedInterior.begin(), pSourceBeamM->ma_LinkedInterior.end(), ma_LinkedInterior.begin());


	// 대각보강근
	m_IsDiagonalAddBar = pSourceBeamM->GetIsDiagonalAddBar();
	m_nDiagonalAddBarType = pSourceBeamM->GetDiagonalAddBarType();
	m_DiagonalBar.CopyFromMe(pSourceBeamM->GetDiagonalBar());
	m_DiagonalStirrupBar.CopyFromMe(pSourceBeamM->GetDiagonalStirrupBar());
	m_nDiagonalStirrupBarNum = pSourceBeamM->GetDiagonalStirrupBarNum();
	m_nStrHor = pSourceBeamM->GetSTRHor();
	m_nStrVer = pSourceBeamM->GetSTRVer();

	m_nBeamMType = pSourceBeamM->m_nBeamMType;

	MDMember::CopyFromMe(pSource);
}

void MSRcBeamM::Dump(EFS::EFS_Buffer& buffer)
{
	MDMember::Dump(buffer);

	int secCombi = (int)me_SecCombi;
	buffer << secCombi << m_bCrossBar << m_IsPC;
	mp_LargeSec->Dump(buffer);
 	mp_MidSec->Dump(buffer);
 	mp_SmallSec->Dump(buffer);
	buffer << m_IsSubBeam;
	buffer << m_IsCornerPlacing;

	int nExterior = ma_LinkedExterior.size();
	buffer << nExterior;
	for(int i = 0; i < nExterior; i++)
		buffer << ma_LinkedExterior[i];

	int nInterior = ma_LinkedInterior.size();
	buffer << nInterior;
	for(int i = 0; i < nInterior; i++)
		buffer << ma_LinkedInterior[i];

	// 대각보강근
	buffer << m_IsDiagonalAddBar;
	buffer << m_nDiagonalAddBarType;
	m_DiagonalBar.Dump(buffer);
	m_DiagonalStirrupBar.Dump(buffer);
	buffer << m_nDiagonalStirrupBarNum;
	buffer << m_nStrHor;
	buffer << m_nStrVer;

	buffer << m_nBeamMType;
}

void MSRcBeamM::Recovery(EFS::EFS_Buffer& buffer)
{
	MDMember::Recovery(buffer);
	int secCombi = 0;
	buffer >> secCombi >> m_bCrossBar >> m_IsPC;
	me_SecCombi = (SEC_COMBI)secCombi;
	mp_LargeSec->Recovery(buffer);
 	mp_MidSec->Recovery(buffer);
 	mp_SmallSec->Recovery(buffer);

	buffer >> m_IsSubBeam;
	buffer >> m_IsCornerPlacing;

	CString sData;
	int nExterior = 0;
	buffer >> nExterior;
	ma_LinkedExterior.clear();
	for(int i = 0; i < nExterior; i++)
	{
		buffer >> sData;
		ma_LinkedExterior.push_back(sData);
	}

	int nInterior = 0;
	buffer >> nInterior;
	ma_LinkedInterior.clear();
	for(int i = 0; i < nInterior; i++)
	{
		buffer >> sData;
		ma_LinkedInterior.push_back(sData);
	}

	// 대각보강근
	buffer >> m_IsDiagonalAddBar;
	buffer >> m_nDiagonalAddBarType;
	m_DiagonalBar.Recovery(buffer);
	m_DiagonalStirrupBar.Recovery(buffer);
	buffer >> m_nDiagonalStirrupBarNum;
	buffer >> m_nStrHor;
	buffer >> m_nStrVer;

	buffer >> m_nBeamMType;
}

void MSRcBeamM::Serialize(CArchive &ar)
{
	MDMember::Serialize(ar);
	long nZero = 0;
	long nOne = 1;
	int nExterior = 0;
	int nInterior = 0;
	if(ar.IsStoring())
	{
		ar << (long)me_SecCombi;		
		ar << m_bCrossBar;
		// 20120730
		ar << m_IsPC;

		if(mp_LargeSec)
		{
			ar << nOne;
			mp_LargeSec->Serialize(ar);
		}
		else				
			ar << nZero;
		if(mp_MidSec)
		{
			ar << nOne;
			mp_MidSec->Serialize(ar);
		}
		else
			ar << nZero;
		if(mp_SmallSec)
		{
			ar << nOne;
			mp_SmallSec->Serialize(ar);
		}
		else				
			ar << nZero;

		ar << m_IsSubBeam;
		ar << m_IsCornerPlacing;

		nExterior = ma_LinkedExterior.size();
		ar << nExterior;
		for(int i = 0; i < nExterior; i++)
			ar << ma_LinkedExterior[i];

		nInterior = ma_LinkedInterior.size();
		ar << nInterior;
		for(int i = 0; i < nInterior; i++)
			ar << ma_LinkedInterior[i];

#pragma region 특수 전단벽 DB
		// 대각보강근
		ar << m_IsDiagonalAddBar;
		ar << m_nDiagonalAddBarType;
		m_DiagonalBar.Serialize(ar);
		m_DiagonalStirrupBar.Serialize(ar);
		ar << m_nDiagonalStirrupBarNum;
		ar << m_nStrHor;
		ar << m_nStrVer;
#pragma endregion

#pragma region 전이 부재
		ar << m_nBeamMType;
#pragma endregion
	}
	else
	{
		long nSecCombi = 0;
		ar >> nSecCombi;
		if(nSecCombi==0)			me_SecCombi = ALL;
		else if(nSecCombi==1)	me_SecCombi = SMALL_BIGGER;
		else if(nSecCombi==2)	me_SecCombi = ALL_DIFF;
		else if(nSecCombi==3)	me_SecCombi = BOTH_MID;
		else if(nSecCombi==4)	me_SecCombi = SMALL_MID;

		ar >> m_bCrossBar;
// 		// 20120730
 		ar >> m_IsPC;

		long nValue;
		ar >> nValue;
		if(nValue)
		{
			delete mp_LargeSec;
			mp_LargeSec = new MSRcBeamSec;
			mp_LargeSec->Serialize(ar);
		}
		ar >> nValue;
		if(nValue)
		{
			delete mp_MidSec;
			mp_MidSec = new MSRcBeamSec;
			mp_MidSec->Serialize(ar);
		}
		ar >> nValue;
		if(nValue)
		{
			delete mp_SmallSec;
			mp_SmallSec = new MSRcBeamSec;
			mp_SmallSec->Serialize(ar);
		}

		if(MSVersionInfo::GetCurrentVersion() <= 20140915)
			RegenerateSectionCombination();
		if(MSVersionInfo::GetCurrentVersion() >= 20150420)
			ar >> m_IsSubBeam;
		if (MSVersionInfo::GetCurrentVersion() >= 20150805)
			ar >> m_IsCornerPlacing;

		if (MSVersionInfo::GetCurrentVersion() >= 20160404)
		{
			CString sData;
			ar >> nExterior;
			for(int i = 0; i < nExterior; i++)
			{		
				ar >> sData;
				ma_LinkedExterior.push_back(sData );
			}
			ar >> nInterior; 
			for(int i = 0; i < nInterior; i++)
			{
				ar >> sData;
				ma_LinkedInterior.push_back(sData);
			}
		}

#pragma region 특수 전단벽 DB
		if (IsMVAddToBHA() && MSVersionInfo::GetCurrentVersion() >= 20231010)
		{
			// 대각보강근
			ar >> m_IsDiagonalAddBar;
			ar >> m_nDiagonalAddBarType;
			m_DiagonalBar.Serialize(ar);
			m_DiagonalStirrupBar.Serialize(ar);
			ar >> m_nDiagonalStirrupBarNum;
			ar >> m_nStrHor;
			ar >> m_nStrVer;
		}
#pragma endregion

#pragma region 전이 부재
		if(MSVersionInfo::GetCurrentVersion() >= 20240626)
			ar >> m_nBeamMType;
#pragma endregion
	}
}
void MSRcBeamM::SetSecCombi(long Type)
{	
	if(Type == ALL)						me_SecCombi = ALL;
	else if(Type == BOTH_MID)	me_SecCombi = BOTH_MID;
	else if(Type == ALL_DIFF)	me_SecCombi = ALL_DIFF;
	else if(Type == SMALL_BIGGER)	me_SecCombi = SMALL_BIGGER;
	else if(Type == SMALL_MID)	me_SecCombi = SMALL_MID;	
}

void MSRcBeamM::CreateSections()
{
	if(mp_SmallSec==NULL)	mp_SmallSec = new MSRcBeamSec;
	if(mp_MidSec==NULL)		mp_MidSec = new MSRcBeamSec;
	if(mp_LargeSec==NULL)	mp_LargeSec = new MSRcBeamSec;
}

BOOL MSRcBeamM::IsPC()
{
	return GetPC();
}

double MSRcBeamM::GetMainBarDiaByPC()
{
	return max(max(mp_SmallSec->GetMaxMainBarDia(true), mp_MidSec->GetMaxMainBarDia(true)), mp_LargeSec->GetMaxMainBarDia(true));
}

double MSRcBeamM::GetMainBarDia()
{
	return max(max(mp_SmallSec->GetMaxMainBarDia(false), mp_MidSec->GetMaxMainBarDia(false)), mp_LargeSec->GetMaxMainBarDia(false));
}
double MSRcBeamM::getStrpBarDia()
{
	double dSmallDia = mp_SmallSec->m_SBars.GetDeformedBarDiameter();
	double dMidDia = mp_MidSec->m_SBars.GetDeformedBarDiameter();
	double dLargeDia = mp_LargeSec->m_SBars.GetDeformedBarDiameter();
	return max(max(dSmallDia, dMidDia), dLargeDia);
}

int MSRcBeamM::RegenerateSectionCombination()
{
	me_SecCombi = ALL_DIFF;
	bool IsSameDataSM = mp_SmallSec->IsSameRebarData(mp_MidSec);
	bool IsSameDataSL = mp_SmallSec->IsSameRebarData(mp_LargeSec);
	bool IsSameDataML = mp_MidSec->IsSameRebarData(mp_LargeSec);

//	enum SEC_COMBI {ALL=0, SMALL_BIGGER, ALL_DIFF, BOTH_MID, SMALL_MID};
// 모두 같은 단면;외단부 = 내단부;모두 다른 단면;내단부 = 중앙부;외단부 = 중앙부;
	if(IsSameDataSM && IsSameDataSL && IsSameDataML)
		me_SecCombi = ALL;
	else if(IsSameDataSM == false && IsSameDataSL == false && IsSameDataML == false)
		me_SecCombi = ALL_DIFF;
	else
	{
		if(IsSameDataSL && IsSameDataSM == false)
			me_SecCombi = SMALL_BIGGER;
		else if(IsSameDataML && IsSameDataSL == false)
			me_SecCombi = BOTH_MID;
		else if(IsSameDataSM && IsSameDataSL == false && IsSameDataML == false)
			me_SecCombi = SMALL_MID;
	}
	return (int)me_SecCombi;
}

vector<double> MSRcBeamM::GetMainBarDiaAllLayer()
{
	vector<double> DiaArr;

	vector<double> SmallSecDiaArr = mp_SmallSec->GetMainBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), SmallSecDiaArr.begin(), SmallSecDiaArr.end());

	vector<double> MidSecDiaArr = mp_MidSec->GetMainBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), MidSecDiaArr.begin(), MidSecDiaArr.end());

	vector<double> LargeSecDiaArr = mp_LargeSec->GetMainBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), LargeSecDiaArr.begin(), LargeSecDiaArr.end());

	sort(DiaArr.begin(), DiaArr.end(), 
		[](const long& a, const long& b)->bool {return a < b;}
	);

	vector<double>::iterator new_end;
	new_end = unique(DiaArr.begin(), DiaArr.end());
	DiaArr.erase(new_end, DiaArr.end());

	return DiaArr;
}

vector<double> MSRcBeamM::GetStirrupBarDiaAllLayer()
{
	vector<double> DiaArr;

	vector<double> SmallSecDiaArr = mp_SmallSec->GetStirrupBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), SmallSecDiaArr.begin(), SmallSecDiaArr.end());

	vector<double> MidSecDiaArr = mp_MidSec->GetStirrupBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), MidSecDiaArr.begin(), MidSecDiaArr.end());

	vector<double> LargeSecDiaArr = mp_LargeSec->GetStirrupBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), LargeSecDiaArr.begin(), LargeSecDiaArr.end());

	sort(DiaArr.begin(), DiaArr.end(), 
		[](const long& a, const long& b)->bool {return a < b;}
	);

	vector<double>::iterator new_end;
	new_end = unique(DiaArr.begin(), DiaArr.end());
	DiaArr.erase(new_end, DiaArr.end());

	return DiaArr;
}

vector<CString> MSRcBeamM::GetLinkedExteriors()
{
	return ma_LinkedExterior;
}

vector<CString> MSRcBeamM::GetLinkedInteriors()
{
	return ma_LinkedInterior;
}

void MSRcBeamM::SetLinkedExteriors(vector<CString>& linkeds)
{
	ma_LinkedExterior.resize((int)linkeds.size());
	std::copy(linkeds.begin(), linkeds.end(), ma_LinkedExterior.begin());
}

void MSRcBeamM::SetLinkedInteriors(vector<CString>& linkeds)
{
	ma_LinkedInterior.resize((int)linkeds.size());
	std::copy(linkeds.begin(), linkeds.end(), ma_LinkedInterior.begin());
}
MSRcBeamM::SEC_COMBI MSRcBeamM::GetSecCombi()
{
	return me_SecCombi;
}

#pragma region 전이 부재
int MSRcBeamM::GetBeamMType()
{
	return m_nBeamMType;
}

void MSRcBeamM::SetBeamMType(int nType)
{
	m_nBeamMType = nType;
}

bool MSRcBeamM::IsTransfer()
{
	if(m_nBeamMType == BeamM_TYPE::Transfer)
		return true;

	return false;
}
#pragma endregion

bool MSRcBeamM::IsDeck()
{
	if (m_nBeamMType == BeamM_TYPE::Deck)
		return true;

	return false;
}