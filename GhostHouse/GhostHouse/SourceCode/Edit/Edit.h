#ifndef EDIT_H
#define EDIT_H

#include "EditHeader.h"
#include "EditState/EachItem/EachItem.h"
#include "EditState/EachObject/EachObject.h"
#include "EditState/ObjectArrangement/ObjectArrangement.h"

#include "SelectMenu/SelectMenu.h"

#include "FileManager/FileManager.h"
#include "EditImGui/EditImGui.h"
#include "Player/Player.h"
#include "Ground/Ground.h"

class CEdit
{
public:
	CEdit();
	~CEdit();

	// �e�I�u�W�F�N�g�̓ǂݍ���.
	void Load( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 );
	// �X�V����.
	void Updete();
	// �`�揈��.
	void Render( D3DXMATRIX& mView, D3DXMATRIX& mProj,
		Light& stLight, stCAMERA& stCamera );

	// �I�u�W�F�N�g��u������.
	void PutObject();
	// �I�u�W�F�N�g����������.
	void DeleteObject();

	// �G�f�B�b�g�I������.
	bool IsEditEnd(){ return m_isEditEnd; }

private:
	// ���쏈��.
	void Control();

	// ���X�e�[�W�̕`��.
	void TmpStageRender( 
		D3DXMATRIX& mView,
		D3DXMATRIX& mProj,
		Light& stLight,
		stCAMERA& stCamera );

	// �I���I�u�W�F�N�g�̍X�V.
	void ObjArrangementUpdate();
	// �I���I�u�W�F�N�g�̕`��.
	void ObjArrangementRender( 
		D3DXMATRIX& mView, 
		D3DXMATRIX& mProj,
		Light& stLight, 
		stCAMERA& stCamera );


private:
	std::vector<edit::RENDER_OBJRCT>	m_pTmpStage;	// ���X�e�[�W.

	unique_ptr<CObjectArrangement>	m_pObjArrangement;
	unique_ptr<CSelectMenu>			m_pSelectMenu;
	unique_ptr<CEditImGui>			m_pEditImGui;
	shared_ptr<CPlayer>				m_pPlayer;
	shared_ptr<CGround>				m_pGround;

	bool m_isEditEnd;
	edit::RENDER_OBJRCT m_TmpObj;
};

#endif	// #ifndef EDIT_H.