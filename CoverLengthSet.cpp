#include "StdAfx.h"
#include "CoverLengthSet.h"
#include "MSDeformedBar.h"
#include "..\GMLib\MSVersionInfo.h"

using namespace RebarOption;
CoverLengthSet::CoverLengthSet(CString name)
{
	m_sName = name;
	Initialize();
}

CoverLengthSet::CoverLengthSet()
{
	Initialize();
}

CoverLengthSet::~CoverLengthSet(void)
{
}

void CoverLengthSet::Serialize( CArchive& ar )
{

	if(ar.IsStoring())
	{
		ar << m_sName;
		ar << (int)m_mapTypeToCoverLengths.size();
		for(auto it= m_mapTypeToCoverLengths.begin(); it!= m_mapTypeToCoverLengths.end(); it++)
		{
			ar << it->first;
			auto CoverLenths = it->second;
			ar << (int)CoverLenths.size();
			for(auto it2 = CoverLenths.begin(); it2 != CoverLenths.end(); it2++)
			{
				(*it2).Serialize(ar);
			}
		}
	}
	else
	{

		ar >> m_sName;
		
		int mapSize;
		ar >> mapSize;
		
		m_mapTypeToCoverLengths.clear();
		for(int i=0; i< mapSize; i++)
		{
			int type;
			ar >> type;

			auto coverLengthType = (eCoverLengthType)type;
			
			int itemSize;
			ar >> itemSize;
			vector<CoverLengthItem> CoverLengthItems;

			if (itemSize == 0)
			{
				CoverLengthItems = GetTypeToDefaultData(coverLengthType);
			}
			else
			{
				CoverLengthItems.reserve(itemSize);
				for (int j = 0; j < itemSize; j++)
				{
					CoverLengthItem item;
					item.Serialize(ar);

					if (j == itemSize - 1 && item.m_sDiaName != m_sMaxDiaName)
					{
						item.m_sDiaName = m_sMaxDiaName;
					}

					CoverLengthItems.push_back(item);
				}
			}

			m_mapTypeToCoverLengths.insert(make_pair(coverLengthType,CoverLengthItems));
		}

		if (MSVersionInfo::GetCurrentVersion() < 20180907)
		{
			vector<CoverLengthItem> CoverLengthItems = GetTypeToDefaultData(CL_DECKSLAB);
			m_mapTypeToCoverLengths.insert(make_pair(CL_DECKSLAB, CoverLengthItems));
		}
	}
}

void CoverLengthSet::Initialize()
{
	m_mapTypeToCoverLengths.clear();

	CStringArray BarDiaArr;
	long NumDia = MSDeformedBar::GetRebarNameList(BarDiaArr);
	m_sMaxDiaName = BarDiaArr[NumDia - 1];

	vector<CoverLengthItem> CoverLengthItems;

	CoverLengthItems = GetTypeToDefaultData(CL_COLUMN);
	m_mapTypeToCoverLengths.insert(make_pair(CL_COLUMN, CoverLengthItems));
	
	CoverLengthItems = GetTypeToDefaultData(CL_WALL);
	m_mapTypeToCoverLengths.insert(make_pair(CL_WALL, CoverLengthItems));

	CoverLengthItems = GetTypeToDefaultData(CL_BEAM);
	m_mapTypeToCoverLengths.insert(make_pair(CL_BEAM, CoverLengthItems));

	CoverLengthItems = GetTypeToDefaultData(CL_SLAB);
	m_mapTypeToCoverLengths.insert(make_pair(CL_SLAB, CoverLengthItems));

	CoverLengthItems = GetTypeToDefaultData(CL_DECKSLAB);
	m_mapTypeToCoverLengths.insert(make_pair(CL_DECKSLAB, CoverLengthItems));

	CoverLengthItems = GetTypeToDefaultData(CL_FOUNDATION);
	m_mapTypeToCoverLengths.insert(make_pair(CL_FOUNDATION, CoverLengthItems));
}

vector<int> CoverLengthSet::GetTypeToDefaultValue(RebarOption::eCoverLengthType type)
{
	vector<int> CoverLengthValues;

	if (type == CL_COLUMN)
	{
		CoverLengthValues.push_back(40);
		CoverLengthValues.push_back(100);
	}
	else if (type == CL_WALL)
	{
		CoverLengthValues.push_back(40);
		CoverLengthValues.push_back(80);
		CoverLengthValues.push_back(50);
		CoverLengthValues.push_back(100);
		CoverLengthValues.push_back(40);
	}
	else if (type == CL_BEAM)
	{
		CoverLengthValues.push_back(50);
		CoverLengthValues.push_back(40);
		CoverLengthValues.push_back(40);
	}
	else if (type == CL_SLAB)
	{
		CoverLengthValues.push_back(30);
		CoverLengthValues.push_back(30);
		CoverLengthValues.push_back(50);
	}
	else if (type == CL_DECKSLAB)
	{
		CoverLengthValues.push_back(30);
		CoverLengthValues.push_back(30);
		CoverLengthValues.push_back(50);
	}
	else //if (type == CL_FOUNDATION)
	{
		CoverLengthValues.push_back(50);
		CoverLengthValues.push_back(80);
		CoverLengthValues.push_back(80);
	}

	return CoverLengthValues;
}

vector<CoverLengthItem> CoverLengthSet::GetTypeToDefaultData(RebarOption::eCoverLengthType type)
{
	vector<CoverLengthItem> CoverLengthItems;

	vector<int> CoverLengthValues = GetTypeToDefaultValue(type);
	CoverLengthItems.push_back(CoverLengthItem(type, m_sMaxDiaName, CoverLengthValues));

	return CoverLengthItems;
}

CoverLengthItem* CoverLengthSet::GetCoverLengthOptByTypeAndDia( RebarOption::eCoverLengthType type, CString sDiaName )
{
	auto coverLengths = m_mapTypeToCoverLengths[type];
	auto coverLength = find_if(coverLengths.begin(), coverLengths.end(), [sDiaName](CoverLengthItem& item){return item.m_sDiaName >= sDiaName; });// nDia보다 큰 m_nDia가 처음 나오면 리턴한다.
	if(coverLength == coverLengths.end())
		throw runtime_error("cannot find cover length option values");

	return &(*coverLength);
}

vector<CoverLengthItem>& CoverLengthSet::GetCoverLengthOptByType( RebarOption::eCoverLengthType type )
{
	return m_mapTypeToCoverLengths[type];
}

void CoverLengthSet::SetCoverLengthOptByType( RebarOption::eCoverLengthType type, vector<CoverLengthItem>& coverLengths )
{
	//받고나면 nDia에 따라 정렬해서 저장한다.
	sort(coverLengths.begin(), coverLengths.end(), 
		[](const CoverLengthItem& item1, const CoverLengthItem& item2)->int 
		{
			double dItem1Dia = _ttof(item1.m_sDiaName);
			double dItem2Dia = _ttof(item2.m_sDiaName);

			if (dItem1Dia > dItem2Dia) return 1;
			else if (dItem1Dia < dItem2Dia) return -1;
			else return 0;
		}
	);
	
	m_mapTypeToCoverLengths[type] = coverLengths;
}
int CoverLengthSet::GetCoverLengthByTypeAndFaceAndDia(RebarOption::eCoverLengthType type, RebarOption::eCoverLengthFace eFaceType, CString sDiaName) const
{
	map<RebarOption::eCoverLengthType, vector<CoverLengthItem>>::const_iterator it;
	it = m_mapTypeToCoverLengths.find(type);
	if(it == m_mapTypeToCoverLengths.end())
		throw runtime_error("cannot find cover length option values");
	vector<CoverLengthItem>::const_iterator itItem;

	double dDia = _ttof(sDiaName);
	for(itItem = it->second.begin();itItem!=it->second.end();itItem++)
	{
		double dItItemDia = _ttof(itItem->m_sDiaName);
		if (dItItemDia >= dDia)
			return itItem->GetCoverLength(eFaceType);
	}
	return 10;
}

void CoverLengthSet::Copy(shared_ptr<CoverLengthSet> desCoverLengthSet)
{
	m_sName = desCoverLengthSet->m_sName;
	m_sMaxDiaName = desCoverLengthSet->m_sMaxDiaName;

	m_mapTypeToCoverLengths.clear();
	for (auto it = desCoverLengthSet->m_mapTypeToCoverLengths.begin(); it != desCoverLengthSet->m_mapTypeToCoverLengths.end(); it++)
	{
		eCoverLengthType coverLengthType = it->first;
		vector<CoverLengthItem> CoverLengthItems;

		for (int index = 0; index < it->second.size(); index++)
		{
			CoverLengthItem item;
			item.Copy(it->second[index]);

			CoverLengthItems.push_back(item);
		}

		m_mapTypeToCoverLengths.insert(make_pair(coverLengthType, CoverLengthItems));
	}
}