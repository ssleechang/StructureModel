#include "StdAfx.h"
#include "CoverLengthItem.h"
#include "..\GMLib\MSVersionInfo.h"

using namespace RebarOption;


CoverLengthItem::~CoverLengthItem(void)
{
}

void CoverLengthItem::Serialize( CArchive& ar )
{

	if(ar.IsStoring())
	{
		ar << m_sDiaName;
		ar<< m_eType;
		ar << (int)m_aCoverLengths.size();
		for(auto it = m_aCoverLengths.begin(); it != m_aCoverLengths.end(); it++)
		{
			ar<<*it;
		}
	}
	else
	{
		if (MSVersionInfo::GetCurrentVersion() < 20160108)
		{
			long nDia = 0;

			ar >> nDia;
			m_sDiaName.Format(_T("%ld"), nDia);
		}
		else
		{
			ar >> m_sDiaName;
		}

		int type;
		ar>> type;
		m_eType = (RebarOption::eCoverLengthType)type;

		int size;
		ar >> size;
		m_aCoverLengths.reserve(size);
		for(int i=0; i<size; i++)
		{
			int value;
			ar>> value;
			m_aCoverLengths.push_back(value);
		}
		if(MSVersionInfo::GetCurrentVersion() < 20241115 && m_eType == CL_WALL && m_aCoverLengths.size() > 0)
		{
			m_aCoverLengths.push_back(m_aCoverLengths[0]);
		}
		PrepareCoverLengthMap();
	}
}

CoverLengthItem::CoverLengthItem( RebarOption::eCoverLengthType type, CString diaName, vector<int>& coverLengths )
{
	m_sDiaName = diaName;
	m_eType = type;
	m_aCoverLengths = coverLengths;

	PrepareCoverLengthMap();
}

void CoverLengthItem::PrepareCoverLengthMap()
{
	m_mapCoverLength.clear();
	switch(m_eType)
	{
	case CL_COLUMN:
		{
			if(m_aCoverLengths.size() != 2 )
				throw invalid_argument("CoverLengthItem Argument list is invalid");
			m_mapCoverLength.insert(make_pair(CL_SIDE_FACE,m_aCoverLengths[0]));
			m_mapCoverLength.insert(make_pair(CL_TOP_FACE,m_aCoverLengths[1]));
			break;
		}

	case CL_WALL:
		{
			if(m_aCoverLengths.size() != 5 )
				throw invalid_argument("CoverLengthItem Argument list is invalid");
			m_mapCoverLength.insert(make_pair(CL_INNER_FACE,m_aCoverLengths[0]));
			m_mapCoverLength.insert(make_pair(CL_OUTTER_FACE,m_aCoverLengths[1]));
			m_mapCoverLength.insert(make_pair(CL_END_FACE,m_aCoverLengths[2]));
			m_mapCoverLength.insert(make_pair(CL_TOP_FACE,m_aCoverLengths[3]));
			m_mapCoverLength.insert(make_pair(CL_RWALL_INNER_FACE,m_aCoverLengths[4]));
			break;
		}

	case CL_BEAM:
		{
			if(m_aCoverLengths.size() != 3 )
				throw invalid_argument("CoverLengthItem Argument list is invalid");
			m_mapCoverLength.insert(make_pair(CL_END_FACE,m_aCoverLengths[0]));			
			m_mapCoverLength.insert(make_pair(CL_UPDOWN_FACE,m_aCoverLengths[1]));			
			m_mapCoverLength.insert(make_pair(CL_SIDE_FACE,m_aCoverLengths[2]));			
			break;
		}

	case CL_SLAB:
	case CL_DECKSLAB:
	case CL_FOUNDATION:
		{
			if(m_aCoverLengths.size() != 3 )
				throw invalid_argument("CoverLengthItem Argument list is invalid");
			m_mapCoverLength.insert(make_pair(CL_UP_FACE,m_aCoverLengths[0]));
			m_mapCoverLength.insert(make_pair(CL_DOWN_FACE,m_aCoverLengths[1]));			
			m_mapCoverLength.insert(make_pair(CL_SIDE_FACE,m_aCoverLengths[2]));
			break;
		}
	default:
		throw invalid_argument("CoverLengthItem type is invalid");
			break;
	}
}

void CoverLengthItem::SetCoverLengths( vector<int>& coverLengths )
{
	m_aCoverLengths = coverLengths;
	PrepareCoverLengthMap();
}

vector<int> CoverLengthItem::GetCoverLengths()
{
	return m_aCoverLengths;
}

int CoverLengthItem::GetCoverLength( eCoverLengthFace face ) const
{
	auto it = m_mapCoverLength.find(face);
	if(it != m_mapCoverLength.end())
		return it->second;

	else 
	{
		throw runtime_error("No value is defined for the coverLength face type requested!");
	}
}

void CoverLengthItem::Copy(CoverLengthItem desCoverLengthItem)
{
	m_sDiaName = desCoverLengthItem.m_sDiaName;
	m_eType = desCoverLengthItem.m_eType;

	m_aCoverLengths.clear();
	for (int i = 0; i < desCoverLengthItem.m_aCoverLengths.size(); i++)
	{
		m_aCoverLengths.push_back(desCoverLengthItem.m_aCoverLengths[i]);
	}

	PrepareCoverLengthMap();
}