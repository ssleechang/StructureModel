#include "StdAfx.h"
#include "MSFaceMemberM.h"
#include "MSRcSlabM.h"

IMPLEMENT_SERIAL(MSFaceMemberM, MSCompoM, VERSIONABLE_SCHEMA | 1)
MSFaceMemberM::MSFaceMemberM(void)
{
	m_dThick = 200.;
}

MSFaceMemberM::~MSFaceMemberM(void)
{
}

void MSFaceMemberM::CopyFromMe(MSCompoM* pSource)
{
	if(pSource==NULL)	return;
	MSFaceMemberM* pSourceFaceM = (MSFaceMemberM*)pSource;
	m_dThick =	pSourceFaceM->m_dThick;
	MSCompoM::CopyFromMe(pSource);
}

void MSFaceMemberM::Dump(EFS::EFS_Buffer& buffer)
{
	MSCompoM::Dump(buffer);
	buffer << m_dThick;
}

void MSFaceMemberM::Recovery(EFS::EFS_Buffer& buffer)
{
	MSCompoM::Recovery(buffer);
	buffer >> m_dThick;
}

void MSFaceMemberM::Serialize(CArchive &ar)
{
	MSCompoM::Serialize(ar);
	if(ar.IsStoring())
	{
		ar << m_dThick;
	}
	else
	{
		ar >> m_dThick;
	}
}

double MSFaceMemberM::GetDepth()
{
	return GetThick();
}

double MSFaceMemberM::GetWidth()
{
	return GetThick();
}

double MSFaceMemberM::GetThick()
{
	return m_dThick;
}

void MSFaceMemberM::SetThick(double Thick)
{
	m_dThick = Thick;
}

double MSFaceMemberM::GetThickXMLExport()
{
	/* BH-A Code
	MSRcSlabM* rcSlabM = dynamic_cast<MSRcSlabM*>(GetRebarM());
	if (rcSlabM != nullptr)
	{
		if (rcSlabM->GetSlabMType() == MSRcSlabM::PC_IMT || rcSlabM->GetSlabMType() == MSRcSlabM::PC_RPS)
		{
			double h1 = rcSlabM->GetH1();
			double h2 = rcSlabM->GetH2();
			double h3 = rcSlabM->GetH3();

			return h1 + h2 + h3;
		}
	}
	*/
	return m_dThick;
}


CString MSFaceMemberM::ToString()
{
	auto str = MSCompoM::ToString();
	str.AppendFormat(L" Thickness:%f,", m_dThick);
	return str;
}