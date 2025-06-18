#include "StdAfx.h"
#include "MSPartCompoG.h"
#include "GMLib/MSVersionInfo.h"

IMPLEMENT_SERIAL(MSPartCompoG, MSCompoG, VERSIONABLE_SCHEMA | 1)
MSPartCompoG::MSPartCompoG( )
{
	Init(true);
}

MSPartCompoG::~MSPartCompoG(void)
{
}
void MSPartCompoG::Serialize(CArchive &ar)
{
	if(ar.IsStoring())
	{
		MSCompoG::Serialize(ar);
	}
	else
	{
		if(MSVersionInfo::GetCurrentVersion() < 20140303)
			MSElement::Serialize(ar);
		else
			MSCompoG::Serialize(ar);
	}
}

void MSPartCompoG::Init( bool bInConstructor/*=false*/ )
{

}
