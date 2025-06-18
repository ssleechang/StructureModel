#pragma once
#include "dtobject.h"

class DTGroup;
class DTOpeningWall;

class StructureModel_CLASS DTManager :
	public DTObject
{
public:
	DTManager();
	~DTManager();

	//DTFileOpen();
	bool MakeTestDefaultSample();


	map<long,DTGroup*> mm_Group;

	//DTOpenningWall
	vector<DTOpeningWall*> ma_OpenningWall;
	//key::WallOpeningID
	map<long,DTOpeningWall*> mm_OpenningWall;


	//


};

 