#pragma once
class StructureModel_CLASS HookLengthItem
{
public:
	HookLengthItem(void);
	~HookLengthItem(void);

	void Serialize(CArchive& ar);
	void Initialize();

	double GetHookLength(bool isStirrup = false, double dDegree = 90) const;

	//Mainbar
	double m_dMainbarHookLength90;
	double m_dMainbarHookLength180;
	//Stirrup
	double m_dStirrupHookLength90;
	double m_dStirrupHookLength135;

	void Copy(HookLengthItem desHookLengthItem);
};

