#ifndef EDIT_SCENE_H
#define EDIT_SCENE_H

#include "..\SceneBase\SceneBase.h"
#include "..\..\Edit\Edit.h"

class CEditScene : public CSceneBase
{
public:
	CEditScene( shared_ptr<CSceneManager> sceneManager );
	~CEditScene();

	// �Ǎ������֐�.
	void Load( 
		HWND hWnd, 
		ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11,
		LPDIRECT3DDEVICE9 pDevice9 ) override;
	// �X�V�����֐�.
	void Update() override;
	// �`�揈���֐�.
	void Render( 
		D3DXMATRIX& mView, 
		D3DXMATRIX& mProj,
		Light& stLight, 
		stCAMERA& stCamera ) override;
private:
	// ImGui�̍쐬�֐�.
	void ImGuiCreate(
		HWND hWnd, 
		ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11 );
	// ���C�g�̐ݒ�.
	void SetingLight( Light& stLight );
	// �V�[���؂�ւ��֐�.
	void ChangeScene();

private:
	unique_ptr<CEdit> m_pEdit;	// �G�f�B�b�g�N���X.
	bool m_isImGuiCreate;		// ImGui���쐬������.
	bool m_isLightSet;			// ���C�g��ݒ肵����.
};

#endif // #ifndef EDIT_SCENE_H.