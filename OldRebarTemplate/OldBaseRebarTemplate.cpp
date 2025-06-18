#include "StdAfx.h"
#include "OldBaseRebarTemplate.h"
#include "..\MSDeformedBar.h"

OldBaseRebarTemplate::OldBaseRebarTemplate()
{
}

OldBaseRebarTemplate::~OldBaseRebarTemplate(void)
{
}

void OldBaseRebarTemplate::InitAddBarDiaIndexMap(map<long, long> &mapAddBarDiaIndex)
{
	mapAddBarDiaIndex.clear();
	CStringArray BarDiaArr;
	long NumDia = MSDeformedBar::GetRebarNameList(BarDiaArr);
	for (long iIndex = 0; iIndex < NumDia; iIndex++)
	{
		if (iIndex < 3)
			mapAddBarDiaIndex.insert(make_pair(iIndex, iIndex + 1));
		else
			mapAddBarDiaIndex.insert(make_pair(iIndex, iIndex));
	}
}

void OldBaseRebarTemplate::AddBarDiaIndexMapDataCopy(map<long, long> &mapSrcAddBarDiaIndex, map<long, long> &mapDesAddBarDiaIndex)
{
	mapSrcAddBarDiaIndex.clear();

	map<long, long>::iterator it;
	for (it = mapDesAddBarDiaIndex.begin(); it != mapDesAddBarDiaIndex.end(); it++)
	{
		mapSrcAddBarDiaIndex.insert(make_pair(it->first, it->second));
	}
}

void OldBaseRebarTemplate::InitEndBarDiaIndexMap(map<long, long> &mapEndBarDiaIndex)
{
	mapEndBarDiaIndex.clear();
	CStringArray BarDiaArr;
	long NumDia = MSDeformedBar::GetRebarNameList(BarDiaArr);
	for (long iIndex = 0; iIndex < NumDia; iIndex++)
	{
		if (iIndex < 3)
			mapEndBarDiaIndex.insert(make_pair(iIndex, iIndex + 1));
		else
			mapEndBarDiaIndex.insert(make_pair(iIndex, iIndex));
	}
}

void OldBaseRebarTemplate::EndBarDiaIndexMapDataCopy(map<long, long> &mapSrcEndBarDiaIndex, map<long, long> &mapDesEndBarDiaIndex)
{
	mapSrcEndBarDiaIndex.clear();

	map<long, long>::iterator it;
	for (it = mapDesEndBarDiaIndex.begin(); it != mapDesEndBarDiaIndex.end(); it++)
	{
		mapSrcEndBarDiaIndex.insert(make_pair(it->first, it->second));
	}
}

void OldBaseRebarTemplate::InitDefaultUbarLenMap(map<long, long> &mapDefaultUbarLen)
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

void OldBaseRebarTemplate::DefaultUbarLenMapDataCopy(map<long, long> &mapSrcDefaultUbarLen, map<long, long> &mapDesDefaultUbarLen)
{
	mapSrcDefaultUbarLen.clear();

	map<long, long>::iterator it;
	for (it = mapDesDefaultUbarLen.begin(); it != mapDesDefaultUbarLen.end(); it++)
	{
		mapSrcDefaultUbarLen.insert(make_pair(it->first, it->second));
	}
}

void OldBaseRebarTemplate::InitUserDowelHookLenMap(map<long, long> &mapUserDowelHookLen)
{
	mapUserDowelHookLen.clear();
	CStringArray BarDiaArr;
	long NumDia = MSDeformedBar::GetRebarNameList(BarDiaArr);
	for (long iIndex = 0; iIndex < NumDia; iIndex++)
	{
		mapUserDowelHookLen.insert(make_pair(iIndex, 0));
	}
}

void OldBaseRebarTemplate::UserDowelHookLenMapDataCopy(map<long, long> &mapSrcUserDowelHookLen, map<long, long> &mapDesUserDowelHookLen)
{
	mapSrcUserDowelHookLen.clear();

	map<long, long>::iterator it;
	for (it = mapDesUserDowelHookLen.begin(); it != mapDesUserDowelHookLen.end(); it++)
	{
		mapSrcUserDowelHookLen.insert(make_pair(it->first, it->second));
	}
}

void OldBaseRebarTemplate::InitUserDowelSpliceLenMap(map<long, long> &mapUserDowelSpliceLen)
{
	mapUserDowelSpliceLen.clear();
	CStringArray BarDiaArr;
	long NumDia = MSDeformedBar::GetRebarNameList(BarDiaArr);
	for (long iIndex = 0; iIndex < NumDia; iIndex++)
	{
		mapUserDowelSpliceLen.insert(make_pair(iIndex, 0));
	}
}

void OldBaseRebarTemplate::UserDowelSpliceLenMapDataCopy(map<long, long> &mapSrcUserDowelSpliceLen, map<long, long> &mapDesUserDowelSpliceLen)
{
	mapSrcUserDowelSpliceLen.clear();

	map<long, long>::iterator it;
	for (it = mapDesUserDowelSpliceLen.begin(); it != mapDesUserDowelSpliceLen.end(); it++)
	{
		mapSrcUserDowelSpliceLen.insert(make_pair(it->first, it->second));
	}
}

void OldBaseRebarTemplate::InitSubStirrupHookLenMap(map<long, long> &mapHookLen)
{
	mapHookLen.clear();
	CStringArray BarDiaArr;
	long NumDia = MSDeformedBar::GetRebarNameList(BarDiaArr);
	for (long iIndex = 0; iIndex < NumDia; iIndex++)
	{
		mapHookLen.insert(make_pair(iIndex, 80));
	}
}

void OldBaseRebarTemplate::SubStirrupHookLenMapDataCopy(map<long, long> &mapSrcHookLen, map<long, long> &mapDesHookLen)
{
	mapSrcHookLen.clear();

	map<long, long>::iterator it;
	for (it = mapDesHookLen.begin(); it != mapDesHookLen.end(); it++)
	{
		mapSrcHookLen.insert(make_pair(it->first, it->second));
	}
}

void OldBaseRebarTemplate::InitShearBarHookLenMap(map<long, long> &mapHookLenA, map<long, long> &mapHookLenB)
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

void OldBaseRebarTemplate::ShearBarHookLenMapDataCopy(map<long, long> &mapSrcHookLenA, map<long, long> &mapSrcHookLenB, map<long, long> &mapDesHookLenA, map<long, long> &mapDesHookLenB)
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

void OldBaseRebarTemplate::InitSubStirrupDiaIndexMap(map<long, long> &mapSubStirrupDiaIndex)
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

void OldBaseRebarTemplate::SubStirrupDiaIndexMapDataCopy(map<long, long> &mapSrcSubStirrupDiaIndex, map<long, long> &mapDesSubStirrupDiaIndex)
{
	mapSrcSubStirrupDiaIndex.clear();

	map<long, long>::iterator it;
	for (it = mapDesSubStirrupDiaIndex.begin(); it != mapDesSubStirrupDiaIndex.end(); it++)
	{
		mapSrcSubStirrupDiaIndex.insert(make_pair(it->first, it->second));
	}
}