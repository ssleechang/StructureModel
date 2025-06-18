#include "StdAfx.h"
#include "MSPcLineMemberM.h"
#include "GMLib/GM2DRectangle.h"

IMPLEMENT_SERIAL(MSPcLineMemberM, MSRcLineMemberM, VERSIONABLE_SCHEMA | 1)
MSPcLineMemberM::MSPcLineMemberM(void)
{
	m_dPcWidth = 1000;
	m_dPcDepth = 1000;
}

MSPcLineMemberM::~MSPcLineMemberM(void)
{
}

void MSPcLineMemberM::CopyFromMe(MSCompoM* pSource)
{
	if(pSource==NULL)	return;
	MSPcLineMemberM* pSourceLineM = (MSPcLineMemberM*)pSource;

	m_dPcWidth = pSourceLineM->m_dPcWidth;
	m_dPcDepth = pSourceLineM->m_dPcDepth;
	MSRcLineMemberM::CopyFromMe(pSource);
}

bool MSPcLineMemberM::IsGeometryDifferent(MSCompoM* compoM)
{
	if (MSRcLineMemberM::IsGeometryDifferent(compoM)) return true;

	MSPcLineMemberM* pPCM = dynamic_cast<MSPcLineMemberM*>(compoM);
	if (pPCM == NULL) return true;

	if (fabs(m_dPcWidth - pPCM->m_dPcWidth) > DTOL_GM) return true;
	if (fabs(m_dPcDepth - pPCM->m_dPcDepth) > DTOL_GM) return true;

	return false;
}

void MSPcLineMemberM::Dump(EFS::EFS_Buffer& buffer)
{
	MSRcLineMemberM::Dump(buffer);
	buffer << m_dPcWidth;
	buffer << m_dPcDepth;
}

void MSPcLineMemberM::Recovery( EFS::EFS_Buffer& buffer )
{
	MSRcLineMemberM::Recovery(buffer);
	buffer >> m_dPcWidth;
	buffer >> m_dPcDepth;
}

void MSPcLineMemberM::Serialize(CArchive &ar)
{
	MSRcLineMemberM::Serialize(ar);

	if(ar.IsStoring())
	{
		ar << m_dPcWidth;
		ar << m_dPcDepth;
	}
	else
	{
		ar >> m_dPcWidth;
		ar >> m_dPcDepth;
	}
}