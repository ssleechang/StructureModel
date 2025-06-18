#pragma once
#include "MSElement.h"

class MSBaseBuilding;
class MSMaterial;
class MSCompoG;
class StructureModel_CLASS MSAddRebarBase :public MSElement
{
public:
	MSAddRebarBase();
	~MSAddRebarBase();

	void Serialize(CArchive& archive);
	DECLARE_SERIAL(MSAddRebarBase)
	virtual void CopyFromMe(MSObject* pSource, bool bNewID = true);

	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);

	virtual void Translate(GM2DVector &pDelta);
	virtual void MirrorByAxis(GM2DLineVector& pAxis);
	virtual void RotateZ(double theta, GM2DVector *pCenter);

	static MSAddRebarBase* CreateAddRebar(MSElement::Type_MSCompoG type);
	enum eAddBarType { AddBar= 0, MaterialLift, EVFailPrevention};
	enum eOffsetType  { Cover = 0, User = 1};

	virtual MSElement* GetOwnerCompoG() override;
	GM2DPolyline* GetProfile() {return m_pProfile;};
	eAddBarType GetAddBarType() { return m_eAddBarType; };
	eOffsetType GetTopOffsetType() { return m_eTopOffsetType; };
	double GetTopUserOffset() { return m_TopUserOffset; };
	eOffsetType GetBotOffsetType() { return m_eBotOffsetType; };
	double GetBotUserOffset() { return m_BotUserOffset; };
	bool GetIsUsedOwnerMaterial() { return m_IsUsedOwnerMaterial; };
	int GetMaterialID();
	MSMaterial* GetMaterial();
	bool isPlainConcrete();
	bool GetIsConstraint() {return m_IsConstraint;};
	double GetPlacingRebarDegree() { return m_PlacingRebarDegree; };

	void SetBuilding(MSBaseBuilding* pBuilding) { m_pBuilding = pBuilding; };
	virtual void SetOwnerCompoG(MSElement* pOwnerCompoG) override;
	
	void SetProfile(GM2DPolyline* profile);
	void SetAddBarType(eAddBarType addBarType) { m_eAddBarType = addBarType; };
	void SetTopOffsetType(eOffsetType offsetType) { m_eTopOffsetType = offsetType; };
	void SetTopUserOffset(double topUserOffset) { m_TopUserOffset = topUserOffset; };
	void SetBotOffsetType(eOffsetType offsetType) { m_eBotOffsetType = offsetType; };
	void SetBotUserOffset(double botUserOffset) { m_BotUserOffset = botUserOffset; };

	void SetIsUsedOwnerMaterial(bool bUsedOwnerMaterial) { m_IsUsedOwnerMaterial = bUsedOwnerMaterial; };
	void SetMaterialID(int materialID) { m_MaterialID = materialID;};
	void SetIsConstraint(bool isConstraint) { m_IsConstraint = isConstraint; };
	void SetPlacingRebarDegree(double degree) { m_PlacingRebarDegree = degree; };

	void SetIsMainAddBarType(bool isMainAddBarType) { m_IsMainAddBarType = isMainAddBarType; };
	bool GetIsMainAddBarType() { return m_IsMainAddBarType; };

protected:
	MSBaseBuilding* m_pBuilding;
	MSCompoG* m_pOwnerCompoG;

	GM2DPolyline* m_pProfile;
	eAddBarType m_eAddBarType;
	eOffsetType m_eTopOffsetType;
	double m_TopUserOffset;
	eOffsetType m_eBotOffsetType;
	double m_BotUserOffset;

	bool m_IsUsedOwnerMaterial;
	int m_MaterialID;
	bool m_IsConstraint;
	double m_PlacingRebarDegree;

	bool m_IsMainAddBarType;
};

