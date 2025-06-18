#pragma once
#include "dtobject.h"

class StructureModel_CLASS DTDowel :	public DTObject
{
public:
	DTDowel(void);
	virtual ~DTDowel();


protected:
	DevType me_DevType;
	SpliceType me_SpliceType;

	double m_dUserDevLen;
	double m_dUserHookLen;
	double m_dUserSpliceLenA;
	double m_dUserSpliceLenB;

public:
	void SetDevType(DevType val){me_DevType = val;};
	DevType GetDevType(){return me_DevType;};
	void SetSpliceType(SpliceType val){me_SpliceType = val;};
	SpliceType GetSpliceType(){return me_SpliceType;};
	void SetUserDevLen(double val){m_dUserDevLen = val;};
	double GetUserDevLen(){return m_dUserDevLen;};
	void SetUserHookLen(double val){m_dUserHookLen = val;};
	double GetUserHookLen(){return m_dUserHookLen;};
	void SetUserSpliceLenA(double val){m_dUserSpliceLenA = val;};
	double GetUserSpliceLenA(){return m_dUserSpliceLenA;};
	void SetUserSpliceLenB(double val){m_dUserSpliceLenB = val;};
	double GetUserSpliceLenB(){return m_dUserSpliceLenB;};
};
