#pragma once
#include "dtdowel.h"
class StructureModel_CLASS DTDowelDevelopment :
	public DTDowel
{
public:
	DTDowelDevelopment();
	~DTDowelDevelopment();



	static long MakeTestDefaultSample(vector<DTDowelDevelopment*>& DTDowelDevelopmentArr);

private:
	//수평지지근
	long m_nSupportBarDia;//다월지지근 지름
	long m_numSupportBar;//다월지지근 갯수
	double m_dSupportBarLen;//다월지지근의 길이
	//보조스터럽 배근타입 - 간격배근, 갯수배근
	long m_nStirrupShapeType;//0;기둥의 스터럽과 같은 형태 , 기타:shapeNumber
	long m_numStirrupBar;

	



};

