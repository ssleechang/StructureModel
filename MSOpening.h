#pragma once
#include "MSElement.h"

class GM2DPolyline;
class MSSlabG;
class MSElement;

////////////////////////////////////////////////////////////////////////
// NOT USED anymore - use MSOpeningG
////////////////////////////////////////////////////////////////////////
class StructureModel_CLASS MSOpening :
	public MSElement
{
public:
	MSOpening(void);
	~MSOpening(void);
	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MSOpening)

	enum eOpeningType { General = 0, MaterialLift};
	virtual MSObject* Copy(bool bNewID = true);
	void CopyFromMe(MSOpening* pSource, bool bNewID=true);
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);

	virtual Type_MSCompoG GetType(){return msOpening;};
	virtual MSFloor* GetFloor();

	MSElement* m_pOwner;

	virtual MSElement* GetOwnerCompoG();	// MSOpening의 Owner은 MSPolylineObject 이다.(오프닝이 있고 입력 받은 PolyObject 가 CurPolyobject를 완전 포함함 삭제 해줘야함)
	void SetOwner(MSElement* pElement);
    bool MergeToMe(MSOpening* Other);

	void RemoveMadeByCompoGIDAll();
	void RemoveMadeByCompoGID(long nCompoGID);
	bool HasMadeByCompoGID(long nCompoGID);

	void AddMadeByCompoGID(long nCompoGID);
	bool IsMadeByCompoG();

	bool IsSameOpening(MSOpening* urOpening);
	eOpeningType GetOpeningType() { return m_eOpeningType; };


	void SetOpeningType(eOpeningType type) { m_eOpeningType = type; };
	vector<long> ma_MadeByCompoGID;

	bool HasSamePolyMember();
protected:
	eOpeningType m_eOpeningType;
};
