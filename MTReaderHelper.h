#pragma once
#include "afx.h"

#include "MTReader.h"
#include "MTResultElement.h"
class MSCompoM;
class MSBaseBuilding;
class MTResultElement;
class MTResultItem;

class StructureModel_CLASS MTReaderHelper :	public CObject
{
public:
	MTReaderHelper(MSBaseBuilding* pBldg, MTReader* pReader);
	~MTReaderHelper(void);

	virtual CString GetGroupName(){ return _T("");};
	CString GetCompoMName();

	static bool CheckDia(double nDia);

	int CreateErrorObject(CString GroupName, int Level, int ErrorCode, CString Message);
	void AddErrorFigureIDs(vector<int> figureIDs);
	void AddErrorFigureID(int figureID);
	bool GetIsMaked() { return m_IsMaked; };
	void SetIsMaked(bool isMaked) { m_IsMaked = isMaked; };

	MTResultElement* GetResultCompoM() { return mp_ResultElement; };
protected:
	vector<double> Parsing_SteelName(CString strData);
	void Parsing_StudBolt(CString strData, int& numBolt, MSStirrupBar* pStirrup);
	int ParsingTextData(CString text, vector<CString>& dataArr, CString parser);
	bool IsPlainConcreteReinforceBar();

	void CheckName(CString Name, int figureID);
	void CheckName(CString Name, vector<int> figureIDs);

	MTResultItem* AddResultItem(MTResultItem::eItemType itemType, CString text);
	MTResultItem* GetResultItem(MTResultItem::eItemType itemType);

	MTResultElement* AddSubElement(MTResultItem::eItemType itemType);
	MTResultElement* GetSubElement(MTResultItem::eItemType itemType);
	MTResultItem* AddSubResultItem(MTResultItem::eItemType elementType, MTResultItem::eItemType itemType, CString text);
	MTResultItem* GetSubResultItem(MTResultItem::eItemType elementType, MTResultItem::eItemType itemType);
	void SetLocationList(MTResultItem::eItemType itemType, vector<GM2DVector>& locArr);

    BOOL IsDiffrenentMainBarSet(CString topResult, CString textTopResult);
public:
	GM2DRectangle* GetResultArea();
protected:
	bool m_IsMaked;
	MSBaseBuilding* mp_Bldg;
	MTReader* mp_Reader;
	MSCompoM* mp_CompoM;

	MTResultElement* mp_ResultElement;
};

