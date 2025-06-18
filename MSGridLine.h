#pragma once
#include "MSElement.h"

class MSRebarBuilding;
class MSContainer;
class StructureModel_CLASS MSGridLine : public MSElement
{
public:
	operator GMLineVector();
	operator GMLine();

public:
	MSGridLine();
	virtual ~MSGridLine();
	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MSGridLine)
	virtual MSObject* Copy(bool bNewID = true);
	virtual void CopyFromMe(MSObject* pSource, bool bNewID=true);
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);
	virtual void Translate(GM2DVector &pDelta);
	virtual void RotateZ(double theta, GM2DVector *pCenter);
	virtual void MirrorByAxis(GM2DLineVector& pAxis);

	enum Type_GridLine { Type_Numeic = 0, Type_Alphabe, Type_X1, Type_Y1};

	CString m_Name;
	Type_GridLine m_Type;
	double m_X1, m_Y1, m_X2, m_Y2;

	BOOL m_HasBubble1, m_HasBubble2;
	long m_BubbleDia;
	MSContainer* m_pOwner;

	virtual void SetNewName() {};

	static long ReadMasterPolylinePoints();
	long GetGPointArr(CArray<long, long>& GPointIDArr);

	virtual BOOL CheckOverlap(MSGridLine* TheLine);
	CString GetNextHName();
	CString GetNextVName();

	Type_GridLine AnalyzeGridLineType();

	static long GetMGLineIDArr(CArray<long, long>& MGLineIDArr);
	static long SorttingNames(CArray<long, long>& MGLineIDArr);

	static void SortNewMGLineArr(CTypedPtrArray<CObArray, MSGridLine*>& aDXFMGLine, BOOL bHasBubble = FALSE);

	static CString GetNextName(MSRebarBuilding* pBuilding, Type_GridLine LabelType);
	static CString GetNextName(MSFloor* pFloor, Type_GridLine LabelType);

	static CString GetName(Type_GridLine LabelType, long LineNum);
	static long GetHLabelType();
	static long GetVLabelType();
	static CString ms_LastName;
	static Type_GridLine ms_LastType;
	static CString GetNextName();

	GM2DVector GetTextStartVec(double dDist = 100);
	GM2DVector GetTextEndVec(double dDist = 100);
};

#pragma make_public(MSGridLine)