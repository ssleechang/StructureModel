#pragma once
#include "MTReaderHelper.h"


class MSRcFaceMemberM;
class MSRcBWallM;
#include "XWallMInfo.h"
class StructureModel_CLASS MTBWallMHelper_LH :	public MTReaderHelper
{
public:
	MTBWallMHelper_LH(MSBaseBuilding* pBldg, MTReader* pReader);
	~MTBWallMHelper_LH();

	virtual CString GetGroupName() { return _T("BWall"); };
	virtual void MakeData(SRSectionTable* pTable);

	vector<MSCompoM*> MakeBWallMs();
	CString GetName();
	int GetThick();
	GM2DRectangle* GetCopyRect();
	SRSectionTable* GetTable() { return mp_Table; };
	CTypedPtrArray<CObArray, XWallMInfo*> ma_WallMInfo;

	bool IsSameData(MTBWallMHelper_LH* urHelper);
	void AddTextFigureID(GMText* pText);
	map<int, CString> GetFigureIDsMap() { return mm_FigureIDToData; };
	vector<MTResultElement*> GetResults() { return ma_Result; };

	int GetMinThick() { return m_minThick; };
	void SetMinThick(int thick) { m_minThick = thick; };
protected:
	long GetFromStory();
	long GetToStory();

	bool CheckWallInfos();
	bool MakeWallDatas();
	void MakeVBars(MSRcBWallM* pRebarM, CString rebarText);
	void MakeHBars(MSRcBWallM* pRebarM, CString rebarText);

	void MakeMainBars(CString rebarText, MSMainBar* pMainBar, MTResultItem::eItemType itemType);
	void MakeStirrupBars(CString rebarText, MSStirrupBar* pStirrupBar, int cellType, MTResultItem::eItemType itemType);

	bool FindMarkText();
	bool FindNameAndThickText();
	bool IsVMarkUpper();
	bool AnalyzeTexts(CTypedPtrArray<CObArray, GMText*>& rebarTextArr, CTypedPtrArray<CObArray, GMText*>& floorTextArr);
	long MakeWallMInfo(CTypedPtrArray<CObArray, GMText*>& FloorArr, CTypedPtrArray<CObArray, GMText*>& RebarArr);
	void SetRebarText(XWallMInfo* pInfo, CString rebarText, bool IsHBarText);
	void FixWallMInfoRebarText();
	void SortWallMInfo();
	XWallMInfo* GetWallMInfo(long fromStory, long toStory);
	MSCompoM* MakeBWallMData(XWallMInfo* pInfo);
	MSRcFaceMemberM* CreateBWallM();

	GM2DLineVector GetRefLineVector();
	void DeleteWallMInfos();
	void FixRebarTexts(XWallMInfo* pInfo);
public:
	bool GetIsDummy() { return m_isDummy; };
	void SetIsDummy(bool isRegister) { m_isDummy = isRegister; };
protected:
	SRSectionTable* mp_Table;
	GMText* mp_Name, *mp_Thick;
	GMText* mp_VMarkText, *mp_HMarkText;
	map<int/*MTReader::CellType*/, CString> mm_FigureIDToData;
	int m_minThick;
	bool m_isDummy;
	vector<MTResultElement*> ma_Result;
};

