#pragma once

#include "mscontainer.h"

class XBarSetInfo;
class MSBarSet;
class MSCompoG;

class StructureModel_CLASS MSGroup : public MSContainer
{
public:
	MSGroup(void);
	~MSGroup(void);
	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MSGroup)
	virtual void Init(bool bInConstructor=false);

	MSRebarBuilding* mp_Building;
	virtual void SetOwner(MSRebarBuilding* val){mp_Building = val; };
	virtual MSRebarBuilding* GetOwner(){return mp_Building; };

	virtual bool WriteFileBinary();
	virtual bool ReadFileBinary(CString strFilePath);

	CString m_strFilePath;
	long m_nGroupType;
	long m_nFloorNum;
	vector<XBarSetInfo*> ma_BarSetInfo;	// .dwg파일 추가로 인해 추가되는 바셋정보 - by Gracias 130111
	int AddBarSetInfo(XBarSetInfo* pBarSetInfo);
	void DeleteBarSetInfoArr();
	vector<XBarSetInfo*> GetBarSetInfoArr(long nCZoneID);

	bool Add(MSBarSet* pBarSet);
	void DeleteBarSets();
	double GetTotalLength();
	//void GetRebarTotalLengthByDia( map<double,double>& mapLengthByDia );
	void ConvertBarSet4Member();
	void DeConvertBarSet4Member();//by LSS NX

	vector <MSBarSet*> ma_BarSet;
	MSBarSet* GetBarSet(long nBarSetID);
	BOOL DeleteBarSet(long nBarSetID);
	bool IsSafetyGroup();
};
