#pragma once
#include "MSElement.h"

class MSElement;
class MSFloor;

class StructureModel_CLASS MSWell : public MSElement
{
public:
	MSWell(void);
	~MSWell(void);
	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MSWell)

	virtual MSObject* Copy(bool bNewID = true);
	void CopyFromMe(MSWell* pSource, bool bNewID=true);
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);

	virtual Type_MSCompoG GetType(){return msWell;};

	MSElement* m_pOwner;	// MSSlabG
	MSFloor* GetFloor();

	int m_Depth;

	MSElement* GetOwner();	// MSWell의 Owner은 MSPolylineObject 이다.(집수정이 있고 입력 받은 PolyObject 가 CurPolyobject를 완전 포함함 삭제 해줘야함)
	void SetOwner(MSElement* pElement);
	bool MergeToMe(MSWell* Other);
	
	virtual void SetOwnerCompoG(MSElement* pOwner) { m_pOwner = pOwner; };
	virtual MSElement* GetOwnerCompoG(){ return m_pOwner; };
	virtual CString GetKey(bool bUseID = true);
};
