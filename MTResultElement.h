#pragma once
#include "afx.h"
#include "MSCompoM.h"
#include "MTReader.h"

class StructureModel_CLASS MTResultItem :public CObject
{
public:
	enum eItemType
	{
		None = 0,
		Name = 1,
		FromStory, ToStory,
		Width, Depth,
		DimensionWidth, DimensionDepth,
		Thick,
		MainBars, TBBars, LRBars,
		StirrupBars,
		CenHoop, EndHoop,
		XTieBars, YTieBars,
		SpecificTieBars,
		Shape,
		SideBars,
		VBars,
		HBars,
		ShearBars,
        TopBars,
        BotBars,
		SubMainBars,
        // Slab Bars
        SlabBar1,
        SlabBar2, SlabBar3, SlabBar4, SlabBar5, SlabBar6, SlabBar7, SlabBar8, SlabBar9, SlabBar10,
		// Steel Element
		SteelSection,
		// Sub Element
		LargeSection = 100,
		MiddelSection,
		SmallerSection,
        SlabShortStrip,
        SlabLongStrip,
		// Text
		TextTopBar = 300,
		TextBotBar,
		TextStirrupBar,
		TextSideBar,
		TextSubMainBar,
		// Option
		DifferentTextToData = 500,
	};
	enum eItemState
	{
		Error = -1,
		OK = 0,
		Warning = 1
	};
public:
	MTResultItem();
	MTResultItem(eItemType itemType, CString resultText);
	~MTResultItem();

	MTResultItem(MTResultItem& aData) { *this = aData; }
	MTResultItem& operator = (MTResultItem& aData);


	void Initialize();
	void SetItemState(eItemState state) { m_eItemStata = state; };

	MTResultItem::eItemType GetItemType() { return m_eItemType; };
	CString GetItemText() { return m_Text; };
	eItemState GetItemState() { return m_eItemStata; };

	void SetItemText(CString text) { m_Text = text; };
private:
	MTResultItem::eItemType m_eItemType;
	CString m_Text;
	eItemState m_eItemStata;
};

class StructureModel_CLASS MTResultElement :public CObject
{
public:
	MTResultElement();
	~MTResultElement();

	void Initialize();
	vector<MTResultItem*> GetResultItems();
	vector<MTResultItem*> GetSubResultItems();
	map<MTResultItem::eItemType, vector<MTResultItem*>> GetDictionarySubResultItem();
	map<MTResultItem::eItemType, vector<GM2DVector>> GetDictionaryLocation() { return mm_Location;};
    CString GetName();
protected:
	MSCompoM::Type_MSCompoM m_CompoMType;
	bool isFlatSlab;
	bool isDummy;
	map<MTResultItem::eItemType, MTResultItem*> mm_Result;
	map<MTResultItem::eItemType, MTResultElement*> mm_SubElement;
	map<MTResultItem::eItemType, vector<GM2DVector>> mm_Location;
public:
	MTResultItem* AddResultItem(MTResultItem::eItemType itemType, CString text = _T(""));

	MTResultItem* GetResultItem(MTResultItem::eItemType itemType);

	void SetCompoMType(MSCompoM::Type_MSCompoM type) { m_CompoMType = type; };
	MSCompoM::Type_MSCompoM GetCompoMType() { return m_CompoMType; };

	void CopyResultItems(MTResultElement* pSource);
	bool GetIsFlatSlab() { return isFlatSlab; }
	void SetIsFlatSlab(bool isFlat) { isFlatSlab = isFlat; }

	bool GetIsDummy() { return isDummy; }
	void SetIsDummy(bool flag) { isDummy = flag; };
#pragma region --- SubElement (BeamSection, )

	MTResultElement* AddSubElement(MTResultItem::eItemType itemType);
	MTResultElement* GetSubElement(MTResultItem::eItemType itemType);
	MTResultItem* GetSubItem(MTResultItem::eItemType elementType, MTResultItem::eItemType itemType);
	MTResultItem* AddSubItem(MTResultItem::eItemType elementType, MTResultItem::eItemType itemType, CString text);
	void SetSubItemState(MTResultItem::eItemType elementType, MTResultItem::eItemType itemType, MTResultItem::eItemState state);

#pragma endregion
	
#pragma region Location
	void SetLocationList(MTResultItem::eItemType itemType, vector<GM2DVector>& locArr);
	vector<GM2DVector> GetLocationList(MTResultItem::eItemType itemType);
#pragma endregion


protected:
	void DeleteResults();
	void DeleteSubResults();
protected:
	GM2DRectangle* mp_Area;
public :
	GM2DRectangle* GetArea() { return mp_Area; }
	void SetArea(GM2DRectangle* pArea);
};

