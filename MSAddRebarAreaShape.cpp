#include "stdafx.h"
#include "MSAddRebarAreaShape.h"

#include "MSShapeParamGenerator.h"
#include "MSShapeParams.h"

#include "MSDeformedBar.h"

MSAddRebarAreaShape* MSAddRebarAreaShape::ms_AddAreaShape = new MSAddRebarAreaShape();
MSAddRebarAreaShape* MSAddRebarAreaShape::GetStaticAddAreaShape()
{
	return ms_AddAreaShape;
}

IMPLEMENT_SERIAL(MSAddRebarAreaShape, MSAddRebarBase, VERSIONABLE_SCHEMA | 1)
MSAddRebarAreaShape::MSAddRebarAreaShape()
{
	CString sBar = MSDeformedBar::GetStirrupBarStr(300);
	m_VBars.InitData(sBar);
	m_HBars.InitData(sBar);

	m_IsCutOpening = true;
	m_GCode = 1;	// T1
	m_SCode = 9; //  09

	m_pShapeParam = nullptr;
	MakeShapeParams();
}


MSAddRebarAreaShape::~MSAddRebarAreaShape()
{
	DeleteShapeParam();
}

void MSAddRebarAreaShape::DeleteShapeParam()
{
	if (m_pShapeParam != nullptr)
		delete m_pShapeParam;
	m_pShapeParam = nullptr;
}

MSShapeParams* MSAddRebarAreaShape::MakeShapeParams()
{
	if (m_pShapeParam == nullptr)
		m_pShapeParam = MSShapeParamGenerator::GeneratorShapeParms(m_GCode, m_SCode);
	else
	{
		if (m_pShapeParam->GetGCode() != m_GCode || m_pShapeParam->GetSCode() != m_SCode)
		{
			DeleteShapeParam();
			m_pShapeParam = MSShapeParamGenerator::GeneratorShapeParms(m_GCode, m_SCode);
		}
	}
	return m_pShapeParam;
}

MSObject* MSAddRebarAreaShape::Copy(bool bNewID /* = true*/)
{
	MSAddRebarAreaShape* pNewObj = new MSAddRebarAreaShape();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;
}

void MSAddRebarAreaShape::CopyFromMe(MSObject* pSource_in, bool bNewID/*=true*/)
{
	if (pSource_in == nullptr) return;
	MSAddRebarBase::CopyFromMe(pSource_in, bNewID);
	MSAddRebarAreaShape* pSource = dynamic_cast<MSAddRebarAreaShape*>(pSource_in);
	if (pSource == nullptr)	return;

	m_VBars.CopyFromMe(&pSource->m_VBars);
	m_HBars.CopyFromMe(&pSource->m_HBars);
	m_IsCutOpening = pSource->GetIsCutOpening();
	SetShapeCode(pSource->GetShapeCode());

	DeleteShapeParam();
	m_pShapeParam = MakeShapeParams();
	m_pShapeParam->CopyFromMe(pSource->m_pShapeParam);
}

void MSAddRebarAreaShape::Dump(EFS::EFS_Buffer& ar)
{
	MSAddRebarBase::Dump(ar);
	m_VBars.Dump(ar);
	m_HBars.Dump(ar);
	ar << m_IsCutOpening;
	ar << m_GCode << m_SCode;

	bool IsNullShape = true;
	if (m_pShapeParam != nullptr)	IsNullShape = false;

	ar << IsNullShape;
	if(IsNullShape == false)
		m_pShapeParam->Dump(ar);
}

void MSAddRebarAreaShape::Recovery(EFS::EFS_Buffer& ar)
{
	MSAddRebarBase::Recovery(ar);
	m_VBars.Recovery(ar);
	m_HBars.Recovery(ar);
	ar >> m_IsCutOpening;
	ar >> m_GCode >> m_SCode;

	bool IsNullShape = true;
	ar >> IsNullShape;

	DeleteShapeParam();
	if (IsNullShape == false)
	{
		m_pShapeParam = MakeShapeParams();
		m_pShapeParam->Recovery(ar);
	}
}

void MSAddRebarAreaShape::Serialize(CArchive &ar)
{
	MSAddRebarBase::Serialize(ar);
	if (ar.IsStoring())
	{
		ar << m_GCode;
		ar << m_SCode;
		ar << m_IsCutOpening;
		m_VBars.Serialize(ar);
		m_HBars.Serialize(ar);

		m_pShapeParam->Serialize(ar);
	}
	else
	{
		ar >> m_GCode;
		ar >> m_SCode;
		ar >> m_IsCutOpening;
		m_VBars.Serialize(ar);
		m_HBars.Serialize(ar);

		MakeShapeParams();
		m_pShapeParam->Serialize(ar);
	}
}

int MSAddRebarAreaShape::GetShapeCode()
{
	return (m_GCode * 1000) + m_SCode;
}

void MSAddRebarAreaShape::SetShapeCode(int shapeCode)
{
	m_GCode = shapeCode / 1000;
	m_SCode = shapeCode % 1000;
}
