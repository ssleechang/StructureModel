#pragma once
#include "MSElement.h"
#include "MSStirrupBar.h"
#include "MSMainBar.h"

class StructureModel_CLASS MSSlabRebarInfo : public MSObject
{
public:
    enum RebarType
    {
        TopX=0, TopCut, Bent, BottomX, BottomCut, TopY, BottomY, Shear
    };
    MSSlabRebarInfo(RebarType rbType = TopX);
    ~MSSlabRebarInfo(void);

	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MSSlabRebarInfo)
	virtual MSObject* Copy(bool bNewID = true);
	virtual void CopyFromMe(MSObject* pSource, bool bNewID=true);

	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);

	void SetRebarText(CString sBarText);

	bool CheckReBarStr();

	RebarType GetRebarType() { return m_RebarType; };
	void SetRebarType(RebarType type) { m_RebarType = type; };

	bool GetNumPlaceBar() { return m_IsNumPlaceBar; };
	void SetNumPlaceBar(bool isNumPlaceBar) { m_IsNumPlaceBar = isNumPlaceBar; };

protected:
	void SetShearBarInfo(CString sBarText);

public:
	MSStirrupBar m_SBar;
	MSMainBar m_MainBar;

protected:
    RebarType m_RebarType;
	bool m_IsNumPlaceBar;	// 0 : 간격배근, 1: 갯수배근

public:
	bool IsSameBar(MSSlabRebarInfo* pRebarLine);

};


