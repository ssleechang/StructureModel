#pragma once
#include "dtobject.h"
class StructureModel_CLASS DTLevelDif :
	public DTObject
{
public:
	DTLevelDif(void);
	~DTLevelDif(void);

private:
	BOOL m_IsHaunch;
	LevelDifWidthType m_nWidthType;//폭계산타입
	double m_dUserValWidth;//
	//슬래브단차만 먼저 처리
	double m_dCover;
	double m_dMaxBentDist;//값이하는 주근을Bent로 연결처리한다.
	//m_dMaxBentDist이상 단차값이 두께값 이하의 경우
	
	//정착타입
	DevType m_DevTypeTopBarUpLevel;
	DevType m_DevTypeBotBarUpLevel;
	DevType m_DevTypeTopBarDownLevel;
	DevType m_DevTypeBotBarDownLevel;


	//단차값이 두께값 이상인 경우-스트럽 또는 Z바 등의 수직근이 추가로 들어감.
	LevelDifVerBarType m_nVerBarType;
	long m_nVerBarShapeType;//LevelDifVerBarType_Stirrup일경우 스트럽 형상정보
	//LevelDifVerBarType_ZBar일경우 
	//m_DevTypeTopBarUpLevel==표준후크 && m_DevTypeBotBarDownLevel==표준후크
	//LevelDifVerBarType_LBar일경우
	//m_DevTypeTopBarUpLevel==이음후크 && m_DevTypeBotBarDownLevel=DevType_Stright_Cover

	//수직연결근 지지 철근정보




};

