#pragma once
#include "msfloor.h"

#include "GMLib/GM2DVector.h"

class MSBlockInstance;
class GM2DPolyline;

class StructureModel_CLASS MSBlock : public MSFloor
{
public:
	MSBlock(void);
	~MSBlock(void);
	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MSBlock)

	virtual MSObject* Copy(bool bNewID = true);
	void CopyFromMe(MSBlock* pSource, bool bNewID=true);

	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer); 

	vector<MSBlockInstance*> ma_Instance;
	void DeleteBlockInstances();

	MSBlockInstance* MakeInstance(GM2DVector& FirstPoint, GM2DVector& SecondPoint, bool bReverse);
	virtual bool IsBlock() { return true;};

	virtual bool ExportBlock(CString fileName);
	virtual bool ImportBlock(CString fileName);

	void Serialize_Block(CArchive& archive);
	MSCompoM* Serialize_Block_CompoM(CArchive& ar, MSCompoM* pCompoM);

	void Init_ImportData();
	vector<MSCompoG*> ma_ImportCompoG;
	vector<MSCompoM*> ma_ImportCompoM;
	vector<MSLevelZone*> ma_ImportLevelZone;
	vector<MSSlabRebar*> ma_ImportFloorRebar;

	int m_RefFloorNum;
	virtual int GetRefFloorNum(){return m_RefFloorNum;};
	virtual void SetRefFloorNum(int refFloorNum){ m_RefFloorNum = refFloorNum;};
};

