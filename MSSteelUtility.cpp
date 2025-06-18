#include "StdAfx.h"
#include "MSSteelUtility.h"

#include "MSSteelHSectionKr.h"
#include "MSSteelRectPipeKr.h"
#include "MSSteelRoundPipeKr.h"
#include "MSSteelTSectionKr.h"
#include "MSSteelCSectionKr.h"
#include "MSSteelDAngleKr.h"
#include "MSSteelAngleKr.h"
#include "MSSteelDeckPlateKr.h"

#define _SECTION_ERROR 1
#define _SECTION_EXIST   2
// MSSteelUtility* MSSteelUtility::m_pSteelUtil = NULL;
long MSSteelUtility::m_nSteelCode = STEEL_CODE_KOREAN;

IMPLEMENT_SERIAL( MSSteelUtility, CObject, VERSIONABLE_SCHEMA | 1 )
MSSteelUtility::MSSteelUtility()
{
	m_nErrorCode = 0;
}

MSSteelUtility::~MSSteelUtility(void)
{
	DeleteSections();
	ClearAddSections();
}

void MSSteelUtility::ClearAddSections()
{
	mm_AddSection.clear();
}

void MSSteelUtility::DeleteSections()
{
	DeleteSections(ma_HSectionKr);
	DeleteSections(ma_RectPipeKr);
	DeleteSections(ma_RoundPipeKr);
	DeleteSections(ma_TSectionKr);
	DeleteSections(ma_CSectionKr);
	DeleteSections(ma_DAngleKr);
	DeleteSections(ma_AngleKr);
	DeleteSections(ma_DeckPlateKr);
}

void MSSteelUtility::DeleteSections(vector<MSSteelSection*>& SectionArr)
{
	int NumSection = SectionArr.size();
	for(int i = 0; i < NumSection; i++)
		delete SectionArr[i];
	SectionArr.clear();
}

MSSteelSection* MSSteelUtility::GetSection(long nSectionType, long nSectionID)
{
	switch(m_nSteelCode)
	{
	case STEEL_CODE_KOREAN:
		return GetSection_Kr(nSectionType, nSectionID);	break;
	default:
		ASSERT(0);
	}
	return NULL;
}

MSSteelSection* MSSteelUtility::GetSection(long nSectionType, CString name)
{
	switch(m_nSteelCode)
	{
	case STEEL_CODE_KOREAN:
		return GetSection_Kr(nSectionType, name);	break;
	default:
		ASSERT(0);
	}
	return NULL;
}

long MSSteelUtility::MakeSections(msSteelSecType nSectionType, vector<MSSteelFileInfo*>& datas)
{
	switch(nSectionType)
	{
	case SECTION_H_TYPE:					return MakeSections_HSectionKr(datas);
	case SECTION_RECT_TYPE:				return MakeSections_RectPipeKr(datas);
	case SECTION_ROUND_TYPE:		return MakeSections_RoundPipeKr(datas);
	case SECTION_T_TYPE:						return MakeSections_TSectionKr(datas);
	case SECTION_C_TYPE:					return MakeSections_CSectionKr(datas);
	case SECTION_DANGLE_TYPE:		return MakeSections_DAngleKr(datas);
	case SECTION_ANGLE_TYPE:			return MakeSections_AngleKr(datas);
	case SECTION_DECK_TYPE:				return MakeSections_DeckPlateKr(datas);
	}
	return 0;
}
MSSteelSection* MSSteelUtility::GetSection_Kr(long nSectionType, long nSectionID)
{
	switch(nSectionType)
	{
	case SECTION_H_TYPE:					return GetSection_Kr(nSectionID, ma_HSectionKr);
	case SECTION_RECT_TYPE:				return GetSection_Kr(nSectionID, ma_RectPipeKr);
	case SECTION_ROUND_TYPE:		return GetSection_Kr(nSectionID, ma_RoundPipeKr);
	case SECTION_T_TYPE:						return GetSection_Kr(nSectionID, ma_TSectionKr);
	case SECTION_C_TYPE:					return GetSection_Kr(nSectionID, ma_CSectionKr);
	case SECTION_DANGLE_TYPE:		return GetSection_Kr(nSectionID, ma_DAngleKr);
	case SECTION_ANGLE_TYPE:			return GetSection_Kr(nSectionID, ma_AngleKr);
	case SECTION_DECK_TYPE:				return GetSection_Kr(nSectionID, ma_DeckPlateKr);
	}
	return NULL;
}

MSSteelSection* MSSteelUtility::GetSection_Kr(long nSectionType, CString SectionName)
{
	switch(nSectionType)
	{
	case SECTION_H_TYPE:					return GetSection_Kr(SectionName, ma_HSectionKr);
	case SECTION_RECT_TYPE:				return GetSection_Kr(SectionName, ma_RectPipeKr);
	case SECTION_ROUND_TYPE:		return GetSection_Kr(SectionName, ma_RoundPipeKr);
	case SECTION_T_TYPE:						return GetSection_Kr(SectionName, ma_TSectionKr);
	case SECTION_C_TYPE:					return GetSection_Kr(SectionName, ma_CSectionKr);
	case SECTION_DANGLE_TYPE:		return GetSection_Kr(SectionName, ma_DAngleKr);
	case SECTION_ANGLE_TYPE:			return GetSection_Kr(SectionName, ma_AngleKr);
	case SECTION_DECK_TYPE:				return GetSection_Kr(SectionName, ma_DeckPlateKr);
	}
	return NULL;
}


MSSteelSection* MSSteelUtility::BuiltUpSteelSection(long type, vector<double>& valueArr)
{
	MSSteelSection* pBuiltUp = NULL;
	switch(type)
	{
	case SECTION_H_TYPE:					pBuiltUp = BuiltUpSection_HSectionKr(valueArr); break;
	case SECTION_RECT_TYPE:				pBuiltUp = BuiltUpSection_RectPipeKr(valueArr); break;
	case SECTION_ROUND_TYPE:		pBuiltUp = BuiltUpSection_RoundPipeKr(valueArr); break; 
	case SECTION_T_TYPE:						pBuiltUp = BuiltUpSection_TSectionKr(valueArr); break;
	case SECTION_C_TYPE:					pBuiltUp = BuiltUpSection_CSectionKr(valueArr); break;
	//case SECTION_DANGLE_TYPE:		pBuiltUp = BuiltUpSection_DAngleKr(valueArr); break;
	case SECTION_ANGLE_TYPE:			pBuiltUp = BuiltUpSection_AngleKr(valueArr); break;
	//case SECTION_DECK_TYPE:				pBuiltUp = BuiltUpSection_DeckPlateKr(valueArr) break;
	}

	if (pBuiltUp)
	{
		if (CheckSteelSection(type, pBuiltUp) == false)
		{
			XErrorObject* pError = MSSteelUtility::CreateErrorObject(MESSAGE_ERROR_INPUT, ERROR_LEVEL, LocalDic::GetLocal(_T("철골 단면 정보에 오류가 있습니다.")), XErrorObject::AreaSelect);
			pError->SetGroupName(_T("Steel Section"));
			delete pBuiltUp;
			return nullptr;
		}

		MSSteelSection* pExistSection = IsSectionExist(type, pBuiltUp);
		if (pExistSection != nullptr)
		{
			XErrorObject* pError = MSSteelUtility::CreateErrorObject(MESSAGE_ERROR_OVERLAP, ERROR_LEVEL, LocalDic::GetLocal(_T("이미 존재하는 철골 단면입니다."), _T("TEXT")), XErrorObject::AreaSelect);
			pError->SetGroupName(_T("Steel Section"));
			delete pBuiltUp;
			return nullptr;
		}
		AddSection(type, pBuiltUp);
		map<long, vector<MSSteelSection*>>::iterator it = mm_AddSection.find(type);
		if (it != mm_AddSection.end())
		{
			it->second.push_back(pBuiltUp);
		}
		else
		{
			vector<MSSteelSection*> SectionArr;
			SectionArr.push_back(pBuiltUp);
			mm_AddSection.insert(make_pair(type, SectionArr));
		}
	}
	return pBuiltUp;
}

bool MSSteelUtility::CheckSteelSection(long type, MSSteelSection* section)
{
	bool rtnVal = false;
	switch (type)
	{
	case SECTION_H_TYPE:					rtnVal = CheckSection_HSectionKr(section);	break;
	case SECTION_RECT_TYPE:				rtnVal = CheckSection_RectPipeKr(section);		break;
	case SECTION_ROUND_TYPE:		rtnVal = CheckSection_RoundPipeKr(section);	break;
	case SECTION_T_TYPE:					rtnVal = CheckSection_TSectionKr(section);	break;
	case SECTION_C_TYPE:					rtnVal = CheckSection_CSectionKr(section);	break;
		//case SECTION_DANGLE_TYPE:	rtnVal = CheckSection_HSectionKr(section);	beak;
	case SECTION_ANGLE_TYPE:		rtnVal = CheckSection_AngleSectionKr(section);	break;
		//case SECTION_DECK_TYPE:			rtnVal = CheckSection_HSectionKr(section);	break;
	}
	return rtnVal;
}

MSSteelSection* MSSteelUtility::IsSectionExist(long type, MSSteelSection* section, bool findAll)
{
	switch (type)
	{
	case SECTION_H_TYPE:					return ExistSection_HSectionKr(section, findAll);
	case SECTION_RECT_TYPE:				return ExistSection_RectPipeKr(section, findAll);
	case SECTION_ROUND_TYPE:		return ExistSection_RoundPipeKr(section, findAll);
	case SECTION_T_TYPE:					return ExistSection_TSectionKr(section, findAll);
	case SECTION_C_TYPE:					return  ExistSection_CSectionKr(section, findAll);
		//case SECTION_DANGLE_TYPE:	return ExistSection_HSectionKr(section, findAll);
	case SECTION_ANGLE_TYPE:		return ExistSection_AngleSectionKr(section, findAll);
		//case SECTION_DECK_TYPE:			return ExistSection_HSectionKr(section, findAll);
	}
	return nullptr;
}

vector<MSSteelSection*> MSSteelUtility::GetSectionList(msSteelSecType type)
{
	switch(type)
	{
	case SECTION_H_TYPE:					return ma_HSectionKr;
	case SECTION_RECT_TYPE:				return ma_RectPipeKr;
	case SECTION_ROUND_TYPE:		return ma_RoundPipeKr;
	case SECTION_T_TYPE:						return ma_TSectionKr;
	case SECTION_C_TYPE:					return ma_CSectionKr;
	case SECTION_DANGLE_TYPE:		return ma_DAngleKr;
	case SECTION_ANGLE_TYPE:			return ma_AngleKr;
	case SECTION_DECK_TYPE:				return ma_DeckPlateKr;
	}
	
	vector<MSSteelSection*> rtnList;
	return rtnList;

}

long MSSteelUtility::GetSectionType(CString name)
{
	long sectionType = -1;
	name = name.MakeUpper();
	int indexDash = name.Find(_T("-"));
	if(indexDash == -1)							return SECTION_DECK_TYPE;

	if(name.Find(_T("H-")) != -1)			return SECTION_H_TYPE;
	if(name.Find(_T("B-")) != -1)			return SECTION_RECT_TYPE;
	if(name.Find(_T("P-")) != -1)			return SECTION_ROUND_TYPE;
	if(name.Find(_T("C-")) != -1) 			return SECTION_C_TYPE;
	if(name.Find(_T("T-")) != -1)			return SECTION_T_TYPE;
	if(name.Find(_T("2L-")) != -1)		return SECTION_DANGLE_TYPE;
	if(name.Find(_T("L-")) != -1)			return SECTION_ANGLE_TYPE;

	return sectionType;
}

XErrorObject* MSSteelUtility::CreateErrorObject(long nErrorCode, long nLevel, CString strMsg, XErrorObject::SelectType type)
{
	XErrorContainer* pMessageContainer = XErrorContainer::GetMessageContainer();
	XErrorObject* pMsgObject = pMessageContainer->CreateErrorObject(nErrorCode, nLevel, strMsg, type);
	pMessageContainer->AddErrorObject(pMsgObject);
	pMsgObject->m_Message.Add(strMsg);
	pMsgObject->SetShowSelectCompoG(FALSE);

	return pMsgObject;
}

vector<double> MSSteelUtility::Parsing_SteelName(CString steelName)
{
	vector<double> params;
	int indexDash = steelName.Find('-');
	if(indexDash == -1)	return params;

	CString sSectionData = steelName.Mid(indexDash+1, steelName.GetLength()-(indexDash+1));
	vector<CString> stringArr;
	int nData = ParsingTextData(sSectionData, stringArr, _T("X"));
	for(int iData = 0; iData < nData; iData++)
	{
		double value = _ttof(stringArr[iData]);
		params.push_back(value);
	}
	return params;
}

int MSSteelUtility::ParsingTextData(CString text, vector<CString>& dataArr, CString parser)
{
	CString sText = text.MakeUpper();

	parser = parser.MakeUpper();
	int length = sText.GetLength();
	int startIdx = 0;
	for (int i = 0; i < length; i++)
	{
		if (sText.GetAt(i) == parser)
		{
			CString data = sText.Mid(startIdx, (i - startIdx));
			dataArr.push_back(data);
			startIdx = i + 1;
		}
	}
	CString data = sText.Mid(startIdx, (length - startIdx));
	dataArr.push_back(data);
	return (int)dataArr.size();
}

MSSteelSection* MSSteelUtility::GetSection_Kr(long nSectionID, vector<MSSteelSection*>& SectionArr)
{
	long NumAngle = SectionArr.size();
	if(nSectionID >= NumAngle || nSectionID <0)
		return NULL;
	return SectionArr[nSectionID];
}

MSSteelSection* MSSteelUtility::GetSection_Kr(CString name, vector<MSSteelSection*>& SectionArr)
{
	name = name.MakeUpper();
	long NumSection = SectionArr.size();
	for(int i = 0; i < NumSection; i++)
	{
		MSSteelSection* pSection = SectionArr[i];
		if(pSection == NULL)	continue;
		if (pSection->IsSameSectionName(name, pSection->m_Name))
			return pSection;
	}
	return NULL;
}

MSSteelSection* MSSteelUtility::GetDefaultHSection()
{
	int NumSection = ma_HSectionKr.size();
	for(int i = 0; i < NumSection; i++)
	{
		MSSteelSection* pSection = ma_HSectionKr[i];
		if(pSection != NULL)	return pSection;
	}
	return NULL;
}

void MSSteelUtility::CreateDefaultHSection()
{
	MSSteelHSectionKr* pHSectionKr = new MSSteelHSectionKr;
	pHSectionKr->m_ID		= ma_HSectionKr.size();
	pHSectionKr->m_Name	= _T("H-100X50X5X7");
	pHSectionKr->m_H		= 100;
	pHSectionKr->m_B1		= 50;
	pHSectionKr->m_B2		= 50;
	pHSectionKr->m_WThk	= 5;
	pHSectionKr->m_FThk1	= 7;
	pHSectionKr->m_FThk2	= 7;
	pHSectionKr->m_r		    = 8;
	pHSectionKr->m_A    	= 1185;
	pHSectionKr->m_w		= 0.0093;
	pHSectionKr->m_DataType	= 0;

	ma_HSectionKr.push_back(pHSectionKr);
}

long MSSteelUtility::MakeSections_HSectionKr(vector<MSSteelFileInfo*>& datas)
{
	long NumData = datas.size();
	ma_HSectionKr.resize(NumData+1);
	for(long iData=0; iData<=NumData; iData++)
		ma_HSectionKr[iData] = NULL;

	long nHSectionKrData = 12;
	for(iData=0; iData<NumData; iData++)
	{
		MSSteelFileInfo* pInfo = datas[iData];
		long nData = pInfo->m_Data.GetSize();
		if(nData != nHSectionKrData)	continue;

		MSSteelHSectionKr* pHSectionKr = new MSSteelHSectionKr;
		pHSectionKr->m_ID		= _ttol(pInfo->m_Data[0]);
		pHSectionKr->m_Name	= pInfo->m_Data[1];
		pHSectionKr->m_H		= _tstof(pInfo->m_Data[2]);
		pHSectionKr->m_B1		= _tstof(pInfo->m_Data[3]);
		pHSectionKr->m_B2		= _tstof(pInfo->m_Data[4]);
		pHSectionKr->m_WThk	= _tstof(pInfo->m_Data[5]);
		pHSectionKr->m_FThk1	= _tstof(pInfo->m_Data[6]);
		pHSectionKr->m_FThk2	= _tstof(pInfo->m_Data[7]);
		pHSectionKr->m_r		    = _tstof(pInfo->m_Data[8]);
		pHSectionKr->m_A    	= _tstof(pInfo->m_Data[9]);
		pHSectionKr->m_w		= _tstof(pInfo->m_Data[10])*1000;//Ton/m->kg/m
		pHSectionKr->m_DataType	= _ttol(pInfo->m_Data[11]);

		ma_HSectionKr[pHSectionKr->m_ID] = pHSectionKr;
		delete pInfo;
	}
	return long(ma_HSectionKr.size());
}

long MSSteelUtility::MakeSections_RectPipeKr(vector<MSSteelFileInfo*>& datas)
{
	long NumData = datas.size();
	ma_RectPipeKr.resize(NumData+1);
	for(long iData=0; iData<=NumData; iData++)
		ma_RectPipeKr[iData] = NULL;

	long nRectPipeKrData = 11;
	for(iData=0; iData<NumData; iData++)
	{
		MSSteelFileInfo* pInfo = datas[iData];
		long nData = pInfo->m_Data.GetSize();
		if(nData != nRectPipeKrData)	continue;

		MSSteelRectPipeKr* pRectPipeKr = new MSSteelRectPipeKr;
		pRectPipeKr->m_ID		= _ttol(pInfo->m_Data[0]);
		pRectPipeKr->m_Name	= pInfo->m_Data[1];
		pRectPipeKr->m_H		= _tstof(pInfo->m_Data[2]);
		pRectPipeKr->m_B		= _tstof(pInfo->m_Data[3]);
		pRectPipeKr->m_WThk1 = _tstof(pInfo->m_Data[4]);
		pRectPipeKr->m_WThk2 = _tstof(pInfo->m_Data[5]);
		pRectPipeKr->m_FThk1 = _tstof(pInfo->m_Data[6]);
		pRectPipeKr->m_FThk2 = _tstof(pInfo->m_Data[7]);
		pRectPipeKr->m_A		= _tstof(pInfo->m_Data[8]);
		pRectPipeKr->m_w		= _tstof(pInfo->m_Data[9])*1000;//Ton/m->kg/m
		pRectPipeKr->m_DataType	= _ttol(pInfo->m_Data[10]);

		ma_RectPipeKr[pRectPipeKr->m_ID] = pRectPipeKr;
		delete pInfo;
	}
	return long(ma_RectPipeKr.size());}

long MSSteelUtility::MakeSections_RoundPipeKr(vector<MSSteelFileInfo*>& datas)
{
	long NumData = datas.size();
	ma_RoundPipeKr.resize(NumData+1);
	for(long iData=0; iData<=NumData; iData++)
		ma_RoundPipeKr[iData] = NULL;

	long nRoundPipeKrData = 7;
	for(iData=0; iData<NumData; iData++)
	{
		MSSteelFileInfo* pInfo = datas[iData];
		long nData = pInfo->m_Data.GetSize();
		if(nData != nRoundPipeKrData)	continue;

		MSSteelRoundPipeKr* pRoundPipeKr = new MSSteelRoundPipeKr;
		pRoundPipeKr->m_ID		= _ttol(pInfo->m_Data[0]);
		pRoundPipeKr->m_Name	= pInfo->m_Data[1];
		pRoundPipeKr->m_D		= _tstof(pInfo->m_Data[2]);
		pRoundPipeKr->m_Thk	= _tstof(pInfo->m_Data[3]);
		pRoundPipeKr->m_A = _tstof(pInfo->m_Data[4]);
		pRoundPipeKr->m_w		= _tstof(pInfo->m_Data[5])*1000; //Ton/m->kg/m
		pRoundPipeKr->m_DataType	= _ttol(pInfo->m_Data[6]);

		ma_RoundPipeKr[pRoundPipeKr->m_ID] = pRoundPipeKr;
		delete pInfo;
	}
	return long(ma_RoundPipeKr.size());}

long MSSteelUtility::MakeSections_TSectionKr(vector<MSSteelFileInfo*>& datas)
{
	long NumData = datas.size();
	ma_TSectionKr.resize(NumData+1);
	for(long iData=0; iData<=NumData; iData++)
		ma_TSectionKr[iData] = NULL;

	long nTSectionKrData = 10;
	for(iData=0; iData<NumData; iData++)
	{
		MSSteelFileInfo* pInfo = datas[iData];
		long nData = pInfo->m_Data.GetSize();
		if(nData != nTSectionKrData)	continue;

		MSSteelTSectionKr* pTSectionKr = new MSSteelTSectionKr;
		pTSectionKr->m_ID		= _ttol(pInfo->m_Data[0]);
		pTSectionKr->m_Name	= pInfo->m_Data[1];
		pTSectionKr->m_H		= _tstof(pInfo->m_Data[2]);
		pTSectionKr->m_B		= _tstof(pInfo->m_Data[3]);
		pTSectionKr->m_WThk	= _tstof(pInfo->m_Data[4]);
		pTSectionKr->m_FThk	= _tstof(pInfo->m_Data[5]);
		pTSectionKr->m_r		    = _tstof(pInfo->m_Data[6]);
		pTSectionKr->m_A		= _tstof(pInfo->m_Data[7]);
		pTSectionKr->m_w		= _tstof(pInfo->m_Data[8])*1000;//Ton/m->kg/m;
		pTSectionKr->m_DataType	= _ttol(pInfo->m_Data[9]);

		ma_TSectionKr[pTSectionKr->m_ID] = pTSectionKr;
		delete pInfo;
	}
	return long(ma_TSectionKr.size());
}

long MSSteelUtility::MakeSections_CSectionKr(vector<MSSteelFileInfo*>& datas)
{
	long NumData = datas.size();
	ma_CSectionKr.resize(NumData+1);
	for(long iData=0; iData<=NumData; iData++)
		ma_CSectionKr[iData] = NULL;

	long nCSectionKrData = 13;
	for(iData=0; iData<NumData; iData++)
	{
		MSSteelFileInfo* pInfo = datas[iData];
		long nData = pInfo->m_Data.GetSize();
		if(nData != nCSectionKrData)	continue;

		MSSteelCSectionKr* pCSectionKr = new MSSteelCSectionKr;
		pCSectionKr->m_ID		= _ttol(pInfo->m_Data[0]);
		pCSectionKr->m_Name	= pInfo->m_Data[1];
		pCSectionKr->m_H		= _tstof(pInfo->m_Data[2]);
		pCSectionKr->m_B1		= _tstof(pInfo->m_Data[3]);
		pCSectionKr->m_B2		= _tstof(pInfo->m_Data[4]);
		pCSectionKr->m_WThk	= _tstof(pInfo->m_Data[5]);
		pCSectionKr->m_FThk1	= _tstof(pInfo->m_Data[6]);
		pCSectionKr->m_FThk2	= _tstof(pInfo->m_Data[7]);
		pCSectionKr->m_r1	    = _tstof(pInfo->m_Data[8]);
		pCSectionKr->m_r2	    = _tstof(pInfo->m_Data[9]);
		pCSectionKr->m_A		= _tstof(pInfo->m_Data[10]);
		pCSectionKr->m_w		= _tstof(pInfo->m_Data[11])*1000;//Ton/m->kg/m
		pCSectionKr->m_DataType	= _ttol(pInfo->m_Data[12]);

		ma_CSectionKr[pCSectionKr->m_ID] = pCSectionKr;
		delete pInfo;
	}
	return long(ma_CSectionKr.size());}

long MSSteelUtility::MakeSections_DAngleKr(vector<MSSteelFileInfo*>& datas)
{
	long NumData = datas.size();
	ma_DAngleKr.resize(NumData+1);
	for(long iData=0; iData<=NumData; iData++)
		ma_DAngleKr[iData] = NULL;

	long nDAngleKrData = 11;
	for(iData=0; iData<NumData; iData++)
	{
		MSSteelFileInfo* pInfo = datas[iData];
		long nData = pInfo->m_Data.GetSize();
		if(nData != nDAngleKrData)	continue;

		MSSteelDAngleKr* pDAngleKr = new MSSteelDAngleKr;
		pDAngleKr->m_ID		= _ttol(pInfo->m_Data[0]);
		pDAngleKr->m_Name	= pInfo->m_Data[1];
		pDAngleKr->m_H		= _tstof(pInfo->m_Data[2]);
		pDAngleKr->m_B		= _tstof(pInfo->m_Data[3]);
		pDAngleKr->m_WThk	= _tstof(pInfo->m_Data[4]);
		pDAngleKr->m_FThk	= _tstof(pInfo->m_Data[5]);
		pDAngleKr->m_r1		= _tstof(pInfo->m_Data[6]);
		pDAngleKr->m_r2		= _tstof(pInfo->m_Data[7]);
		pDAngleKr->m_A		= _tstof(pInfo->m_Data[8]);
		pDAngleKr->m_w		= _tstof(pInfo->m_Data[9]);
		pDAngleKr->m_DataType	= _ttol(pInfo->m_Data[10]);

		ma_DAngleKr[pDAngleKr->m_ID] = pDAngleKr;
		delete pInfo;
	}
	return long(ma_DAngleKr.size());
}

long MSSteelUtility::MakeSections_AngleKr(vector<MSSteelFileInfo*>& datas)
{
	long NumData = datas.size();
	ma_AngleKr.resize(NumData+1);
	for(long iData=0; iData<=NumData; iData++)
		ma_AngleKr[iData] = NULL;

	long nsize = ma_AngleKr.size();
	long nAngleKrData = 11;
	for(iData=0; iData<NumData; iData++)
	{
		MSSteelFileInfo* pInfo = datas[iData];
		long nData = pInfo->m_Data.GetSize();
		if(nData != nAngleKrData)	continue;

		MSSteelAngleKr* pAngleKr = new MSSteelAngleKr;
		pAngleKr->m_ID		= _ttol(pInfo->m_Data[0]);
		pAngleKr->m_Name	= pInfo->m_Data[1];
		pAngleKr->m_H		= _tstof(pInfo->m_Data[2]);
		pAngleKr->m_B		= _tstof(pInfo->m_Data[3]);
		pAngleKr->m_WThk	= _tstof(pInfo->m_Data[4]);
		pAngleKr->m_FThk	= _tstof(pInfo->m_Data[5]);
		pAngleKr->m_r1		= _tstof(pInfo->m_Data[6]);
		pAngleKr->m_r2		= _tstof(pInfo->m_Data[7]);
		pAngleKr->m_A		= _tstof(pInfo->m_Data[8]);
		pAngleKr->m_w		= _tstof(pInfo->m_Data[9])*1000;//Ton/m->kg/m;;
		pAngleKr->m_DataType	= _ttol(pInfo->m_Data[10]);

		ma_AngleKr[pAngleKr->m_ID] = pAngleKr;
		delete pInfo;
	}
	return long(ma_AngleKr.size());
}

long MSSteelUtility::MakeSections_DeckPlateKr(vector<MSSteelFileInfo*>& datas)
{
	long NumData = datas.size();
	ma_DeckPlateKr.resize(NumData+1);
	for(long iData=0; iData<=NumData; iData++)
		ma_DeckPlateKr[iData] = NULL;

	long nDeckPlateKrData = 14;
	for(iData=0; iData<NumData; iData++)
	{
		MSSteelFileInfo* pInfo = datas[iData];
		long nData = pInfo->m_Data.GetSize();
		if(nData != nDeckPlateKrData)	continue;

		MSSteelDeckPlateKr* pDeckPlateKr = new MSSteelDeckPlateKr;
		pDeckPlateKr->m_ID		= _ttol(pInfo->m_Data[0]);
		pDeckPlateKr->m_Name	= pInfo->m_Data[1];
		pDeckPlateKr->m_H		= _tstof(pInfo->m_Data[2]);
		pDeckPlateKr->m_B     = _tstof(pInfo->m_Data[3]);
		pDeckPlateKr->m_Lw		= _tstof(pInfo->m_Data[4]);
		pDeckPlateKr->m_Uw		= _tstof(pInfo->m_Data[5]);
		pDeckPlateKr->m_Thk		= _tstof(pInfo->m_Data[6]);
		pDeckPlateKr->m_Area		= _tstof(pInfo->m_Data[7]);
		pDeckPlateKr->m_W		= _tstof(pInfo->m_Data[8]);
		pDeckPlateKr->m_y     = _ttol(pInfo->m_Data[9]);
		pDeckPlateKr->m_I		= _tstof(pInfo->m_Data[10]);
		pDeckPlateKr->m_Zp		= _tstof(pInfo->m_Data[11]);
		pDeckPlateKr->m_Zn		= _tstof(pInfo->m_Data[12]);
		pDeckPlateKr->m_ht		= _tstof(pInfo->m_Data[13]);

		ma_DeckPlateKr[pDeckPlateKr->m_ID] = pDeckPlateKr;
		delete pInfo;
	}
	return long(ma_DeckPlateKr.size());}

MSSteelSection* MSSteelUtility::BuiltUpSection_HSectionKr(vector<double>& params)
{
	int nParams = params.size();

	double H = 0, B1 = 0, B2 = 0, WThk = 0, FThk1 = 0, FThk2 = 0;
	if (nParams == 6 )
	{
		H = params[0];	B1 = params[1];	B2 = params[2];	 WThk = params[3]; FThk1 = params[4];	FThk2 = params[5];
	}
	else if(nParams == 4)
	{
		H = params[0];	B1 = B2 = params[1];	WThk = params[2];	FThk1 = FThk2 = params[3];
	}
	else
		return NULL;

	MSSteelHSectionKr* pBuiltUp = new MSSteelHSectionKr();
	pBuiltUp->m_ID = ma_HSectionKr.size();
	pBuiltUp->m_Name = (_T("BH-")+pBuiltUp->MakeName(params));
	pBuiltUp->m_DataType = 1;
	pBuiltUp->SetH(H);
	pBuiltUp->SetB1(B1);
	pBuiltUp->SetB2(B2);
	pBuiltUp->SetWThk(WThk);
	pBuiltUp->SetFThk1(FThk1);
	pBuiltUp->SetFThk2(FThk2);

	double FlngArea, WebArea;
	FlngArea = (pBuiltUp->m_B1)*(pBuiltUp->m_FThk1) + (pBuiltUp->m_B2)*(pBuiltUp->m_FThk2);
	WebArea = (pBuiltUp->m_WThk)*(pBuiltUp->m_H - pBuiltUp->m_FThk1 - pBuiltUp->m_FThk2);

	pBuiltUp->m_A = WebArea+FlngArea;
	pBuiltUp->m_r = 0.0;////
	pBuiltUp->m_w = (pBuiltUp->m_A)*MSSteelSection::_STEEL_DENSITY*1000;////Ton/m -> kg/m

	return pBuiltUp;
}

MSSteelSection* MSSteelUtility::BuiltUpSection_RectPipeKr(vector<double>& params)
{
	int nParams = params.size();

	double H = 0, B = 0, WThk1 = 0, WThk2 = 0, FThk1 = 0, FThk2 = 0;
	if (nParams == 6)
	{
		H = params[0]; B = params[1]; WThk1 = params[2]; WThk2 = params[3]; FThk1 = params[4]; FThk2 = params[5];
	}
	else if (nParams == 4)
	{
		H = params[0]; B = params[1]; WThk1 = WThk2 = params[2]; FThk1 = FThk2 = params[3];
	}
	else if (nParams == 3)
	{
		H = params[0]; B = params[1]; WThk1 = WThk2 = FThk1 = FThk2 = params[2]; 
	}
	else
		return NULL;

	MSSteelRectPipeKr* pBuiltUp = new MSSteelRectPipeKr();
	pBuiltUp->m_ID = ma_RectPipeKr.size();
	pBuiltUp->m_Name = (_T("BB-")+pBuiltUp->MakeName(params));
	pBuiltUp->m_DataType = 1;
	pBuiltUp->SetH(H);
	pBuiltUp->SetB(B);
	pBuiltUp->SetWThk1(WThk1);
	pBuiltUp->SetWThk2(WThk2);
	pBuiltUp->SetFThk1(FThk1);
	pBuiltUp->SetFThk2(FThk2);

	double insideH, insideB;
	insideH = (pBuiltUp->m_H) - (pBuiltUp->m_FThk1) - (pBuiltUp->m_FThk2);
	insideB = pBuiltUp->m_B - pBuiltUp->m_WThk1 - pBuiltUp->m_WThk2;

	pBuiltUp->m_A = pBuiltUp->m_H * pBuiltUp->m_B - insideB*insideH;

	pBuiltUp->m_w = (pBuiltUp->m_A)*MSSteelSection::_STEEL_DENSITY*1000;//Ton/m->kg/m

	return pBuiltUp;
}

MSSteelSection* MSSteelUtility::BuiltUpSection_RoundPipeKr(vector<double>& params)
{
	int nParams = params.size();

	double D = 0, Thk = 0;
	if (nParams == 2)
	{
		D = params[0]; Thk = params[1];
	}
	else
		return NULL;

	MSSteelRoundPipeKr* pBuiltUp = new MSSteelRoundPipeKr();
	pBuiltUp->m_ID = ma_RoundPipeKr.size();
	pBuiltUp->m_Name = (_T("BP-")+pBuiltUp->MakeName(params));
	pBuiltUp->m_DataType = 1;
	pBuiltUp->SetD(D);
	pBuiltUp->SetThk(Thk);

	double outsideR, insideR; 
	outsideR = (pBuiltUp->m_D) / 2;
	insideR = outsideR - (pBuiltUp->m_Thk);

	pBuiltUp->m_A = (outsideR*outsideR - insideR*insideR)*PI;

	pBuiltUp->m_w = (pBuiltUp->m_A)*MSSteelSection::_STEEL_DENSITY*1000;//Ton/m->kg/m

	return pBuiltUp;
}

MSSteelSection* MSSteelUtility::BuiltUpSection_TSectionKr(vector<double>& params)
{
	int nParams = params.size();

	double H = 0, B = 0, WThk = 0, FThk = 0;
	if (nParams == 4)
	{
		H = params[0]; B = params[1]; WThk = params[2]; FThk = params[3];
	}
	else
		return NULL;

	MSSteelTSectionKr* pBuiltUp = new MSSteelTSectionKr();
	pBuiltUp->m_ID = ma_TSectionKr.size();
	pBuiltUp->m_Name = (_T("BT-") + pBuiltUp->MakeName(params));
	pBuiltUp->m_DataType = 1;
	pBuiltUp->SetH(H);
	pBuiltUp->SetB(B);
	pBuiltUp->SetWThk(WThk);
	pBuiltUp->SetFThk(FThk);

	pBuiltUp->m_A = ((H - FThk) * WThk) + (B * FThk);

	pBuiltUp->m_w = (pBuiltUp->m_A)*MSSteelSection::_STEEL_DENSITY * 1000;//Ton/m->kg/m

	return pBuiltUp;
}

MSSteelSection* MSSteelUtility::BuiltUpSection_CSectionKr(vector<double>& params)
{
	int nParams = params.size();

	double H = 0, B1 = 0, B2 = 0, WThk = 0, FThk1 = 0, FThk2 = 0;
	if (nParams == 6)
	{
		H = params[0];	B1 = params[1];	B2 = params[2];	 WThk = params[3]; FThk1 = params[4];	FThk2 = params[5];
	}
	else if (nParams == 4)
	{
		H = params[0];	B1 = B2 = params[1];	WThk = params[2];	FThk1 = FThk2 = params[3];
	}
	else
		return NULL;

	MSSteelCSectionKr* pBuiltUp = new MSSteelCSectionKr();
	pBuiltUp->m_ID = ma_CSectionKr.size();
	pBuiltUp->m_Name = (_T("BC-")+pBuiltUp->MakeName(params));
	pBuiltUp->m_DataType = 1;
	pBuiltUp->SetH(H);
	pBuiltUp->SetB1(B1);
	pBuiltUp->SetB2(B2);
	pBuiltUp->SetWThk(WThk);
	pBuiltUp->SetFThk1(FThk1);
	pBuiltUp->SetFThk2(FThk2);

	double FlngArea, WebArea;
	FlngArea = (pBuiltUp->m_B1)*(pBuiltUp->m_FThk1) + (pBuiltUp->m_B2)*(pBuiltUp->m_FThk2);
	WebArea = (pBuiltUp->m_WThk)*(pBuiltUp->m_H - pBuiltUp->m_FThk1 - pBuiltUp->m_FThk2);

	pBuiltUp->m_A = WebArea + FlngArea;
	pBuiltUp->m_r1 = 0.0;
	pBuiltUp->m_r2 = 0.0;
	pBuiltUp->m_w = (pBuiltUp->m_A)*MSSteelSection::_STEEL_DENSITY*1000; //Ton/m->kg/m

	return pBuiltUp;
}

//		MSSteelSection* MSSteelUtility::BuiltUpSection_DAngleKr			(vector<double>& valueArr);
MSSteelSection* MSSteelUtility::BuiltUpSection_AngleKr(vector<double>& params)
{
	int nParams = params.size();

	double H = 0, B = 0, WThk = 0, FThk = 0;
	if (nParams == 4)
	{
		H = params[0]; B = params[1]; WThk = params[2]; FThk = params[3];
	}
	else
		return NULL;

	MSSteelAngleKr* pBuiltUp = new MSSteelAngleKr();
	pBuiltUp->m_ID = ma_AngleKr.size();
	pBuiltUp->m_Name = (_T("BL-") + pBuiltUp->MakeName(params));
	pBuiltUp->m_DataType = 1;
	pBuiltUp->SetH(H);
	pBuiltUp->SetB(B);
	pBuiltUp->SetWThk(WThk);
	pBuiltUp->SetFThk(FThk);

	pBuiltUp->m_A = ((B - WThk) * FThk) + (H * WThk);

	pBuiltUp->m_w = (pBuiltUp->m_A)*MSSteelSection::_STEEL_DENSITY * 1000;//Ton/m->kg/m
	return pBuiltUp;
}
//		MSSteelSection* MSSteelUtility::BuiltUpSection_DeckPlateKr		(vector<double>& valueArr);
#pragma region  --- Check Section
bool MSSteelUtility::CheckSection_HSectionKr(MSSteelSection* section)
{
	if (section->m_nSecType != SECTION_H_TYPE)
		return false;
	MSSteelHSectionKr* HSection = (MSSteelHSectionKr*)section;

	if (HSection->GetH() == 0 || HSection->GetB1() == 0 || HSection->GetB2() == 0
		|| HSection->GetWThk() == 0 || HSection->GetFThk1() == 0 || HSection->GetFThk2() == 0)
		return false;

	if (HSection->GetB1() <= HSection->GetWThk())
		return false;
	if (HSection->GetB2() <= HSection->GetWThk())
		return false;
	if (HSection->GetH() <= (HSection->GetFThk1() + HSection->GetFThk2()))
		return false;

	return true;
}

bool MSSteelUtility::CheckSection_RectPipeKr(MSSteelSection* section)
{
	if (section->m_nSecType != SECTION_RECT_TYPE)
		return false;
	MSSteelRectPipeKr* BSection = (MSSteelRectPipeKr*)section;

	if (BSection->GetH() == 0 || BSection->GetB() == 0 
		|| BSection->GetWThk1() == 0 || BSection->GetWThk2() == 0 || BSection->GetFThk1() == 0 || BSection->GetFThk2() == 0)
		return false;

	if (BSection->GetB() <= (BSection->GetWThk1()+ BSection->GetWThk2()))
		return false;
	if (BSection->GetH() <= (BSection->GetFThk1() + BSection->GetFThk2()))
		return false;

	return true;
}

bool MSSteelUtility::CheckSection_RoundPipeKr(MSSteelSection* section)
{
	if (section->m_nSecType != SECTION_ROUND_TYPE)
		return false;
	MSSteelRoundPipeKr* PSection = (MSSteelRoundPipeKr*)section;

	if (PSection->GetD() == 0 || PSection->GetWThk() == 0)
		return false;

	if (PSection->GetD() <= 2 * PSection->GetWThk())
		return false;

	return true;
}

bool MSSteelUtility::CheckSection_TSectionKr(MSSteelSection* section)
{
	if (section->m_nSecType != SECTION_T_TYPE)
		return false;
	MSSteelTSectionKr* TSection = (MSSteelTSectionKr*)section;

	if (TSection->GetH() == 0 || TSection->GetB() == 0 || TSection->GetFThk() == 0 || TSection->GetWThk() == 0)
		return false;

	if (TSection->GetH() <= TSection->GetFThk())
		return false;
	if (TSection->GetB() <= TSection->GetWThk())
		return false;

	return true;
}

bool MSSteelUtility::CheckSection_CSectionKr(MSSteelSection* section)
{
	if (section->m_nSecType != SECTION_C_TYPE)
		return false;
	MSSteelCSectionKr* CSection = (MSSteelCSectionKr*)section;

	if (CSection->GetH() == 0 || CSection->GetB1() == 0 || CSection->GetB2() == 0
		|| CSection->GetWThk() == 0 || CSection->GetFThk1() == 0 || CSection->GetFThk2() == 0)
		return false;

	if (CSection->GetB1() <= CSection->GetWThk())
		return false;
	if (CSection->GetB2() <= CSection->GetWThk())
		return false;
	if (CSection->GetH() <= (CSection->GetFThk1() + CSection->GetFThk2()))
		return false;

	return true;
}

//		bool MSSteelUtility::CheckSection_DAngleKr			(MSSteelSection* pSection);
bool MSSteelUtility::CheckSection_AngleSectionKr(MSSteelSection* section)
{
	if (section->m_nSecType != SECTION_ANGLE_TYPE)
		return false;
	MSSteelAngleKr* ASection = (MSSteelAngleKr*)section;

	if (ASection->GetH() == 0 || ASection->GetB() == 0 || ASection->GetFThk() == 0 || ASection->GetWThk() == 0)
		return false;

	if (ASection->GetH() <= ASection->GetFThk())
		return false;
	if (ASection->GetB() <= ASection->GetWThk())
		return false;

	return true;
}
//		bool MSSteelUtility::CheckSection_DeckPlateKr		(MSSteelSection* pSection);
#pragma endregion

#pragma region --- exist section

bool MSSteelUtility::IsSameValue(double myVal, double urVal, double tolerance)
{
	if(fabs(myVal - urVal) < tolerance)
		return true;
	return false;
}

MSSteelSection* MSSteelUtility::ExistSection_HSectionKr(MSSteelSection* section, bool findAll)
{
	if (findAll == false)
	{
		int count = 0;
		for (int i = 1; i < ma_HSectionKr.size(); i++)
		{
			if (ma_HSectionKr.at(i)->m_DataType == 1)
				count += 1;
		}
		if (count == 0)	return nullptr;
	}
	if (section->m_nSecType != SECTION_H_TYPE)	return nullptr;

	MSSteelHSectionKr* mySection = (MSSteelHSectionKr*)section;

	double myH = mySection->GetH();
	double myB1 = mySection->GetB1();
	double myB2 = mySection->GetB2();
	double myWThk = mySection->GetWThk();
	double myFThk1 = mySection->GetFThk1();
	double myFThk2 = mySection->GetFThk2();
	for (int i = 1; i < ma_HSectionKr.size(); i++)
	{
		MSSteelSection* urSection = ma_HSectionKr[i];
		if (urSection->m_DataType == 1 || findAll == true) //Check in only BuiltUpSection
		{
			double urH = urSection->GetH();
			double urB1 = urSection->GetB1();
			double urB2 = urSection->GetB2();
			double urWThk = urSection->GetWThk();
			double urFThk1 = urSection->GetFThk1();
			double urFThk2 = urSection->GetFThk2();

			if (IsSameValue(myH, urH) && IsSameValue(myB1, urB1) && IsSameValue(myB2, urB2) && 
				IsSameValue(myWThk, urWThk) && IsSameValue(myFThk1, urFThk1) && IsSameValue(myFThk2, urFThk2))
					return urSection;
		}
	}
	return nullptr;
}

MSSteelSection* MSSteelUtility::ExistSection_RectPipeKr(MSSteelSection* section, bool findAll)
{
	if (findAll == false)
	{
		int count = 0;
		for (int i = 1; i < ma_RectPipeKr.size(); i++)
		{
			if (ma_RectPipeKr.at(i)->m_DataType == 1)
				count += 1;
		}
		if (count == 0)	return nullptr;
	}
	if (section->m_nSecType != SECTION_RECT_TYPE)	return nullptr;
	MSSteelRectPipeKr* mySection = (MSSteelRectPipeKr*)section;

	double myH = mySection->GetH();
	double myB = mySection->GetB();
	double myWThk1 = mySection->GetWThk1();
	double myWThk2 = mySection->GetWThk2();
	double myFThk1 = mySection->GetFThk1();
	double myFThk2 = mySection->GetFThk2();
	for (int i = 1; i < ma_RectPipeKr.size(); i++)
	{
		MSSteelSection* urSection = ma_RectPipeKr[i];
		if (urSection->m_DataType == 1 || findAll == true) //Check in only BuiltUpSection
		{
			double urH = urSection->GetH();
			double urB = urSection->GetB();
			double urWThk1 = urSection->GetWThk1();
			double urWThk2 = urSection->GetWThk2();
			double urFThk1 = urSection->GetFThk1();
			double urFThk2 = urSection->GetFThk2();
			if (IsSameValue(myH, urH) && IsSameValue(myB, urB) && IsSameValue(myWThk1, urWThk1) &&
				IsSameValue(myWThk2, urWThk2) && IsSameValue(myFThk1, urFThk1) && IsSameValue(myFThk2, urFThk2))
					return urSection;
		}
	}
	return nullptr;
}

MSSteelSection* MSSteelUtility::ExistSection_RoundPipeKr(MSSteelSection* section, bool findAll)
{
	if (findAll == false)
	{
		int count = 0;
		for (int i = 1; i < ma_RoundPipeKr.size(); i++)
		{
			if (ma_RoundPipeKr.at(i)->m_DataType == 1)
				count += 1;
		}
		if (count == 0)	return nullptr;
	}
	if (section->m_nSecType != SECTION_ROUND_TYPE)	return nullptr;
	MSSteelRoundPipeKr* mySection = (MSSteelRoundPipeKr*)section;

	double myH = mySection->GetH();
	double myWThk = mySection->GetWThk();
	for (int i = 1; i < ma_RoundPipeKr.size(); i++)
	{
		MSSteelSection* urSection = ma_RoundPipeKr[i];
		if (urSection->m_DataType == 1 || findAll == true) //Check in only BuiltUpSection
		{
			double urH = urSection->GetH();
			double urWThk = urSection->GetWThk();
			if (IsSameValue(myH, urH) && IsSameValue(myWThk, urWThk))
				return urSection;
		}
	}
	return nullptr;
}

MSSteelSection* MSSteelUtility::ExistSection_TSectionKr(MSSteelSection* section, bool findAll)
{
	if (findAll == false)
	{
		int count = 0;
		for (int i = 1; i < ma_TSectionKr.size(); i++)
		{
			if (ma_TSectionKr.at(i)->m_DataType == 1)
				count += 1;
		}
		if (count == 0)	return nullptr;
	}
	if (section->m_nSecType != SECTION_T_TYPE)	return nullptr;
	MSSteelTSectionKr* mySection = (MSSteelTSectionKr*)section;

	double myH = mySection->GetH();
	double myB = mySection->GetB();
	double myWThk = mySection->GetWThk();
	double myFThk = mySection->GetFThk();
	for (int i = 1; i < ma_TSectionKr.size(); i++)
	{
		MSSteelSection* urSection = ma_TSectionKr[i];
		if (urSection->m_DataType == 1 || findAll == true) //Check in only BuiltUpSection
		{
			double urH = urSection->GetH();
			double urB = urSection->GetB();
			double urWThk = urSection->GetWThk();
			double urFThk = urSection->GetFThk();

			if (IsSameValue(myH, urH) && IsSameValue(myB, urB) &&
				IsSameValue(myWThk, urWThk) && IsSameValue(myFThk, urFThk))
				return urSection;
		}
	}
	return nullptr;
}

MSSteelSection* MSSteelUtility::ExistSection_CSectionKr	(MSSteelSection* section, bool findAll)
{
	if (findAll == false)
	{
		int count = 0;
		for (int i = 1; i < ma_CSectionKr.size(); i++)
		{
			if (ma_CSectionKr.at(i)->m_DataType == 1)
				count += 1;
		}
		if (count == 0)	return nullptr;
	}
	if (section->m_nSecType != SECTION_C_TYPE)	return nullptr;
	MSSteelCSectionKr* mySection = (MSSteelCSectionKr*)section;

	double myH = mySection->GetH();
	double myB1 = mySection->GetB1();
	double myB2 = mySection->GetB2();
	double myWThk = mySection->GetWThk();
	double myFThk1 = mySection->GetFThk1();
	double myFThk2 = mySection->GetFThk2();

	for (int i = 1; i < ma_CSectionKr.size(); i++)
	{
		MSSteelSection* urSection = ma_CSectionKr[i];
		if (urSection->m_DataType == 1 || findAll == true) //Check in only BuiltUpSection
		{
			double urH = urSection->GetH();
			double urB1 = urSection->GetB1();
			double urB2 = urSection->GetB2();
			double urWThk = urSection->GetWThk();
			double urFThk1 = urSection->GetFThk1();
			double urFThk2 = urSection->GetFThk2();

			if (IsSameValue(myH, urH) && IsSameValue(myB1, urB1) && IsSameValue(myB2, urB2) &&
				IsSameValue(myWThk, urWThk) && IsSameValue(myFThk1, urFThk1) && IsSameValue(myFThk2, urFThk2))
					return urSection;
		}
	}
	return nullptr;
}
//		bool ExistSection_DAngleKr			(MSSteelSection* pSection);
MSSteelSection* MSSteelUtility::ExistSection_AngleSectionKr(MSSteelSection* section, bool findAll)
{
	if (findAll == false)
	{
		int count = 0;
		for (int i = 1; i < ma_AngleKr.size(); i++)
		{
			if (ma_AngleKr.at(i)->m_DataType == 1)
				count += 1;
		}
		if (count == 0)	return nullptr;
	}
	if (section->m_nSecType != SECTION_ANGLE_TYPE)	return nullptr;
	MSSteelAngleKr* mySection = (MSSteelAngleKr*)section;

	double myH = mySection->GetH();
	double myB = mySection->GetB();
	double myWThk = mySection->GetWThk();
	double myFThk = mySection->GetFThk();
	for (int i = 1; i < ma_AngleKr.size(); i++)
	{
		MSSteelSection* urSection = ma_AngleKr[i];
		if (urSection->m_DataType == 1 || findAll == true) //Check in only BuiltUpSection
		{
			double urH = urSection->GetH();
			double urB = urSection->GetB();
			double urWThk = urSection->GetWThk();
			double urFThk = urSection->GetFThk();
			if (IsSameValue(myH, urH) && IsSameValue(myB, urB) &&
				IsSameValue(myWThk, urWThk) && IsSameValue(myFThk, urFThk))
					return urSection;
		}
	}
	return false;
}
//		bool ExistSection_DeckPlateKr		(MSSteelSection* pSection);
#pragma endregion

void MSSteelUtility::AddSection(long type, MSSteelSection* pSection)
{
	switch (type)
	{
	case SECTION_H_TYPE:					ma_HSectionKr.push_back(pSection); break;
	case SECTION_RECT_TYPE:			ma_RectPipeKr.push_back(pSection); break;
	case SECTION_ROUND_TYPE:		ma_RoundPipeKr.push_back(pSection); break;
	case SECTION_T_TYPE:					ma_TSectionKr.push_back(pSection); break;
	case SECTION_C_TYPE:					ma_CSectionKr.push_back(pSection); break;
		//case SECTION_DANGLE_TYPE:		ma_DAngleKr.push_back(pSection); break;
	case SECTION_ANGLE_TYPE:			ma_AngleKr.push_back(pSection); break;
		//case SECTION_DECK_TYPE:				ma_DeckPlateKr.push_back(pSection); break;
	}


}
MSSteelSection* MSSteelUtility::FindOrBuiltUpSection(long nSectionType, vector<double>& valueArr)
{
	MSSteelSection* pBuiltUp = NULL;
	switch (nSectionType)
	{
	case SECTION_H_TYPE:					pBuiltUp = BuiltUpSection_HSectionKr(valueArr); break;
	case SECTION_RECT_TYPE:				pBuiltUp = BuiltUpSection_RectPipeKr(valueArr); break;
	case SECTION_ROUND_TYPE:		pBuiltUp = BuiltUpSection_RoundPipeKr(valueArr); break;
	case SECTION_T_TYPE:						pBuiltUp = BuiltUpSection_TSectionKr(valueArr); break;
	case SECTION_C_TYPE:					pBuiltUp = BuiltUpSection_CSectionKr(valueArr); break;
		//case SECTION_DANGLE_TYPE:		pBuiltUp = BuiltUpSection_DAngleKr(valueArr); break;
	case SECTION_ANGLE_TYPE:			pBuiltUp = BuiltUpSection_AngleKr(valueArr); break;
		//case SECTION_DECK_TYPE:				pBuiltUp = BuiltUpSection_DeckPlateKr(valueArr) break;
	}

	if (pBuiltUp)
	{
		if (CheckSteelSection(nSectionType, pBuiltUp) == false)
		{
			XErrorObject* pError = MSSteelUtility::CreateErrorObject(MESSAGE_ERROR_INPUT, ERROR_LEVEL, LocalDic::GetLocal(_T("철골 단면 정보에 오류가 있습니다.")), XErrorObject::AreaSelect);
			pError->SetGroupName(_T("Steel Section"));
			delete pBuiltUp;
			return nullptr;
		}

		MSSteelSection* pExistSection = IsSectionExist(nSectionType, pBuiltUp, true);
		if (pExistSection != nullptr)
		{
			delete pBuiltUp;
			return pExistSection;
		}

		AddSection(nSectionType, pBuiltUp);
		map<long, vector<MSSteelSection*>>::iterator it = mm_AddSection.find(nSectionType);
		if (it != mm_AddSection.end())
		{
			it->second.push_back(pBuiltUp);
		}
		else
		{
			vector<MSSteelSection*> SectionArr;
			SectionArr.push_back(pBuiltUp);
			mm_AddSection.insert(make_pair(nSectionType, SectionArr));
		}
	}
	return pBuiltUp;
}

bool MSSteelUtility::IsMakeBuiltUpSection(CString steelName)
{
	long type = GetSectionType(steelName);
	MSSteelSection* pSteelSection = GetSection(type, steelName);
	if (pSteelSection != nullptr)	return true;

	MSSteelSection* pBuiltUp = NULL;
	vector<double> valueArr = Parsing_SteelName(steelName);

	switch (type)
	{
	case SECTION_H_TYPE:					pBuiltUp = BuiltUpSection_HSectionKr(valueArr); break;
	case SECTION_RECT_TYPE:				pBuiltUp = BuiltUpSection_RectPipeKr(valueArr); break;
	case SECTION_ROUND_TYPE:		pBuiltUp = BuiltUpSection_RoundPipeKr(valueArr); break;
	case SECTION_T_TYPE:						pBuiltUp = BuiltUpSection_TSectionKr(valueArr); break;
	case SECTION_C_TYPE:					pBuiltUp = BuiltUpSection_CSectionKr(valueArr); break;
		//case SECTION_DANGLE_TYPE:		pBuiltUp = BuiltUpSection_DAngleKr(valueArr); break;
	case SECTION_ANGLE_TYPE:			pBuiltUp = BuiltUpSection_AngleKr(valueArr); break;
		//case SECTION_DECK_TYPE:				pBuiltUp = BuiltUpSection_DeckPlateKr(valueArr) break;
	}
	if (pBuiltUp == nullptr)	return false;
	if (pBuiltUp)
	{
		if (CheckSteelSection(type, pBuiltUp) == false)
		{
			XErrorObject* pError = MSSteelUtility::CreateErrorObject(MESSAGE_ERROR_INPUT, ERROR_LEVEL, LocalDic::GetLocal(_T("철골 단면 정보에 오류가 있습니다.")), XErrorObject::AreaSelect);
			pError->SetGroupName(_T("Steel Section"));
			delete pBuiltUp;
			return false;
		}
		return true;
	}
	return false;
}