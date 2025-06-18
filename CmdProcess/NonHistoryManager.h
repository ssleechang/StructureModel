#pragma once

#include "UpdateElementType.h"
using namespace CmdProcess;



	class StructureModel_CLASS AddDelUpdateInfo
	{
	public:
		AddDelUpdateInfo(CommandUpdateElementType type, int elementID, int floorID, int extraID1, int extraID2) 
		{ 
			this->type = type; this->elementID = elementID; this->floorID = floorID; 
			this->extraID1 = extraID1; this->extraID2 = extraID2;
		};
		CommandUpdateElementType type;
		int elementID;
		int floorID;
		int extraID1;//used for OwnerCompoG(Opening등 다른 CompoG에 종속적인 CompoG일경우) , or RAPGroupID for RAPBarset, etc...
		int extraID2;//used for RAPUnitID for RAPBarset, etc...
	};


	class StructureModel_CLASS ModifyUpdateInfo
	{
	public:
		//ModifyUpdateInfo(CommandUpdateElementType type, void* element, int elementID, int floorID, int ownerCompoGID, bool isGeometryUpdated = true) 
		//ModifyUpdateInfo(CommandUpdateElementType type, void* element, int elementID, int ownerCompoGID, bool isGeometryUpdated = true, bool isDummyAlteration = false) 
		ModifyUpdateInfo(CommandUpdateElementType type, void* element, int elementID, int floorID, int ownerCompoGID, bool isGeometryUpdated = true, bool isDummyAlteration = false) 
		{ 
			this->type = type; this->elementID = elementID; this->ownerCompoGID = ownerCompoGID; 
			this->isGeometryUpdated = isGeometryUpdated;
			this->isDummyAlteration = isDummyAlteration;
			this->floorID = floorID;
		};
		CommandUpdateElementType type;
		
		int elementID;
		int floorID;
		int ownerCompoGID;//Opening등 다른 CompoG에 종속적인 CompoG일 경우, OwnerCompoG가 존재
		bool isGeometryUpdated;
		bool isDummyAlteration;
	};



	class StructureModel_CLASS NonHistoryManager
	{
	public:
		NonHistoryManager(void);
		~NonHistoryManager(void);

		void AddAddUpdatedInfo(CommandUpdateElementType type, int elementID, int floorID,  int extraID1=0, int extraID =0);
		void AddDeleteUpdateInfo(CommandUpdateElementType type, int elementID, int floorID,  int extraID1=0, int extraID=0);

		void AddModifyUpdateInfo(CommandUpdateElementType type, void* element, int elementID, int floorID, int ownerCompoGID, bool isGeometryModified = true);

		//View Update하기전, HisotryManager에서 ArrangeCommandContainer할때 호출된다.
		//_CurTR에 쌓여있는 정보를 View가 접근하는 쪽으로 옮기고 _CurTR을 리셋한다.
		void UpdateModifyInfo(); 
		
		//한번 UpdateModifyInfo가 호출되고 난 후, 추가 Update사항이 생겼을 때, 현재의 UpdateInfo를 Reset하지 않고 CurTR의 정보를 추가한다.
		void UpdateAdditionalModify();

		vector<AddDelUpdateInfo> GetAddUpdateInfos();
		vector<AddDelUpdateInfo> GetDeleteUpdateInfos();
		vector<ModifyUpdateInfo> GetModifyUpdateInfos();
		void Clear();
	private:

		vector<AddDelUpdateInfo> m_AddUpdateInfos;
		vector<AddDelUpdateInfo> m_DeleteUpdateInfos;
		vector<ModifyUpdateInfo> m_ModifyUpdateInfos;

		vector<AddDelUpdateInfo> m_AddUpdateInfos_CurTR;
		vector<AddDelUpdateInfo> m_DeleteUpdateInfos_CurTR;
		vector<ModifyUpdateInfo> m_ModifyUpdateInfos_CurTR;

	};



