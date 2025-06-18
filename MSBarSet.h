#pragma once

#include "msobject.h"
#include "mscompog.h"
class MSRebarShape;
class MSRebarBuilding;
class MSCompoG;
class MSFloor;


class StructureModel_CLASS MSBarSet : public MSObject
{
public:
	MSBarSet(void);
	~MSBarSet(void);
	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MSBarSet)

	long m_ID;
	long GetID();  // m_ID가 유효하면 (m_ID > 0)있으면 리턴하고, 0이면 ms_LastID+1으로 m_ID 세팅하고 리턴한다.
	static long ms_LastID;
	static void SetLastID(long NewID); // NewID > ms_LastID 이면 치환. 아니면 무시

	bool Add(MSRebarShape* pShape);
	bool Add(GMVector Loc);
	bool Add(MSCompoG* pCompoG);
	void SetOwner(MSCompoG* pOwner);
	MSCompoG* GetOwner();
	void DeleteShapeArr();

	vector <MSRebarShape*> ma_Shape;
	vector <GMVector> ma_Loc;
	MSCompoG* mp_Owner;
	vector <MSCompoG*> ma_CompoG;
	CString m_Name;
	long m_nFloorNum;
	CString GetName();

	long m_nRootType;		// BarSet이 생성된 근본					ex) BEAM_TOP, Index =2	-> 보의 TopBarSet중 3번째
	long m_nRootIndex;	// BarSet이 생성된 근본의 Index	
	long m_nRootUnitIndex;//RAPUnit이 생성된 근본의 Index
	long m_nBarType;	// SD30, SD40, SD50
	long m_nConsZoneID;	// Add Gracias 13.01.11
	CString GetStrengthName();
	CString GetBarTypeName();

	long GetBarSetType(){return m_nRootType;};
	void SetBarSetType(long aType){m_nRootType=aType;};
	long GetIndex(){return m_nRootIndex;};
	void SetIndex(long aIndex){m_nRootIndex=aIndex;};

	MSRebarBuilding* mp_Bldg;
	long GetNumBars();
	double GetTotalLength();
	MSFloor* GetFloor();
	MSElement::Type_MSCompoG GetOwnerType();
	double GetDia();//첫번째 RebarShape의 Dia, 여러Shape이 오더라도 Dia는 같아야만 한다.
	//void GetTotalLengthByDia(map<double,double>& mapLengthByDia);
};

