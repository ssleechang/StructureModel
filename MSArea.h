#pragma once
#include "MSObject.h"
#include "StructureModel/MSElement.h"

class StructureModel_CLASS MSArea : public MSElement
{
public:
	MSArea(void);
	~MSArea(void);

	virtual Type_MSCompoG GetType() override{ return msArea; };

	enum eAreaType { Added = 0, Deduction};
	DECLARE_SERIAL(MSArea)
	void Serialize(CArchive& archive);
	virtual MSObject* Copy(bool bNewID = true)	override;
	virtual void CopyFromMe(MSObject* pSource, bool bNewID = true)	override;
	virtual void Dump(EFS::EFS_Buffer& buffer)	override;
	virtual void Recovery(EFS::EFS_Buffer& buffer)	override;

	virtual int GetFloorNum() override { return m_FloorNum; };
	CString GetName() { return m_Name; };
	GM2DPolyline* GetProfile() { return m_pProfile; };
	eAreaType GetAreaType() { return m_eType; };
	double GetArea_ACIS();

	void SetFloorNum(long floorNum) { m_FloorNum = floorNum; };
	void SetName(CString name) { m_Name = name; };
	void SetProfile(GM2DPolyline* profile);
	void SetAreaType(eAreaType type) { m_eType = type; };

	static CString GetAreaLayerName() { return _T("NXB_AREA"); };
private:
	eAreaType m_eType;
	long m_FloorNum;
	CString m_Name;
	GM2DPolyline* m_pProfile;
};

