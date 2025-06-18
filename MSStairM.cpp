#include "StdAfx.h"
#include "MSStairM.h"
#include "GMLib/MSVersionInfo.h"

IMPLEMENT_SERIAL(MSStairM, MSCompoM, VERSIONABLE_SCHEMA | 1)
MSStairM::MSStairM(void)
{

}
MSStairM::~MSStairM(void)
{

}

void MSStairM::CopyFromMe(MSCompoM* pSource)
{
    if (pSource == NULL)	return;
    MSStairM* pSourceStairM = (MSStairM*)pSource;

    MSCompoM::CopyFromMe(pSource);
}

void MSStairM::Dump(EFS::EFS_Buffer& buffer)
{
    MSCompoM::Dump(buffer);


}

void MSStairM::Recovery(EFS::EFS_Buffer& buffer)
{
    MSCompoM::Recovery(buffer);


}

void MSStairM::Serialize(CArchive &ar)
{
    MSCompoM::Serialize(ar);

    if (ar.IsStoring())
    {


    }
    else
    {

        //Temporary Compatibility hack
        if (MSVersionInfo::GetCurrentVersion() < 20140312)
        {
            double thickness;
            ar >> thickness;
            int stairType;
            ar >> stairType;
        }

    }
}