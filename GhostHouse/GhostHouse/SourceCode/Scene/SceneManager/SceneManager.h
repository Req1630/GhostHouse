#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "..\SceneBase\SceneBase.h"

using namespace std;

struct Light;
struct stCAMERA;

class CSceneBase;

class CSceneManager
{
public:
	CSceneManager();
	~CSceneManager();
	
	// �ǂݍ��݊֐�.
	void Load( HWND hWnd, ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11, LPDIRECT3DDEVICE9 pDevice9 );
	void LoadRender( D3DXMATRIX& mView, D3DXMATRIX& mProj,
		Light& stLight, stCAMERA& stCamera );
	// �X�V�֐�.
	void Update();
	// �`��֐�.
	void Render( D3DXMATRIX& mView, D3DXMATRIX& mProj,
		Light& stLight, stCAMERA& stCamera );

	// �V�[�����X�^�b�N�ɓ����.
	void Push( shared_ptr<CSceneBase> pScenebase );
	// �V�[�����X�^�b�N������o��.
	void Pop();
	// �V�[�����X�^�b�N�������o��.
	void TwoPop();
	// �V�[�����X�^�b�N�̈�ԏ�Ɠ���ւ���.
	void Change( shared_ptr<CSceneBase> pScenebase );
	// �V�[���X�^�b�N����ꂩ����.
	void Swap( shared_ptr<CSceneBase> pScenebase );
	// �V�[���X�^�b�N�̉��.
	void StackRelease();

private:
	stack<shared_ptr<CSceneBase>> m_pStackScene;
};

#endif // #ifndef SCENE_MANAGER_H.