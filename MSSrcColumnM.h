#pragma once
#include "MSRcLineMemberM.h"

#ifndef _MSSrcColumnM_HeaderFile
#define _MSSrcColumnM_HeaderFile

#include "MSMainBar.h"
#include "MSStirrupBar.h"
#include "MSSrcMaterial.h"
class MSSteelSection;

class StructureModel_CLASS MSSrcColumnM :	public MSRcLineMemberM
{
public:
	MSSrcColumnM(void);
	~MSSrcColumnM(void);
	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MSSrcColumnM)
	virtual void CopyFromMe(MSCompoM* pSource);
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer); 
	virtual void GetSteelSecPolyline(GM2DPolyline& APGon);
	virtual bool IsSRC(){ return true; };
	virtual MSSteelSection* GetSteelSection() { return m_pSection; };

	virtual bool IsGeometryDifferent(MSCompoM* compoM);

	public:
	MSSteelSection* m_pSection;
	long m_nSecType;
	int m_NumStudBoltBars;
	MSStirrupBar m_StudBoltBars;
	double m_StudBoltLength;

	virtual double GetSpecificTieHeight(CString sInitial);

	// [BH-5832] SRC기둥 내 철골닫힌구간(ㅁ, O) 영역의 콘크리트 별도 설정
	bool GetIsUsedStoryInnerConcMaterial() { return m_IsUsedStoryInnerConcMaterial; };
	void SetIsUsedStoryInnerConcMaterial(bool bUsedStoryInnerConcMaterial) { m_IsUsedStoryInnerConcMaterial = bUsedStoryInnerConcMaterial; };
	MSSrcMaterial* GetInnerConcMaterial() { return mp_InnerConcMaterial; };
	void SetInnerConcMaterial(MSSrcMaterial* pMaterial);
	
protected:
	bool m_IsUsedStoryInnerConcMaterial;
	MSSrcMaterial* mp_InnerConcMaterial;
};

#endif
