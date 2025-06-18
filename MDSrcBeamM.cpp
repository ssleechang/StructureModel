#include "StdAfx.h"
#include "MDSrcBeamM.h"
#include "GMLib/MSVersionInfo.h"

IMPLEMENT_SERIAL(MDSrcBeamM, MDMember, VERSIONABLE_SCHEMA | 1)
	MDSrcBeamM::MDSrcBeamM(void)
{
	mp_SmallSec = NULL;
	mp_MidSec = NULL;
	mp_LargeSec = NULL;		
	m_bCrossBar = FALSE;
	m_IsCornerPlacing = false;
	CreateSections();
}

MDSrcBeamM::~MDSrcBeamM(void)
{
	if(mp_LargeSec)	delete mp_LargeSec;
	if(mp_MidSec)		delete mp_MidSec;
	if(mp_SmallSec)	delete mp_SmallSec;
}

void MDSrcBeamM::CopyFromMe(MDMember* pSource)
{
	if(pSource==NULL)	return;
	MDSrcBeamM * pSourceBeamM = (MDSrcBeamM*)pSource;
	
	CreateSections();
	
	mp_SmallSec->CopyFromMe(pSourceBeamM->mp_SmallSec);
	mp_MidSec->CopyFromMe(pSourceBeamM->mp_MidSec);
	mp_LargeSec->CopyFromMe(pSourceBeamM->mp_LargeSec);

	m_bCrossBar = pSourceBeamM->m_bCrossBar;	
	m_IsCornerPlacing = pSourceBeamM->m_IsCornerPlacing;

	ma_LinkedExterior.resize((int)pSourceBeamM->ma_LinkedExterior.size());
	std::copy(pSourceBeamM->ma_LinkedExterior.begin(), pSourceBeamM->ma_LinkedExterior.end(), ma_LinkedExterior.begin());

	ma_LinkedInterior.resize((int)pSourceBeamM->ma_LinkedInterior.size());
	std::copy(pSourceBeamM->ma_LinkedInterior.begin(), pSourceBeamM->ma_LinkedInterior.end(), ma_LinkedInterior.begin());

	MDMember::CopyFromMe(pSource);
}

void MDSrcBeamM::Dump(EFS::EFS_Buffer& buffer)
{
	MDMember::Dump(buffer);

	buffer << m_bCrossBar ;
	mp_LargeSec->Dump(buffer);
 	mp_MidSec->Dump(buffer);
 	mp_SmallSec->Dump(buffer);
	buffer << m_IsCornerPlacing;

	int nExterior = ma_LinkedExterior.size();
	buffer << nExterior;
	for(int i = 0; i < nExterior; i++)
		buffer << ma_LinkedExterior[i];

	int nInterior = ma_LinkedInterior.size();
	buffer << nInterior;
	for(int i = 0; i < nInterior; i++)
		buffer << ma_LinkedInterior[i];

}
void MDSrcBeamM::Recovery(EFS::EFS_Buffer& buffer)
{
	MDMember::Recovery(buffer);
	buffer >> m_bCrossBar;
	mp_LargeSec->Recovery(buffer);
 	mp_MidSec->Recovery(buffer);
 	mp_SmallSec->Recovery(buffer);
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

}

void MDSrcBeamM::Serialize(CArchive &ar)
{
	MDMember::Serialize(ar);
	long nZero = 0;
	long nOne = 1;
	int nExterior = 0;
	int nInterior = 0;
	if(ar.IsStoring())
	{
		ar << m_bCrossBar;
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

		ar << m_IsCornerPlacing;

		nExterior = ma_LinkedExterior.size();
		ar << nExterior;
		for(int i = 0; i < nExterior; i++)
			ar << ma_LinkedExterior[i];

		nInterior = ma_LinkedInterior.size();
		ar << nInterior;
		for(int i = 0; i < nInterior; i++)
			ar << ma_LinkedInterior[i];

	}
	else
	{
		ar >> m_bCrossBar;
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

	}
}
void MDSrcBeamM::CreateSections()
{
	if(mp_SmallSec==NULL)	mp_SmallSec = new MSRcBeamSec;
	if(mp_MidSec==NULL)		mp_MidSec = new MSRcBeamSec;
	if(mp_LargeSec==NULL)	mp_LargeSec = new MSRcBeamSec;
}

double MDSrcBeamM::GetMainBarDia()
{
	double dSmallSecDia = mp_SmallSec->m_BBars.GetMaxDeformedBarDiameter();
	double dMidSecDia = mp_MidSec->m_BBars.GetMaxDeformedBarDiameter();
	double dLargeSecDia = mp_LargeSec->m_BBars.GetMaxDeformedBarDiameter();

	return max(max(dSmallSecDia, dMidSecDia), dLargeSecDia);
}
double MDSrcBeamM::getStrpBarDia()
{
	double dSmallSecDia = mp_SmallSec->m_SBars.GetDeformedBarDiameter();
	double dMidSecDia = mp_MidSec->m_SBars.GetDeformedBarDiameter();
	double dLargeSecDia = mp_LargeSec->m_SBars.GetDeformedBarDiameter();

	return max(max(dSmallSecDia, dMidSecDia), dLargeSecDia);
}

vector<double> MDSrcBeamM::GetMainBarDiaAllLayer()
{
	vector<double> DiaArr;

	vector<double> SmallSecDiaArr = mp_SmallSec->GetMainBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), SmallSecDiaArr.begin(), SmallSecDiaArr.end());

	vector<double> MidSecDiaArr = mp_MidSec->GetMainBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), MidSecDiaArr.begin(), MidSecDiaArr.end());

	vector<double> LargeSecDiaArr = mp_LargeSec->GetMainBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), LargeSecDiaArr.begin(), LargeSecDiaArr.end());

	sort(DiaArr.begin(), DiaArr.end(), 
		[](const double& a, const double& b)->bool {return a < b;}
	);

	vector<double>::iterator new_end;
	new_end = unique(DiaArr.begin(), DiaArr.end());
	DiaArr.erase(new_end, DiaArr.end());

	return DiaArr;
}

vector<double> MDSrcBeamM::GetStirrupBarDiaAllLayer()
{
	vector<double> DiaArr;

	vector<double> SmallSecDiaArr = mp_SmallSec->GetStirrupBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), SmallSecDiaArr.begin(), SmallSecDiaArr.end());

	vector<double> MidSecDiaArr = mp_MidSec->GetStirrupBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), MidSecDiaArr.begin(), MidSecDiaArr.end());

	vector<double> LargeSecDiaArr = mp_LargeSec->GetStirrupBarDiaAllLayer();
	DiaArr.insert(DiaArr.end(), LargeSecDiaArr.begin(), LargeSecDiaArr.end());

	sort(DiaArr.begin(), DiaArr.end(), 
		[](const double& a, const double& b)->bool {return a < b;}
	);

	vector<double>::iterator new_end;
	new_end = unique(DiaArr.begin(), DiaArr.end());
	DiaArr.erase(new_end, DiaArr.end());

	return DiaArr;
}

bool MDSrcBeamM::HasSubMainBar()
{
	if (mp_MidSec == NULL)
		return false;

	long NumSubMainBar = mp_MidSec->m_SubMainBar.ma_Loc.size();

	if (NumSubMainBar == 0)
		return false;

	return true;
}

MSSubMainBar* MDSrcBeamM::GetSubMainBar()
{
	if (mp_MidSec == NULL)
		return NULL;

	return &mp_MidSec->m_SubMainBar;
}

vector<CString> MDSrcBeamM::GetLinkedExteriors()
{
	return ma_LinkedExterior;
}

vector<CString> MDSrcBeamM::GetLinkedInteriors()
{
	return ma_LinkedInterior;
}

void MDSrcBeamM::SetLinkedExteriors(vector<CString>& linkeds)
{
	ma_LinkedExterior.resize((int)linkeds.size());
	std::copy(linkeds.begin(), linkeds.end(), ma_LinkedExterior.begin());
}

void MDSrcBeamM::SetLinkedInteriors(vector<CString>& linkeds)
{
	ma_LinkedInterior.resize((int)linkeds.size());
	std::copy(linkeds.begin(), linkeds.end(), ma_LinkedInterior.begin());
}

