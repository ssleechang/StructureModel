#include "StdAfx.h"
#include "MRHBarPoint.h"

IMPLEMENT_SERIAL(MRHBarPoint, MRObject, VERSIONABLE_SCHEMA | 1)

MRHBarPoint::MRHBarPoint(void)
{
	m_BarSide = 1; // 1 : 콩을 원쪽으로 돈다.};
}

MRHBarPoint::~MRHBarPoint(void)
{
}

void MRHBarPoint::Serialize( CArchive& archive )
{

}

