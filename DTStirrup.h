#pragma once
#include "dtobject.h"

#include "MRHBarByEdge.h"

class StructureModel_CLASS DTStirrup :	public DTObject
{

public:

	DTStirrup();
	~DTStirrup(void);


	virtual void Init(bool bInConstructor=false);


	static map<long,DTStirrup*> ms_StirrupTempletMap;
	static DTStirrup* GetStirrupTemplet(long nID);
	static long GetStirrupTempletArr(int MemberType, int SetType,
																		CTypedPtrArray<CObArray,DTStirrup*>& TempletArr);
	static void AddStirrupTemplet(DTStirrup* pVal);
	static int ReadStirrupTemplet();
	static int WriteStirrupTemplet(); 

	long m_ID;
	long GetID();
	static long ms_LastID;
	static void SetLastID(long NewID); // NewID > ms_LastID 이면 치환. 아니면 무시

	int m_MemberType;
	int m_SectionType;
	bool m_bFullStirrUp;

	AnchorType m_StartAnchorType;//FullStripup일경우에 반영
	AnchorType m_EndAnchorType;//FullStripup일경우에 반영

	CTypedPtrArray<CObArray, MRHBarByEdge*> ma_StirrupBar;

};