#include "EditScene.h"
#include "..\TItleScene\TitleScene.h"
#include "..\..\UI\LoadImage\LoadImage.h"

#include "..\..\Edit\ModelResource\ModelResource.h"
#include "..\..\UI\Fade\FadeUI.h"
#include "..\..\ImGui\imgui.h"
#include "..\..\ImGui\imgui_impl_dx11.h"
#include "..\..\ImGui\imgui_impl_win32.h"

#define _CRT_SECURE_NO_WARNINGS

CEditScene::CEditScene( shared_ptr<CSceneManager> sceneManager )
	: CSceneBase		( sceneManager )
	, m_pEdit			( make_unique<CEdit>() )
	, m_isImGuiCreate	( false )
	, m_isEnd			( false )
	, m_isLightSet		( false )
{
}

CEditScene::~CEditScene()
{
	ImGui_ImplWin32_Shutdown();
	ImGui_ImplDX11_Shutdown();
	ImGui::DestroyContext();
}

void CEditScene::Load( 
	HWND hWnd, 
	ID3D11Device* pDevice11,
	ID3D11DeviceContext* pContext11,
	LPDIRECT3DDEVICE9 pDevice9 )
{
	m_pEdit->Load( pDevice11, pContext11 );
	if( m_isImGuiCreate == false ){
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		ImGui_ImplWin32_Init( hWnd );
		ImGui_ImplDX11_Init( pDevice11, pContext11 );
		ImGui::StyleColorsDark();

		m_isImGuiCreate = true;
		CFadeUI::FadeINStart();
	}
}

void CEditScene::Update()
{
	if( m_isImGuiCreate == false ) return;

	m_pEdit->Updete();
	if( m_pEdit->IsEditEnd() == true ){
		m_pSceneManager->Change( make_shared<CTitleScene>( m_pSceneManager ) );
	}
}

void CEditScene::Render( 
	D3DXMATRIX& mView, 
	D3DXMATRIX& mProj,
	Light& stLight, 
	stCAMERA& stCamera )
{
	if( m_isImGuiCreate == false ) return;
	if( m_isLightSet == false ){
		CLoadImage::Render();
		stLight.dir = D3DXVECTOR3( 0.0f, -1.0f, 0.0f );
		stLight.range = 1000.0f;
		stLight.cone = 1.0f;
		stLight.ambient = D3DXVECTOR4( 1.0f, 1.0f, 1.0f, 1.0f );
		stLight.diffuse = D3DXVECTOR4( 1.0f, 1.0f, 1.0f, 1.0f );
		stLight.pos.x = 0.0f;
		stLight.pos.y = 100.0f;
		stLight.pos.z = 0.0f;
		m_isLightSet = true;
	}
	m_pEdit->Render( mView,  mProj, stLight, stCamera );
}