#pragma once

enum DevType//정착타입
{
	DevType_Stright_Cover, //피복까지 일자정착
	DevType_Stright_Compression,//압축정착
	DevType_Stright_Tension,//인장정착
	DevType_Stright_Splice,//이음정착
	DevType_Stright_User,//정착길이 사용자 입력
	DevType_Hook_Cover,//피복에서 표준후크정착
	DevType_Hook_Dev4Hook,//표준후크를 갖는 표준후크정착
	DevType_Hook_UserDevLen,//사용자 정의 만큼 정착후 사용자 지정후크
	DevType_UserLenHook_Cover,//피복에서 사용자 지정후크정착
	DevType_UserLenHook_Dev4Hook,//표준후크를 갖는 사용자 지정후크정착
	DevType_UserLenHook_UserDevLen//사용자 정의 만큼 정착후 사용자 지정후크
};
enum SpliceType//이음타입
{
	//전수이음(몰이음),반수이음(엇이음)
	SpliceType_All_B = 1,//B급이음
	SpliceType_All_A,//A급이음
	SpliceType_All_User,//사용자입력 이음
	SpliceType_Half_B, //B급 엇이음
	SpliceType_Half_Coupler, //커플러
	SpliceType_Half_welding//가스용접
};
enum RebarPlaceType
{
	RebarPlaceType_HalfSpace = 1,
	RebarPlaceType_Cover,
	RebarPlaceType_Space,
	RebarPlaceType_User
};
enum EndType //마구리타입
{
	EndType_None = 0,//없음
	EndType_Hook,//표준후크
	EndType_Ubar//Ubar마무리
};
enum CoverType
{
	CoverType_Member = 0,//지지부재의 피복
	CoverType_Member_Rebar,//지지부재의 피복+지지면을지나는 철근들의 지름합
	CoverType_User//사용자 입력
};
enum JointBeamType
{
	JointBeamType_Column_Beam = 0,//기둥+보
	JointBeamType_Beam_Column_Beam,//보+기둥+보
	JointBeamType_Beam_Beam,//보+보
	JointBeamType_Beam_Girder_Beam,//보+지지보+보
	JointBeamType_Girder_Beam,//큰보+보
	JointBeamType_Cantilever
};
enum JointColumnType
{
	JointColumnType_TopColumn = 0,//상부만 기둥이 있는 경우
	JointColumnType_TopColumn_BotColumn,//상부,하부 기둥
	JointColumnType_BotColum//하부만 기둥
};
enum JointWallType
{
	JointWallType_Ver_TopWall = 0,//상부만 벽이 있는 경우
	JointWallType_Ver_TopWall_BotWall,//상부,하부 벽
	JointWallType_Ver_BotWall,//하부만 벽
	JointWallType_Hor_End,//연결된 벽이 없을경우
	JointWallType_Hor_Wall,//나란한 벽
	JointWallType_Hor_2Wall_90,//수직으로 두벽이 연결
	JointWallType_Hor_2Wall,//두벽이 연결
	JointWallType_Hor_3OverWall//세벽이상 연결
};

enum  SpacerDepthType
{
	SpacerDepthType_Cover = 0,//공제길이=피복.
	SpacerDepthType_Cover_MainBarDia,//공제길이= 피복+해당면의 주근의 지름
//	SpacerDepthType_Cover_MainBarDia_AddLenUser,//공제길이= 피복+해당면의 주근의 지름+여유치(도리철근)
	SpacerDepthType_UserDepth //사용자가 직접 입력

};
enum  SpacerLegLenType
{
	SpacerLegLenType_Spacing_Ratio = 0, //간격*특정비율
	SpacerLegLenType_Spacing_AddLen,//간격+추가길이
	SpacerLegLenType_User//사용자 입력
};

enum  LevelDifWidthType
{
	LevelDifWidthType_Depth_Ratio = 0, //부재두께*사용자정의 비율
	LevelDifWidthType_Depth_AddLen, //부재두께*사용자정의 추가길이
	LevelDifWidthType_Usert//사용자 입력 폭
};
enum  LevelDifVerBarType
{
	LevelDifVerBarType_Stirrup = 0, //두께가 얇은 슬라브의 경우 
	LevelDifVerBarType_ZBar, //양단이 표준후크인 ZBar사용 내부정착길이 확보판단
	LevelDifVerBarType_LBar//한쪽이 이음으로 연결
};
class StructureModel_CLASS DTObject :	public CObject
{
public:
	DTObject(void);
	~DTObject(void);

	CString GetName(){return m_Name;};
	void SetName(CString val){m_Name = val;};

protected:
	CString m_Name;
};
