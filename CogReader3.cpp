#include "stdafx.h"

#include "GMLib/GMObject.h"
#include "GMLib/GM2DLineVector.h"
#include "GMLib/GMQuadTree.h"
#include "GMLib/GMText.h"

#include "StructureModel/MSCompoM.h"
#include "StructureModel/MSWallMemberG.h"
#include "StructureModel/MSCompoM.h"

#include "CogReader.h"
#include "DG2DLine.h"
#include "DG2DArc.h"

#include <math.h>

long CogReader::GetMSCompoMType(long nDGType)
{
	switch(nDGType)
	{
	case DG_BEAM:
	{
		if(m_MaterialType == MSMaterial::msConcrete)
		{
			return (long)MSCompoM::msBeamM;
		}
		else if(m_MaterialType == MSMaterial::msSteel)
			return (long)MSCompoM::msSteelBeamM;
		else if(m_MaterialType == MSMaterial::msSRC)
			return (long)MSCompoM::msSrcBeamM;
		else if (m_MaterialType == MSMaterial::msFormWork)
			return (long)MSCompoM::msBeamM;
	}
	case DG_WALL:
		return (long)MSCompoM::msBWallM;
	case DG_RWALL:
		return (long)MSCompoM::msRWallM;
	case DG_COLUMN:
	{
		if(m_MaterialType == MSMaterial::msConcrete)
			return (long)MSCompoM::msColumnM;
		else if(m_MaterialType == MSMaterial::msSteel)
			return (long)MSCompoM::msSteelColumnM;
		else if(m_MaterialType == MSMaterial::msSRC)
			return (long)MSCompoM::msSrcColumnM;
		else if(m_MaterialType == MSMaterial::msFormWork)
			return (long)MSCompoM::msColumnM;
	}
	case DG_ISOFOOT:
		return (long)MSCompoM::msIsoFootM;
    case DG_SLAB:
        return (long)MSCompoM::msSlabM;
	case DG_WFOOT:
		return (long)MSCompoM::msWallFootM;
	case DG_LOWWALL:
		return (long)MSCompoM::msHandRailM;
	case DG_PIT:
		return (long)MSCompoM::msPitM;
	default:
		ASSERT(0);
	}
	return 0;
}

MSElement::Type_MSCompoG CogReader::GetMSCompoGType()
{
	switch (m_nMemberType)
	{
	case DG_BEAM:	return MSElement::msBeamG;
	case DG_WALL:	return MSElement::msWallG;
	case DG_RWALL: return MSElement::msWallG;
	case DG_COLUMN:	return MSElement::msColumnG;
	case DG_ISOFOOT:		return MSElement::msIsoFootG;
	case DG_SLAB:	return MSElement::msSlabG;
	case DG_WFOOT:	return MSElement::msWallFootG;
	case DG_LOWWALL: return MSElement::msHandRailG;
	case DG_PIT:	return MSElement::msPitG;
	default:
		ASSERT(0);
	}
	return MSElement::msNULL;
}
// 1. 라인부재 범위내에 TextDistance내에 있는 Text중 Profile이 존재하는 가장가까운 Text를 찾는다(Ortho) 
//     - CenPoint, InsertPoint순서
// 2. 위에서 못찾은 경우, 각 끝점에서 TextDistance내의 Text를 찾는다
GMText* CogReader::FindGMText(DG2DLine* MyLine, DG2DLine* UrLine, long nDGType)
{
	if (m_IsFindMName == false)	return nullptr;

	if(MyLine->m_pLine->m_ObjType == GT_LINE)
	{
		double dDegreeTol = 5;
		double dDegree = ((GM2DLineVector*)MyLine->m_pLine)->GetDegree();
		double dMinDist = m_DTOL_TEXT_DIST;
		MSCompoM* pCompoM = NULL;

		GMText* pGMText=NULL;

		long NumPair = MyLine->ma_Pair.GetSize();
		for(long iPair=0; iPair<NumPair; iPair++)
			if(UrLine == MyLine->ma_Pair[iPair])
				break;
		long PairNum = iPair;

		vector<GMQuadTree*> CrossQTs;
		MergeLineQTs(MyLine, UrLine, CrossQTs);
		double LineDist = MyLine->Distance(UrLine->m_P0);
		double dHeight = LineDist;

		DG2DLine* shortLine = MyLine;
		DG2DLine* longLine = UrLine;
		if (MyLine->Length() > UrLine->Length())
		{
			shortLine = UrLine;
			longLine = MyLine;
		}

		long NumCrossQT = CrossQTs.size();
		CString Name = _T("");
		map<GMText*, GMText*> OutGMTextMap; //외부에 있는 GMText;
		vector<GMText*> FindGMTextArr; // QT에서 찾은 GMText;

		for (long iCrossQT = 0; iCrossQT < NumCrossQT; iCrossQT++)
		{
			GMQuadTree* QTree = CrossQTs[iCrossQT];
			CTypedPtrArray<CObArray, GM2DGeometry*>& TxtGMs = QTree->ma_GMs[DG_TEXT];

			long NumGMs = TxtGMs.GetSize();
			for (long iGMs = 0; iGMs < NumGMs; iGMs++)
			{
				GMText* TheGM = (GMText*)TxtGMs[iGMs];

				if (GetRecognizeMode() == eRecognizeMode::Single && !m_IsSaveM)
				{
					Name.Format(_T("%s%s%s"), m_Prefix, TheGM->m_strText, m_Suffix);
					// M이 정의 되어 있지 않는 객체는 "Def"라는 키워드를 넣어주기 위해, M이 없을때는 skip
					pCompoM = GetCompoM(Name, GetMSCompoMType(nDGType));  //검토요. 시간 많이 걸릴텐데 꼭 필요한가?
					if (pCompoM == NULL) continue;
				}

				double dDegree1 = fabs(dDegree - TheGM->m_dDegree);
				double dDegree2 = fabs(dDegree - (TheGM->m_dDegree + 180.));
				if (!((dDegree1 < dDegreeTol || fabs(dDegree1 - 360) < dDegreeTol || dDegree2 < dDegreeTol || fabs(dDegree2 - 360) < dDegreeTol)))
					continue;

				FindGMTextArr.push_back(TheGM);
			}
		}

		//짧은 라인으로 먼저 찾아본다.
		for (long iText = 0; iText < FindGMTextArr.size(); iText++)
		{
			GMText* TheGM = FindGMTextArr[iText];
			bool IsInnerText = true;
			// 내부점인지 검토 : CenterPoint
			double dLocValue = shortLine->OrthoBaseLocValue(TheGM->m_Vec);
			if (!(-DTOL_GM < dLocValue &&  dLocValue < 1. + DTOL_GM))
				IsInnerText = false;

			// 내부점인지 검토 : InsertPoint
			if (IsInnerText == false)
			{
				dLocValue = shortLine->OrthoBaseLocValue(TheGM->m_InsertVec);
				if ((-DTOL_GM < dLocValue &&  dLocValue < 1. + DTOL_GM))
					IsInnerText = true;
			}

			if (IsInnerText == false)
			{
				OutGMTextMap.insert(make_pair(TheGM, TheGM));
				continue;
			}

			double MyDist = MyLine->Distance(TheGM->m_Vec);
			MyDist = min(MyDist, MyLine->Distance(TheGM->m_InsertVec));
			double UrDist = UrLine->Distance(TheGM->m_Vec);
			UrDist = min(UrDist, UrLine->Distance(TheGM->m_InsertVec));

			double TextDist = min(MyDist, UrDist);
			if (TextDist < dMinDist)
			{
				pGMText = TheGM;
				dMinDist = TextDist;
			}
		}

		if (pGMText != NULL)
			return pGMText;

		// pGMText == NULL 인경우 긴선으로 다시 찾아 본다.
		for (long iText = 0; iText < FindGMTextArr.size(); iText++)
		{
			GMText* TheGM = FindGMTextArr[iText];
			bool IsInnerText = true;
			// 내부점인지 검토 : CenterPoint
			double dLocValue = longLine->OrthoBaseLocValue(TheGM->m_Vec);
			if (!(-DTOL_GM < dLocValue &&  dLocValue < 1. + DTOL_GM))
				IsInnerText = false;

			// 내부점인지 검토 : InsertPoint
			if (IsInnerText == false)
			{
				dLocValue = longLine->OrthoBaseLocValue(TheGM->m_InsertVec);
				if ((-DTOL_GM < dLocValue &&  dLocValue < 1. + DTOL_GM))
					IsInnerText = true;
			}

			if (IsInnerText == false)
			{
				OutGMTextMap.insert(make_pair(TheGM, TheGM));
				continue;
			}

			double MyDist = MyLine->Distance(TheGM->m_Vec);
			MyDist = min(MyDist, MyLine->Distance(TheGM->m_InsertVec));
			double UrDist = UrLine->Distance(TheGM->m_Vec);
			UrDist = min(UrDist, UrLine->Distance(TheGM->m_InsertVec));

			double TextDist = min(MyDist, UrDist);
			if (TextDist < dMinDist)
			{
				pGMText = TheGM;
				dMinDist = TextDist;
			}
		}

		if(pGMText != NULL)
			return pGMText;

		map<GMText*, GMText*>::iterator it;
		for (it = OutGMTextMap.begin(); it != OutGMTextMap.end(); it++)
		{
			GMText* TheGM = it->second;
			double MyP0Dist = MyLine->m_P0.Distance(TheGM->m_Vec);
			double MyP1Dist = MyLine->m_P1.Distance(TheGM->m_Vec);
			double UrP0Dist = UrLine->m_P0.Distance(TheGM->m_Vec);
			double UrP1Dist = UrLine->m_P1.Distance(TheGM->m_Vec);

			MyP0Dist = min(MyP0Dist, MyLine->m_P0.Distance(TheGM->m_InsertVec));
			MyP1Dist = min(MyP1Dist, MyLine->m_P1.Distance(TheGM->m_InsertVec));
			UrP0Dist = min(UrP0Dist, UrLine->m_P0.Distance(TheGM->m_InsertVec));
			UrP1Dist = min(UrP1Dist, UrLine->m_P1.Distance(TheGM->m_InsertVec));

			double TextDist = min(min(MyP0Dist, MyP1Dist), min(UrP0Dist, UrP1Dist));
			if (TextDist < dMinDist)
			{
				pGMText = TheGM;
				dMinDist = TextDist;
			}
		}
		
		return pGMText;
	}
	return NULL;
}

MSCompoM* CogReader::FindCompoM(GM2DVector* pSVec, GM2DVector* pEVec, double halfWidth, long nDGType)
{
	MSCompoM* pDefaultCompoM = GetDefaultCompoM();
	if (pDefaultCompoM != nullptr)	return pDefaultCompoM;

	GM2DLineVector compoGLVec(*pSVec, *pEVec);
	double dDegree = compoGLVec.GetDegree();
	CString Name;
	double dDegreeTol = 5;
	double dMinDist = m_DTOL_TEXT_DIST + halfWidth;
	GMText* pGMText=NULL;

	map<GMText*, MSCompoM*> TextToCompoMMap;
	map<GMText*, GMText*> OutGMTextMap; //외부에 있는 GMText;
	vector<GMText*> FindGMTextArr; // QT에서 찾은 GMText;

	CTypedPtrArray<CObArray, GM2DGeometry*>& TxtGMs = m_pQT->ma_GMs[DG_TEXT];
	int NumText = TxtGMs.GetSize();
	for(int iText = 0; iText < NumText; iText++)
	{
		GMText* TheGM = dynamic_cast<GMText*>(TxtGMs[iText]);
		if(TheGM == NULL)	continue;
		Name.Format(_T("%s%s%s"), m_Prefix, TheGM->m_strText, m_Suffix);
		// M이 정의 되어 있지 않는 객체는 "Def"라는 키워드를 넣어주기 위해, M이 없을때는 skip
		MSCompoM* pCompoM = GetCompoM(Name, GetMSCompoMType(nDGType));  //검토요. 시간 많이 걸릴텐데 꼭 필요한가?
		if (pCompoM == NULL) continue;

		double dDegree1 = fabs(dDegree - TheGM->m_dDegree);
		double dDegree2 = fabs(dDegree - (TheGM->m_dDegree + 180.));
		if (!((dDegree1 < dDegreeTol || fabs(dDegree1 - 360) < dDegreeTol || dDegree2 < dDegreeTol || fabs(dDegree2 - 360) < dDegreeTol)))
			continue;

		FindGMTextArr.push_back(TheGM);
		TextToCompoMMap.insert(make_pair(TheGM, pCompoM));
	}

	for (long iText = 0; iText < FindGMTextArr.size(); iText++)
	{
		GMText* TheGM = FindGMTextArr[iText];
		bool IsInnerText = true;
		// 내부점인지 검토 : CenterPoint
		double dLocValue = compoGLVec.OrthoBaseLocValue(TheGM->m_Vec);
		if (!(-DTOL_GM < dLocValue &&  dLocValue < 1. + DTOL_GM))
			IsInnerText = false;

		// 내부점인지 검토 : InsertPoint
		if (IsInnerText == false)
		{
			dLocValue = compoGLVec.OrthoBaseLocValue(TheGM->m_InsertVec);
			if ((-DTOL_GM < dLocValue &&  dLocValue < 1. + DTOL_GM))
				IsInnerText = true;
		}

		if (IsInnerText == false)
		{
			OutGMTextMap.insert(make_pair(TheGM, TheGM));
			continue;
		}

		double TextDist = compoGLVec.Distance(TheGM->m_Vec);
		TextDist = min(TextDist, compoGLVec.Distance(TheGM->m_InsertVec));

		if (TextDist < dMinDist)
		{
			pGMText = TheGM;
			dMinDist = TextDist;
		}
	}

	if (pGMText != NULL)
	{
		map<GMText*, MSCompoM*>::iterator it = TextToCompoMMap.find(pGMText);
		if(it != TextToCompoMMap.end())	return it->second;
	}

	map<GMText*, GMText*>::iterator it;
	for (it = OutGMTextMap.begin(); it != OutGMTextMap.end(); it++)
	{
		GMText* TheGM = it->second;
		double MyP0Dist = compoGLVec.m_P0.Distance(TheGM->m_Vec);
		double MyP1Dist = compoGLVec.m_P1.Distance(TheGM->m_Vec);

		MyP0Dist = min(MyP0Dist, compoGLVec.m_P0.Distance(TheGM->m_InsertVec));
		MyP1Dist = min(MyP1Dist, compoGLVec.m_P1.Distance(TheGM->m_InsertVec));

		double TextDist = min(MyP0Dist, MyP1Dist);
		if (TextDist < dMinDist)
		{
			pGMText = TheGM;
			dMinDist = TextDist;
		}
	}
	if (pGMText != NULL)
	{
		map<GMText*, MSCompoM*>::iterator it = TextToCompoMMap.find(pGMText);
		if(it != TextToCompoMMap.end())	return it->second;
	}

	return NULL;
}

MSCompoM* CogReader::FindTextNCreateLinkCompoMByHistory(GM2DVector* pSVec, GM2DVector* pEVec, double halfWidth, long nDGType)
{
	GM2DLineVector compoGLVec(*pSVec, *pEVec);
	double dDegree = compoGLVec.GetDegree();
	CString Name;
	double dDegreeTol = 5;
	double dMinDist = m_DTOL_TEXT_DIST + halfWidth;
	GMText* pGMText = NULL;

	map<GMText*, GMText*> OutGMTextMap; //외부에 있는 GMText;
	vector<GMText*> FindGMTextArr; // QT에서 찾은 GMText;

	CTypedPtrArray<CObArray, GM2DGeometry*>& TxtGMs = m_pQT->ma_GMs[DG_TEXT];
	int NumText = TxtGMs.GetSize();
	for (int iText = 0; iText < NumText; iText++)
	{
		GMText* TheGM = dynamic_cast<GMText*>(TxtGMs[iText]);
		if (TheGM == NULL)	continue;
		Name.Format(_T("%s%s%s"), m_Prefix, TheGM->m_strText, m_Suffix);

		double dDegree1 = fabs(dDegree - TheGM->m_dDegree);
		double dDegree2 = fabs(dDegree - (TheGM->m_dDegree + 180.));
		if (!((dDegree1 < dDegreeTol || fabs(dDegree1 - 360) < dDegreeTol || dDegree2 < dDegreeTol || fabs(dDegree2 - 360) < dDegreeTol)))
			continue;

		FindGMTextArr.push_back(TheGM);
	}

	for (long iText = 0; iText < FindGMTextArr.size(); iText++)
	{
		GMText* TheGM = FindGMTextArr[iText];
		bool IsInnerText = true;
		// 내부점인지 검토 : CenterPoint
		double dLocValue = compoGLVec.OrthoBaseLocValue(TheGM->m_Vec);
		if (!(-DTOL_GM < dLocValue &&  dLocValue < 1. + DTOL_GM))
			IsInnerText = false;

		// 내부점인지 검토 : InsertPoint
		if (IsInnerText == false)
		{
			dLocValue = compoGLVec.OrthoBaseLocValue(TheGM->m_InsertVec);
			if ((-DTOL_GM < dLocValue &&  dLocValue < 1. + DTOL_GM))
				IsInnerText = true;
		}

		if (IsInnerText == false)
		{
			OutGMTextMap.insert(make_pair(TheGM, TheGM));
			continue;
		}

		double TextDist = compoGLVec.Distance(TheGM->m_Vec);
		TextDist = min(TextDist, compoGLVec.Distance(TheGM->m_InsertVec));

		if (TextDist < dMinDist)
		{
			pGMText = TheGM;
			dMinDist = TextDist;
		}
	}

	if (pGMText == nullptr)
	{
		map<GMText*, GMText*>::iterator it;
		for (it = OutGMTextMap.begin(); it != OutGMTextMap.end(); it++)
		{
			GMText* TheGM = it->second;
			double MyP0Dist = compoGLVec.m_P0.Distance(TheGM->m_Vec);
			double MyP1Dist = compoGLVec.m_P1.Distance(TheGM->m_Vec);

			MyP0Dist = min(MyP0Dist, compoGLVec.m_P0.Distance(TheGM->m_InsertVec));
			MyP1Dist = min(MyP1Dist, compoGLVec.m_P1.Distance(TheGM->m_InsertVec));

			double TextDist = min(MyP0Dist, MyP1Dist);
			if (TextDist < dMinDist)
			{
				pGMText = TheGM;
				dMinDist = TextDist;
			}
		}
	}

	MSCompoM* pNewCompoM = nullptr;
	if (pGMText != NULL)
	{
		CString name = pGMText->m_strText;
		double dB = halfWidth * 2;
		switch (nDGType)
		{
		case DG_BEAM:	return CreateNewBeamM(dB, name);
		case DG_WALL: 	return CreateNewWallM(dB, name, true);
		case DG_RWALL:	return CreateNewWallM(dB, name, false);
		case DG_WFOOT: return CreateNewWallFootM(dB, name);
		case DG_LOWWALL: return CreateNewHandRailM(dB, name);
		}
	}

	return pNewCompoM;
}
