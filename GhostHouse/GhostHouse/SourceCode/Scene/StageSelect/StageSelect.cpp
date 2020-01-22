#include "StageSelect.h"
#include "..\..\XAudio2\SoundManager.h"
#include "..\..\XAudio2\PlaySEThread.h"

clsStageSelect::clsStageSelect( shared_ptr<clsSceneManager> &sceneManager )
	: clsSceneBase		( sceneManager )
	, m_bLoadFalag		( false )
	, m_pStageSelectUI	( make_unique<CStageSelectUI>() )
	, m_isNextScene		( false )
	, m_bChangeScene	( false )
	, m_isEnd			( false )
	, ThreadExitCode	( false )
	, m_bThreadRelease	( false )
{
	CSoundManager::StopSEByName("OK");
	CFadeUI::Init();
	auto st = [&]()
	{
		CSoundManager::PlayBGMByName( BGM_NAME, m_isEnd, true );
		m_isEnd = true;
	};
	BGMThread = std::thread( st );
	InThreadID = BGMThread.get_id();
	//BGM���t�F�[�h�C��������ׂɁA�{�����[����0��.
	CSoundManager::SetBGMVolme( BGM_NAME, 0.0f );
}

clsStageSelect::~clsStageSelect()
{
	m_isEnd = true;
	while( Release() == false ){}
}

void clsStageSelect::Load( HWND hWnd, ID3D11Device* pDevice11,
	ID3D11DeviceContext* pContext11, LPDIRECT3DDEVICE9 pDevice9 )
{
	if( m_bLoadFalag == true ) return;

	if( m_pStageSelectUI->Load() == true ){
		CFadeUI::FadeINStart();
		m_bLoadFalag = true;
	}
}

void clsStageSelect::Updata()
{
	CPlaySEThread::Updata();
	// -1��������BGM������ or ������Ă���̂ŃX���[.
	if (CSoundManager::GetBGMVolme(BGM_NAME) != -1) {
		if (CSoundManager::GetBGMVolme(BGM_NAME) == 0.0f && m_isNextScene == true) {
			m_bChangeScene = true;
		}
		if (m_bChangeScene == true) {
			Sleep(10);
			m_bChangeScene = false;
			Release();
			m_pSceneManager->Change(make_shared<clsGameScene>(m_pSceneManager));
			return;
		}

		if (CSoundManager::GetBGMVolme(BGM_NAME) != 1.0f && m_isNextScene == false) {
			CSoundManager::FadeInBGMByName(BGM_NAME);
		}
	}
	if( NextScene() == true ) return;
	if( CFadeUI::isActive() == true ) return;

	// �X�e�B�b�N�����.
	if( CXInput::GetLThumbY() > 32500 ||
		( GetAsyncKeyState( VK_UP ) & 0x0001 ) ){
		m_pStageSelectUI->MoveUpStage();
	}
	// �X�e�B�b�N������.
	if( CXInput::GetLThumbY() < -32500 || 
		( GetAsyncKeyState( VK_DOWN ) & 0x0001 ) ){
		m_pStageSelectUI->MoveDownStage();
	}
	// R�{�^������.
	if(( CXInput::IsPress( XINPUT_GAMEPAD_B )) ||
		( GetAsyncKeyState( VK_RETURN ) & 0x0001 )){
		CPlaySEThread::SetSEName( "OK" );
		CSoundManager::FadeOutBGMByName( BGM_NAME );
		CFadeUI::FadeOUTStart();
		m_isNextScene = true;
	}

}

void clsStageSelect::Render( D3DXMATRIX& mView, D3DXMATRIX& mProj,
	Light& stLight, stCAMERA& stCamera )
{
	// �X�e�[�W�Z���N�gUI�̕\��.
	m_pStageSelectUI->Render();
}

bool clsStageSelect::NextScene()
{
	if( CFadeUI::isEnd() == false ) return false;
	if( m_isNextScene == false ) return false;

	if( m_pStageSelectUI->SetStageName() == true ){
		CFadeUI::Init();
		Release();

		return true;
	}
	return false;
}

bool clsStageSelect::Release()
{
	GetExitCodeThread( BGMThread.native_handle(), &ThreadExitCode );

	if( ThreadExitCode == 0 ){
		if( InThreadID == BGMThread.get_id() ){
			m_isEnd = true;				//�Ȃ��X�g�b�v�����ꍇ�A�X�g�b�v�𖽗߂����R�[�h�̒���
			//CSoundManager::CleanBGM(BGM_NAME);
			//false�̊ԃ��[�v�ő҂����Ă���̂ŁASoundManager��������Ă���
			//true�ŃX���b�h����while���[�v����ł�B
			BGMThread.detach();
			m_bThreadRelease = true;
			return true;
		}
		if( m_bThreadRelease == true ){
			return true;
		}
		return false;
	}
	return false;
}