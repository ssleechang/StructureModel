#pragma once

class CogReader;
class MSBaseBuilding;
class MSFloor;
class MSCompoM;
class XErrorObject;
class StructureModel_CLASS NXBoxData : public CObject
{
public:
	NXBoxData();
	~NXBoxData();

public:
	void SetVersion(int version) { m_Version = version; };
	void SetFigureID(int figureID) { m_figureID = figureID; };
	void SetName(CString name) { m_Name = name; };

	int GetVersion() { return m_Version; };
	int GetFigureID() { return m_figureID; };
	CString GetName() { return m_Name; };
protected:
	int m_figureID;
	int m_Version;
	CString m_Name;

};

class MSCompoG;
class StructureModel_CLASS NXBoxHelper : public CObject
{
public:
	NXBoxHelper(CogReader* pReader);
	~NXBoxHelper();

	virtual MSCompoG* MakeData(NXBoxData* pData) { return nullptr; };
	static NXBoxData* CreateNXBoxData(int memberType);

protected:
	MSBaseBuilding* GetBuilding();
	MSFloor* GetFloor();
	MSCompoM* FindCompoM(CString name, long msCompoMType);
	XErrorObject* CreateErrorObject(long figureID, int ErrorCode, int Level, CString GroupName, CString Message);
	virtual CString MakeDefaultName() { return _T("NONAME"); };

protected:
	CogReader* mp_Reader;
	NXBoxData * mp_Data;
	bool m_IsMaked;

};

