#pragma once
#include "mscompom.h"
class StructureModel_CLASS MSStairM :
    public MSCompoM
{
public:
    MSStairM(void);
    ~MSStairM(void);
    void Serialize( CArchive& ar);
    DECLARE_SERIAL(MSStairM)
    virtual void CopyFromMe(MSCompoM* pSource);
    virtual void Dump(EFS::EFS_Buffer& buffer);
    virtual void Recovery(EFS::EFS_Buffer& buffer); 
    
};
