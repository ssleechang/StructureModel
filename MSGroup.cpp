#include "StdAfx.h"
#include "MSGroup.h"
#include "MSBarSet.h"
//#include "GMLib/MSVersionInfo.h"
#include "MSRebarBuilding.h"
#include "MSFloor.h"
#include "XBarSetInfo.h"

IMPLEMENT_SERIAL( MSGroup, MSContainer, VERSIONABLE_SCHEMA | 1 )
MSGroup::MSGroup(void)
{
	Init(true);
}

MSGroup::~MSGroup(void)
{
	DeleteBarSets();
	mm_CompoG.clear();
	DeleteBarSetInfoArr();
}

void MSGroup::DeleteBarSets()
{
	//map<long, MSCompoG*>::iterator itCompoG;
	//for(itCompoG=mm_CompoG.begin(); itCompoG != mm_CompoG.end(); itCompoG++)
	//{
	//	MSCompoG* pCompoG = itCompoG->second;
	//	if(pCompoG == NULL)	continue;
	//	map<long, MSBarSet*> BarSetMap = pCompoG->GetBarsets();
	//	BarSetMap.clear();
	//}

	vector<MSBarSet*>::iterator it;
	for(it=ma_BarSet.begin(); it != ma_BarSet.end(); it++)
		delete *it;
	ma_BarSet.clear();
}
void MSGroup::DeleteBarSetInfoArr()
{
	vector<XBarSetInfo*>::iterator it;
	for(it=ma_BarSetInfo.begin(); it != ma_BarSetInfo.end(); it++)
		delete *it;
	ma_BarSetInfo.clear();
}

bool MSGroup::WriteFileBinary()
{
	CString strFilter	= _T("*.rbr");
	if(m_strFilePath==_T(""))
	{
		CFileDialog FileDlg(TRUE, _T(""), strFilter);
		if(FileDlg.DoModal() == IDCANCEL)
			return false;

		m_strFilePath = FileDlg.GetPathName(); 
	}
	CFile file;
	CFileException e;
	if(file.Open(m_strFilePath, CFile::modeCreate | CFile::modeWrite, &e)==FALSE)
	{
		{
#ifdef _DEBUG
			afxDump << "File could not be opened " << e.m_cause << "\n";
#endif
		}
	}
	CArchive ar(&file, CArchive::store);
	Serialize(ar);
	return true;
}

bool MSGroup::ReadFileBinary(CString strFilePath)
{
	CFile file;
	CFileException e;
	if(file.Open(strFilePath, CFile::modeRead, &e)==FALSE)
	{
#ifdef _DEBUG
		afxDump << "File could not be opened " << e.m_cause << "\n";
#endif
		return false;
	}
	CArchive ar(&file, CArchive::load);
	Serialize(ar);

	return true;
}

bool MSGroup::Add(MSBarSet* pBarSet)
{
	ma_BarSet.push_back(pBarSet);
	return true;
}

int MSGroup::AddBarSetInfo(XBarSetInfo* pInfo)
{
	ma_BarSetInfo.push_back(pInfo);
	return true;
}

void MSGroup::Serialize(CArchive &ar)
{
	
//	if(ar.IsStoring())
//	{
//		ar << MSVersionInfo::GetLatestVersion();
//		
//		// MSContainer::Serialize(ar);
//		MSCompoG::Serialize(ar);
//
//		ar << (int)mm_CompoG.size();
//		map<long, MSCompoG*>::iterator itCompoG;
//		for(itCompoG=mm_CompoG.begin(); itCompoG != mm_CompoG.end(); itCompoG++)
//		{
//			MSCompoG* pCompoG = itCompoG->second;
//			MSContainer* pOwner = pCompoG->GetOwner();
//			if(pOwner==NULL)	continue;
//			if(pOwner->GetType() != MSCompoG::msFloor)
//			{
//				ASSERT(0);
//			}
//			ar << pOwner->m_ID;
//			ar << pCompoG->m_ID;
//		}
//		
//		ar << (int)ma_BarSet.size();
//		vector<MSBarSet*>::iterator it;
//		for(it=ma_BarSet.begin(); it != ma_BarSet.end(); it++)
//			(*it)->Serialize(ar);
//
//		ar << m_nGroupType;
//		ar << m_nFloorNum;
//
//		vector<XBarSetInfo*>::iterator itInfo;
//		ar << (int)ma_BarSetInfo.size();
//		for(itInfo=ma_BarSetInfo.begin(); itInfo != ma_BarSetInfo.end(); itInfo++)
//			(*itInfo)->Serialize(ar);
//
//	}
//	else
//	{
//		int version;
//		ar >> version;
//		MSVersionInfo::PushCurrentVersion(version);
//
////		MSContainer::Serialize(ar);
//
//		long nOwnerID, nID, NumObject;
//		MSCompoG::Serialize(ar);
//		ar >> NumObject;
//		mm_CompoG.clear();
//		for(int iCompoG=0; iCompoG<NumObject; iCompoG++)
//		{
//			ar >> nOwnerID;
//			ar >> nID;
//			MSBaseBuilding* pBldg = (MSBaseBuilding*)GetOwner();
//			MSFloor* pFloor = pBldg->GetFloorByID(nOwnerID);
//			if(pFloor==NULL)	continue;
//			MSCompoG* pCompoG = pFloor->GetCompoG(nID);
//			if(pCompoG==NULL)	continue;
//			mm_CompoG.insert(make_pair(pCompoG->m_ID, pCompoG));
//		}
//
//		ar >> NumObject;
//		DeleteBarSets();
//		for(long iBarSet=0; iBarSet<NumObject; iBarSet++)
//		{
//			MSBarSet* pBarSet = new MSBarSet;
//			pBarSet->mp_Bldg = (MSBaseBuilding*)GetOwner();
//			pBarSet->Serialize(ar);
//			Add(pBarSet);
//		}
//
//		if(MSVersionInfo::GetCurrentVersion() >= 20121018)
//		{
//			ar >> m_nGroupType;
//			ar >> m_nFloorNum;
//		}
//		if(MSVersionInfo::GetCurrentVersion() >= 20130111)
//		{
//			long NumInfo = 0;
//			ar >> NumInfo;
//			for(long iInfo=0; iInfo<NumInfo; iInfo++)
//			{
//				XBarSetInfo* pInfo = new XBarSetInfo;
//				pInfo->Serialize(ar);
//				AddBarSetInfo(pInfo);
//			}
//		}
//
//		MSVersionInfo::PopCurrentVersion();
//	}
}

void MSGroup::ConvertBarSet4Member()
{
//	vector<MSBarSet*>::iterator itBarSet;
//	for(itBarSet=ma_BarSet.begin(); itBarSet != ma_BarSet.end(); itBarSet++)
//	{
//		MSBarSet* pBarSet = *itBarSet;
//		if(pBarSet->mp_Owner)	pBarSet->mp_Owner->AddBarSet(pBarSet);
//		vector<MSCompoG*>::iterator itCompoG;
//		for(itCompoG=pBarSet->ma_CompoG.begin(); itCompoG != pBarSet->ma_CompoG.end(); itCompoG++)
//		{
//			MSCompoG* pCompoG = *itCompoG;
//			if(pCompoG==NULL)	continue;
//			pCompoG->AddBarSet(pBarSet);
//		}
//	}
}


void MSGroup::DeConvertBarSet4Member()//by LSS NX  CompoG와의 연결을 끊는다.	vector<MSBarSet*>::iterator itBarSet;
{
	//vector<MSBarSet*>::iterator itBarSet;
	//for(itBarSet=ma_BarSet.begin(); itBarSet != ma_BarSet.end(); itBarSet++)
	//{
	//	MSBarSet* pBarSet = *itBarSet;
	//	if(pBarSet->mp_Owner)	pBarSet->mp_Owner->RemoveBarSet(pBarSet);
	//	vector<MSCompoG*>::iterator itCompoG;
	//	for(itCompoG=pBarSet->ma_CompoG.begin(); itCompoG != pBarSet->ma_CompoG.end(); itCompoG++)
	//	{
	//		MSCompoG* pCompoG = *itCompoG;
	//		if(pCompoG==NULL)	continue;
	//		pCompoG->RemoveBarSet(pBarSet);
	//	}
	//	
	//}
}

double MSGroup::GetTotalLength()
{
	double dTotalLength = 0.;
	vector<MSBarSet*>::iterator it;
	for(it=ma_BarSet.begin(); it != ma_BarSet.end(); it++)
	{
		MSBarSet* pBarSet = *it;
		dTotalLength += pBarSet->GetTotalLength();
	}
	return dTotalLength;
}

MSBarSet* MSGroup::GetBarSet(long nBarSetID)
{
	vector<MSBarSet*>::iterator it;
	for(it=ma_BarSet.begin(); it != ma_BarSet.end(); it++)
	{
		MSBarSet* pBarSet = *it;
		if(nBarSetID == pBarSet->m_ID)
			return pBarSet;
	}
	return NULL;
}

BOOL MSGroup::DeleteBarSet(long nBarSetID)
{
	vector<MSBarSet*>::iterator it;
	for(it=ma_BarSet.begin(); it != ma_BarSet.end(); it++)
	{
		MSBarSet* pBarSet = *it;
		if(nBarSetID == pBarSet->m_ID)
		{
			ma_BarSet.erase(it);
			delete pBarSet;
			return TRUE;
		}
	}
	return FALSE;
}


// 배근 후 모델의 변화(삭제)로 인해 바셋의 mp_Owner가 없거나,
// 바셋의 ma_CompoG들중에 NULL이 존재하면 이 그룹은 안전하지 못하다.
// .dwg추가로 인해, mp_CompoG가 NULL인 경우 존재할 수 있다.
bool MSGroup::IsSafetyGroup()
{
// 	vector<MSBarSet*>::iterator itBarSet;
// 	for(itBarSet=ma_BarSet.begin(); itBarSet != ma_BarSet.end(); itBarSet++)
// 	{
// 		MSBarSet* pBarSet = *itBarSet;
// 		//
// 		if(pBarSet->mp_Owner == NULL)	return false;
// 		vector<MSCompoG*>::iterator itCompoG;
// 		for(itCompoG=pBarSet->ma_CompoG.begin(); itCompoG != pBarSet->ma_CompoG.end(); itCompoG++)
// 		{
// 			MSCompoG* pCompoG = *itCompoG;
// 			if(pCompoG==NULL)	return false;	
// 		}
// 	}
	return true;
}

vector<XBarSetInfo*> MSGroup::GetBarSetInfoArr(long nCZoneID)
{
	vector<XBarSetInfo*> RtnArr;
	vector<XBarSetInfo*>::iterator it;
	for(it=ma_BarSetInfo.begin(); it!=ma_BarSetInfo.end(); it++)
	{
		XBarSetInfo* pInfo = *it;
		if(pInfo->m_nCZoneID==nCZoneID)	RtnArr.push_back(pInfo);
	}
	return RtnArr;
}

void MSGroup::Init( bool bInConstructor/*=false*/ )
{
	if(!bInConstructor)
		MSContainer::Init(bInConstructor);
	
	mp_Building= NULL;
	m_nGroupType = 0;
	m_nFloorNum = 0;
}

//
//void MSGroup::GetRebarTotalLengthByDia( map<double,double>& mapLengthByDia )
//{
//	vector<MSBarSet*>::iterator it;
//	for(it=ma_BarSet.begin(); it != ma_BarSet.end(); it++)
//	{
//		MSBarSet* pBarSet = *it;
//		pBarSet->GetTotalLengthByDia(mapLengthByDia);
//	}	
//}