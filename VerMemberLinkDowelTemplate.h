#pragma once

class StructureModel_CLASS LinkDowelPlaceInfo
{
public:
	LinkDowelPlaceInfo();
	LinkDowelPlaceInfo(LinkDowelPlaceInfo *pInfo);
	~LinkDowelPlaceInfo();
	void Serialize(CArchive& ar);
	void CopyFromMe(LinkDowelPlaceInfo *pInfo);

	long GetSpliceType();
	void SetSpliceType(long value);
	long GetConstructionHeight();
	void SetConstructionHeight(long value);
	BOOL GetAlterSpliceSet();
	void SetAlterSpliceSet(BOOL value);
	long GetAlterSpliceDist();
	void SetAlterSpliceDist(long value);
	long GetAlterSpliceType();
	void SetAlterSpliceType(long value);
private:
	long m_nSpliceType;
	long m_nConstructionHeight;
	BOOL m_bAlterSpliceSet;
	long m_nAlterSpliceType;
	long m_nAlterSpliceDist;
};

class StructureModel_CLASS VerMemberLinkDowelTemplate
{
public:
	VerMemberLinkDowelTemplate();
	VerMemberLinkDowelTemplate(VerMemberLinkDowelTemplate *pTemplate);
	~VerMemberLinkDowelTemplate();
	void Serialize(CArchive& ar);
	void CopyFromMe(VerMemberLinkDowelTemplate *pTemplate);

	LinkDowelPlaceInfo *GetJointPartTopLinkDowelPlaceInfo();
	LinkDowelPlaceInfo *GetJointPartBotLinkDowelPlaceInfo();
	LinkDowelPlaceInfo *GetTopDownTopLinkDowelPlaceInfo();
	LinkDowelPlaceInfo *GetTopDownBotLinkDowelPlaceInfo();
private:
	void DeleteLinkDowelPlaceInfo();
private:
	LinkDowelPlaceInfo *m_pJointPartTopLinkDowelPlaceInfo;
	LinkDowelPlaceInfo *m_pJointPartBotLinkDowelPlaceInfo;
	LinkDowelPlaceInfo *m_pTopDownTopLinkDowelPlaceInfo;
	LinkDowelPlaceInfo *m_pTopDownBotLinkDowelPlaceInfo;
};

