#include "StdAfx.h"
#include "MSPcColumnM.h"



IMPLEMENT_SERIAL(MSPcColumnM, MSRcLineMemberM, VERSIONABLE_SCHEMA | 1)
	MSPcColumnM::MSPcColumnM(void)
{
}

MSPcColumnM::~MSPcColumnM(void)
{
}

void MSPcColumnM::CopyFromMe(MSCompoM* pSource)
{
	if(pSource==NULL)	return;
	MSPcColumnM* pSourceLineM = (MSPcColumnM*)pSource;

	MSRcLineMemberM::CopyFromMe(pSource);
}

void MSPcColumnM::Dump( EFS::EFS_Buffer& buffer )
{
	MSRcLineMemberM::Dump(buffer);
}

void MSPcColumnM::Recovery( EFS::EFS_Buffer& buffer )
{
	MSRcLineMemberM::Recovery(buffer);
}

bool MSPcColumnM::IsGeometryDifferent(MSCompoM* compoM)
{
	if (MSRcLineMemberM::IsGeometryDifferent(compoM)) return true;

	MSPcColumnM* pPCM = dynamic_cast<MSPcColumnM*>(compoM);
	if (pPCM == NULL) return true;

	return false;
}

void MSPcColumnM::Serialize(CArchive &ar)
{
	MSRcLineMemberM::Serialize(ar);

	if(ar.IsStoring())
	{
	}
	else
	{
	}
}
