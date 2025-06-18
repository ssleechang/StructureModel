#pragma once
#include "MSObject.h"

#include"MSStirrupBar.h"
#include "MSMainBar.h"

#include "GMLib/GM2DVector.h"
#include "GMLib/GM2DPolyline.h"

class MSAddRebarSetting;
class StructureModel_CLASS MSLineRebar :public MSObject
{
public:
	MSLineRebar();
	~MSLineRebar();

	void Serialize(CArchive& archive);
	DECLARE_SERIAL(MSLineRebar)
	virtual void CopyFromMe(MSObject* pSource, bool bNewID = true) override;

	virtual void Dump(EFS::EFS_Buffer& buffer);
	virtual void Recovery(EFS::EFS_Buffer& buffer);

	bool GetIsActivation() {	return m_IsActivation;};
	bool GetIsNumPlaceRebar() { return m_IsNumPlaceRebar; };
	MSAddRebarSetting* GetStartSetting() { return m_pStartSet; };
	MSAddRebarSetting* GetEndSetting() { return m_pEndSet; };
	bool GetIsCutOpening() { return m_IsCutOpening; };

	void SetIsActivation(bool isActivation) { m_IsActivation = isActivation; };
	void SetIsNumPlaceRebar(bool isNumPlaceRebar) { m_IsNumPlaceRebar = isNumPlaceRebar; };
	void SetIsCutOpening(bool isCutOpening) { m_IsCutOpening = isCutOpening; };

	MSMainBar m_MainBars;
	MSStirrupBar m_StirrupBars;

protected:
	bool m_IsActivation;
	bool m_IsNumPlaceRebar;
	bool m_IsCutOpening;

	MSAddRebarSetting* m_pStartSet;
	MSAddRebarSetting* m_pEndSet;
};

