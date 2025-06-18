#pragma once
#include "MSAddRebarBase.h"

class MSBentRebar;
class StructureModel_CLASS MSAddRebarAreaBent :public MSAddRebarBase
{
public:
	MSAddRebarAreaBent();
	~MSAddRebarAreaBent();

	virtual Type_MSCompoG GetType() { return msAddRebarAreaBent; };

	void Serialize(CArchive& archive);
	DECLARE_SERIAL(MSAddRebarAreaBent)
	virtual MSObject* Copy(bool bNewID = true);
	virtual void CopyFromMe(MSObject* pSource, bool bNewID = true) override;

	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);

	MSBentRebar* GetMainBar() {return m_pMainBar;};
	MSBentRebar* GetSubBar() { return m_pSubBar; };

	void CalculateBentLine(MSBentRebar* pMSBentRebar);
	bool CheckBentLocation(GM2DPolyline* profile);

	static MSAddRebarAreaBent* ms_AddAreaBent;
	static MSAddRebarAreaBent* GetStaticAddAreaBent();

protected:
	MSBentRebar* m_pMainBar;
	MSBentRebar* m_pSubBar;

};

