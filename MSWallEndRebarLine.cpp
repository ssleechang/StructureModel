#include "StdAfx.h"
#include "MSWallEndRebarLine.h"
#include "MSWallEndRebarM.h"
#include "MSBaseBuilding.h"

IMPLEMENT_SERIAL(MSWallEndRebarLine, MSObject, VERSIONABLE_SCHEMA | 1)
long MSWallEndRebarLine::ms_LastID = 0;
MSWallEndRebarLine::MSWallEndRebarLine(void)
{
	m_ID = 0;
	m_Name = _T("");
	mp_Building = NULL;
}

MSWallEndRebarLine::~MSWallEndRebarLine(void)
{
	DeleteEndRebarMArr();
}

void MSWallEndRebarLine::DeleteEndRebarMArr()
{
	int NumRebarM = ma_EndRebarM.size();
	for(int iRebarM = 0; iRebarM < NumRebarM; iRebarM++)
		delete ma_EndRebarM[iRebarM];
	ma_EndRebarM.clear();
}

long MSWallEndRebarLine::GetID()
{
	if(m_ID == 0) // 
	{
		m_ID = ms_LastID + 1;
		SetLastID(m_ID);
		return m_ID;
	}

	return m_ID;
}

void MSWallEndRebarLine::SetLastID( long NewID )
{
	if(NewID > ms_LastID)
		ms_LastID = NewID;
}

MSObject* MSWallEndRebarLine::Copy(bool bNewID /*= true*/)
{
	MSWallEndRebarLine* pNewObj = new MSWallEndRebarLine();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;
}

void MSWallEndRebarLine::CopyFromMe(MSWallEndRebarLine* pSource, bool bNewID/*=true*/)
{
	if(pSource==NULL)	return;
	m_Name				=	pSource->m_Name;
	mp_Building        = pSource->GetBuilding();

// 	if(bNewID)
// 		GetID();
}

void MSWallEndRebarLine::Dump( EFS::EFS_Buffer& buffer )
{
	MSObject::Dump(buffer);
	buffer << m_Name;
	
	int NumRebarM = ma_EndRebarM.size();
	buffer << NumRebarM;
	for(int iRebarM = 0; iRebarM < NumRebarM; iRebarM++)
	{
		MSWallEndRebarM* pRebarM = ma_EndRebarM[iRebarM];
		pRebarM->Dump(buffer);
	}
}

void MSWallEndRebarLine::Recovery( EFS::EFS_Buffer& buffer )
{
	MSObject::Recovery(buffer);
	buffer >> m_Name;
	
	DeleteEndRebarMArr();
	int NumRebarM = 0;
	buffer >> NumRebarM;
	for(int iRebarM = 0; iRebarM < NumRebarM; iRebarM++)
	{
		MSWallEndRebarM* pRebarM = new MSWallEndRebarM();
		pRebarM->Recovery(buffer);
		ma_EndRebarM.push_back(pRebarM);
	}
}


void MSWallEndRebarLine::Serialize(CArchive &ar)
{
	MSObject::Serialize(ar);
	int NumRebarM = 0;
	if(ar.IsStoring())
	{
		ar << m_ID;
		ar << m_Name;

		NumRebarM = ma_EndRebarM.size();
		ar << NumRebarM;
		for(int iRebarM = 0; iRebarM < NumRebarM; iRebarM++)
		{
			MSWallEndRebarM* pRebarM = ma_EndRebarM[iRebarM];
			CheckEndRebarMStory(pRebarM);
			pRebarM->Serialize(ar);
		}
	}
	else
	{
		ar >> m_ID;
		ar >> m_Name;
		ar >> NumRebarM;
		for(int iRebarM = 0; iRebarM < NumRebarM; iRebarM++)
		{
			MSWallEndRebarM* pRebarM = new MSWallEndRebarM();
			pRebarM->Serialize(ar);
 			CheckEndRebarMStory(pRebarM);
			ma_EndRebarM.push_back(pRebarM);
		}
		SortRebarMArr();
		SetLastID(m_ID);
	}
}

void MSWallEndRebarLine::CheckEndRebarMStory(MSWallEndRebarM* pRebarM)
{

	int fromStory = pRebarM->GetFromStory();
	int toStory = pRebarM->GetToStory();
	
	int buildingFromStory = -(mp_Building->GetNumUnderStory() - 1);
	int buildingToStory = mp_Building->GetNumAboveStory() - 1;

	if (fromStory < buildingFromStory)	pRebarM->SetFromStory(buildingFromStory);
	if (fromStory > buildingToStory)		pRebarM->SetFromStory(buildingToStory);

	if (toStory < buildingFromStory)	pRebarM->SetToStory(buildingFromStory);
	if (toStory > buildingToStory)		pRebarM->SetToStory(buildingToStory);

}
void MSWallEndRebarLine::Add(MSWallEndRebarM* pRebarM)
{
	ma_EndRebarM.push_back(pRebarM);
}

void MSWallEndRebarLine::SortRebarMArr()
{
	sort(ma_EndRebarM.begin(), ma_EndRebarM.end(), 
		[]( MSWallEndRebarM* a,  MSWallEndRebarM* b)->bool {return a->GetFromStory() < b->GetFromStory();}
	);

// 	int NumRebarM = ma_EndRebarM.size();
// 	for(int iRebarM = 0; iRebarM < NumRebarM; iRebarM++)
// 	{
// 		for(int jRebarM = iRebarM+1; jRebarM < NumRebarM; jRebarM++)
// 		{
// 			MSWallEndRebarM* pIRebarM = ma_EndRebarM[iRebarM];
// 			MSWallEndRebarM* pJRebarM = ma_EndRebarM[jRebarM];
// 			if(pIRebarM->GetFromStory() > pJRebarM->GetFromStory())
// 			{
// 				MSWallEndRebarM* pTempM = pIRebarM;
// 				ma_EndRebarM[iRebarM] = pJRebarM;
// 				ma_EndRebarM[jRebarM] = pTempM;
// 			}
// 		}
// 	}
}

void MSWallEndRebarLine::MergeRebarMArr()
{
	map<MSWallEndRebarM*, int> RemoveRebarMMap;
	int NumRebarM = ma_EndRebarM.size();
	for(int iRebarM = 0; iRebarM < NumRebarM; iRebarM++)
	{
		MSWallEndRebarM* pIRebarM = ma_EndRebarM[iRebarM];
		int startStory = pIRebarM->GetToStory();
		map<MSWallEndRebarM*, int>::iterator it = RemoveRebarMMap.find(pIRebarM);
		if(it != RemoveRebarMMap.end())	continue;
		for(int jRebarM = iRebarM+1; jRebarM < NumRebarM; jRebarM++)
		{
			MSWallEndRebarM* pJRebarM = ma_EndRebarM[jRebarM];
			if(pIRebarM->IsSameData(pJRebarM))
			{
				int endStory = pJRebarM->GetFromStory();

				startStory++;
				if(startStory == 0)	startStory = 1;

				if(startStory != endStory)	continue;
				
				pIRebarM->SetToStory(pJRebarM->GetToStory());
				RemoveRebarMMap.insert(make_pair(pJRebarM, jRebarM));
			}
			else
				break;
		}
	}

	for(int iRebarM = NumRebarM-1; iRebarM >= 0; iRebarM--)
	{
		MSWallEndRebarM* pIRebarM = ma_EndRebarM[iRebarM];
		map<MSWallEndRebarM*, int>::iterator it = RemoveRebarMMap.find(pIRebarM);
		if(it != RemoveRebarMMap.end())
		{
			delete it->first;
			ma_EndRebarM.erase(ma_EndRebarM.begin()+iRebarM);
		}
	}
}

MSWallEndRebarM* MSWallEndRebarLine::GetWallEndRebarM(int story)
{
	int NumRebarM = ma_EndRebarM.size();
	for(int iRebarM = 0; iRebarM < NumRebarM; iRebarM++)
	{
		MSWallEndRebarM* pRebarM = ma_EndRebarM[iRebarM];
		if(pRebarM->IsStoryIncluded(story))
			return pRebarM;
	}
	return NULL;
}

MSWallEndRebarM* MSWallEndRebarLine::CreateWallEndRebarM(int fromStory, int toStory, CString rebarText, int spacing)
{
	MSWallEndRebarM* pNewEndM = new MSWallEndRebarM();
	pNewEndM->SetFromStory(fromStory);
	pNewEndM->SetToStory(toStory);
	pNewEndM->SetMainBar(rebarText);
	pNewEndM->SetSpacing(spacing);
	return pNewEndM;
}

bool MSWallEndRebarLine::SetWallEndRebarM(int story, CString rebarText)
{
	MSWallEndRebarM* pOldRebarM = GetWallEndRebarM(story);
	if (pOldRebarM == NULL)
	{
		MSWallEndRebarM* pNewRebarM= CreateWallEndRebarM(story, story, rebarText);
		ma_EndRebarM.push_back(pNewRebarM);
		SortRebarMArr();
		MergeRebarMArr();
		return true;
	}

	if(pOldRebarM->GetFromStory() == pOldRebarM->GetToStory())
	{
		pOldRebarM->SetMainBar(rebarText);
		return true;
	}
	
	if(pOldRebarM->GetFromStory() < story && story < pOldRebarM->GetToStory())
	{
		int oldFrom = pOldRebarM->GetFromStory();
		int oldTo = pOldRebarM->GetToStory();
		int newTo = story-1;
		if(newTo == 0)	newTo = -1;
		pOldRebarM->SetToStory(newTo);

		MSWallEndRebarM* pNewRebarM = CreateWallEndRebarM(story, story, rebarText);
		ma_EndRebarM.push_back(pNewRebarM);

		int newFrom = story+1;
		if(newFrom == 0)	newFrom = 1;
		MSMainBar oldMainBar = pOldRebarM->GetMainBar();
		MSWallEndRebarM* pNewToM = CreateWallEndRebarM(newFrom, oldTo, oldMainBar.GetTotalMainBarString());
		ma_EndRebarM.push_back(pNewToM);
	}
	else if(pOldRebarM->GetFromStory() == story)
	{
		MSWallEndRebarM* pNewRebarM = CreateWallEndRebarM(story, story, rebarText);
		ma_EndRebarM.push_back(pNewRebarM);

		int newFrom = pOldRebarM->GetFromStory()+1;
		if(newFrom == 0)	newFrom = 1;
		pOldRebarM->SetFromStory(newFrom);
	}
	else if(pOldRebarM->GetToStory() == story)
	{
		MSWallEndRebarM* pNewRebarM = CreateWallEndRebarM(story, story, rebarText);
		ma_EndRebarM.push_back(pNewRebarM);

		int newTo = pOldRebarM->GetToStory()-1;
		if(newTo == 0)	newTo = -1;
		pOldRebarM->SetToStory(newTo);
	}

	SortRebarMArr();
	MergeRebarMArr();
	return true;
}

bool MSWallEndRebarLine::IsSameData(MSWallEndRebarLine* urEndRebarLine)
{
	int myRebarMCount = ma_EndRebarM.size();
	int urRebarMCount = urEndRebarLine->ma_EndRebarM.size();
	if (myRebarMCount != urRebarMCount)	return false;

	for (int i = 0; i < myRebarMCount; i++)
	{
		MSWallEndRebarM* myRebarM = ma_EndRebarM[i];
		MSWallEndRebarM* urRebarM = urEndRebarLine->ma_EndRebarM[i];
		if (!myRebarM->IsSameData(urRebarM))	return false;
	}
	return true;
}

void MSWallEndRebarLine::DeleteEndRebarM(MSWallEndRebarM* delM)
{
	for (int i = 0; i < ma_EndRebarM.size(); i++)
	{
		if (ma_EndRebarM[i] == delM)
		{
			delete ma_EndRebarM[i];
			ma_EndRebarM.erase(ma_EndRebarM.begin() + i);
		}
	}
}
