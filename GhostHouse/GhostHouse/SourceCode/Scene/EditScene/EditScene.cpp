#include "EditScene.h"
#include "..\TItleScene\TitleScene.h"
#include "..\..\UI\LoadImage\LoadImage.h"

#include "..\..\Edit\ModelResource\ModelResource.h"
#include "..\..\UI\Fade\FadeUI.h"
#include "..\..\ImGui\imgui.h"
#include "..\..\ImGui\imgui_impl_dx11.h"
#include "..\..\ImGui\imgui_impl_win32.h"

CEditScene::CEditScene( shared_ptr<CSceneManager> sceneManager )
	: CSceneBase		( sceneManager )
	, m_pEdit			( make_unique<CEdit>() )
	, m_isImGuiCreate	( false )
	, m_isLightSet		( false )
{
}

CEditScene::~CEditScene()
{
	// ImGui�̉��.
	ImGui_ImplWin32_Shutdown();
	ImGui_ImplDX11_Shutdown();
	ImGui::DestroyContext();
}

//-----------------------------------.
//	�Ǎ������֐�.
//-----------------------------------.
void CEditScene::Load( 
	HWND hWnd, 
	ID3D11Device* pDevice11,
	ID3D11DeviceContext* pContext11,
	LPDIRECT3DDEVICE9 pDevice9 )
{
	// �G�f�B�b�g�N���X�̓ǂݍ���.
	m_pEdit->Load( pDevice11, pContext11 );
	// ImGui�̍쐬.
	ImGuiCreate( hWnd, pDevice11, pContext11 );
}

//-----------------------------------.
//	�X�V�����֐�.
//-----------------------------------.
void CEditScene::Update()
{
	// ImGui���쐬���ĂȂ���ΏI��.
	if( m_isImGuiCreate == false ) return;
	// �G�f�B�b�g�̍X�V.
	m_pEdit->Updete();
	// �V�[���؂�ւ��֐�.
	ChangeScene();
}

//-----------------------------------.
//	�`�揈���֐�.
//-----------------------------------.
void CEditScene::Render( 
	D3DXMATRIX& mView, 
	D3DXMATRIX& mProj,
	Light& stLight, 
	stCAMERA& stCamera )
{
	// ImGui���쐬���ĂȂ���ΏI��.
	if( m_isImGuiCreate == false ) return;
	// ���C�g�̐ݒ�.
	SetingLight( stLight );
	// �G�f�B�b�g�̕`��.
	m_pEdit->Render( mView,  mProj, stLight, stCamera );
}

//-----------------------------------.
//	ImGui�̍쐬�֐�.
//-----------------------------------.
void CEditScene::ImGuiCreate(
	HWND hWnd,
	ID3D11Device* pDevice11,
	ID3D11DeviceContext* pContext11 )
{
	// ����ImGui���쐬����Ă�����I��.
	if( m_isImGuiCreate == true ) return;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init( hWnd );
	ImGui_ImplDX11_Init( pDevice11, pContext11 );
	ImGui::StyleColorsDark();

	CFadeUI::FadeINStart();

	m_isImGuiCreate = true;
}

//-----------------------------------.
//	���C�g�̐ݒ�.
//-----------------------------------.
void CEditScene::SetingLight( Light& stLight )
{
	// ���Ƀ��C�g���ݒ肳��Ă�����I��.
	if( m_isLightSet == true ) return;

	CLoadImage::Render();
	stLight.dir		= { 0.0f, -1.0f, 0.0f };
	stLight.range	= 1000.0f;
	stLight.cone	= 1.0f;
	stLight.ambient = { 1.0f, 1.0f, 1.0f, 1.0f };
	stLight.diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	stLight.pos		= { 0.0f, 100.0f, 0.0f };

	m_isLightSet	= true;
}
//-----------------------------------.
//	�V�[���؂�ւ��֐�.
//-----------------------------------.
void CEditScene::ChangeScene()
{
	// �G�f�B�b�g�N���X�̏I���t���O������Ă�����I��.
	if( m_pEdit->IsEditEnd() == false ) return;
	// �V�[���̐؂�ւ�.
	m_pSceneManager->Change( make_shared<CTitleScene>( m_pSceneManager ) );
}