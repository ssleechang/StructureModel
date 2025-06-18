#pragma once
#include "msrcobject.h"
#include "MSMainBar.h"

class StructureModel_CLASS MSMainBarSet : public MSRcObject
{
public:
	MSMainBarSet(void);
	~MSMainBarSet(void);
	void Serialize( CArchive& ar);
	DECLARE_SERIAL(MSMainBarSet)
	virtual void CopyFromMe(MSMainBarSet* pSource);
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer); 

	void SetDataByOldVersion(MSMainBar* mainBar);

	void InitData(CString strBar);
	void Add(MSMainBar mainBar);
	void RemoveAll();

	CString GetParser();
	void SetParser(CString parser);

	CString GetBarString();
	long GetNumSets();
	long GetTotalBars();
	long GetNumBarsOnSet(long setNum);
	long GetNumLayersOnSet(long setNum);
	bool IsSameBar(MSMainBarSet& aBarset);
	map<CString, int> GetDiaNameToNumBarsOnSet(long setNum);

	vector<double> GetBarDiaAllLayer();
	double GetMaxDeformedBarDiameterOnSet(long setNum);
	double GetMaxDeformedBarDiameter();
	CString GetMaxDiaNameOnSet(long setNum);

	//기둥주근 위치 계산-나중 위치 변경할 계획임 -쓰지마세요
	vector<CString> MakeLayerDiaArr(long setNum);

	void SetBarString(CString fullData);

	static int GetNumBarsOnSet(CString fullData, int setNum);
	static CString GetDiaNameOnSet(CString fullData, int setNum);
	static bool CheckMainBarSet(CString fullData);
	static int GetNumSets(CString fullData);

	bool GetUserSteelGradeS();
private:
	CString GetBarString_BySlash();
	CString GetBarString_BySemiColon();

	void ParsingMainBar_ByOnlyPlus(MSMainBar* pMainBar, CString barData);
	void ParsingMainBar_ContainSlash(CString fullData);
	void ParsingMainBar_ContainSemiColon(CString fullData);

	void ParsingMainBar_BySlash(CString sData);

	bool CheckMainBar_ByPlus(CString sData);
	bool CheckMainBar_ContainSemiColon(CString fullData);
	bool CheckMainBar_ContainSlash(CString fullData);
	
	bool CheckMainBar_BySlash(CString sData);
	bool CheckMainBar(CString sData);

public:
	static CString Parser_PLUS;
	static CString Parser_SLASH;
	static CString Parser_SEMICOLON;

private:
	CString m_Parser;
	vector<MSMainBar> ma_Bars;

};

