#include "stdafx.h"
#include "MTResultElement.h"

#pragma region MTResultItem
MTResultItem::MTResultItem()
{
	Initialize();
}
MTResultItem::MTResultItem(MTResultItem::eItemType itemType, CString text)
{
	Initialize();
	m_eItemType = itemType;
	m_Text = text;
}

void MTResultItem::Initialize()
{
	m_eItemType = MTResultItem::None;
	m_Text = _T("");
	m_eItemStata = MTResultItem::OK;
}
MTResultItem& MTResultItem::operator = (MTResultItem& aData)
{
	Initialize();
	m_eItemType = aData.m_eItemType;
	m_Text = aData.m_Text;
	m_eItemStata = aData.m_eItemStata;

	return *this;
}

MTResultItem::~MTResultItem()
{

}

#pragma endregion

#pragma region MTResultElement
MTResultElement::MTResultElement()
{
	Initialize();
}


MTResultElement::~MTResultElement()
{
	DeleteResults();
	DeleteSubResults();
	if (mp_Area != nullptr)	delete mp_Area;
}

void MTResultElement::Initialize()
{
	DeleteResults();
	DeleteSubResults();
	m_CompoMType = MSCompoM::msNONE;
	isFlatSlab = false;

	mp_Area = nullptr;
	mm_Location.clear();
	isDummy = false;
}

void MTResultElement::SetArea(GM2DRectangle* pArea)
{
	if (pArea == nullptr)	return;
	mp_Area = new GM2DRectangle(*pArea);
}


void MTResultElement::DeleteResults()
{
	for (auto it = mm_Result.begin(); it != mm_Result.end(); it++)
		delete it->second;
	mm_Result.clear();
}

void MTResultElement::DeleteSubResults()
{
	for (auto it = mm_SubElement.begin(); it != mm_SubElement.end(); it++)
		delete it->second;
	mm_SubElement.clear();
}

CString MTResultElement::GetName()
{
    auto it = mm_Result.find(MTResultItem::Name);
    if (it != mm_Result.end())
    {
        return it->second->GetItemText();
    }
    return _T("");
}

vector<MTResultItem*> MTResultElement::GetResultItems()
{
	vector<MTResultItem*> itemList;
	for (auto it = mm_Result.begin(); it != mm_Result.end(); it++)
		itemList.push_back(it->second);
	return itemList;
}
vector<MTResultItem*> MTResultElement::GetSubResultItems()
{
	vector<MTResultItem*> itemList;
	for (auto itSub = mm_SubElement.begin(); itSub != mm_SubElement.end(); itSub++)
	{
		vector<MTResultItem*> subItemList = itSub->second->GetResultItems();
		itemList.insert(itemList.end(), subItemList.begin(), subItemList.end());
	}
	return itemList;
}
map<MTResultItem::eItemType, vector<MTResultItem*>> MTResultElement::GetDictionarySubResultItem()
{
	map<MTResultItem::eItemType, vector<MTResultItem*>> dicData;
	for (auto itSub = mm_SubElement.begin(); itSub != mm_SubElement.end(); itSub++)
	{
		vector<MTResultItem*> subItemList = itSub->second->GetResultItems();
		dicData.insert(make_pair(itSub->first, subItemList));
	}
	return dicData;
}

MTResultItem* MTResultElement::AddResultItem(MTResultItem::eItemType itemType, CString text)
{
	auto it = mm_Result.find(itemType);
	if (it != mm_Result.end())
	{
		it->second->SetItemText(text);
		return it->second;
	}
	else
	{
		MTResultItem* item = new MTResultItem(itemType, text);
		mm_Result.insert(make_pair(itemType, item));
		return item;
	}
}

MTResultItem* MTResultElement::GetResultItem(MTResultItem::eItemType itemType)
{
	auto it = mm_Result.find(itemType);
	if (it != mm_Result.end())
		return it->second;

	return AddResultItem(itemType, _T(""));
}

#pragma region Sub Element
MTResultElement* MTResultElement::AddSubElement(MTResultItem::eItemType itemType)
{
	auto it = mm_SubElement.find(itemType);
	if (it != mm_SubElement.end())
		return it->second;
	else
		return GetSubElement(itemType);
}
MTResultElement* MTResultElement::GetSubElement(MTResultItem::eItemType itemType)
{
	auto it = mm_SubElement.find(itemType);
	if (it != mm_SubElement.end())
		return it->second;
	else
	{
		MTResultElement* pSubElement = new MTResultElement();
		mm_SubElement.insert(make_pair(itemType, pSubElement));
		return pSubElement;
	}
}

MTResultItem* MTResultElement::GetSubItem(MTResultItem::eItemType elementType, MTResultItem::eItemType itemType)
{
	MTResultElement* subElement = GetSubElement(elementType);
	return subElement->GetResultItem(itemType);
}
MTResultItem* MTResultElement::AddSubItem(MTResultItem::eItemType elementType, MTResultItem::eItemType itemType, CString text)
{
	MTResultElement* subElement = GetSubElement(elementType);
	return subElement->AddResultItem(itemType, text);
}
void MTResultElement::SetSubItemState(MTResultItem::eItemType elementType, MTResultItem::eItemType itemType, MTResultItem::eItemState state)
{
	MTResultElement* subElement = GetSubElement(elementType);
	MTResultItem* subItem = subElement->GetResultItem(itemType);
	subItem->SetItemState(state);
}

void MTResultElement::CopyResultItems(MTResultElement* pSource)
{
	DeleteResults();
	DeleteSubResults();
	for (auto it = pSource->mm_Result.begin(); it != pSource->mm_Result.end(); it++)
	{
		MTResultItem* pItem = AddResultItem(it->first, it->second->GetItemText());
		pItem->SetItemState(it->second->GetItemState());
	}
}
#pragma endregion

void MTResultElement::SetLocationList(MTResultItem::eItemType itemType, vector<GM2DVector>& locArr)
{
	auto it = mm_Location.find(itemType);
	if (it != mm_Location.end())	it->second = locArr;
	else
		mm_Location.insert(make_pair(itemType, locArr));
}
vector<GM2DVector> MTResultElement::GetLocationList(MTResultItem::eItemType itemType)
{
	auto it = mm_Location.find(itemType);
	if (it != mm_Location.end())	return it->second;

	vector<GM2DVector> locArr;
	return locArr;
}


#pragma endregion

