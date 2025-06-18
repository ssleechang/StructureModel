#pragma once
#include "MSFaceMemberM.h"
class MSRibTypeM;
class StructureModel_CLASS MSRcFaceMemberM :
	public MSFaceMemberM
{
public:
	MSRcFaceMemberM(void);
	~MSRcFaceMemberM(void);
	void Serialize( CArchive& ar);
	DECLARE_SERIAL(MSRcFaceMemberM)
	virtual void CopyFromMe(MSCompoM* pSource);
	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer); 

	virtual bool IsGeometryDifferent(MSCompoM* compoM) override;

	virtual double GetThick() override;
	virtual double GetThickXMLExport() override;
	virtual bool IsMatSlab();
	virtual bool IsSOGSlab();
	bool IsDeckSlab();
	bool IsGeneralDeckSlab();
	//enum SLAB_TYPE { MAT_TYPE = 0, FLAT_TYPE, GENERAL_TYPE, PC_TYPE, DECK_TYPE, SOG, CIRCULAR_RAMP, DECK_GENERAL, PC_IMT, PC_RPS, PC_HCS };
	virtual long GetSlabMType();
	double GetRibSpacing(){return m_RibSpacing;};
	void SetRibSpacing(double ribSpacing){m_RibSpacing = ribSpacing;};

	virtual double GetPcThick();
	void SetPcThick(double thick);

	double GetDeckFormThick()				{ return m_DeckFormThick;};
	double GetDeckFormTopWidth()	{ return m_DeckFormTopWidth;};
	double GetDeckFormBotWidth()	{ return m_DeckFormBotWidth;};
	double GetDeckFormSpace()			{ return m_DeckFormSpace;};

	void SetDeckFormThick(double thick)				{ m_DeckFormThick = thick;};
	void SetDeckFormTopWidth(double width)	{ m_DeckFormTopWidth = width;};
	void SetDeckFormBotWidth(double width)	{ m_DeckFormBotWidth = width;};	
	void SetDeckFormSpace(double space)			{ m_DeckFormSpace = space;};

	double GetIMT_L1();
	double GetIMT_L2();
	double GetIMT_L3();
	double GetIMT_H1();
	double GetIMT_H2();
	double GetIMT_H3();
		
	void SetIMT_L1(double param);
	void SetIMT_L2(double param);
	void SetIMT_L3(double param);
	void SetIMT_H1(double param);
	void SetIMT_H2(double param);
	void SetIMT_H3(double param);

	int GetHoleType();
	void SetHoleType(int val);
	long GetHoleNum();
	void SetHoleNum(long val);
	long GetFillHoleNum();
	void SetFillHoleNum(long val);
	double GetFillHoleLength();
	void SetFillHoleLength(double val);

	double GetHCS_L1();
	double GetHCS_L2();
	double GetHCS_L3();
	double GetHCS_H1();
	double GetHCS_H2();
	void SetHCS_L1(double value);
	void SetHCS_L2(double value);
	void SetHCS_L3(double value);
	void SetHCS_H1(double value);
	void SetHCS_H2(double value);
	
	double GetRPS_L1();
	double GetRPS_L2();
	double GetRPS_L3();
	double GetRPS_H1();
	double GetRPS_H2();
	double GetRPS_H3();
	void SetRPS_L1(double value);
	void SetRPS_L2(double value);
	void SetRPS_L3(double value);
	void SetRPS_H1(double value);
	void SetRPS_H2(double value);
	void SetRPS_H3(double value);


	MSRibTypeM* mp_RibM;
protected:
	double m_RibSpacing;
	double m_dPcThick;

	double m_DeckFormThick;
	double m_DeckFormTopWidth;
	double m_DeckFormBotWidth;
	double m_DeckFormSpace;

	double m_IMT_L1, m_IMT_L2, m_IMT_L3;
	double m_IMT_H1, m_IMT_H2, m_IMT_H3;

	double m_HCS_L1, m_HCS_L2, m_HCS_L3;
	double m_HCS_H1, m_HCS_H2;

	double m_RPS_L1, m_RPS_L2, m_RPS_L3;
	double m_RPS_H1, m_RPS_H2, m_RPS_H3;

	int m_HoleType; //0:원형, 1:타원형
	long m_HoleNum, m_FillHoleNum;
	double m_FillHoleLength;

};
