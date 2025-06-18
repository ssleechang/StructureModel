#pragma once
#include "msrcobject.h"
#include "MSMainBar.h"

class StructureModel_CLASS MSSubMainBar : public MSRcObject
{
public:
	MSSubMainBar(void);
	~MSSubMainBar(void);

	void Serialize( CArchive& ar);
	DECLARE_SERIAL(MSSubMainBar)
	virtual void CopyFromMe(MSSubMainBar* pSource);
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer); 

	MSMainBar m_MainBar;
	vector<GM2DVector*> ma_Loc;
	void DeleteLocArr();
};

