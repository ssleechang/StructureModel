#pragma once
#include "afx.h"
#include "EFS_Buffer.h"
#include "CmdModifyModel.h"
#include <vector>

#ifndef _MSObject_HeaderFile
#define _MSObject_HeaderFile

class XErrorObject;
class MSRevision;
class MSBaseBuilding;

using namespace std;

/** 
 * MS 데이터모델의 루트. 
 * 1) 객체 변경 시간, 2) 프로젝트 리비전, 3) 객체 메시지 관리. 
 */
class StructureModel_CLASS MSObject : public CObject
{
public:  
	/**       
	 * 생성자.
	 * @param pOrigin 카피할 소스 객체.
	 * @param bNewID 새로운 ID를 부여하지 여부.
	 */
	MSObject();
	/**       
	 * 소멸자.
	 */	
	~MSObject(void);
	void Serialize( CArchive& archive );
	DECLARE_SERIAL(MSObject)
	/**       
	 * 나의 카피 생성.
	 * @param pNewID 생성된 객체에 새로운 아이디를 부여할 지 여부.
	 * @see CopyFrom()
	 * @return 새로 생성된 카피의 포인터
	 */
	virtual MSObject* Copy(bool bNewID = true);
	/**       
	 * 소스의 속성을 나에게 카피.
	 * @param pSource 카피할 원본 객체.
	 * @param bNewID 나에게 새로운 아이디를 부여할 지 여부.
	 * @param bInConstructor 생성자에서 호출했는지 여부. 아닐 때에는 상위 클래스의 CopyFrom 호출해 줌
	 * @see Copy()
	 */
	virtual void CopyFromMe( MSObject* pSource, bool bNewID=true );
	/**       
	 * 멤버변수 초기화.
	 */	
	virtual void Init(bool bInConstructor=false);
	/**       
	 * 현재 시간으로 최종수정시간 세팅.
	 */
	void SetTime();
// 	/**       
// 	 * 현재 액티브 리비전으로 객체 리비전 세팅.
// 	 */
// 	void SetRevision();

	/**       
	 * 내가 속해 있는 빌딩객체 반환하는 가상함수.
	 * 하위 클래스에서 자기 상황에 따라 오버라이드된 함수 호출됨.
	 * @return 내가 속해 있는 빌딩객체
	 */
	virtual MSBaseBuilding* GetBuilding() {return NULL;};
    template <class T>
    void DeleteVecArray(vector<T*>* aObject);
	
	virtual void Dump(EFS::EFS_Buffer& buffer){};// buffer에 객체의 undo용 runtime 상태값을 저장해 둔다.
	virtual void Recovery(EFS::EFS_Buffer& buffer){}; // buffer로부터 runtime 상태값을 얻어온다. Undo/Redo 동작
	virtual bool IsElement() { return false; };
	virtual long GetID(){return 0;};

	void StartEdit( CmdProcess::CommandUpdateElementType elementType, long ModelID, long FloorID, bool isGeometryModified = true, bool isDummyAlteration = false);
//	void StartEdit( CmdProcess::CommandUpdateElementType elementType, long ModelID, long OwnerCompoGID, long FloorID, bool isGeometryModified = true, bool isCheckModelSkip = false);
//	void StartEdit( CmdProcess::CommandUpdateElementType elementType, long ModelID, long OwnerCompoGID, long FloorID, bool isGeometryModified = true, bool isDummyAlteration = false);
	void StartEdit( CmdProcess::CommandUpdateElementType elementType, long ModelID, long OwnerCompoGID, long FloorID, bool isGeometryModified = true, bool isCheckModelSkip = false, bool isDummyAlteration = false);
	void EndEdit();
	void EndEdit_Force();
	void EndEdit_SkipTrimAgain();

	//코드 통합 이후 BH-Q에 추가 되는 변수
	static bool IsMVAddToBHQ();
	//코드 통합 이후 BH-A에 추가 되는 변수
	static bool IsMVAddToBHA();
	
	static bool IsBHQMode();
	static bool IsBHAMode();
	static bool IsBHFMode();
	static bool IsBHCEMode();
	static bool IsBHQorBHCEMode();
	static bool IsBHForBHAMode();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

private:
	CmdModifyModelObjectControl _CmdModifyModelControl;
// 	/**       
// 	 * 리비전 객체.
// 	 * 내가 최종적으로 생성되거나 수정된 리비전 객체
// 	 */
// 	MSRevision* mp_Revision;
	/**       
	 * 수정 시간.
	 * 내가 최종적으로 수정된 시간
	 */
//	CTime m_TimeStamp;
};

//http://cafe.daum.net/changsoft/OPNN/93 참고
class StructureModel_CLASS MSHistoryObject : public MSObject
{
public:
	//historyID는 타입마다 달라야 하고, 같은 생성자라면 같은 Unique한 ID를 만들어서 내 보내야 한다.
	MSHistoryObject(CString& historyID);
	~MSHistoryObject();
	bool IsDeleted(){ return _isDeleted; }
	virtual void SetDeleted(bool isDeleted);
	CString GetHistoryID(){ return _historyID; };
protected:
	bool _isDeleted;
	CString _historyID;
};

#pragma make_public(MSObject)
#endif