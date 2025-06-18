#include "StdAfx.h"
#include "CorrectiveLengthSet.h"
#include "GMLib/MSVersionInfo.h"

using namespace RebarOption;
CorrectiveLengthSet::CorrectiveLengthSet(CString name)
{
	m_sName = name;
	Initialize();
}

CorrectiveLengthSet::CorrectiveLengthSet()
{
	Initialize();
}

CorrectiveLengthSet::~CorrectiveLengthSet(void)
{
}

void CorrectiveLengthSet::Serialize( CArchive& ar )
{

	if(ar.IsStoring())
	{
		ar << m_sName;
		ar << (int)m_aCorrectiveLengths.size();
		for(auto it = m_aCorrectiveLengths.begin(); it != m_aCorrectiveLengths.end(); it++)
		{
			(*it).Serialize(ar);
		}
	}
	else
	{

		ar >> m_sName;
		int size;
		ar >> size;

		m_aCorrectiveLengths.clear();
		for(int index=0; index< size; index++)
		{
			CorrectiveLengthItem item;
			item.Serialize(ar);
			//Ver1.0.18 버전에서 다국어 적용 오류로 기둥타입에 Mat가 적용되는 경우가 있어 수정
			if (index == 1)
			{
				if (item.m_eType == MAT)
					item.m_eType = COLUMN;
			}
			m_aCorrectiveLengths.push_back(item);
		}

		if(MSVersionInfo::GetCurrentVersion() < 20150102)
		{
			Initialize();
		}
		if(MSVersionInfo::GetCurrentVersion() < 20240724)
		{
			auto slabItem = std::find_if(m_aCorrectiveLengths.begin(), m_aCorrectiveLengths.end(), [](CorrectiveLengthItem const &item) { return item.m_eType == SLAB; } );
			auto matItem = std::find_if(m_aCorrectiveLengths.begin(), m_aCorrectiveLengths.end(), [](CorrectiveLengthItem const &item) { return item.m_eType == MAT; } );
			if(slabItem != m_aCorrectiveLengths.end() && matItem != m_aCorrectiveLengths.end())
			{
				matItem->m_nSpliceAddLen = slabItem->GetSpliceAddLen();
			}
		}
	}
}

CorrectiveLengthItem* CorrectiveLengthSet::GetCorrectiveLengthOptByType( CorLen_Type type )
{
	auto it = find_if(m_aCorrectiveLengths.begin(), m_aCorrectiveLengths.end(), [type](CorrectiveLengthItem const& item){return item.m_eType == type;});
	if(it == m_aCorrectiveLengths.end())
		return NULL;
	else
		return &(*it);
}

void CorrectiveLengthSet::Initialize()
{
	m_aCorrectiveLengths.clear();
	m_aCorrectiveLengths.push_back(CorrectiveLengthItem(MAT, true, L"100", false, 0));
	m_aCorrectiveLengths.push_back(CorrectiveLengthItem(COLUMN, true, L"10", false, 0));
	m_aCorrectiveLengths.push_back(CorrectiveLengthItem(WALL_VER, true, L"10", false, 0));
	m_aCorrectiveLengths.push_back(CorrectiveLengthItem(WALL_HOR, true, L"10", false, 0));
	m_aCorrectiveLengths.push_back(CorrectiveLengthItem(RWALL_VER, true, L"10", false, 0));
	m_aCorrectiveLengths.push_back(CorrectiveLengthItem(RWALL_HOR, true, L"10", false, 0));
	m_aCorrectiveLengths.push_back(CorrectiveLengthItem(BEAM, true, L"100", false, 0));
	m_aCorrectiveLengths.push_back(CorrectiveLengthItem(SLAB, true, L"100", false, 0));
}

void CorrectiveLengthSet::Copy(shared_ptr<CorrectiveLengthSet> desCorrectiveLengthSet)
{
	m_sName = desCorrectiveLengthSet->m_sName;

	m_aCorrectiveLengths.clear();
	for (int index = 0; index < desCorrectiveLengthSet->m_aCorrectiveLengths.size(); index++)
	{
		CorrectiveLengthItem item;
		item.Copy(desCorrectiveLengthSet->m_aCorrectiveLengths[index]);
		m_aCorrectiveLengths.push_back(item);
	}
}
