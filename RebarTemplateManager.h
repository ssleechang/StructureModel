#pragma once
#include "RebarTemplate.h"

enum RT_MemberType
{
	RT_Column = 0, RT_Beam, RT_TransferBeam, RT_BWall, RT_RWall, 
	RT_Pit, RT_Slab, RT_TransferSlab, RT_IsoFoot, RT_WallFoot, 
	RT_LowWall, RT_WallColumn, RT_TransferColumn, RT_DeckBeam,
	RT_None = 1000,
};

class StructureModel_CLASS RebarTemplateManager
{
public:
	static RebarTemplateManager* GetInstance();
	~RebarTemplateManager(void);

	bool WriteFileBinary(CString strFilePath);
	int ReadFileBinary(CString strFilePath);
	bool WriteRebarTemplateFileBinary(CString strFilePath, CString sRebarTemplateName);
	RebarTemplate* ReadRebarTemplateFileBinary(CString strFilePath);

	void Serialize(CArchive& archive);

	map<long, RebarTemplate*> mm_RebarTemplate;

	BaseRebarTemplate* GetRebarTemplateByMemberType(RT_MemberType type, RebarTemplate* rebarTemplate);
	BaseRebarTemplate* GetRebarTemplate(RT_MemberType type, int nMemberTemplateID);
	RebarTemplate* GetRebarTemplateByID(long nID);
	RebarTemplate* GetRebarTemplateByName(CString sName);

	void Add(RebarTemplate* rebarTemplate);
	long Remove(long nID);
	void DeleteRebarTemplates();

	long GetBuildingTemplateID();
	void SetBuildingTemplateID(long nID);

	long m_nID;
	long GetNewID();
	static long ms_LastID;
	static void SetLastID(long NewID);

	CString GetNoneRebarTemplateName();

	static BaseRebarTemplate* CreateRebarResultOption(RT_MemberType gType);
private:
	static RebarTemplateManager* m_instance;
	RebarTemplateManager();
	long m_nBuildingTemplateID;
};
