#include "stdafx.h"
#include "FormWorkTemplate.h"
#include "FormWorkTemplateManager.h"
#include "..\RADFCore_CLI\AppOptionsWrapper.h"
#include "..\GMLib\MSVersionInfo.h"

IMPLEMENT_SERIAL(FormWorkTemplate, MSObject, VERSIONABLE_SCHEMA | 1)

FormWorkTemplate::FormWorkTemplate(void)
{
	//Localization 은 LocalDic으로 진행
	InitSettingData();
}


FormWorkTemplate::~FormWorkTemplate(void)
{
}

MSObject* FormWorkTemplate::Copy(bool bNewID /*= true*/)
{
	FormWorkTemplate* pNewObj;

	pNewObj = new FormWorkTemplate();
	pNewObj->CopyFromMe(this, bNewID);
	return pNewObj;

}

void FormWorkTemplate::CopyFromMe(MSObject* pSource_in, bool bNewID/*=true*/)
{
	if (pSource_in == nullptr) return;

	FormWorkTemplate* Source = (FormWorkTemplate*)pSource_in;

	m_nID = Source->m_nID;
	m_sName = Source->m_sName;
	AllFormWorkSettingData.clear();
	for (int index = 0; index < Source->AllFormWorkSettingData.size(); index++)
	{
		SuperMemberType superMember = Source->AllFormWorkSettingData[index]->superMember;
		SubMemberType subMember = Source->AllFormWorkSettingData[index]->subMember;
		FormWorkPosition formPosition = Source->AllFormWorkSettingData[index]->formPosition;
		CString form = Source->AllFormWorkSettingData[index]->form;

		AllFormWorkSettingData.push_back(new FormWorkSettingData(superMember, subMember, formPosition, form));
	}
	
	MSObject::CopyFromMe(Source, bNewID);
}
void FormWorkTemplate::Dump(EFS::EFS_Buffer& buffer)
{
	MSObject::Dump(buffer);
	buffer << m_nID;
	buffer << m_sName;

	int count = (int)AllFormWorkSettingData.size();
	buffer << count;
	for (int i = 0; i < AllFormWorkSettingData.size(); i++)
	{
		int iSuperMember = (int)AllFormWorkSettingData[i]->superMember;
		int iMember = (int)AllFormWorkSettingData[i]->subMember;
		int iPosition = (int)AllFormWorkSettingData[i]->formPosition;
		buffer << iSuperMember;
		buffer << iMember;
		buffer << iPosition;
		buffer << AllFormWorkSettingData[i]->form;
	}
}

void FormWorkTemplate::Recovery(EFS::EFS_Buffer& buffer)
{
	MSObject::Recovery(buffer);

	AllFormWorkSettingData.clear();
	buffer >> m_nID;
	buffer >> m_sName;

	int count = 0;
	buffer >> count;

	for (int i = 0; i < count; i++)
	{
		int iSuperMember;
		int iSubMember;
		int iPosition;
		CString form = _T("");
		buffer >> iSuperMember;
		buffer >> iSubMember;
		buffer >> iPosition;
		buffer >> form;

		AllFormWorkSettingData.push_back(new FormWorkSettingData((SuperMemberType)iSuperMember, (SubMemberType)iSubMember, (FormWorkPosition)iPosition, form));
	}
}

void FormWorkTemplate::Serialize(CArchive &ar)
{
	MSObject::Serialize(ar);
	if (ar.IsStoring())
	{
		ar << m_nID;
		ar << m_sName;

		ar << (int)AllFormWorkSettingData.size();
		for (int i = 0; i < AllFormWorkSettingData.size(); i++)
		{
			int iSuperMember = (int)(AllFormWorkSettingData[i]->superMember);
			int iSubMember = (int)(AllFormWorkSettingData[i]->subMember);
			int iPosition = (int)(AllFormWorkSettingData[i]->formPosition);
			CString iForm = AllFormWorkSettingData[i]->form;
			ar << iSuperMember;
			ar << iSubMember;
			ar << iPosition;
			ar << iForm;
		}
	}
	else
	{
		AllFormWorkSettingData.clear();
		ar >> m_nID;
		ar >> m_sName;

		int count = 0;
		ar >> count;

		for (int i = 0; i < count; i++)
		{
			int iSuperMember =0 ;
			int iSubMember =0 ;
			int iPosition =0;
			CString form = _T("");
			ar >> iSuperMember;
			ar >> iSubMember;
			ar >> iPosition;
			ar >> form;

			if ((SuperMemberType)iSuperMember == SuperMemberType::SLAB && (SubMemberType)iSubMember == SubMemberType::GENERALDECK
				&& (FormWorkPosition)iPosition == FormWorkPosition::SIDE)
			{
				form = _T("");
			}

			// BH-5548 벽체 마구리면 거푸집 설정 추가
			if (MSVersionInfo::GetCurrentVersion() < 20200910)
			{
				if ((SuperMemberType)iSuperMember == SuperMemberType::BWALL && (SubMemberType)iSubMember == SubMemberType::BWALL && (FormWorkPosition)iPosition == FormWorkPosition::CLOSE)
				{					
					AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::BWALL, SubMemberType::BWALL, FormWorkPosition::STARTEND, form));
					AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::BWALL, SubMemberType::BWALL, FormWorkPosition::ENDEND, form));
					continue;
				}

				if ((SuperMemberType)iSuperMember == SuperMemberType::BWALL && (SubMemberType)iSubMember == SubMemberType::CUSTOMWALLCOLUMN && (FormWorkPosition)iPosition == FormWorkPosition::CLOSE)
				{
					AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::BWALL, SubMemberType::CUSTOMWALLCOLUMN, FormWorkPosition::STARTEND, _T("")));
					AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::BWALL, SubMemberType::CUSTOMWALLCOLUMN, FormWorkPosition::ENDEND, _T("")));
					continue;
				}

				if ((SuperMemberType)iSuperMember == SuperMemberType::RWALL && (SubMemberType)iSubMember == SubMemberType::RWALL && (FormWorkPosition)iPosition == FormWorkPosition::CLOSE)
				{
					AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::RWALL, SubMemberType::RWALL, FormWorkPosition::STARTEND, form));
					AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::RWALL, SubMemberType::RWALL, FormWorkPosition::ENDEND, form));
					continue;
				}
			}

			// 2020.08.14 [BH-5496] 계단거푸집설정 개선 [계단밑판(경사면),챌판] , 계단디딤판 거푸집설정 개선
			// 계단 거푸집 변경으로, 기존 거푸집 템플릿 자리에 새로운 템플릿을 넣는다.
			// https://changsoft.atlassian.net/browse/DQ1-427 
			// 1.4.X버전에 계단 거푸집 추가 이후, 1.3.X에도 반영이 되었는데, m_LatestVersion의 차이로 인해
			// 이미 계단 거푸집이 반영된 1.3.X의 모델을 1.4.X에서 로드 할 때 FormWorkTemplate::Serialize에서 계단 거푸집을 초기화 하는 것을 방지 하기 위해 추가
			if (MSVersionInfo::GetCurrentVersion() < 20200814 && MSVersionInfo::GetIsVersionChanged() == false)
			{
				if ((SuperMemberType)iSuperMember == SuperMemberType::STAIR && (SubMemberType)iSubMember == SubMemberType::STAIRSTEP && (FormWorkPosition)iPosition == FormWorkPosition::SIDE)
				{
					AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::STAIR, SubMemberType::STAIRSTEP, FormWorkPosition::SIDE, _T("")));
					AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::STAIR, SubMemberType::STAIRSTEP, FormWorkPosition::BOTTOM, _T("")));
					AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::STAIR, SubMemberType::STAIRSTEP, FormWorkPosition::TOP, form));
					AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::STAIR, SubMemberType::STAIRSTRINGER, FormWorkPosition::SIDE, form));
					AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::STAIR, SubMemberType::STAIRSTRINGER, FormWorkPosition::BOTTOM, _T("")));
					AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::STAIR, SubMemberType::STAIRSTRINGER, FormWorkPosition::TOP, _T("")));

					continue;
				}

				if ((SuperMemberType)iSuperMember == SuperMemberType::STAIR && (SubMemberType)iSubMember == SubMemberType::STAIRSTEP && (FormWorkPosition)iPosition == FormWorkPosition::BOTTOM)
				{
					AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::STAIR, SubMemberType::STAIRRISER, FormWorkPosition::SIDE, form));
					AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::STAIR, SubMemberType::STAIRRISER, FormWorkPosition::BOTTOM, _T("")));
					AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::STAIR, SubMemberType::STAIRRISER, FormWorkPosition::TOP, _T("")));
					AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::STAIR, SubMemberType::STAIRSOFFIT, FormWorkPosition::SIDE, _T("")));
					AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::STAIR, SubMemberType::STAIRSOFFIT, FormWorkPosition::BOTTOM, form));
					AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::STAIR, SubMemberType::STAIRSOFFIT, FormWorkPosition::TOP, _T("")));
					
					continue;
				}

				if ((SuperMemberType)iSuperMember == SuperMemberType::STAIR && (SubMemberType)iSubMember == SubMemberType::STAIRLANDING && (FormWorkPosition)iPosition == FormWorkPosition::BOTTOM)
				{
					AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::STAIR, SubMemberType::STAIRLANDING, FormWorkPosition::SIDE, _T("")));
					AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::STAIR, SubMemberType::STAIRLANDING, FormWorkPosition::BOTTOM, form));
					AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::STAIR, SubMemberType::STAIRLANDING, FormWorkPosition::TOP, _T("")));

					continue;
				}

				if (MSVersionInfo::GetCurrentVersion() < 20190214)
				{
					if ((SuperMemberType)iSuperMember == SuperMemberType::STAIR && (SubMemberType)iSubMember == SubMemberType::STAIRSOFFIT && (FormWorkPosition)iPosition == FormWorkPosition::TOP)
					{
						//HAUNCH
						AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::HAUNCH, SubMemberType::HAUNCH, FormWorkPosition::SIDE, LocalDic::GetLocal(_T("합판3회"), _T("FormWorkBase"))));
						AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::HAUNCH, SubMemberType::HAUNCH, FormWorkPosition::SLOPE, LocalDic::GetLocal(_T("합판3회"), _T("FormWorkBase"))));
						AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::HAUNCH, SubMemberType::HAUNCH, FormWorkPosition::CLOSE, LocalDic::GetLocal(_T("합판3회"), _T("FormWorkBase"))));


						continue;
					}
				}

				if (((SuperMemberType)iSuperMember == SuperMemberType::STAIR && (SubMemberType)iSubMember == SubMemberType::STAIRSTEP && (FormWorkPosition)iPosition == FormWorkPosition::SIDE) ||
					((SuperMemberType)iSuperMember == SuperMemberType::STAIR && (SubMemberType)iSubMember == SubMemberType::STAIRSTEP && (FormWorkPosition)iPosition == FormWorkPosition::BOTTOM) ||
					((SuperMemberType)iSuperMember == SuperMemberType::STAIR && (SubMemberType)iSubMember == SubMemberType::STAIRSTEP && (FormWorkPosition)iPosition == FormWorkPosition::TOP) ||
					((SuperMemberType)iSuperMember == SuperMemberType::STAIR && (SubMemberType)iSubMember == SubMemberType::STAIRLANDING && (FormWorkPosition)iPosition == FormWorkPosition::SIDE) ||
					((SuperMemberType)iSuperMember == SuperMemberType::STAIR && (SubMemberType)iSubMember == SubMemberType::STAIRLANDING && (FormWorkPosition)iPosition == FormWorkPosition::BOTTOM) ||
					((SuperMemberType)iSuperMember == SuperMemberType::STAIR && (SubMemberType)iSubMember == SubMemberType::STAIRLANDING && (FormWorkPosition)iPosition == FormWorkPosition::TOP) ||
					((SuperMemberType)iSuperMember == SuperMemberType::STAIR && (SubMemberType)iSubMember == SubMemberType::STAIRSTRINGER && (FormWorkPosition)iPosition == FormWorkPosition::SIDE) ||
					((SuperMemberType)iSuperMember == SuperMemberType::STAIR && (SubMemberType)iSubMember == SubMemberType::STAIRSTRINGER && (FormWorkPosition)iPosition == FormWorkPosition::BOTTOM) ||
					((SuperMemberType)iSuperMember == SuperMemberType::STAIR && (SubMemberType)iSubMember == SubMemberType::STAIRSTRINGER && (FormWorkPosition)iPosition == FormWorkPosition::TOP) ||
					((SuperMemberType)iSuperMember == SuperMemberType::STAIR && (SubMemberType)iSubMember == SubMemberType::STAIRRISER && (FormWorkPosition)iPosition == FormWorkPosition::SIDE) ||
					((SuperMemberType)iSuperMember == SuperMemberType::STAIR && (SubMemberType)iSubMember == SubMemberType::STAIRRISER && (FormWorkPosition)iPosition == FormWorkPosition::BOTTOM) ||
					((SuperMemberType)iSuperMember == SuperMemberType::STAIR && (SubMemberType)iSubMember == SubMemberType::STAIRRISER && (FormWorkPosition)iPosition == FormWorkPosition::TOP) ||
					((SuperMemberType)iSuperMember == SuperMemberType::STAIR && (SubMemberType)iSubMember == SubMemberType::STAIRSOFFIT && (FormWorkPosition)iPosition == FormWorkPosition::SIDE) ||
					((SuperMemberType)iSuperMember == SuperMemberType::STAIR && (SubMemberType)iSubMember == SubMemberType::STAIRSOFFIT && (FormWorkPosition)iPosition == FormWorkPosition::BOTTOM) ||
					((SuperMemberType)iSuperMember == SuperMemberType::STAIR && (SubMemberType)iSubMember == SubMemberType::STAIRSOFFIT && (FormWorkPosition)iPosition == FormWorkPosition::TOP))
					continue;

				//if (((SuperMemberType)iSuperMember == SuperMemberType::STAIR && (SubMemberType)iSubMember == SubMemberType::STAIRSTEP && (FormWorkPosition)iPosition == FormWorkPosition::SIDE) ||
				//	((SuperMemberType)iSuperMember == SuperMemberType::STAIR && (SubMemberType)iSubMember == SubMemberType::STAIRSTEP && (FormWorkPosition)iPosition == FormWorkPosition::BOTTOM) ||
				//	((SuperMemberType)iSuperMember == SuperMemberType::STAIR && (SubMemberType)iSubMember == SubMemberType::STAIRLANDING && (FormWorkPosition)iPosition == FormWorkPosition::SIDE))
				//{
				//	continue;
				//}
				//if ((SuperMemberType)iSuperMember == SuperMemberType::STAIR && (SubMemberType)iSubMember == SubMemberType::STAIRLANDING && (FormWorkPosition)iPosition == FormWorkPosition::BOTTOM)
				//{
				//	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::STAIR, SubMemberType::STAIRSTEP, FormWorkPosition::SIDE, _T("")));
				//	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::STAIR, SubMemberType::STAIRSTEP, FormWorkPosition::BOTTOM, _T("")));
				//	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::STAIR, SubMemberType::STAIRSTEP, FormWorkPosition::TOP, LocalDic::GetLocal(_T("None"), _T("FormWorkBase"))));
				//	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::STAIR, SubMemberType::STAIRLANDING, FormWorkPosition::SIDE, _T("")));
				//	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::STAIR, SubMemberType::STAIRLANDING, FormWorkPosition::BOTTOM, LocalDic::GetLocal(_T("합판3회"), _T("FormWorkBase"))));
				//	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::STAIR, SubMemberType::STAIRLANDING, FormWorkPosition::TOP, _T("")));
				//	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::STAIR, SubMemberType::STAIRSTRINGER, FormWorkPosition::SIDE, LocalDic::GetLocal(_T("합판3회"), _T("FormWorkBase"))));
				//	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::STAIR, SubMemberType::STAIRSTRINGER, FormWorkPosition::BOTTOM, _T("")));
				//	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::STAIR, SubMemberType::STAIRSTRINGER, FormWorkPosition::TOP, _T("")));
				//	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::STAIR, SubMemberType::STAIRRISER, FormWorkPosition::SIDE, LocalDic::GetLocal(_T("합판3회"), _T("FormWorkBase"))));
				//	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::STAIR, SubMemberType::STAIRRISER, FormWorkPosition::BOTTOM, _T("")));
				//	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::STAIR, SubMemberType::STAIRRISER, FormWorkPosition::TOP, _T("")));
				//	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::STAIR, SubMemberType::STAIRSOFFIT, FormWorkPosition::SIDE, _T("")));
				//	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::STAIR, SubMemberType::STAIRSOFFIT, FormWorkPosition::BOTTOM, LocalDic::GetLocal(_T("합판3회"), _T("FormWorkBase"))));
				//	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::STAIR, SubMemberType::STAIRSOFFIT, FormWorkPosition::TOP, _T("")));

				//	if (MSVersionInfo::GetCurrentVersion() < 20190214)
				//	{
				//		//HAUNCH
				//		AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::HAUNCH, SubMemberType::HAUNCH, FormWorkPosition::SIDE, LocalDic::GetLocal(_T("합판3회"), _T("FormWorkBase"))));
				//		AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::HAUNCH, SubMemberType::HAUNCH, FormWorkPosition::SLOPE, LocalDic::GetLocal(_T("합판3회"), _T("FormWorkBase"))));
				//		AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::HAUNCH, SubMemberType::HAUNCH, FormWorkPosition::CLOSE, LocalDic::GetLocal(_T("합판3회"), _T("FormWorkBase"))));
				//	}
				//	continue;
				//}
			}

			AllFormWorkSettingData.push_back(new FormWorkSettingData((SuperMemberType)iSuperMember, (SubMemberType)iSubMember, (FormWorkPosition)iPosition, form));

			if (MSVersionInfo::GetCurrentVersion() < 20190214)
			{
				if ((SuperMemberType)iSuperMember == SuperMemberType::STAIR && (SubMemberType)iSubMember == SubMemberType::STAIRSOFFIT && (FormWorkPosition)iPosition == FormWorkPosition::TOP)
				{
					//HAUNCH
					AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::HAUNCH, SubMemberType::HAUNCH, FormWorkPosition::SIDE, LocalDic::GetLocal(_T("합판3회"), _T("FormWorkBase"))));
					AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::HAUNCH, SubMemberType::HAUNCH, FormWorkPosition::SLOPE, LocalDic::GetLocal(_T("합판3회"), _T("FormWorkBase"))));
					AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::HAUNCH, SubMemberType::HAUNCH, FormWorkPosition::CLOSE, LocalDic::GetLocal(_T("합판3회"), _T("FormWorkBase"))));
				}
			}
		}

		if (MSVersionInfo::GetCurrentVersion() < 20241018)
		{
			AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::BEAM, SubMemberType::DECKBEAM, FormWorkPosition::SIDEOUT, LocalDic::GetLocal(_T("None"), _T("FormWorkBase"))));
			AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::BEAM, SubMemberType::DECKBEAM, FormWorkPosition::SIDEIN, LocalDic::GetLocal(_T("None"), _T("FormWorkBase"))));
			AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::BEAM, SubMemberType::DECKBEAM, FormWorkPosition::BOTTOM, LocalDic::GetLocal(_T("None"), _T("FormWorkBase"))));
		}

		FormWorkTemplateManager::SetLastID(m_nID);		
	}
}

void FormWorkTemplate::InitSettingData()
{
	AllFormWorkSettingData.clear();
	m_nID = -1;
	m_sName = _T("");

	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::COLUMN, SubMemberType::RECTCOLUMN, FormWorkPosition::SIDEOUT, LocalDic::GetLocal(_T("합판3회"), _T("FormWorkBase"))));
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::COLUMN, SubMemberType::RECTCOLUMN, FormWorkPosition::SIDEIN, LocalDic::GetLocal(_T("합판3회"), _T("FormWorkBase")))); 	
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::COLUMN, SubMemberType::CIRCOLUMN, FormWorkPosition::SIDEOUT, LocalDic::GetLocal(_T("합판3회"), _T("FormWorkBase")))); 	
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::COLUMN, SubMemberType::CIRCOLUMN, FormWorkPosition::SIDEIN, _T(""))); 	
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::COLUMN, SubMemberType::BUTTRESS, FormWorkPosition::SIDEOUT, LocalDic::GetLocal(_T("합판3회"), _T("FormWorkBase")))); 	
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::COLUMN, SubMemberType::BUTTRESS, FormWorkPosition::SIDEIN, LocalDic::GetLocal(_T("합판3회"), _T("FormWorkBase")))); 	
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::COLUMN, SubMemberType::CUSTOMCOLUMN, FormWorkPosition::SIDEOUT, LocalDic::GetLocal(_T("합판3회"), _T("FormWorkBase")))); 
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::COLUMN, SubMemberType::CUSTOMCOLUMN, FormWorkPosition::SIDEIN, LocalDic::GetLocal(_T("합판3회"), _T("FormWorkBase")))); 

    //BWall
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::BWALL, SubMemberType::BWALL, FormWorkPosition::SIDEOUT, LocalDic::GetLocal(_T("합판3회"), _T("FormWorkBase")))); 	
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::BWALL, SubMemberType::BWALL, FormWorkPosition::SIDEIN, LocalDic::GetLocal(_T("합판3회"), _T("FormWorkBase"))));
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::BWALL, SubMemberType::BWALL, FormWorkPosition::STARTEND, LocalDic::GetLocal(_T("합판3회"), _T("FormWorkBase"))));
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::BWALL, SubMemberType::BWALL, FormWorkPosition::ENDEND, LocalDic::GetLocal(_T("합판3회"), _T("FormWorkBase"))));
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::BWALL, SubMemberType::CUSTOMWALLCOLUMN, FormWorkPosition::SIDEOUT, LocalDic::GetLocal(_T("합판3회"), _T("FormWorkBase")))); 
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::BWALL, SubMemberType::CUSTOMWALLCOLUMN, FormWorkPosition::SIDEIN, LocalDic::GetLocal(_T("합판3회"), _T("FormWorkBase")))); 	
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::BWALL, SubMemberType::CUSTOMWALLCOLUMN, FormWorkPosition::STARTEND, _T("")));
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::BWALL, SubMemberType::CUSTOMWALLCOLUMN, FormWorkPosition::ENDEND, _T("")));

	//RWall
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::RWALL, SubMemberType::RWALL, FormWorkPosition::SIDEOUT, LocalDic::GetLocal(_T("합판3회"), _T("FormWorkBase")))); 	
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::RWALL, SubMemberType::RWALL, FormWorkPosition::SIDEIN, LocalDic::GetLocal(_T("합판3회"), _T("FormWorkBase"))));
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::RWALL, SubMemberType::RWALL, FormWorkPosition::STARTEND, LocalDic::GetLocal(_T("합판3회"), _T("FormWorkBase"))));
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::RWALL, SubMemberType::RWALL, FormWorkPosition::ENDEND, LocalDic::GetLocal(_T("합판3회"), _T("FormWorkBase"))));

	//LowWall
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::LOWWALL, SubMemberType::LOWWALL, FormWorkPosition::SIDEOUT, LocalDic::GetLocal(_T("합판3회"), _T("FormWorkBase")))); 
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::LOWWALL, SubMemberType::LOWWALL, FormWorkPosition::SIDEIN, LocalDic::GetLocal(_T("합판3회"), _T("FormWorkBase"))));
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::LOWWALL, SubMemberType::LOWWALL, FormWorkPosition::TOP, LocalDic::GetLocal(_T("None"), _T("FormWorkBase"))));
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::LOWWALL, SubMemberType::LOWWALL, FormWorkPosition::BOTTOM, LocalDic::GetLocal(_T("합판3회"), _T("FormWorkBase"))));

	//Beam
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::BEAM, SubMemberType::BEAM, FormWorkPosition::SIDEOUT, LocalDic::GetLocal(_T("합판3회"), _T("FormWorkBase")))); 	
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::BEAM, SubMemberType::BEAM, FormWorkPosition::SIDEIN, LocalDic::GetLocal(_T("합판3회"), _T("FormWorkBase"))));
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::BEAM, SubMemberType::BEAM, FormWorkPosition::BOTTOM, LocalDic::GetLocal(_T("합판3회"), _T("FormWorkBase"))));
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::BEAM, SubMemberType::PCBEAM, FormWorkPosition::SIDEOUT, LocalDic::GetLocal(_T("None"), _T("FormWorkBase")))); 	
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::BEAM, SubMemberType::PCBEAM, FormWorkPosition::SIDEIN, LocalDic::GetLocal(_T("None"), _T("FormWorkBase")))); 
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::BEAM, SubMemberType::PCBEAM, FormWorkPosition::BOTTOM, LocalDic::GetLocal(_T("None"), _T("FormWorkBase")))); 	
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::BEAM, SubMemberType::CUSTOMBEAM, FormWorkPosition::SIDEOUT, LocalDic::GetLocal(_T("합판3회"), _T("FormWorkBase")))); 	
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::BEAM, SubMemberType::CUSTOMBEAM, FormWorkPosition::SIDEIN, LocalDic::GetLocal(_T("합판3회"), _T("FormWorkBase")))); 	
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::BEAM, SubMemberType::CUSTOMBEAM, FormWorkPosition::BOTTOM, LocalDic::GetLocal(_T("합판3회"), _T("FormWorkBase")))); 	
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::BEAM, SubMemberType::DECKBEAM, FormWorkPosition::SIDEOUT, LocalDic::GetLocal(_T("None"), _T("FormWorkBase"))));
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::BEAM, SubMemberType::DECKBEAM, FormWorkPosition::SIDEIN, LocalDic::GetLocal(_T("None"), _T("FormWorkBase"))));
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::BEAM, SubMemberType::DECKBEAM, FormWorkPosition::BOTTOM, LocalDic::GetLocal(_T("None"), _T("FormWorkBase"))));

	//SLAB
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::SLAB, SubMemberType::SLABONBEAM, FormWorkPosition::SIDE, LocalDic::GetLocal(_T("합판3회"), _T("FormWorkBase"))));
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::SLAB, SubMemberType::SLABONBEAM, FormWorkPosition::BOTTOM, LocalDic::GetLocal(_T("합판3회"), _T("FormWorkBase")))); 	
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::SLAB, SubMemberType::SLABONBEAM, FormWorkPosition::BACKFILL, LocalDic::GetLocal(_T("None"), _T("FormWorkBase"))));
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::SLAB, SubMemberType::FLATSLAB, FormWorkPosition::SIDE, LocalDic::GetLocal(_T("합판3회"), _T("FormWorkBase")))); 	
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::SLAB, SubMemberType::FLATSLAB, FormWorkPosition::BOTTOM, LocalDic::GetLocal(_T("합판3회"), _T("FormWorkBase")))); 	
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::SLAB, SubMemberType::FLATSLAB, FormWorkPosition::BACKFILL, LocalDic::GetLocal(_T("None"), _T("FormWorkBase")))); 	
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::SLAB, SubMemberType::SOGSLAB, FormWorkPosition::SIDE, LocalDic::GetLocal(_T("합판3회"), _T("FormWorkBase")))); 
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::SLAB, SubMemberType::SOGSLAB, FormWorkPosition::BOTTOM, _T(""))); 
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::SLAB, SubMemberType::SOGSLAB, FormWorkPosition::BACKFILL, LocalDic::GetLocal(_T("None"), _T("FormWorkBase")))); 	
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::SLAB, SubMemberType::MATSLAB, FormWorkPosition::SIDE, LocalDic::GetLocal(_T("합판3회"), _T("FormWorkBase")))); 	
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::SLAB, SubMemberType::MATSLAB, FormWorkPosition::BOTTOM, _T("")));
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::SLAB, SubMemberType::MATSLAB, FormWorkPosition::BACKFILL, LocalDic::GetLocal(_T("None"), _T("FormWorkBase")))); 	
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::SLAB, SubMemberType::CIRCULARRAMP, FormWorkPosition::SIDE, LocalDic::GetLocal(_T("합판3회"), _T("FormWorkBase")))); 	
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::SLAB, SubMemberType::CIRCULARRAMP, FormWorkPosition::BOTTOM, LocalDic::GetLocal(_T("합판3회"), _T("FormWorkBase")))); 	
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::SLAB, SubMemberType::CIRCULARRAMP, FormWorkPosition::BACKFILL, LocalDic::GetLocal(_T("None"), _T("FormWorkBase")))); 	
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::SLAB, SubMemberType::REBAREMBEDDEDDECK, FormWorkPosition::SIDE, LocalDic::GetLocal(_T("Deck"), _T("FormWorkBase"))));
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::SLAB, SubMemberType::REBAREMBEDDEDDECK, FormWorkPosition::BOTTOM, LocalDic::GetLocal(_T("Deck"), _T("FormWorkBase")))); 
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::SLAB, SubMemberType::REBAREMBEDDEDDECK, FormWorkPosition::BACKFILL, LocalDic::GetLocal(_T("None"), _T("FormWorkBase")))); 
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::SLAB, SubMemberType::GENERALDECK, FormWorkPosition::SIDE, _T("")));
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::SLAB, SubMemberType::GENERALDECK, FormWorkPosition::BOTTOM, LocalDic::GetLocal(_T("Deck"), _T("FormWorkBase")))); 	
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::SLAB, SubMemberType::GENERALDECK, FormWorkPosition::BACKFILL, LocalDic::GetLocal(_T("None"), _T("FormWorkBase")))); 	
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::SLAB, SubMemberType::PCSLAB, FormWorkPosition::SIDE, LocalDic::GetLocal(_T("None"), _T("FormWorkBase")))); 	
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::SLAB, SubMemberType::PCSLAB, FormWorkPosition::BOTTOM, LocalDic::GetLocal(_T("None"), _T("FormWorkBase")))); 
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::SLAB, SubMemberType::PCSLAB, FormWorkPosition::BACKFILL, LocalDic::GetLocal(_T("None"), _T("FormWorkBase")))); 
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::SLAB, SubMemberType::DROPPANEL, FormWorkPosition::SIDE, LocalDic::GetLocal(_T("합판3회"), _T("FormWorkBase"))));
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::SLAB, SubMemberType::DROPPANEL, FormWorkPosition::BOTTOM, LocalDic::GetLocal(_T("합판3회"), _T("FormWorkBase"))));
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::SLAB, SubMemberType::DROPPANEL, FormWorkPosition::BACKFILL, LocalDic::GetLocal(_T("None"), _T("FormWorkBase"))));
	
	//PIT
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::PIT, SubMemberType::PIT, FormWorkPosition::SIDEOUT, LocalDic::GetLocal(_T("합판3회"), _T("FormWorkBase")))); 	
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::PIT, SubMemberType::PIT, FormWorkPosition::SIDEIN, LocalDic::GetLocal(_T("합판3회"), _T("FormWorkBase"))));
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::PIT, SubMemberType::PIT, FormWorkPosition::BACKFILL, LocalDic::GetLocal(_T("None"), _T("FormWorkBase"))));

	//ISOFOOT
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::ISOFOOT, SubMemberType::ISOFOOT, FormWorkPosition::SIDE, LocalDic::GetLocal(_T("합판3회"), _T("FormWorkBase")))); 	
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::ISOFOOT, SubMemberType::ISOFOOT, FormWorkPosition::BACKFILL, LocalDic::GetLocal(_T("None"), _T("FormWorkBase"))));

	//WALLFOOT
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::WALLFOOT, SubMemberType::WALLFOOT, FormWorkPosition::SIDEOUT, LocalDic::GetLocal(_T("합판3회"), _T("FormWorkBase")))); 
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::WALLFOOT, SubMemberType::WALLFOOT, FormWorkPosition::SIDEIN, LocalDic::GetLocal(_T("합판3회"), _T("FormWorkBase")))); 	
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::WALLFOOT, SubMemberType::WALLFOOT, FormWorkPosition::CLOSE, LocalDic::GetLocal(_T("합판3회"), _T("FormWorkBase"))));
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::WALLFOOT, SubMemberType::WALLFOOT, FormWorkPosition::BACKFILL, LocalDic::GetLocal(_T("None"), _T("FormWorkBase"))));

	//STAIR
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::STAIR, SubMemberType::STAIRSTEP, FormWorkPosition::SIDE, _T("")));
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::STAIR, SubMemberType::STAIRSTEP, FormWorkPosition::BOTTOM, _T("")));
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::STAIR, SubMemberType::STAIRSTEP, FormWorkPosition::TOP, LocalDic::GetLocal(_T("None"), _T("FormWorkBase"))));
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::STAIR, SubMemberType::STAIRLANDING, FormWorkPosition::SIDE, _T("")));
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::STAIR, SubMemberType::STAIRLANDING, FormWorkPosition::BOTTOM, LocalDic::GetLocal(_T("합판3회"), _T("FormWorkBase"))));
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::STAIR, SubMemberType::STAIRLANDING, FormWorkPosition::TOP, _T("")));
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::STAIR, SubMemberType::STAIRSTRINGER, FormWorkPosition::SIDE, LocalDic::GetLocal(_T("합판3회"), _T("FormWorkBase"))));
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::STAIR, SubMemberType::STAIRSTRINGER, FormWorkPosition::BOTTOM, _T("")));
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::STAIR, SubMemberType::STAIRSTRINGER, FormWorkPosition::TOP, _T("")));
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::STAIR, SubMemberType::STAIRRISER, FormWorkPosition::SIDE, LocalDic::GetLocal(_T("합판3회"), _T("FormWorkBase"))));
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::STAIR, SubMemberType::STAIRRISER, FormWorkPosition::BOTTOM, _T("")));
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::STAIR, SubMemberType::STAIRRISER, FormWorkPosition::TOP, _T("")));
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::STAIR, SubMemberType::STAIRSOFFIT, FormWorkPosition::SIDE, _T("")));
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::STAIR, SubMemberType::STAIRSOFFIT, FormWorkPosition::BOTTOM, LocalDic::GetLocal(_T("합판3회"), _T("FormWorkBase"))));
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::STAIR, SubMemberType::STAIRSOFFIT, FormWorkPosition::TOP, _T("")));

	//HAUNCH
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::HAUNCH, SubMemberType::HAUNCH, FormWorkPosition::SIDE, LocalDic::GetLocal(_T("합판3회"), _T("FormWorkBase"))));
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::HAUNCH, SubMemberType::HAUNCH, FormWorkPosition::SLOPE, LocalDic::GetLocal(_T("합판3회"), _T("FormWorkBase"))));
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::HAUNCH, SubMemberType::HAUNCH, FormWorkPosition::CLOSE, LocalDic::GetLocal(_T("합판3회"), _T("FormWorkBase"))));

	//SRC Column
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::SRCCOLUMN, SubMemberType::SRCRECTCOLUMN, FormWorkPosition::SIDEOUT, LocalDic::GetLocal(_T("합판3회"), _T("FormWorkBase")))); 	
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::SRCCOLUMN, SubMemberType::SRCRECTCOLUMN, FormWorkPosition::SIDEIN, LocalDic::GetLocal(_T("합판3회"), _T("FormWorkBase")))); 
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::SRCCOLUMN, SubMemberType::SRCCIRCOLUMN, FormWorkPosition::SIDEOUT, LocalDic::GetLocal(_T("합판3회"), _T("FormWorkBase")))); 	
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::SRCCOLUMN, SubMemberType::SRCCIRCOLUMN, FormWorkPosition::SIDEIN, _T(""))); 

	//SRC Beam
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::SRCBEAM, SubMemberType::SRCBEAM, FormWorkPosition::SIDEOUT, LocalDic::GetLocal(_T("합판3회"), _T("FormWorkBase")))); 	
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::SRCBEAM, SubMemberType::SRCBEAM, FormWorkPosition::SIDEIN, LocalDic::GetLocal(_T("합판3회"), _T("FormWorkBase"))));
	AllFormWorkSettingData.push_back(new FormWorkSettingData(SuperMemberType::SRCBEAM, SubMemberType::SRCBEAM, FormWorkPosition::BOTTOM, LocalDic::GetLocal(_T("합판3회"), _T("FormWorkBase"))));

}