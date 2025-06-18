#pragma once
#include "mscompom.h"
#include "MSMainBar.h"
class StructureModel_CLASS MSOpeningReinforceM : public MSCompoM
{
public:
	MSOpeningReinforceM(void);
	~MSOpeningReinforceM(void);

	void Serialize( CArchive& ar);
	DECLARE_SERIAL(MSOpeningReinforceM)
	virtual void CopyFromMe(MSCompoM* pSource);
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer); 

	enum eUsage		{ Opening = 0, TakeOutMaterial	};
	enum eCalcBy		{ Program = 0, User	};
	enum eLayer		{ Single = 0, Double	};

	eUsage	GetUsage();
	eCalcBy GetCalcBy();
	eLayer	GetLayer();
	bool		GetIsReinforceBar1();
	bool		GetIsReinforceBar2();
	bool		GetIsUsedMainBarDia1();
	bool		GetIsUsedMainBarDia2();
	bool		GetIsCalcFormArea();
	bool		GetIsCalcConcVolumn();

	void		SetUsage(eUsage usage);
	void		SetCalcBy(eCalcBy calcBy);
	void		SetLayer(eLayer layer);
	void		SetIsReinforceBar1(bool isReinforceBar);
	void		SetIsReinforceBar2(bool isReinforceBar);
	void		SetIsUsedMainBarDia1(bool isUsedSlabDia);
	void		SetIsUsedMainBarDia2(bool isUsedSlabDia);
	void		SetIsCalcFormArea(bool isCalc);
	void		SetIsCalcConcVolumn(bool isCalc);

	bool GetIsSameTopBotBar();
	void SetIsSameTopBotBar(bool value);

	MSMainBar m_ReinforceBar1_Top, m_ReinforceBar2_Top;
	MSMainBar m_ReinforceBar1_Bot, m_ReinforceBar2_Bot;
protected:
	eUsage m_Usage;
	eCalcBy m_CalcBy;
	eLayer   m_Layer;
	bool m_IsReinforceBar1;
	bool m_IsReinforceBar2;

	bool m_IsUsedMainBarDia1;
	bool m_IsUsedMainBarDia2;
	bool m_IsCalcFormArea;
	bool m_IsCalcConcVolumn;

	bool m_IsSameTopBotBar;
};

