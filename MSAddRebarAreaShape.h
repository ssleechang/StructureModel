#pragma once
#include "MSAddRebarBase.h"

#include "MSStirrupBar.h"
class MSShapeParams;
class StructureModel_CLASS MSAddRebarAreaShape : public MSAddRebarBase
{
public:
	MSAddRebarAreaShape();
	~MSAddRebarAreaShape();

	virtual Type_MSCompoG GetType() { return msAddRebarAreaShape; };

	void Serialize(CArchive& archive);
	DECLARE_SERIAL(MSAddRebarAreaShape)
	virtual MSObject* Copy(bool bNewID = true) override;
	virtual void CopyFromMe(MSObject* pSource, bool bNewID = true) override;

	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);

	MSStirrupBar m_VBars;
	MSStirrupBar m_HBars;
	MSShapeParams* m_pShapeParam;
	void DeleteShapeParam();
	MSShapeParams* MakeShapeParams();
	MSShapeParams* GetShapeParams() { return m_pShapeParam; };

	bool GetIsCutOpening() { return m_IsCutOpening; };
	int GetShapeCode();

	void SetIsCutOpening(bool isCutOpening) { m_IsCutOpening = isCutOpening; };
	void SetShapeCode(int shapeCode);
	int GetGCode() { return m_GCode; };
	int GetSCode() { return m_SCode; };

	static MSAddRebarAreaShape* ms_AddAreaShape;
	static MSAddRebarAreaShape* GetStaticAddAreaShape();

protected:
	bool m_IsCutOpening;
	int m_GCode;	// T1
	int m_SCode; //  09

};

