#include "StdAfx.h"
#include "MSPointMemberG.h"
#include "MSRebarBuilding.h"
#include "MSAssembly.h"
#include "MSFloor.h"
#include "MSConnection.h"
#include "MSRcLineMemberM.h"
#include "MSColumnHeaderM.h"
#include "MSCompoGUtil.h"
#include "MSSteelSection.h"

#include "GMLib/GM2DRectangle.h"
#include "GMLib/GM2DBound.h"

IMPLEMENT_SERIAL(MSPointMemberG, MSCompoG, VERSIONABLE_SCHEMA | 1)

MSPointMemberG::MSPointMemberG()
{
	Init(true);
}

MSPointMemberG::~MSPointMemberG(void)
{
}
void MSPointMemberG::Dump(EFS::EFS_Buffer& buffer)
{
	MSCompoG::Dump(buffer);
	buffer << m_SOffsetZ << m_dAxisRotation;
	int tmp = m_ePointAlign;
	buffer << tmp;
}

void MSPointMemberG::Recovery(EFS::EFS_Buffer& buffer)
{
	MSCompoG::Recovery(buffer);
	buffer >> m_SOffsetZ >> m_dAxisRotation;
	int tmp = 0;
	buffer >> tmp;
	m_ePointAlign = (ePOINTALIGN_TYPE)tmp;
}

void MSPointMemberG::Serialize( CArchive& ar)
{
	MSCompoG::Serialize(ar);
	if(ar.IsStoring())
	{
		ar << m_SOffsetZ;
		ar << m_dAxisRotation;
		ar << (int)m_ePointAlign;
	}
	else
	{
		ar >> m_SOffsetZ;
		ar >> m_dAxisRotation;
		if(MSVersionInfo::GetCurrentVersion() >= 20150430)
		{
			int ePointAlign;
			ar >> ePointAlign;
			m_ePointAlign = (ePOINTALIGN_TYPE)ePointAlign;
		}

		if (m_ePointAlign == ePOINTALIGN_TYPE::COLUMNALIGN_CENTER)
		{
			if (ma_Vec.size() == ma_Vec_Input.size() == 1)
			{
				ma_Vec_Input[0]->m_X = ma_Vec[0]->m_X;
				ma_Vec_Input[0]->m_Y = ma_Vec[0]->m_Y;
			}
		}
	}
}

void MSPointMemberG::CopyFromMe( MSObject* pSource_in, bool bNewID/*=true*/)
{
	if(pSource_in == nullptr) return;
	MSCompoG::CopyFromMe(pSource_in, bNewID);

	if( !pSource_in->IsElement() || ((MSElement*)pSource_in)->GetType() == msNULL) { ASSERT(0); return; }
	MSPointMemberG* pSource = (MSPointMemberG*)pSource_in;

	m_SOffsetZ = pSource->m_SOffsetZ;
	m_dAxisRotation = pSource->m_dAxisRotation;
	m_ePointAlign = pSource->m_ePointAlign;
}

void MSPointMemberG::Init( bool bInConstructor/*=false*/ )
{
	if(!bInConstructor)
		MSCompoG::Init(bInConstructor);

	ma_Vec.push_back(new GM2DVector(INT_MAX, INT_MAX));
	ma_Vec_Input.push_back(new GM2DVector(INT_MAX, INT_MAX));

	m_SOffsetZ = 0;
	m_dAxisRotation = 0;
	m_ePointAlign = COLUMNALIGN_CENTER;
}

bool MSPointMemberG::IsInvalidVecInput()
{
	GM2DVector MaxVec(INT_MAX, INT_MAX);

	if (ma_Vec_Input.size() != 1)
		return true;
	
	if (fabs(ma_Vec_Input[0]->m_X - MaxVec.m_X) < DTOL_GM && fabs(ma_Vec_Input[0]->m_Y - MaxVec.m_Y) < DTOL_GM)
		return true;

	return false;
}

bool MSPointMemberG::VisibleInRect( GM2DRectangle* pRect)
{
    if(GetSVec()->VisibleInRect(*pRect))
        return true;

	if (GetProfile() == NULL && GetProfile()->GetNumLines() == 0)
		MakeBoundary();

	GM2DBound BoundBox;
	GetProfile()->Get2DBound(BoundBox);
	if (BoundBox.VisibleInRect(*pRect))
		return true;
	
    GM2DPolyline SecPGon(TRUE);
    GetBoundary(SecPGon);
    long nSecLine = SecPGon.GetNumLines();
    for(long iSecLine=0; iSecLine<nSecLine; iSecLine++)
    {
        GM2DLineVector aLine = SecPGon.GetLine(iSecLine);
        if(aLine.VisibleInRect(*pRect))
            return true;
    }
    return false;
}

bool MSPointMemberG::MakeBoundary()
{
	/*
	GM2DPolyline TheGon(TRUE);
    GetSecPolylineByInsertVec(TheGon );
	if (TheGon.GetSize() == 0)
	{
		return false;
	}

	mp_Boundary->CopyFrom(&TheGon);
	*/

	GM2DPolyline HaderPoly;
	GetHeaderPolylineByInsertVec(HaderPoly);

	GM2DPolyline TheGon(TRUE);
	GetSecPolylineByInsertVec(TheGon);
	if (TheGon.GetSize() == 0)
	{
		return false;
	}

	if (HaderPoly.GetSize() > 0)
		mp_Boundary->CopyFrom(&HaderPoly);
	else
		mp_Boundary->CopyFrom(&TheGon);

	return true;
}

long MSPointMemberG::GetColumnHeaderMID()
{
	MSRcLineMemberM* pCompoM = dynamic_cast<MSRcLineMemberM*>(GetCompoM());
	if (pCompoM == NULL)
		return 0;
	return pCompoM->GetColumnHeaderMID();
}

void MSPointMemberG::MirrorByAxis( GM2DLineVector& pAxis )
{
	MSCompoG::MirrorByAxis(pAxis);
	double AxisDeg = pAxis.GetDegree();
	double DeltaDeg = AxisDeg - m_dAxisRotation;
	m_dAxisRotation = AxisDeg + DeltaDeg;
}

void MSPointMemberG::RotateZ( double theta, GM2DVector *pCenter )
{
	MSCompoG::RotateZ(theta, pCenter);
	m_dAxisRotation += theta*Rad2Deg;
}
void MSPointMemberG::SetRotation(double dDeg)
{
	double dRad = dDeg*Deg2Rad - m_dAxisRotation*Deg2Rad;
	m_dAxisRotation = dDeg;

	GM2DVector BaseVec = *GetSVec();
	MSCompoG::RotateZ(dRad, &BaseVec);
}

vector<MSCompoG*> MSPointMemberG::FindConnectMember(FindType_MSElement memberType, long nFloorType, double dTol, bool bUseTrimedBoundry, bool bFindPolyMemberOnlyOnLine, bool bIncludeOnLine)
{
	return MSCompoGUtil::FindConnectMember(this, memberType, nFloorType, dTol, bUseTrimedBoundry, bFindPolyMemberOnlyOnLine, bIncludeOnLine);
}

double MSPointMemberG::GetMemberDegree()
{
	return m_dAxisRotation;
}

ePOINTALIGN_TYPE MSPointMemberG::GetPointAlignType()
{
	return m_ePointAlign;
}

void MSPointMemberG::SetPointAlignType(ePOINTALIGN_TYPE eType, bool bUpdateVecInput)
{
	m_ePointAlign = eType;

	/*
	if(bUpdateVecInput)
		UpdateVecInputByCenVec();
	UpdateCenVecByVecInput();
	*/
}

void MSPointMemberG::UpdateVecInputByCenVec()
{
	GM2DVector* pCenVec = GetSVec();
	if(pCenVec == NULL)
		return;
	GM2DVector* pNewVec = (GM2DVector*)pCenVec->Copy();
	double dWidth = GetWidth();
	double dDepth = GetDepth();
	
	GM2DVector vecMove(0, 0);
	switch (m_ePointAlign)
	{
	case COLUMNALIGN_TOPLEFT:
		vecMove.m_Y += dDepth/2;
		vecMove.m_X += -dWidth/2;
		break;
	case COLUMNALIGN_TOP:
		vecMove.m_Y += dDepth/2;
		break;
	case COLUMNALIGN_TOPRIGHT:
		vecMove.m_Y += dDepth/2;
		vecMove.m_X += dWidth/2;
		break;
	case COLUMNALIGN_LEFT:
		vecMove.m_X += -dWidth/2;
		break;
	case COLUMNALIGN_CENTER:
		break;
	case COLUMNALIGN_RIGHT:
		vecMove.m_X += dWidth/2;
		break;
	case COLUMNALIGN_BOTTOMLEFT:
		vecMove.m_Y += -dDepth/2;
		vecMove.m_X += -dWidth/2;
		break;
	case COLUMNALIGN_BOTTOM:
		vecMove.m_Y += -dDepth/2;
		break;
	case COLUMNALIGN_BOTTOMRIGHT:
		vecMove.m_Y += -dDepth/2;
		vecMove.m_X += dWidth/2;
		break;
	default:
		break;
	}

	if (m_ePointAlign != COLUMNALIGN_CENTER)
	{
		vecMove.RotateZ(m_dAxisRotation * Deg2Rad);
		pNewVec->m_X += vecMove.m_X;
		pNewVec->m_Y += vecMove.m_Y;
	}

	DeleteVecInput();
	ma_Vec_Input.push_back(pNewVec);
}

void MSPointMemberG::UpdateCenVecByVecInput()
{
	GM2DVector* pVecInput = GetSVecInput();
	if(pVecInput == NULL)
		return;
	GM2DVector* pNewCenVec = (GM2DVector*)pVecInput->Copy();
	double dWidth = GetWidth();
	double dDepth = GetDepth();

	GM2DVector vecMove(0, 0);
	switch (m_ePointAlign)
	{
	case COLUMNALIGN_TOPLEFT:
		vecMove.m_Y += -dDepth/2;
		vecMove.m_X += +dWidth/2;
		break;
	case COLUMNALIGN_TOP:
		vecMove.m_Y += -dDepth/2;
		break;
	case COLUMNALIGN_TOPRIGHT:
		vecMove.m_Y += -dDepth/2;
		vecMove.m_X += -dWidth/2;
		break;
	case COLUMNALIGN_LEFT:
		vecMove.m_X += dWidth/2;
		break;
	case COLUMNALIGN_CENTER:
		break;
	case COLUMNALIGN_RIGHT:
		vecMove.m_X += -dWidth/2;
		break;
	case COLUMNALIGN_BOTTOMLEFT:
		vecMove.m_Y += dDepth/2;
		vecMove.m_X += dWidth/2;
		break;
	case COLUMNALIGN_BOTTOM:
		vecMove.m_Y += dDepth/2;
		break;
	case COLUMNALIGN_BOTTOMRIGHT:
		vecMove.m_Y += dDepth/2;
		vecMove.m_X += -dWidth/2;
		break;
	default:
		break;
	}

	if (m_ePointAlign != COLUMNALIGN_CENTER)
	{
		vecMove.RotateZ(m_dAxisRotation * Deg2Rad);
		pNewCenVec->m_X += vecMove.m_X;
		pNewCenVec->m_Y += vecMove.m_Y;
	}

	ReplaceVec(0, pNewCenVec);
}

bool MSPointMemberG::SetCompoM(long MID, bool bUpdateVecInput)
{
	bool bRVal = MSCompoG::SetCompoM(MID);
	if(bRVal == false)
		return bRVal;
	
	if(bUpdateVecInput)
		UpdateCenVecByVecInput();
	return bRVal;
}

bool MSPointMemberG::SetCompoM(MSCompoM* pCompoM, bool bUpdateVecInput)
{
	bool bRVal = MSCompoG::SetCompoM(pCompoM);
	if(bRVal == false)
		return bRVal;
	
	if(bUpdateVecInput)
		UpdateCenVecByVecInput();
	return bRVal;
}

void MSPointMemberG::UpdateVecInput()
{
	UpdateVecInputByCenVec();
}

void MSPointMemberG::UpdateVecCenter()
{
	UpdateCenVecByVecInput();
}

void MSPointMemberG::GetSecPolylineByInsertVec(GM2DPolyline& APGon)
{
	MSCompoG::GetSecPolyline(APGon);
	TransformByInsertVecNRotation(APGon);
}

bool MSPointMemberG::GetSteelSecPolylineByInsertVec(GM2DPolyline& APGon)
{
	if (mp_CompoM == nullptr)
		return false;
	MSSteelSection* pSteelSection = mp_CompoM->GetSteelSection();
	if (pSteelSection == nullptr)
		return false;

	pSteelSection->GetSecPolyline(APGon);
	TransformByInsertVecNRotation(APGon);
	return true;
}

void MSPointMemberG::GetHeaderPolylineByInsertVec(GM2DPolyline& APGon)
{
	MSCompoG::GetHeaderPolyline(APGon);
	TransformByInsertVecNRotation(APGon);
}

void MSPointMemberG::TransformByInsertVecNRotation(GM2DPolyline& APGon)
{
	GM2DVector* pSVec = GetSVec();
	APGon.Translate(*pSVec);
	APGon.RotateZ(Deg2Rad*m_dAxisRotation, pSVec);
}

void MSPointMemberG::GetTrimmedBoundaryZLevel(GM2DPolyline& APoly, double dTopLevel, double dBotLevel)
{
	int nCHMID = GetColumnHeaderMID();
	MSColumnHeaderM* pColumnHeaderM = (MSColumnHeaderM*)GetBuilding()->GetCompoM(nCHMID);
	if (IsCustomM() || pColumnHeaderM == nullptr)
		return MSCompoG::GetTrimmedBoundaryZLevel(APoly, dTopLevel, dBotLevel);
	double dT1 = pColumnHeaderM->GetT1();
	double dTopPointLevel = 0, dBotPointLevel = 0;
	GetZLevels(dTopPointLevel, dBotPointLevel);

	GetSecPolylineByInsertVec(APoly);

	if (dTopPointLevel >= dTopLevel && dTopPointLevel - dT1 <= dTopLevel &&
		dTopPointLevel >= dBotLevel && dTopPointLevel - dT1 <= dBotLevel)
	{
		long nCHMID = GetColumnHeaderMID();
		if (nCHMID != 0)
			GetHeaderPolyline(APoly);
	}
}

CString MSPointMemberG::GetKey(bool bUseID)
{
	// ID, FloorNum, Type, CompoM, 좌표, 옵셋

	CString strCompoMKey;
	if (mp_CompoM == NULL || mp_CompoM->m_ID == 0)
		strCompoMKey.Format(_T("%d"), 0);
	else
		strCompoMKey.Format(_T("%d"), &mp_CompoM->m_ID);
	CString strGeo;
	strGeo.Format(_T("%d_%d"), (int)ma_Vec[0]->m_X, (int)ma_Vec[0]->m_Y);
	CString strOffSet;
	strOffSet.Format(_T("%d_%d"), (int)m_SOffsetZ, (int)m_dAxisRotation);

	CString strKey;
	strKey.Format(_T("%d_%d_%s_%s_%s"), GetFloorNum(), (int)GetType(), strCompoMKey, strGeo, strOffSet);
	if (bUseID)
	{
		CString strID;
		strID.Format(_T("%d_"), m_ID);
		strKey = strID + strKey;
	}

	return strKey;
}

bool MSPointMemberG::IsSameGeometry(MSCompoG* urCompoG, double distTolerance, double degreeTolerance)
{
	GM2DVector* myCen = GetSVec();
	GM2DVector* urCen = urCompoG->GetSVec();
	if(!myCen->SameVector4DTOLByCeil(*urCen, distTolerance))
		return false; 

	double myDegree = m_dAxisRotation;
	double urDegree = ((MSPointMemberG*)urCompoG)->m_dAxisRotation;

	if (fabs(myDegree - urDegree) > degreeTolerance)
		return false;
	return true;
}


CString MSPointMemberG::ToString()
{
	auto str = MSCompoG::ToString();
	str.AppendFormat(L"Position:%s, BotOffsetZ : %f, AxisRotation: %f, AlignType:%d, ", GetSVec()->ToString(), m_SOffsetZ, m_dAxisRotation, (int)m_ePointAlign);
	return str;
}