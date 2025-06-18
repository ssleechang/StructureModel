#pragma once
#include "mscompom.h"
class StructureModel_CLASS MSOpeningM : public MSCompoM
{
public:
	MSOpeningM(void);
	~MSOpeningM(void);
	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MSOpeningM)
	virtual void CopyFromMe(MSCompoM* pSource);

	virtual double GetWidth() {return m_dWidth;}
	virtual double GetHeight() {return m_dHeight;}
	virtual double GetHeightBySlabLevel(double dSlabLevel) { return m_dHeight; }
	virtual void SetWidth(double Width) {m_dWidth = Width;}
	virtual void SetHeight(double Height) {m_dHeight = Height;}

	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer); 

	long	GetOpeningReinforceMID()	{ return m_nOpeningReinforceMID;}
	long	GetExtraSize()								{ return m_ExtraSize;}

	void	SetOpeningReinforceMID(long ReinforceMID)	{ m_nOpeningReinforceMID = ReinforceMID;}
	void	SetExtraSize(long extraSize)									{ m_ExtraSize = extraSize;}

protected:
	double m_dWidth;
	double m_dHeight;
	long m_nOpeningReinforceMID;
	long m_ExtraSize;
};

