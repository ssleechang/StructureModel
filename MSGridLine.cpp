#include "StdAfx.h"
#include "MSGridLine.h"
#include "MSRebarBuilding.h"
#include "MSFloor.h"

#include "GMLib\GMLineVector.h"
#include "GMLib\GMLine.h"
#include "GMLib\GMVector.h"
#include "GMLib\GM2DVector.h"
#include "GMLib\GM2DLineVector.h"

#include <math.h>
IMPLEMENT_SERIAL(MSGridLine, MSElement, VERSIONABLE_SCHEMA | 1)

CString MSGridLine::ms_LastName = _T("1");
MSGridLine::Type_GridLine MSGridLine::ms_LastType = MSGridLine::Type_Numeic;
MSGridLine::operator GMLine()
{
	GMLine ALineEqa;
	ALineEqa.SetTwoPoint(m_X1, m_X2, m_Y1, m_Y2);
	return ALineEqa;
}

MSGridLine::operator GMLineVector()
{
	GMLineVector ALineVec(m_X1, m_X2, m_Y1, m_Y2, 0., 0.);
	return ALineVec;
}

MSGridLine::MSGridLine() 
{
	m_ID = 0;
	m_Type = Type_Numeic;
	m_HasBubble1 = m_HasBubble2 = 1;
	m_X1 = m_Y1 = m_X2 = m_Y2 = 0.;
	m_BubbleDia = 6;
	m_pOwner = NULL;
}

MSGridLine::~MSGridLine()
{

}

MSObject* MSGridLine::Copy( bool bNewID /*= true*/ )
{
	MSGridLine* pNewObj = new MSGridLine();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;
}

void MSGridLine::CopyFromMe( MSObject* pSource_in, bool bNewID/*=true*/ )
{
	if(pSource_in == NULL)	return;
	MSGridLine* TheSource = (MSGridLine*)pSource_in;

	m_HasBubble1 = TheSource->m_HasBubble1;
	m_HasBubble2 = TheSource->m_HasBubble2;
	m_X1 = TheSource->m_X1;
	m_Y1 = TheSource->m_Y1;
	m_X2 = TheSource->m_X2;
	m_Y2 = TheSource->m_Y2;
	m_BubbleDia = TheSource->m_BubbleDia;
	m_Name = TheSource->m_Name;
	m_Type = TheSource->m_Type;
	m_pOwner = TheSource->m_pOwner;
}

void MSGridLine::Dump(EFS::EFS_Buffer& buffer)
{
	MSElement::Dump(buffer);
	buffer << m_HasBubble1;
	buffer << m_HasBubble2;
	buffer << m_X1 << m_Y1;
	buffer << m_X2 << m_Y2;
	buffer << m_BubbleDia;
	buffer << m_Name;
	int Type = (int)m_Type;
	buffer << Type;

	buffer <<&m_pOwner;
}

void MSGridLine::Recovery(EFS::EFS_Buffer& buffer)
{
	MSElement::Recovery(buffer);
	buffer >> m_HasBubble1;
	buffer >> m_HasBubble2;
	buffer >> m_X1 >> m_Y1;
	buffer >> m_X2 >> m_Y2;
	buffer >> m_BubbleDia;
	buffer >> m_Name;
	int Type = 0;
	buffer >> Type;

	m_Type = (Type_GridLine)Type;

	buffer >>&m_pOwner;
}

void MSGridLine::Translate(GM2DVector &pDelta)
{
	GM2DVector SVec(m_X1, m_Y1);
	GM2DVector EVec(m_X2, m_Y2);
	
	SVec.Translate(pDelta);
	EVec.Translate(pDelta);

	m_X1 = SVec.m_X;
	m_Y1 = SVec.m_Y;
	m_X2 = EVec.m_X;
	m_Y2 = EVec.m_Y;
}

void MSGridLine::RotateZ(double theta, GM2DVector *pCenter)
{
	GM2DVector SVec(m_X1, m_Y1);
	GM2DVector EVec(m_X2, m_Y2);

	SVec.RotateZ(theta, pCenter);
	EVec.RotateZ(theta, pCenter);

	m_X1 = SVec.m_X;
	m_Y1 = SVec.m_Y;
	m_X2 = EVec.m_X;
	m_Y2 = EVec.m_Y;
}

void MSGridLine::MirrorByAxis(GM2DLineVector& pAxis)
{
	GM2DVector SVec(m_X1, m_Y1);
	GM2DVector EVec(m_X2, m_Y2);

	SVec.MirrorByAxis(pAxis);
	EVec.MirrorByAxis(pAxis);

	m_X1 = SVec.m_X;
	m_Y1 = SVec.m_Y;
	m_X2 = EVec.m_X;
	m_Y2 = EVec.m_Y;
}


static double DirectionTest(double LineX1, double LineY1, double LineX2, double LineY2, double PointX, double PointY, BOOL* OnLine)
{
	double ParaA, ParaB;
	double Dx, Dy;
	double Fxy;
	double MinX, MaxX, MinY, MaxY;

	MaxX = LineX1 > LineX2 ? LineX1 : LineX2;
	MaxY = LineY1 > LineY2 ? LineY1 : LineY2;
	MinX = LineX1 < LineX2 ? LineX1 : LineX2;
	MinY = LineY1 < LineY2 ? LineY1 : LineY2;

	Dx = LineX2 - LineX1;
	Dy = LineY2 - LineY1;

	if(fabs(Dx) < DTOL10)
		Fxy = PointX - LineX1;
	else
	{
		ParaA = Dy/Dx;
		ParaB = LineY1 - ParaA * LineX1;
		Fxy = ParaA*PointX - PointY + ParaB;
	}

	if(PointX >= MinX && PointX <= MaxX && PointY >= MinY && PointY <= MaxY)
		*OnLine = TRUE;
	else
		*OnLine = FALSE;

	return Fxy;
}

MSGridLine::Type_GridLine MSGridLine::AnalyzeGridLineType()
{
	int nLength = m_Name.GetLength();

	int nLastCharIndex = -1;
	bool hasNumeric = false;
	for(int index = nLength-1; index >= 0; index--)
	{
		CString cData = m_Name.Right(nLength-index);
		int nNumber = _ttoi(cData);
		if(nNumber > 0 || cData == '0')
		{
			hasNumeric = true;
			continue;
		}
		else
		{
			nLastCharIndex = index;
			break;
		}
	}
	if(nLastCharIndex == -1)
		return Type_Numeic;
	else
	{
		if(hasNumeric)
		{
			GM2DLineVector aGridLVec(m_X1, m_Y1, m_X2, m_Y2);
			double dDeg = aGridLVec.GetDegree();
			if(dDeg >= 0 && dDeg <= 45 || dDeg >= 135 && dDeg <= 225 || dDeg >= 315 && dDeg <= 360)
				return Type_Y1;
			else
				return Type_X1;
		}
		else
			return Type_Alphabe;
	}
}


BOOL MSGridLine::CheckOverlap(MSGridLine* TheLine)
{
	GMLineVector MyLVec = *this;
	GMLineVector UrLVec = *TheLine;
	return MyLVec.OverlapTest(UrLVec);
}

CString MSGridLine::GetNextName()
{
	CString NextName = _T("");

	int nLastCharIndex = -1;
	int nLength = ms_LastName.GetLength();
	for(int index = nLength-1; index >= 0; index--)
	{
		CString cData = ms_LastName.Right(nLength-index);
		int nNumber = _ttoi(cData);
		if(nNumber > 0 || cData == '0')
			continue;
		else
		{
			nLastCharIndex = index;
			break;
		}
	}
	if(nLastCharIndex == -1)
	{
		int NameInt = _ttoi(ms_LastName);
		NextName.Format(_T("%ld"), NameInt++);
	}
	else 
	{
		CString NameStr = ms_LastName.Mid(0, nLastCharIndex+1);
		CString RemainText = ms_LastName.Mid(nLastCharIndex+1, nLength-(nLastCharIndex+1));
		int NameInt = _ttoi(RemainText);
		NextName.Format(_T("%ld"), NameInt++);
	}

	return NextName;
}

CString MSGridLine::GetNextName(MSRebarBuilding* pBuilding, Type_GridLine LabelType)
{
	int     NameInt;
	TCHAR   NameChar;

// 	MSGridLine* pLastGridLine = pBuilding->GetGLine(ms_LastID);
// 	if(pLastGridLine && ms_LastType != LabelType)
// 		return GetNextName();

	CString Stmt, Name;
	switch(LabelType)
	{
	case Type_Numeic:
		Name = _T("1");
		break;
	case Type_Alphabe:
		Name = _T("A");
		break;
	case Type_X1:
		Name = _T("X1");
		break;
	case Type_Y1:
		Name = _T("Y1");
		break;
	default:
		{
			ASSERT(0);
			return _T("");
		}
	}

	long Count = 1;
// 	long MGLineSize = MSGridLine::m_MasterGridLineArr.GetSize();
	while(Count <= ms_LastID)
	{
		MSGridLine* pMGLine = pBuilding->GetGLine(Count);
		Count++;
		if(pMGLine == NULL)
			continue;

		if(pMGLine->m_Name == Name)
		{
			Count = 1;
			switch(LabelType)
			{
			case Type_Numeic:
				NameInt = _ttoi(Name);
				Name.Format(_T("%d"), NameInt+1);
				break;
			case Type_Alphabe:
				NameChar = Name.GetAt(0);
				Name.Format(_T("%c"), NameChar+1);
				break;
			case Type_X1:
				NameInt = _ttoi(Name.Right(Name.GetLength()-1));
				Name.Format(_T("X%d"), NameInt+1);
				break;
			case Type_Y1:
				NameInt = _ttoi(Name.Right(Name.GetLength()-1));
				Name.Format(_T("Y%d"), NameInt+1);
				break;
			}
		}
	}
	return Name;
}

CString MSGridLine::GetNextName(MSFloor* pFloor, Type_GridLine LabelType)
{
	int     NameInt = 0;
	TCHAR   NameChar;

	// 	MSGridLine* pLastGridLine = pBuilding->GetGLine(ms_LastID);
	// 	if(pLastGridLine && ms_LastType != LabelType)
	// 		return GetNextName();

	CString Stmt, Name;
	switch(LabelType)
	{
	case Type_Numeic:
		Name = _T("1");
		break;
	case Type_Alphabe:
		Name = _T("A");
		break;
	case Type_X1:
		Name = _T("X1");
		break;
	case Type_Y1:
		Name = _T("Y1");
		break;
	default:
		{
			ASSERT(0);
			return _T("");
		}
	}

	long Count = 0;
	// 	long MGLineSize = MSGridLine::m_MasterGridLineArr.GetSize();
	long nCount = pFloor->mm_GridLine.size();
	map<long, MSGridLine*>::iterator it;
	for(it = pFloor->mm_GridLine.begin(); it != pFloor->mm_GridLine.end(); it++)
	{
		MSGridLine* pMGLine = it->second;
		Count++;
		if(pMGLine == NULL)
			continue;

		if(pMGLine->m_Name == Name)
		{
			Count = 1;
			switch(LabelType)
			{
			case Type_Numeic:
				NameInt = _ttoi(Name);
				Name.Format(_T("%d"), NameInt+1);
				break;
			case Type_Alphabe:
				NameChar = Name.GetAt(0);
				Name.Format(_T("%c"), NameChar+1);
				break;
			case Type_X1:
				NameInt = _ttoi(Name.Right(Name.GetLength()-1));
				Name.Format(_T("X%d"), NameInt+1);
				break;
			case Type_Y1:
				NameInt = _ttoi(Name.Right(Name.GetLength()-1));
				Name.Format(_T("Y%d"), NameInt+1);
				break;
			}
		}
	}
	return Name;
}
CString MSGridLine::GetName(Type_GridLine LabelType, long LineNum)
{
	int     NameInt;
	TCHAR   NameChar;
	CString Name;
	switch(LabelType)
	{
	case Type_Numeic:
		NameInt = 1;
		Name.Format(_T("%d"), NameInt+LineNum-1);
		break;
	case Type_Alphabe:
		NameChar = 'A';
		Name.Format(_T("%c"), NameChar+LineNum-1);
		break;
	case Type_X1:
		Name.Format(_T("X%ld"), LineNum);
		break;
	case Type_Y1:
		Name.Format(_T("Y%d"), LineNum);
		break;
	default:
		{
			ASSERT(0);
			return _T("");
		}
	}
	return Name;
}

GM2DVector MSGridLine::GetTextStartVec(double dDist)
{
	GM2DLineVector LVec;
	LVec.m_P0.SetXY(m_X1, m_Y1);
	LVec.m_P1.SetXY(m_X2, m_Y2);

	LVec.ExtendLineVector(TRUE, dDist, FALSE);
	return LVec.m_P0;
}

GM2DVector MSGridLine::GetTextEndVec(double dDist)
{
	GM2DLineVector LVec;
	LVec.m_P0.SetXY(m_X1, m_Y1);
	LVec.m_P1.SetXY(m_X2, m_Y2);

	LVec.ExtendLineVector(FALSE, dDist, TRUE);
	return LVec.m_P1;
}

void MSGridLine::Serialize(CArchive &ar)
{
	MSElement::Serialize(ar);

	if(ar.IsStoring())
	{
		int Type = (int)m_Type;
		ar << m_ID;
		ar << m_Name;
		ar << Type;
		ar << m_X1;
		ar << m_Y1;
		ar << m_X2;
		ar << m_Y2;
		ar << m_HasBubble1;
		ar << m_HasBubble2;
		ar << m_BubbleDia;
	}
	else
	{
		int Type = 0;
		ar >> m_ID;
		ar >> m_Name;
		ar >> Type;
		ar >> m_X1;
		ar >> m_Y1;
		ar >> m_X2;
		ar >> m_Y2;
		ar >> m_HasBubble1;
		ar >> m_HasBubble2;
		ar >> m_BubbleDia;
// 		SetLastID(m_ID);

		m_Type = (Type_GridLine)Type;
		ms_LastName = m_Name;
	}
}
