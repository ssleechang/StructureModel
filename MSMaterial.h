#pragma once
#include "msobject.h"

class MSReinforceBarSetting;
class MSRebarBuilding;
class StructureModel_CLASS MSMaterial :
	public MSObject
{
public:
	MSMaterial(void);
	~MSMaterial(void);

	//enum Type_MSMaterial { msConcrete = 1, msPC, msSRC, msSteel, msAnchor, msTimber, msFormWork };
	enum Type_MSMaterial {msConcrete=1, msPC, msSRC, msSteel, msFormWork};
	Type_MSMaterial m_nType;
	Type_MSMaterial GetType(){return m_nType;};

	long m_ID;
	long GetID();  // m_ID가 유효하면 (m_ID > 0)있으면 리턴하고, 0이면 ms_LastID+1으로 m_ID 세팅하고 리턴한다.
	static long ms_LastID;
	static void SetLastID(long NewID); // NewID > ms_LastID 이면 치환. 아니면 무시

	//Global ID 관리
	long m_GlobalID;
	long GetGlobalID();
	static long ms_GlobalLastID;
	static void SetGlobalLastID(long NewID);

    void SetName(CString name);
    CString GetName();

	MSBaseBuilding* m_pBldg;
	void SetBuilding(MSBaseBuilding* pBldg){m_pBldg = pBldg;};

	CString m_Name;
	CString m_Type;
	double m_ElasModulus;
	double m_ElasModulusAnal;
	double m_PoisonRat;
	double m_Density;
	bool m_IsAddedMat = false;

	long GetMaterialIndex();
	void SetMaterialIndex(int Index);

	vector<CString> GetMaterialList();

	void Serialize( CArchive& archive );
	virtual void GlobalOptionSerialize( CArchive& archive );
	DECLARE_SERIAL(MSMaterial)
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);
	virtual MSObject* Copy(bool bNewID = true);
	virtual void CopyFromMe(MSObject* pSource, bool bNewID=true);

	virtual double GetFck()					{ return 0;};
	virtual CString GetFckName()		{ return _T("");};
	virtual double GetConcStrength()	{ return 0; };
	virtual MSReinforceBarSetting* GetReinforceBarSetting()	{ return NULL;};
	virtual bool IsPlainConcreteReinforceBar() { return false; };

	COLORREF m_Color;
	long m_Alpha;

	void GetColor(int& alpha, int& red, int& green, int& blue);
	void SetColor(int alpha, int red, int green, int blue);
	void SetColor(int alpha, COLORREF color);
	bool m_bIsUserSetColor = false;
};
