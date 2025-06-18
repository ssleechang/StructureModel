#include "StdAfx.h"
#include "MRHBarPointByVBar.h"

IMPLEMENT_SERIAL(MRHBarPointByVBar, MRHBarPoint, VERSIONABLE_SCHEMA | 1)

MRHBarPointByVBar::MRHBarPointByVBar(void)
{
	m_EdgeIdx = -1;
	m_VBarIdx = -1;
	m_BarNum = 0;
}

MRHBarPointByVBar::~MRHBarPointByVBar(void)
{
}

void MRHBarPointByVBar::Serialize( CArchive& archive )
{

}
