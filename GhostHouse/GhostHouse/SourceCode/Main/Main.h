#ifndef MAIN_H
#define MAIN_H

#include "MyMacro.h"
#include "Global.h"

// �V�[���N���X.
#include "..\Scene\SceneManager\SceneManager.h"
#include "..\Scene\TItleScene\TitleScene.h"

#include "..\DebugMode\DebugText\DebugText.h"

//=================================================
//	�萔.
//=================================================
const char WND_TITLE[] = "�I�o�P�m�C�G";
const char APP_NAME[]  = "GhostHouse";


/**************************************************
*	���C���N���X.
**/
class CMain
{
public:
	CMain();	// �R���X�g���N�^.
	~CMain();	// �f�X�g���N�^.

	// �E�B���h�E�������֐�.
	HRESULT InitWindow(
		HINSTANCE hInstance,
		INT x, INT y, INT width, INT height,
		const char* WindowName );

	// �E�B���h�E�֐�(���b�Z�[�W���̏���).
	LRESULT MsgProc(
		HWND hWnd, UINT uMsg,
		WPARAM wParam, LPARAM lParam);

	HRESULT Init();
	void Destroy();

	// ���b�Z�[�W���[�v.
	void Loop();
	// �A�v���[�P�[�V��������.
	void AppMain();

//----------------------------
// ��������3D�֌W.
	// DirectX������.
	HRESULT InitD3D11();
	HRESULT InitDx9();

//----------------------------

private:
	HWND	m_hWnd;	// �E�B���h�E�n���h��.

	// �X�V�֐�.
	void Update();
	// �`��֐�.
	void Render();

	// �o�b�N�o�b�t�@�N���A�֐�.
	void ClearBackBuffer();
	// �J�����֐�.
	void Camera();
	// �v���W�F�N�V�����֐�.
	void Proj();
	// �J�����R���g���[��.
	void CameraControl();

//----------------------------
// ��������3D�֌W.
	//======================================
	//	�\����.
	//======================================
	// �J�������.
	

	// ���b�V���̓ǂݍ��݊֐�.
	HRESULT LoadMesh();

	// �[�x(Z)�e�X�gON/OFF�ؑւ�.
	void SetDeprh( bool flag );

	//���A�v���ɂЂƂ�.
	ID3D11Device*			m_pDevice11;	// �f�o�C�X�I�u�W�F�N�g.
	ID3D11DeviceContext*	m_pContext11;	// �f�o�C�X�R���e�L�X�g.
	LPDIRECT3DDEVICE9		m_pDevice9;		// Dx9���޲���޼ު��.
	IDXGISwapChain*			m_pSwapChain;	// �X���b�v�`�F�[��.
	ID3D11RenderTargetView*	m_pBackBuffer_TexRTV;	// �����_�[�^�[�Q�b�g�r���[.
	ID3D11Texture2D*		m_pBackBuffer_DSTex;	// �f�v�X�X�e���V���p�e�N�X�`��.
	ID3D11DepthStencilView*	m_pBackBuffer_DSTexDSV;	// �f�v�X�X�e���V���r���[.
	
	// �[�x(z)�e�X�g�ݒ�.
	ID3D11DepthStencilState*	m_pDepthStencilState;
	ID3D11DepthStencilState*	m_pDepthStencilStateOff;

	stCAMERA		m_Camera;	// �J�����ʒu.
	D3DXVECTOR3		m_vLight;	// ���C�g�̕���.
	D3DXMATRIX		m_mView;	// �r���[�s��.
	D3DXMATRIX		m_mProj;	// �v���W�F�N�V�����s��.

	Light			m_Light;
	
//----------------------------

	// �V�[���Ǘ��N���X.
	shared_ptr<CSceneManager> pSceneManager;
	bool	m_isLoadCheck;
};

#endif//#ifndef MAIN_H
