#include "StdAfx.h"
#include "BaseRebarTemplate.h"
#include "MSDeformedBar.h"

BaseRebarTemplate::BaseRebarTemplate()
{
}

BaseRebarTemplate::~BaseRebarTemplate(void)
{
}

void BaseRebarTemplate::InitAddBarDiaIndexMap(map<long, long> &mapAddBarDiaIndex, bool bSameDia)
{
	mapAddBarDiaIndex.clear();
	CStringArray BarDiaArr;
	long NumDia = MSDeformedBar::GetRebarNameList(BarDiaArr);
	for (long iIndex = 0; iIndex < NumDia; iIndex++)
	{
		if(bSameDia)
			mapAddBarDiaIndex.insert(make_pair(iIndex, iIndex));
		else
		{
			if (iIndex < 3)
				mapAddBarDiaIndex.insert(make_pair(iIndex, iIndex + 1));
			else
				mapAddBarDiaIndex.insert(make_pair(iIndex, iIndex));
		}
	}
}

void BaseRebarTemplate::AddBarDiaIndexMapDataCopy(map<long, long> &mapSrcAddBarDiaIndex, map<long, long> &mapDesAddBarDiaIndex)
{
	mapSrcAddBarDiaIndex.clear();

	map<long, long>::iterator it;
	for (it = mapDesAddBarDiaIndex.begin(); it != mapDesAddBarDiaIndex.end(); it++)
	{
		mapSrcAddBarDiaIndex.insert(make_pair(it->first, it->second));
	}
}

void BaseRebarTemplate::InitEndBarDiaIndexMap(map<long, long> &mapEndBarDiaIndex, bool bSameDia)
{
	mapEndBarDiaIndex.clear();
	CStringArray BarDiaArr;
	long NumDia = MSDeformedBar::GetRebarNameList(BarDiaArr);
	for (long iIndex = 0; iIndex < NumDia; iIndex++)
	{
		if(bSameDia)
			mapEndBarDiaIndex.insert(make_pair(iIndex, iIndex));
		else
		{
			if (iIndex < 3)
				mapEndBarDiaIndex.insert(make_pair(iIndex, iIndex + 1));
			else
				mapEndBarDiaIndex.insert(make_pair(iIndex, iIndex));
		}
	}
}

void BaseRebarTemplate::EndBarDiaIndexMapDataCopy(map<long, long> &mapSrcEndBarDiaIndex, map<long, long> &mapDesEndBarDiaIndex)
{
	mapSrcEndBarDiaIndex.clear();

	map<long, long>::iterator it;
	for (it = mapDesEndBarDiaIndex.begin(); it != mapDesEndBarDiaIndex.end(); it++)
	{
		mapSrcEndBarDiaIndex.insert(make_pair(it->first, it->second));
	}
}

void BaseRebarTemplate::InitDefaultUbarLenMap(map<long, long> &mapDefaultUbarLen)
{
	mapDefaultUbarLen.clear();
	long nSpliceLen = 400;
	CStringArray BarDiaArr;
	long NumDia = MSDeformedBar::GetRebarNameList(BarDiaArr);
	for (long iIndex = 0; iIndex < NumDia; iIndex++)
	{
		mapDefaultUbarLen.insert(make_pair(iIndex, nSpliceLen));
	}
}

void BaseRebarTemplate::DefaultUbarLenMapDataCopy(map<long, long> &mapSrcDefaultUbarLen, map<long, long> &mapDesDefaultUbarLen)
{
	mapSrcDefaultUbarLen.clear();

	map<long, long>::iterator it;
	for (it = mapDesDefaultUbarLen.begin(); it != mapDesDefaultUbarLen.end(); it++)
	{
		mapSrcDefaultUbarLen.insert(make_pair(it->first, it->second));
	}
}

void BaseRebarTemplate::InitUserDowelHookLenMap(map<long, long> &mapUserDowelHookLen)
{
	mapUserDowelHookLen.clear();
	CStringArray BarDiaArr;
	long NumDia = MSDeformedBar::GetRebarNameList(BarDiaArr);
	for (long iIndex = 0; iIndex < NumDia; iIndex++)
	{
		mapUserDowelHookLen.insert(make_pair(iIndex, 0));
	}
}

void BaseRebarTemplate::UserDowelHookLenMapDataCopy(map<long, long> &mapSrcUserDowelHookLen, map<long, long> &mapDesUserDowelHookLen)
{
	mapSrcUserDowelHookLen.clear();

	map<long, long>::iterator it;
	for (it = mapDesUserDowelHookLen.begin(); it != mapDesUserDowelHookLen.end(); it++)
	{
		mapSrcUserDowelHookLen.insert(make_pair(it->first, it->second));
	}
}

void BaseRebarTemplate::InitUserDowelSpliceLenMap(map<long, long> &mapUserDowelSpliceLen)
{
	mapUserDowelSpliceLen.clear();
	CStringArray BarDiaArr;
	long NumDia = MSDeformedBar::GetRebarNameList(BarDiaArr);
	for (long iIndex = 0; iIndex < NumDia; iIndex++)
	{
		mapUserDowelSpliceLen.insert(make_pair(iIndex, 0));
	}
}

void BaseRebarTemplate::UserDowelSpliceLenMapDataCopy(map<long, long> &mapSrcUserDowelSpliceLen, map<long, long> &mapDesUserDowelSpliceLen)
{
	mapSrcUserDowelSpliceLen.clear();

	map<long, long>::iterator it;
	for (it = mapDesUserDowelSpliceLen.begin(); it != mapDesUserDowelSpliceLen.end(); it++)
	{
		mapSrcUserDowelSpliceLen.insert(make_pair(it->first, it->second));
	}
}

void BaseRebarTemplate::InitSubStirrupHookLenMap(map<long, long> &mapHookLen)
{
	mapHookLen.clear();
	CStringArray BarDiaArr;
	long NumDia = MSDeformedBar::GetRebarNameList(BarDiaArr);
	for (long iIndex = 0; iIndex < NumDia; iIndex++)
	{
		mapHookLen.insert(make_pair(iIndex, 80));
	}
}

void BaseRebarTemplate::SubStirrupHookLenMapDataCopy(map<long, long> &mapSrcHookLen, map<long, long> &mapDesHookLen)
{
	mapSrcHookLen.clear();

	map<long, long>::iterator it;
	for (it = mapDesHookLen.begin(); it != mapDesHookLen.end(); it++)
	{
		mapSrcHookLen.insert(make_pair(it->first, it->second));
	}
}

void BaseRebarTemplate::InitShearBarHookLenMap(map<long, long> &mapHookLenA, map<long, long> &mapHookLenB)
{
	mapHookLenA.clear();
	mapHookLenB.clear();
	CStringArray BarDiaArr;
	long NumDia = MSDeformedBar::GetRebarNameList(BarDiaArr);
	for (long iIndex = 0; iIndex < NumDia; iIndex++)
	{
		mapHookLenA.insert(make_pair(iIndex, 150));
		mapHookLenB.insert(make_pair(iIndex, 80));
	}
}

void BaseRebarTemplate::ShearBarHookLenMapDataCopy(map<long, long> &mapSrcHookLenA, map<long, long> &mapSrcHookLenB, map<long, long> &mapDesHookLenA, map<long, long> &mapDesHookLenB)
{
	mapSrcHookLenA.clear();

	map<long, long>::iterator it;
	for (it = mapDesHookLenA.begin(); it != mapDesHookLenA.end(); it++)
	{
		mapSrcHookLenA.insert(make_pair(it->first, it->second));
	}

	mapSrcHookLenB.clear();

	for (it = mapDesHookLenB.begin(); it != mapDesHookLenB.end(); it++)
	{
		mapSrcHookLenB.insert(make_pair(it->first, it->second));
	}
}

void BaseRebarTemplate::InitSubStirrupDiaIndexMap(map<long, long> &mapSubStirrupDiaIndex)
{
	mapSubStirrupDiaIndex.clear();
	CStringArray BarDiaArr;
	long NumDia = MSDeformedBar::GetRebarNameList(BarDiaArr);
	for (long iIndex = 0; iIndex < NumDia; iIndex++)
	{
		if (iIndex < 3)
			mapSubStirrupDiaIndex.insert(make_pair(iIndex, iIndex + 1));
		else
			mapSubStirrupDiaIndex.insert(make_pair(iIndex, iIndex));
	}
}

void BaseRebarTemplate::SubStirrupDiaIndexMapDataCopy(map<long, long> &mapSrcSubStirrupDiaIndex, map<long, long> &mapDesSubStirrupDiaIndex)
{
	mapSrcSubStirrupDiaIndex.clear();

	map<long, long>::iterator it;
	for (it = mapDesSubStirrupDiaIndex.begin(); it != mapDesSubStirrupDiaIndex.end(); it++)
	{
		mapSrcSubStirrupDiaIndex.insert(make_pair(it->first, it->second));
	}
}

void BaseRebarTemplate::InitShearBarVerUserDiaIndexMap(map<long, long> &mapShearBarVerUserDiaIndex)
{
	mapShearBarVerUserDiaIndex.clear();
	CStringArray BarDiaArr;
	long NumDia = MSDeformedBar::GetRebarNameList(BarDiaArr);
	for (long iIndex = 0; iIndex < NumDia; iIndex++)
	{
		if (iIndex < 3)
			mapShearBarVerUserDiaIndex.insert(make_pair(iIndex, iIndex + 1));
		else
			mapShearBarVerUserDiaIndex.insert(make_pair(iIndex, iIndex));
	}
}

void BaseRebarTemplate::InitShearBarHorUserDiaIndexMap(map<long, long> &mapShearBarHorUserDiaIndex)
{
	mapShearBarHorUserDiaIndex.clear();
	CStringArray BarDiaArr;
	long NumDia = MSDeformedBar::GetRebarNameList(BarDiaArr);
	for (long iIndex = 0; iIndex < NumDia; iIndex++)
	{
		if (iIndex < 3)
			mapShearBarHorUserDiaIndex.insert(make_pair(iIndex, iIndex + 1));
		else
			mapShearBarHorUserDiaIndex.insert(make_pair(iIndex, iIndex));
	}
}

void BaseRebarTemplate::ShearBarVerUserDiaIndexMapDataCopy(map<long, long> &mapSrcShearBarVerUserDiaIndex, map<long, long> &mapDesShearBarVerUserDiaIndex)
{
	mapSrcShearBarVerUserDiaIndex.clear();

	map<long, long>::iterator it;
	for (it = mapDesShearBarVerUserDiaIndex.begin(); it != mapDesShearBarVerUserDiaIndex.end(); it++)
	{
		mapSrcShearBarVerUserDiaIndex.insert(make_pair(it->first, it->second));
	}
}

void BaseRebarTemplate::ShearBarHorUserDiaIndexMapDataCopy(map<long, long> &mapSrcShearBarHorUserDiaIndex, map<long, long> &mapDesShearBarHorUserDiaIndex)
{
	mapSrcShearBarHorUserDiaIndex.clear();

	map<long, long>::iterator it;
	for (it = mapDesShearBarHorUserDiaIndex.begin(); it != mapDesShearBarHorUserDiaIndex.end(); it++)
	{
		mapSrcShearBarHorUserDiaIndex.insert(make_pair(it->first, it->second));
	}
}

void BaseRebarTemplate::InitUserLenMap(map<long, long> &mapUserLen, long value)
{
	mapUserLen.clear();
	CStringArray BarDiaArr;
	long NumDia = MSDeformedBar::GetRebarNameList(BarDiaArr);
	for (long iIndex = 0; iIndex < NumDia; iIndex++)
	{
		mapUserLen.insert(make_pair(iIndex, value));
	}
}

void BaseRebarTemplate::UserMapDataCopy(map<long, long> &mapSrcUser, map<long, long> &mapDesUser)
{
	mapSrcUser.clear();

	map<long, long>::iterator it;
	for (it = mapDesUser.begin(); it != mapDesUser.end(); it++)
	{
		mapSrcUser.insert(make_pair(it->first, it->second));
	}
}
void BaseRebarTemplate::InitUserDiaIndexMap(map<long, long> &mapBarDiaIndex)
{
	mapBarDiaIndex.clear();
	CStringArray BarDiaArr;
	long NumDia = MSDeformedBar::GetRebarNameList(BarDiaArr);
	for (long iIndex = 0; iIndex < NumDia; iIndex++)
	{
		if (iIndex < 3)
			mapBarDiaIndex.insert(make_pair(iIndex, iIndex + 1));
		else
			mapBarDiaIndex.insert(make_pair(iIndex, iIndex));
	}
}