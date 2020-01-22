#include "Main.h"
#include <crtdbg.h>	//_ASSERT_EXPR()�ŕK�v.

#include "..\DebugMode\Sphere\Sphere.h"
#include "..\.\XAudio2\PlaySEThread.h"
#include "..\XAudio2\SoundManager.h"

#include "..\Resource\TutorialText\TutorialTextLoad.h"

#include "..\Resource\MeshResource\MeshResource.h"
#include "..\Resource\EffectResource\EffectResource.h"
#include "..\Edit\ModelResource\ModelResource.h"
#include "..\UI\LoadImage\LoadImage.h"

#include "..\Command\Command.h"

#include "..\ImGui\imgui.h"
#include "..\ImGui\imgui_impl_dx11.h"
#include "..\ImGui\imgui_impl_win32.h"

//#pragma optimize("", off)

// ImGui�Ŏg�p.
extern LRESULT ImGui_ImplWin32_WndProcHandler( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

//��۰��ٕϐ�.
CMain*	g_pCMain = nullptr;

//================================================
//	Ҳ݊֐�.
//================================================
INT WINAPI WinMain(
	HINSTANCE hInstance,	//�ݽ�ݽ�ԍ�(����޳�̔ԍ�).
	HINSTANCE hPrevInstance,
	PSTR lpCmdLine,
	INT nCmdShow)
{
#ifdef _DEBUG
	// ���������[�N���o
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif	// #ifdef _DEBUG.

	g_pCMain = new CMain();	//���������׽�錾.

	if (g_pCMain != nullptr) {

		//����޳�쐬����������.
		if (SUCCEEDED(
			g_pCMain->InitWindow(
				hInstance,
				0, 0,
				WND_W, WND_H,
				WND_TITLE)))
		{
			//Dx11�p�̏�����.
			if (SUCCEEDED(g_pCMain->Init()))
			{
				//ү����ٰ��.
				g_pCMain->Loop();
			}
		}
		//�I��.
		g_pCMain->Destroy();//Direct3D�̉��.

		SAFE_DELETE(g_pCMain);//�׽�̔j��.
	}

	return 0;
}

//================================================
//	����޳��ۼ��ެ�.
//================================================
LRESULT CALLBACK WndProc(
	HWND hWnd, UINT uMsg,
	WPARAM wPara, LPARAM lParam)
{
	//��ۼ��ެ�.
	return g_pCMain->MsgProc(hWnd, uMsg, wPara, lParam);
}

/**********************************************************************
*	���C���N���X.
**/
//=================================================
//	�R���e�L�X�g.
//=================================================
CMain::CMain()
	//���������X�g.
	: m_hWnd					( nullptr )
	, m_pDevice11				( nullptr )
	, m_pContext11				( nullptr )
	, m_pSwapChain				( nullptr )
	, m_pBackBuffer_TexRTV		( nullptr )
	, m_pBackBuffer_DSTex		( nullptr )
	, m_pBackBuffer_DSTexDSV	( nullptr )
	, m_pDepthStencilState		( nullptr )
	, m_pDepthStencilStateOff	( nullptr )
	, m_vLight					( 0.0f, 0.5f, -1.0f )	// ���C�g����.
	, pSceneManager				( nullptr )
	, m_isLoadCheck				( false )
{
	pSceneManager = make_shared<clsSceneManager>();
	pSceneManager->Push( make_shared<clsTitleScene>(pSceneManager));
	m_Light = Light();
}


//=================================================
//	�f�X�g���N�^.
//=================================================
CMain::~CMain()
{
}

//����޳�������֐�.
HRESULT CMain::InitWindow(
	HINSTANCE hInstance,	//�ݽ�ݽ.
	INT x, INT y,			//����޳x,y���W.
	INT width, INT height,	//����޳��,����.
	const char* WindowName)	//����޳��.
{
	//����޳�̒�`.
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(wc));//������(0��ݒ�).

	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wc.lpszClassName = APP_NAME;
	wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

	//����޳�׽��Windows�ɓo�^.
	if (!RegisterClassEx(&wc)) {
//		MessageBox(nullptr,
//			"����޳�׽�̓o�^�Ɏ��s", "�װ", MB_OK);
		_ASSERT_EXPR(false, L"����޳�׽�̓o�^�Ɏ��s");
		return E_FAIL;
	}

	//����޳�̍쐬.
	m_hWnd = CreateWindow(
		APP_NAME,		//���ؖ�.
		WindowName,		//����޳����.
		WS_OVERLAPPEDWINDOW,	//����޳���(����).
		x, y,			//�\���ʒux,y���W.
		width, height,	//����޳��,����.
		nullptr,		//�e����޳�����.
		nullptr,		//�ƭ��ݒ�.
		hInstance,		//�ݽ�ݽ�ԍ�.
		nullptr);		//����޳�쐬���ɔ����������Ăɓn���ް�.
	if (!m_hWnd) {
		_ASSERT_EXPR(false, "����޳�쐬���s");
		return E_FAIL;
	}

	RECT size;
	RECT wndsize;
	GetClientRect( m_hWnd, &size );
	GetWindowRect( m_hWnd, &wndsize );
	wndsize.right = wndsize.right - wndsize.left;
	wndsize.bottom = wndsize.bottom - wndsize.top;
	SetWindowPos(
		m_hWnd,
		nullptr,
		0, 0,
		width + wndsize.right - size.right,
		height + wndsize.bottom - size.bottom,
		SWP_NOMOVE | SWP_NOREPOSITION | SWP_NOZORDER );

	//����޳�̕\��.
	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	return S_OK;
}


//����޳�֐�(ү���ޖ��̏���).
LRESULT CMain::MsgProc(
	HWND hWnd, UINT uMsg,
	WPARAM wParam, LPARAM lParam)
{
	if( ImGui_ImplWin32_WndProcHandler( hWnd, uMsg, wParam, lParam ) )
		return true;
	IMGUI_API
	switch (uMsg) {
	case WM_DESTROY://����޳���j�����ꂽ�Ƃ�.
		//���ع���݂̏I����Windows�ɒʒm����.
		PostQuitMessage(0);
		break;
	
	case WM_KEYDOWN://���ް�ނ������ꂽ�Ƃ�.
		//���ʂ̏���.
		switch (static_cast<char>(wParam)) {
		case VK_ESCAPE:	//ESC��.
			if (MessageBox(nullptr,
				"�ްт��I�����܂����H", "�x��", MB_YESNO) == IDYES)
			{
				//����޳��j������.
				PostMessage( hWnd, WM_CLOSE, wParam, lParam );
			}
			break;
		}
		break;
	case WM_CLOSE:
		//�~��Esc�ŃA�v���E�B���h�E������Ƃ��̂��߂�BGM,SE�I������.
		CSoundManager::SetEmergencyStopFlag(true);
		//SE�X���b�h�����ׂă����[�X�����܂�while���[�v�ő҂�.
		while (CPlaySEThread::Release() == false) {};
		//�T�E���h�}�l�[�W���[�����[�X.
		CSoundManager::Release();
		//����޳��j������.
		DestroyWindow(hWnd);
		break;
	}

	//Ҳ݂ɕԂ����.
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


//ү����ٰ��.
void CMain::Loop()
{
	//ү���ǂݍ��݊֐�.
	LoadMesh();

	//------------------------------------------------
	//	�ڰ�ڰĒ�������.
	//------------------------------------------------
	float Rate = 0.0f;	//ڰ�.
	float FPS = 30.0f;	//FPS�l.
	DWORD sync_old = timeGetTime();	//�ߋ�����.
	DWORD sync_now;					//���ݎ���.

	//���ԏ����̂��߁A�ŏ��P�ʂ�1�ؕb�ɕύX.
	timeBeginPeriod(1);
	Rate = 1000.0f / FPS; //���z���Ԃ��Z�o.

	//ү����ٰ��.
	MSG msg = { 0 };
	ZeroMemory(&msg, sizeof(msg));

	while (msg.message != WM_QUIT)
	{
		Sleep(1);
		sync_now = timeGetTime();	//���݂̎��Ԃ��擾.

		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if( sync_now - sync_old >= Rate)
		{
			sync_old = sync_now;	//���ݎ��Ԃɒu������.

			//���ع���݂̏����͂���������.
			AppMain();
		}
	}
	//���ع���݂̏I��.
}

//���ع����Ҳݏ���.
void CMain::AppMain()
{
	m_isLoadCheck = false;
	if( CMeshResorce::ThreadRelease() == false		||
		CMapLoad ::ThreadRelease() == false			||
		CSpriteResource::ThreadRelease() == false	||
		CEffectResource ::ThreadRelease() == false	||
		CEditModelResource::ThreadRelease() == false ){
		m_isLoadCheck = true;
	}

	// �X�V.
	Update();
	// �`��.
	Render();
}

// �X�V�֐�.
void CMain::Update()
{
	if( m_isLoadCheck == true ) return;
	CCommand::Update();
	CXInput::UpdateStatus();
	// �e�V�[���̍X�V����.
	pSceneManager->Load( m_hWnd, m_pDevice11, m_pContext11, m_pDevice9 );
	pSceneManager->Update();
#ifdef _DEBUG
	// �J��������.
	CameraControl();
#endif	// #ifdef _DEBUG.
}

// �`��֐�.
void CMain::Render()
{
	ClearBackBuffer();
	Camera();
	Proj();

	if( m_isLoadCheck == true ){
		CLoadImage::Render();
		//�����ݸނ��ꂽ�Ұ�ނ�\��.
		m_pSwapChain->Present( 0, 0 );
		return;
	}

	// �e�V�[���̕`�揈��.
	pSceneManager->Render( m_mView, m_mProj, m_Light, m_Camera );
	CSpriteResource::GetSprite("Fade")->Render();
	CFadeUI::Render();
	//�����ݸނ��ꂽ�Ұ�ނ�\��.
	m_pSwapChain->Present( 0, 0 );
}

HRESULT CMain::Init()
{
	InitDx9();
	InitD3D11();
	//////////////////////////////////////////////
	//ALT + Enter�Ńt���X�N���[���𖳌�������.
	IDXGIFactory* pfac = nullptr;
	//��ō����IDXGISwapChain���g��.
	m_pSwapChain->GetParent( __uuidof( IDXGIFactory ), (void**)&pfac );

	// �]�v�ȋ@�\�𖳌��ɂ���ݒ������B
	pfac->MakeWindowAssociation( m_hWnd, DXGI_MWA_NO_WINDOW_CHANGES
		| DXGI_MWA_NO_ALT_ENTER );

	////////////////////////////////////////////
	return S_OK;
}

/************************************************************
*	��������3D.
*/
//DirectX������.
HRESULT CMain::InitD3D11()
{
	//---------------------------------------------------------
	//	���޲��ƽܯ�����݊֌W.
	//---------------------------------------------------------

	//�ܯ�����ݍ\����.
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));//0�ŏ�����.
	sd.BufferCount = 1;	//�ޯ��ޯ̧�̐�.
	sd.BufferDesc.Width = WND_W;//�ޯ��ޯ̧�̕�.
	sd.BufferDesc.Height = WND_H;//�ޯ��ޯ̧�̍���.
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//̫�ϯ�(32�ޯĶװ).
	sd.BufferDesc.RefreshRate.Numerator = 60;//��گ��ڰ�(����) ��FPS:60.
	sd.BufferDesc.RefreshRate.Denominator = 1;//��گ��ڰ�(���q).
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;//�g����(�\����).
	sd.OutputWindow = m_hWnd;//����޳�����.
	sd.SampleDesc.Count = 1;//�������ق̐�.
	sd.SampleDesc.Quality = 0;//�������ق̸��è.
	sd.Windowed = TRUE;//����޳Ӱ��(�ٽ�ذݎ���FALSE).

	//�쐬�����݂�@�\���ق̗D����w��.
	// (GPU����߰Ă���@�\��Ă̒�`).
	// D3D_FEATURE_LEVEL�񋓌^�̔z��.
	// D3D_FEATURE_LEVEL_11_0:Direct3D 11.0 �� GPU����.
	D3D_FEATURE_LEVEL pFeatureLevels = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL* pFeatureLevel = nullptr;//�z��̗v�f��.

	//���޲��ƽܯ�����݂̍쐬.
	//ʰ�޳��(GPU)���޲��ł̍쐬.
	if (FAILED(
		D3D11CreateDeviceAndSwapChain(
			nullptr,		//���޵�������ւ��߲��.
			D3D_DRIVER_TYPE_HARDWARE,//�쐬�������޲��̎��.
			nullptr,		//��ĳ�� ׽�ײ�ނ���������DLL�������.
			0,				//�L���ɂ��������ڲ԰.
			&pFeatureLevels,//�쐬�����݂�@�\���ق̏������w�肷��z��ւ��߲��.
			1,				//���̗v�f��.
			D3D11_SDK_VERSION,//SDK���ް�ޮ�.
			&sd,			//�ܯ�����݂̏��������Ұ����߲��.
			&m_pSwapChain,	//(out)�����ݸނɎg�p����ܯ������.
			&m_pDevice11,	//(out)�쐬���ꂽ���޲�.
			pFeatureLevel,	//�@�\���ق̔z��ɂ���ŏ��̗v�f��\���߲��.
			&m_pContext11)))//(out)���޲� ��÷��.
	{
		//WARP���޲��̍쐬.
		// D3D_FEATURE_LEVEL_9_1�`D3D_FEATURE_LEVEL_10_1.
		if (FAILED(
			D3D11CreateDeviceAndSwapChain(
				nullptr, D3D_DRIVER_TYPE_WARP, nullptr,
				0, &pFeatureLevels, 1, D3D11_SDK_VERSION,
				&sd, &m_pSwapChain, &m_pDevice11,
				pFeatureLevel, &m_pContext11)))
		{
			//�̧�ݽ���޲��̍쐬.
			// DirectX SDK���ݽİق���Ă��Ȃ��Ǝg���Ȃ�.
			if (FAILED(
				D3D11CreateDeviceAndSwapChain(
					nullptr, D3D_DRIVER_TYPE_REFERENCE, nullptr,
					0, &pFeatureLevels, 1, D3D11_SDK_VERSION,
					&sd, &m_pSwapChain, &m_pDevice11,
					pFeatureLevel, &m_pContext11)))
			{
				MessageBox(nullptr,
					"���޲��ƽܯ�����ݍ쐬���s",
					"Error", MB_OK);
				return E_FAIL;
			}
		}
	}

	//�e��ø����Ƃ���ɕt�т���e���ޭ�(���)���쐬.

	//----------------------------------------------------------
	//	�ޯ��ޯ̧�����F�װ�ޯ̧�ݒ�.
	//----------------------------------------------------------
	//�ޯ��ޯ̧ø������擾(���ɂ���̂ō쐬�ł͂Ȃ�).
	ID3D11Texture2D* pBackBuffer_Tex = nullptr;
	m_pSwapChain->GetBuffer(
		0,
		__uuidof(ID3D11Texture2D),	//__uuidof:���Ɋ֘A�t�����ꂽGUID���擾
									//         Texture2D�̗B��̕��Ƃ��Ĉ���
		(LPVOID*)&pBackBuffer_Tex);	//(out)�ޯ��ޯ̧ø���.

	//����ø����ɑ΂������ް���ޯ��ޭ�(RTV)���쐬.
	m_pDevice11->CreateRenderTargetView(
		pBackBuffer_Tex,
		nullptr,
		&m_pBackBuffer_TexRTV);	//(out)RTV.
	//�ޯ��ޯ̧ø��������.
	SAFE_RELEASE(pBackBuffer_Tex);

	//----------------------------------------------------------
	//	�ޯ��ޯ̧�����F���߽(�[�x)��ݼي֌W.
	//----------------------------------------------------------
	//���߽(�[��or�[�x)��ݼ��ޭ��p��ø������쐬.
	D3D11_TEXTURE2D_DESC	descDepth;
	descDepth.Width = WND_W;	//��.
	descDepth.Height = WND_H;	//����.
	descDepth.MipLevels = 1;	//Я��ϯ������:1.
	descDepth.ArraySize = 1;	//�z��:1.
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;//32�ޯ�̫�ϯ�.
	descDepth.SampleDesc.Count = 1;		//�������ق̐�.
	descDepth.SampleDesc.Quality = 0;	//�������ق̸��è.
	descDepth.Usage = D3D11_USAGE_DEFAULT;//�g�p���@:��̫��.
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;//�[�x(��ݼقƂ��Ďg�p).
	descDepth.CPUAccessFlags = 0;	//CPU����ͱ������Ȃ�.
	descDepth.MiscFlags = 0;		//���̑��̐ݒ�Ȃ�.

	m_pDevice11->CreateTexture2D(
		&descDepth,
		nullptr,
		&m_pBackBuffer_DSTex);	//(out)���߽��ݼٗpø���.

	//����ø����ɑ΂������߽��ݼ��ޭ�(DSV)���쐬.
	m_pDevice11->CreateDepthStencilView(
		m_pBackBuffer_DSTex,
		nullptr,
		&m_pBackBuffer_DSTexDSV);	//(out)DSV.
	//���ް���ޯ��ޭ������߽��ݼ��ޭ����߲��ײ݂ɾ��.
	m_pContext11->OMSetRenderTargets(
		1,
		&m_pBackBuffer_TexRTV,
		m_pBackBuffer_DSTexDSV);
//
	// �[�x�e�X�g(z�e�X�g)��L���ɂ���.
	D3D11_DEPTH_STENCIL_DESC dsDesc = D3D11_DEPTH_STENCIL_DESC();
	
	dsDesc.DepthEnable		= TRUE;	// �L��.
	dsDesc.DepthWriteMask	= D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc		= D3D11_COMPARISON_LESS;
	dsDesc.StencilEnable	= FALSE;
	dsDesc.StencilReadMask	= D3D11_DEFAULT_STENCIL_READ_MASK;
	dsDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	// �[�x�ݒ�쐬.
	if( FAILED( m_pDevice11->CreateDepthStencilState(
		&dsDesc, &m_pDepthStencilState )))
	{
		_ASSERT_EXPR( false, L"�[�x�ݒ�쐬���s" );
		return E_FAIL;
	}
	// �[�x�e�X�g(z�e�X�g)�𖳌��ɂ���.
	dsDesc.DepthEnable = FALSE;	// ����.
	// �[�x�ݒ�쐬.
	if( FAILED( m_pDevice11->CreateDepthStencilState(
		&dsDesc, &m_pDepthStencilStateOff )))
	{
		_ASSERT_EXPR( false, L"�[�x�ݒ�쐬���s" );
		return E_FAIL;
	}

	//------------------------------------------------
	//	�ޭ��߰Đݒ�.
	//------------------------------------------------
	D3D11_VIEWPORT vp;
	vp.Width = WND_W;	//��.
	vp.Height = WND_H;	//����.
	vp.MinDepth = 0.0f;	//�ŏ��[�x(��O).
	vp.MaxDepth = 1.0f;	//�ő�[�x(��).
	vp.TopLeftX = 0.0f;	//����ʒux.
	vp.TopLeftY = 0.0f;	//����ʒuy.

	m_pContext11->RSSetViewports(1, &vp);


	//------------------------------------------------
	//	׽�ײ��(�ʂ̓h��Ԃ���)�̐ݒ�.
	//------------------------------------------------
	D3D11_RASTERIZER_DESC rdc;
	ZeroMemory(&rdc, sizeof(rdc));
	rdc.FillMode = D3D11_FILL_SOLID;	//�h��Ԃ�(�د��).
	rdc.CullMode = D3D11_CULL_BACK;		//D3D11_CULL_BACK:�w�ʂ�`�悵�Ȃ�.
										//D3D11_CULL_FRONT:���ʂ�`�悵�Ȃ�.
										//D3D11_CULL_NONE:��ݸނ�؂�(���w�ʂ�`�悷��).
	rdc.FrontCounterClockwise = FALSE;//��غ�݂̕\�������肷���׸�.
									//TRUE:�����Ȃ�O�����B�E���Ȃ�������B
									//FALSE:�t�ɂȂ�.
	rdc.DepthClipEnable = FALSE;	//�����ɂ��Ă̸د��ݸޗL��.

	ID3D11RasterizerState* pRs = nullptr;
	m_pDevice11->CreateRasterizerState(&rdc, &pRs);
	m_pContext11->RSSetState(pRs);
	SAFE_RELEASE(pRs);

	return S_OK;
}

HRESULT CMain::InitDx9()
{
	LPDIRECT3D9 m_pD3d9; //Dx9��޼ު��.

	//�uDirect3D�v��޼ު�Ă̍쐬.
	m_pD3d9 = Direct3DCreate9( D3D_SDK_VERSION );
	if( m_pD3d9 == nullptr ){
		_ASSERT_EXPR( false, L"Dx9��޼ު�č쐬���s" );
		return E_FAIL;
	}

	// Diret3D���޲���޼ު�Ă̍쐬.
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory( &d3dpp, sizeof( d3dpp ) );
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;	//�ޯ��ޯ̧��̫�ϯ�(��̫��).
	d3dpp.BackBufferCount = 1;					//�ޯ��ޯ̧�̐�.
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	//�ܯ�ߴ̪��(��̫��).
	d3dpp.Windowed = true;						//����޳Ӱ��.
	d3dpp.EnableAutoDepthStencil = true;		//��ݼٗL��.
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;	//��ݼق�̫�ϯ�(16bit).

	//���޲��쐬(HALӰ��:�`��ƒ��_������GPU�ōs��).
	if( FAILED( m_pD3d9->CreateDevice(
		D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp, &m_pDevice9 ) ) ){
		//���޲��쐬(HALӰ��:�`���GPU�A���_������CPU�ōs��).
		if( FAILED( m_pD3d9->CreateDevice(
			D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, m_hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp, &m_pDevice9 ) ) ){
			MessageBox( nullptr,
				"HALӰ�ނ����޲��쐬�ł��܂���\nREFӰ�ނōĎ��s���܂�",
				"�x��", MB_OK );

			//���޲��쐬(REFӰ��:�`���CPU�A���_������GPU�ōs��).
			if( FAILED( m_pD3d9->CreateDevice(
				D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, m_hWnd,
				D3DCREATE_HARDWARE_VERTEXPROCESSING,
				&d3dpp, &m_pDevice9 ) ) ){
				//���޲��쐬(REFӰ��:�`��ƒ��_������CPU�ōs��).
				if( FAILED( m_pD3d9->CreateDevice(
					D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, m_hWnd,
					D3DCREATE_SOFTWARE_VERTEXPROCESSING,
					&d3dpp, &m_pDevice9 ) ) ){
					MessageBox( nullptr,
						"Direct3D���޲��쐬���s", "�x��", MB_OK );
					return E_FAIL;
				}
			}
		}
	}

	SAFE_RELEASE( m_pD3d9 );

	return S_OK;
}

//Direct3D�I������.
void CMain::Destroy()
{
	pSceneManager->StackRelease();

	//Direct3D��޼ު�Ă����.
	SAFE_RELEASE( m_pDepthStencilStateOff );
	SAFE_RELEASE( m_pDepthStencilState );
	SAFE_RELEASE( m_pBackBuffer_DSTexDSV );
	SAFE_RELEASE( m_pBackBuffer_DSTex );
	SAFE_RELEASE( m_pBackBuffer_TexRTV );
	SAFE_RELEASE( m_pSwapChain );
	SAFE_RELEASE( m_pContext11 );
	SAFE_RELEASE( m_pDevice11 );
	SAFE_RELEASE( m_pDevice9 );

	m_hWnd = nullptr;
}

//ү���̓ǂݍ��݊֐�.
HRESULT CMain::LoadMesh()
{
	CLoadImage::Load( m_pDevice11, m_pContext11 );

	// �X�v���C�g�̓ǂݍ���.
	CSpriteResource::Load( m_pDevice11, m_pContext11 );
	// �X�e�[�W�����X�g�̎擾.
	CMapLoad::StageNameListLoad( m_pDevice11, m_pContext11 );
	// ���b�V���ǂݍ���.
	CMeshResorce::Load( m_hWnd, m_pDevice11, m_pContext11, m_pDevice9 );
	// �G�t�F�N�g�ǂݍ���.
	CEffectResource::Load( m_pDevice11, m_pContext11 );

	CEditModelResource::Load( m_hWnd, m_pDevice11, m_pContext11, m_pDevice9 );

	CDebugText::Init( m_pContext11, 40.0f, D3DXVECTOR4( 1.0f, 1.0f, 1.0f, 1.0f ) );
	CTutorialTextLoad::TextDataLoad();	//�`���[�g���A���e�L�X�g���[�h.

	m_Camera.Pos	= D3DXVECTOR3( 0.0f, 4.0f, -10.0f );
	m_Camera.vLook	= D3DXVECTOR3( 0.0f, -2.0f, 10.0f );

	return S_OK;
}

// �o�b�N�o�b�t�@�N���A�֐�.
void CMain::ClearBackBuffer()
{
	//��ʂ̸ر.
	float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };	//�ر�F(RGBA�̏�).
	//�װ�ޯ��ޯ̧.
	m_pContext11->ClearRenderTargetView(
		m_pBackBuffer_TexRTV, ClearColor );
	//���߽��ݼ��ޯ��ޯ̧.
	m_pContext11->ClearDepthStencilView(
		m_pBackBuffer_DSTexDSV,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f, 0 );
}

// �J�����֐�.
void CMain::Camera()
{
	//�ޭ�(���)�ϊ�.
	D3DXVECTOR3	vUpVec( 0.0f, 1.0f, 0.0f );	//���(�޸��).
	D3DXMatrixLookAtLH(
		&m_mView,	//(out)�ޭ��v�Z����.
		&m_Camera.Pos, &m_Camera.vLook, &vUpVec );
}

// �v���W�F�N�V�����֐�.
void CMain::Proj()
{
	//��ۼު����(�ˉe)�ϊ�.
	D3DXMatrixPerspectiveFovLH(
		&m_mProj,	//(out)��ۼު���݌v�Z����.
		static_cast<FLOAT>( D3DX_PI / 4.0f ),	//y�����̎���(׼ޱݎw��)���l��傫�������王�삪�����Ȃ�.
		static_cast<FLOAT>( WND_W ) / static_cast<FLOAT>( WND_H ),//���߸Ĕ�(��������).
		0.1f,		//�߂��ޭ����ʂ�z�l.
		200.0f );	//�����ޭ����ʂ�z�l.
}


// �[�x(Z)�e�X�gON/OFF�ؑւ�.
void CMain::SetDeprh( bool flag )
{
	ID3D11DepthStencilState* pTmp
		= ( flag == true ) ? m_pDepthStencilState : m_pDepthStencilStateOff;
	
	// �[�x�ݒ���Z�b�g.
	m_pContext11->OMSetDepthStencilState( pTmp, 1 );
}

// �J�����R���g���[��.
void CMain::CameraControl()
{
	//// �����ʒu�̈ړ�.
	//static bool isCamera = false;
	//if( isCamera == true ){
	//	if( GetAsyncKeyState( 'W' ) & 0x8000 )		m_Camera.vLook.y += 0.1f;
	//	if( GetAsyncKeyState( 'S' ) & 0x8000 )		m_Camera.vLook.y -= 0.1f;
	//	if( GetAsyncKeyState( 'D' ) & 0x8000 )		m_Camera.vLook.x += 0.1f;
	//	if( GetAsyncKeyState( 'A' ) & 0x8000 )		m_Camera.vLook.x -= 0.1f;
	//	if( GetAsyncKeyState( 'Q' ) & 0x8000 )		m_Camera.vLook.z += 0.1f;
	//	if( GetAsyncKeyState( 'E' ) & 0x8000 )		m_Camera.vLook.z -= 0.1f;
	//}
	//if( GetAsyncKeyState( 'C' ) & 0x8000 )	isCamera = !isCamera;
	//

	//// �J�����ʒu�ړ�.
	//if( GetAsyncKeyState( VK_LEFT ) & 0x8000 )	m_Camera.Pos.x -= 0.1f;
	//if( GetAsyncKeyState( VK_RIGHT ) & 0x8000 )	m_Camera.Pos.x += 0.1f;
	//if( GetAsyncKeyState( VK_UP ) & 0x8000 )	m_Camera.Pos.y += 0.1f;
	//if( GetAsyncKeyState( VK_DOWN ) & 0x8000 )	m_Camera.Pos.y -= 0.1f;
	//if( GetAsyncKeyState( VK_F1 ) & 0x8000 )	m_Camera.Pos.z += 0.1f;
	//if( GetAsyncKeyState( VK_F2 ) & 0x8000 )	m_Camera.Pos.z -= 0.1f;
}