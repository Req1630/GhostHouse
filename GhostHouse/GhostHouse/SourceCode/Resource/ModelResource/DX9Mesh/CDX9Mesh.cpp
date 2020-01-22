#include "CDX9Mesh.h"
#include <crtdbg.h>	// _ASSERT�}�N���Ŏg�p.

const char SHADER_NAME[] = "Data\\Shader\\SpotLight.hlsl";

// �R���X�g���N�^.
CDX9Mesh::CDX9Mesh()
	: m_hWnd				( nullptr )
	, m_pDevice9			( nullptr )
	, m_pDevice11			( nullptr )
	, m_pContext11			( nullptr )
	, m_pVertexShader		( nullptr )
	, m_pVertexLayout		( nullptr )
	, m_pPixelShader		( nullptr )
	, m_pCBufferPerMesh		( nullptr )
	, m_pCBufferPerMaterial	( nullptr )
	, m_pCBufferPerFrame	( nullptr )
	, m_pVertexBuffer		( nullptr )
	, m_ppIndexBuffer		( nullptr )
	, m_pSampleLinear		( nullptr )

	, m_pAlphaBlend			( nullptr )
	, m_pNoAlphaBlend		( nullptr )
	, m_CoverageBlend		( nullptr )
	, m_RasterizerWireON	( nullptr )
	, m_RasterizerWireOFF	( nullptr )

	, m_pMaterials			( nullptr )
	, m_pMesh				( nullptr )
	, m_pMeshForRay			( nullptr )
	, m_EnableTexture		( false )
	, m_vPos				( 0.0f, 0.0f, 0.0f )
	, m_vRot				( 0.0f, 0.0f, 0.0f )
	, m_fScale				( 1.0f )
	, m_NumMaterials		(0)
	, m_NumAttr				(0)
	, m_AttrID				()
	, m_fAlpha				( 1.0f )
{
}

CDX9Mesh::CDX9Mesh(
	HWND hWnd,
	ID3D11Device* pDevice11,
	ID3D11DeviceContext* pContext11,
	LPDIRECT3DDEVICE9 pDevice9,
	const char* fileName )
	: CDX9Mesh()
{
	Init( hWnd,  pDevice11, pContext11, pDevice9, fileName );
}
// �f�X�g���N�^.
CDX9Mesh::~CDX9Mesh()
{
	// �������.
	Release();
}

HRESULT CDX9Mesh::Init(
	HWND hWnd,
	ID3D11Device* pDevice11,
	ID3D11DeviceContext* pContext11,
	LPDIRECT3DDEVICE9 pDevice9,
	const char* fileName )
{
	m_hWnd			= hWnd;
	m_pDevice11		= pDevice11;
	m_pContext11	= pContext11;
	m_pDevice9		= pDevice9;

	if( FAILED( LoadXMesh( fileName ))){
		return E_FAIL;
	}
	if( FAILED( InitShader() )){
		return E_FAIL;
	}
	if( FAILED( InitBlend() )){
		return E_FAIL;
	}
	if( FAILED( InitWtire() )){
		return E_FAIL;
	}
	if( FAILED( InitCullBack() ) ){
		return E_FAIL;
	}
	
	return S_OK;
}

// ���b�V���ǂݍ���.
HRESULT CDX9Mesh::LoadXMesh( const char* filename )
{

	// �}�e���A���o�b�t�@.
	LPD3DXBUFFER pD3DXMtrlBuffer = nullptr;

	// X�t�@�C���̃��[�h.
	if ( FAILED( D3DXLoadMeshFromXA(
		filename,			// �t�@�C����.
		D3DXMESH_SYSTEMMEM	// �V�X�e���������ɓǂݍ���.
		| D3DXMESH_32BIT,	
		m_pDevice9,			// 32bit.			
		nullptr,
		&pD3DXMtrlBuffer,	// (out)�}�e���A�����.
		nullptr,
		&m_NumMaterials,	// (out)�}�e���A����.
		&m_pMesh )))		// (out)���b�V���I�u�W�F�N�g.
	{
		// ���̃t�@�C���t�@�C�����ǂݍ��߂ĂȂ����@�C���K�v.
		_ASSERT_EXPR( false, L"X�t�@�C���ǂݍ��ݎ��s" );
		return E_FAIL;
	}

	//X̧�ق�۰��(���C�Ƃ̔���p�ɕʐݒ�œǂݍ���).
	if( FAILED( D3DXLoadMeshFromXA(
		filename,	//̧�ٖ�.
		D3DXMESH_SYSTEMMEM,	//������؂ɓǂݍ���.
		m_pDevice9, nullptr,
		&pD3DXMtrlBuffer,	//(out)��رُ��.
		nullptr,
		&m_NumMaterials,	//(out)��رِ�.
		&m_pMeshForRay ) ) )//(out)ү����޼ު��.
	{
		_ASSERT_EXPR( false, L"X̧�ٓǍ����s" );
		return E_FAIL;
	}


	D3D11_BUFFER_DESC		bd;			// Dx11�o�b�t�@�\����.
	D3D11_SUBRESOURCE_DATA	InitData;	// �������f�[�^.

										// �ǂݍ��񂾏�񂩂�K�v�ȏ��𔲂��o��.
	D3DXMATERIAL *d3dxMaterials
		= static_cast<D3DXMATERIAL*>(pD3DXMtrlBuffer->GetBufferPointer());
	// �}�e���A�������̗̈���m��.
	m_pMaterials = new MY_MATERIAL[m_NumMaterials]();
	m_ppIndexBuffer = new ID3D11Buffer*[m_NumMaterials]();

	// �}�e���A�������J��Ԃ�.
	for( DWORD No = 0; No < m_NumMaterials; No++ )
	{
		// �C���f�b�N�X�o�b�t�@�̏�����.
		m_ppIndexBuffer[No] = nullptr;

		// �}�e���A�����̃R�s�[.
		// �A���r�G���g.
		m_pMaterials[No].Ambient.x = d3dxMaterials[No].MatD3D.Ambient.r;
		m_pMaterials[No].Ambient.y = d3dxMaterials[No].MatD3D.Ambient.g;
		m_pMaterials[No].Ambient.z = d3dxMaterials[No].MatD3D.Ambient.b;
		m_pMaterials[No].Ambient.w = d3dxMaterials[No].MatD3D.Ambient.a;
		// �f�B�t�[�Y.
		m_pMaterials[No].Diffuse.x = d3dxMaterials[No].MatD3D.Diffuse.r;
		m_pMaterials[No].Diffuse.y = d3dxMaterials[No].MatD3D.Diffuse.g;
		m_pMaterials[No].Diffuse.z = d3dxMaterials[No].MatD3D.Diffuse.b;
		m_pMaterials[No].Diffuse.w = d3dxMaterials[No].MatD3D.Diffuse.a;
		// �X�y�L����.
		m_pMaterials[No].Specular.x = d3dxMaterials[No].MatD3D.Specular.r;
		m_pMaterials[No].Specular.y = d3dxMaterials[No].MatD3D.Specular.g;
		m_pMaterials[No].Specular.z = d3dxMaterials[No].MatD3D.Specular.b;
		m_pMaterials[No].Specular.w = d3dxMaterials[No].MatD3D.Specular.a;

		// (���̖ʂ�)�e�N�X�`�����\���Ă��邩�H.
		if ( d3dxMaterials[No].pTextureFilename != nullptr &&
			lstrlen(d3dxMaterials[No].pTextureFilename) > 0 )
		{
			// �e�N�X�`���Ƀt���O�𗧂Ă�.
			m_EnableTexture = true;

			char path[128] = "";
			int path_count = lstrlen( filename );
			for( int k = path_count; k >= 0; k-- ){
				if( filename[k] == '\\' ){
					for( int j = 0; j <= k; j++ ){
						path[j] = filename[j];
					}
					path[k + 1] = '\0';
					break;
				}
			}
			// �p�X�ƃe�N�X�`���t�@�C������A��.
			strcat_s( path, sizeof(path), d3dxMaterials[No].pTextureFilename );

			// �e�N�X�`���t�@�C�������R�s�[.
			strcpy_s( 
				m_pMaterials[No].szTexTureName,
				sizeof( m_pMaterials[No].szTexTureName ), path );
			//------------------------------------------------------------.
			// �e�N�X�`���쐬.
			if( FAILED( D3DX11CreateShaderResourceViewFromFileA(
				m_pDevice11, 
				m_pMaterials[No].szTexTureName,	// �e�N�X�`���t�@�C����.
				nullptr, 
				nullptr, 
				&m_pMaterials[No].pTexture,		// (out)�e�N�X�`���I�u�W�F�N�g.
				nullptr )))
			{
				_ASSERT_EXPR( false, L"�e�N�X�`���쐬���s" );
				return E_FAIL;
			}
		}
	}


	//--------------------------------------------------------
	// �C���f�b�N�X�o�b�t�@�쐬.
	//--------------------------------------------------------
	// ���b�V���̑������𓾂�.
	// �������ŃC���f�b�N�X�o�b�t�@����ׂ����}�e���A�����Ƃ̃C���f�b�N�X�o�b�t�@�𕪗��ł���.
	D3DXATTRIBUTERANGE* pAttrTable = nullptr;

	// ���b�V���̖ʋy�ђ��_�̏��ԕύX�𐧌䂵�A�p�t�H�[�}���X���œK������.
	// D3DXMESHOPT_COMPACT : �ʂ̏��Ԃ�ύX���A�g�p����Ă��Ȃ����_�Ɩʂ��폜����.
	// D3DXMESHOPT_ATTRSORT : �p�t�H�[�}���X���グ�邽�߁A�ʂ̏��Ԃ�ύX���čœK�����s��.
	m_pMesh->OptimizeInplace( 
		D3DXMESHOPT_COMPACT | D3DXMESHOPT_ATTRSORT, 
		nullptr, nullptr, nullptr, nullptr );
	// �����e�[�u���̎擾.
	m_pMesh->GetAttributeTable( nullptr, &m_NumAttr );
	pAttrTable = new D3DXATTRIBUTERANGE[m_NumAttr];
	if( FAILED( m_pMesh->GetAttributeTable( pAttrTable, &m_NumAttr )))
	{
		_ASSERT_EXPR( false, L"�����e�[�u���擾���s" );
		return E_FAIL;
	}

	// ������Lock���Ȃ��Ǝ��o���Ȃ�.
	int*	pIndex = nullptr;
	m_pMesh->LockIndexBuffer(
		D3DLOCK_READONLY, (void**)&pIndex );
	// �������Ƃ̃C���f�b�N�X�o�b�t�@���쐬.
	for( DWORD No = 0; No < m_NumAttr; No++ ){
		m_AttrID[No] = pAttrTable[No].AttribId;
		// Dx9�̃C���f�b�N�X�o�b�t�@����̏��ŁADx11�̃C���f�b�N�X�o�b�t�@���쐬,
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.ByteWidth = sizeof( int )*pAttrTable[No].FaceCount * 3;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;

		// �傫���C���f�b�N�X�o�b�t�@���̃I�t�Z�b�g(x3����).
		InitData.pSysMem = &pIndex[pAttrTable[No].FaceStart*3];

		if( FAILED( m_pDevice11->CreateBuffer(
			&bd, &InitData, &m_ppIndexBuffer[No] ))){
			_ASSERT_EXPR( false, L"�C���f�b�N�X�o�b�t�@�쐬���s" );
			return E_FAIL;
		}
		// �ʂ̐����R�s�[.
		m_pMaterials[m_AttrID[No]].dwNumFace = pAttrTable[No].FaceCount;
	}
	// �����e�[�u���̍폜.
	delete[] pAttrTable;
	// �g�p�ς݂̃C���f�b�N�X�o�b�t�@�̉��.
	m_pMesh->UnlockIndexBuffer();

	// �s�v�ɂȂ����}�e���A���o�b�t�@�����.
	SAFE_RELEASE( pD3DXMtrlBuffer );


	//---------------------------------------------------.
	// ���_�o�b�t�@�̍쐬.
	//---------------------------------------------------.
	// Dx9�̏ꍇ�Amap�ł͂Ȃ�Lock�Œ��_�o�b�t�@����f�[�^�����o��.
	LPDIRECT3DVERTEXBUFFER9	pVB = nullptr;
	m_pMesh->GetVertexBuffer( &pVB );
	DWORD dwStride	= m_pMesh->GetNumBytesPerVertex();	// ���_�o�C�g��.
	BYTE* pVertices	= nullptr;
	VERTEX* pVertex	= nullptr;
	if( SUCCEEDED(
		pVB->Lock( 0, 0, (VOID**)&pVertices, 0 )))
	{
		pVertex = (VERTEX*)pVertices;
		// Dx9�̒��_�o�b�t�@����̏��ŁADx11���_�o�b�t�@���쐬.
		bd.Usage		= D3D11_USAGE_DEFAULT;
		// ���_���i�[����̂ɕK�v�ȃo�C�g��.
		bd.ByteWidth	= m_pMesh->GetNumBytesPerVertex()*m_pMesh->GetNumVertices();
		bd.BindFlags	= D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags	= 0;
		bd.MiscFlags	= 0;
		InitData.pSysMem = pVertex;
		if ( FAILED( m_pDevice11->CreateBuffer( 
			&bd, &InitData, &m_pVertexBuffer )))
		{
			_ASSERT_EXPR( false, L"���_�o�b�t�@�쐬���s" );
			return E_FAIL;
		}
		pVB->Unlock();
	}
	SAFE_RELEASE( pVB );	// ���_�o�b�t�@���.


							//ø����p�̻���׍\����.
	D3D11_SAMPLER_DESC samDesc;
	ZeroMemory( &samDesc, sizeof( samDesc ) );
	samDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;//�Ʊ̨��(���`���).
													 //POINT:���������e��.
	samDesc.AddressU
		= D3D11_TEXTURE_ADDRESS_WRAP;//ׯ��ݸ�Ӱ��(WRAP:�J��Ԃ�).
	samDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	//MIRROR: ���]�J��Ԃ�.
	//CLAMP : �[�̖͗l�������L�΂�.
	//BORDER: �ʓr���E�F�����߂�.
	//����׍쐬.
	if( FAILED( m_pDevice11->CreateSamplerState(
		&samDesc, &m_pSampleLinear ) ) )//(out)�����.
	{
		_ASSERT_EXPR( false, L"����׍쐬���s" );
		return E_FAIL;
	}

	return S_OK;
}

// ����֐�.
void CDX9Mesh::Release()
{

	if( m_ppIndexBuffer != nullptr ){
		for( int No = m_NumMaterials-1; No >= 0; No-- ){
			if( m_ppIndexBuffer[No] != nullptr ){
				SAFE_RELEASE( m_ppIndexBuffer[No] );
			}
		}
		delete[] m_ppIndexBuffer;
		m_ppIndexBuffer = nullptr;
	}
	// �}�e���A���̉��.
	SAFE_DELETE_ARY	( m_pMaterials);

	SAFE_RELEASE	( m_pMeshForRay );
	SAFE_RELEASE	( m_pMesh );

	SAFE_RELEASE	( m_pAlphaBlend );
	SAFE_RELEASE	( m_pNoAlphaBlend );
	SAFE_RELEASE	( m_CoverageBlend );

	SAFE_RELEASE	( m_RasterizerWireON );
	SAFE_RELEASE	( m_RasterizerWireOFF );

	SAFE_RELEASE	( m_pSampleLinear );
	SAFE_RELEASE	( m_pVertexBuffer );
	SAFE_RELEASE	( m_pCBufferPerMaterial );
	SAFE_RELEASE	( m_pCBufferPerMesh );
	SAFE_RELEASE	( m_pPixelShader );
	SAFE_RELEASE	( m_pVertexLayout );
	SAFE_RELEASE	( m_pVertexShader );
	
	m_pDevice9 = nullptr;
	m_pContext11 = nullptr;
	m_pDevice11 = nullptr;
	m_hWnd = nullptr;
}

//===========================================================
//	HLSĻ�ق�ǂݍ��ݼ���ނ��쐬����.
//	HLSL: High Level Shading Language �̗�.
//===========================================================
HRESULT CDX9Mesh::InitShader()
{
	ID3DBlob* pCompiledShader = nullptr;
	ID3DBlob* pErrors = nullptr;
	UINT uCompileFlag = 0;
#ifdef _DEBUG
	uCompileFlag =
		D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION;
#endif//#ifdef _DEBUG


	std::string sVS_Name = "VS";
	//if( m_EnableTexture == true ){
	//	sVS_Name += "Main";
	//} else{
	//	sVS_Name += "NoTex";
	//}
	//HLSL�����ްï������ނ�����ނ��쐬.
	if( FAILED(
		D3DX11CompileFromFile(
			SHADER_NAME,		//�����̧�ٖ�(HLSĻ��).
			nullptr,			//ϸے�`�̔z��ւ��߲��(���g�p).
			nullptr,			//�ݸٰ��̧�ق���������̪���ւ��߲��(���g�p).
			sVS_Name.c_str(),	//����޴��ذ�߲�Ċ֐��̖��O.
			"vs_5_0",			//����ނ����ق��w�肷�镶����(���̧��).
			uCompileFlag,		//����޺��߲��׸�.
			0,					//�̪�ĺ��߲��׸�(���g�p).
			nullptr,			//�گ�� ����� ����̪���ւ��߲��(���g�p).
			&pCompiledShader,	//����ނ��i�[������؂ւ��߲��.
			&pErrors,			//�װ�ƌx���ꗗ���i�[������؂ւ��߲��.
			nullptr ) ) )		//�߂�l�ւ��߲��(���g�p).
	{
		_ASSERT_EXPR( false, L"hlsl�ǂݍ��ݎ��s" );
		return E_FAIL;
	}
	SAFE_RELEASE( pErrors );

	//��L�ō쐬��������ނ���u�ްï������ށv���쐬.
	if( FAILED(
		m_pDevice11->CreateVertexShader(
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			nullptr,
			&m_pVertexShader ) ) )	//(out)�ްï�������.
	{
		_ASSERT_EXPR( false, L"�ްï������ލ쐬���s" );
		return E_FAIL;
	}

	//���_���߯�ڲ��Ă��`.
	D3D11_INPUT_ELEMENT_DESC layout[3];
	//���_���߯�ڲ��Ă̔z��v�f�����Z�o.
	UINT numElements = 0;
	if( m_EnableTexture == true ){
		D3D11_INPUT_ELEMENT_DESC tmp[] =
		{
			{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",	0 ,DXGI_FORMAT_R32G32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		numElements = sizeof(tmp) / sizeof(tmp[0]);	// �v�f���Z�o.
		memcpy_s( layout, sizeof(layout), tmp, 
			sizeof(D3D11_INPUT_ELEMENT_DESC) * numElements );
	} else {
		D3D11_INPUT_ELEMENT_DESC tmp[] =
		{
			{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		numElements = sizeof( tmp ) / sizeof( tmp[0] );	// �v�f���Z�o.
		memcpy_s( layout, sizeof( layout ), tmp, 
			sizeof( D3D11_INPUT_ELEMENT_DESC ) * numElements );
	}

	//���_���߯�ڲ��Ă��쐬.
	if( FAILED(
		m_pDevice11->CreateInputLayout(
			layout,
			numElements,
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			&m_pVertexLayout ) ) )	//(out)���_���߯�ڲ���.
	{
		_ASSERT_EXPR( false, L"���_���߯�ڲ��č쐬���s" );
		return E_FAIL;
	}
	SAFE_RELEASE( pCompiledShader );

	std::string sPS_Name = "PS";
	//if( m_EnableTexture == true ){
	//	sPS_Name += "Main";
	//} else{
	//	sPS_Name += "NoTex";
	//}
	//HLSL�����߸�ټ���ނ�����ނ��쐬.
	if( FAILED(
		D3DX11CompileFromFile(
			SHADER_NAME,	//�����̧�ٖ�(HLSĻ��).
			nullptr,			//ϸے�`�̔z��ւ��߲��(���g�p).
			nullptr,			//�ݸٰ��̧�ق���������̪���ւ��߲��(���g�p).
			sPS_Name.c_str(),	//����޴��ذ�߲�Ċ֐��̖��O.
			"ps_5_0",			//����ނ����ق��w�肷�镶����(���̧��).
			uCompileFlag,		//����޺��߲��׸�.
			0,					//�̪�ĺ��߲��׸�(���g�p).
			nullptr,			//�گ�� ����� ����̪���ւ��߲��(���g�p).
			&pCompiledShader,	//����ނ��i�[������؂ւ��߲��.
			&pErrors,			//�װ�ƌx���ꗗ���i�[������؂ւ��߲��.
			nullptr ) ) )		//�߂�l�ւ��߲��(���g�p).
	{
		_ASSERT_EXPR( false, L"hlsl�ǂݍ��ݎ��s" );
		return E_FAIL;
	}

	SAFE_RELEASE( pErrors );

	//��L�ō쐬��������ނ���u�߸�ټ���ށv���쐬.
	if( FAILED(
		m_pDevice11->CreatePixelShader(
			pCompiledShader->GetBufferPointer(),
			pCompiledShader->GetBufferSize(),
			nullptr,
			&m_pPixelShader ) ) )	//(out)�߸�ټ����.
	{
		_ASSERT_EXPR( false, L"�߸�ټ���ލ쐬���s" );
		return E_FAIL;
	}
	SAFE_RELEASE( pCompiledShader );

	//�ݽ���(�萔)�ޯ̧�쐬.
	//����ނɓ���̐��l�𑗂��ޯ̧.
	//�����ł͕ϊ��s��n���p.
	//����ނ� World, View, Projection �s���n��.
	D3D11_BUFFER_DESC cb;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	//�ݽ����ޯ̧���w��.
	cb.ByteWidth = sizeof( CBUFFER_PER_MESH );	//�ݽ����ޯ̧�̻���.
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	//�������݂ű���.
	cb.MiscFlags = 0;							//���̑����׸�(���g�p).
	cb.StructureByteStride = 0;					//�\���̻̂���(���g�p).
	cb.Usage = D3D11_USAGE_DYNAMIC;				//�g�p���@:���ڏ�������.

												//�ݽ����ޯ̧�̍쐬.
	if( FAILED(
		m_pDevice11->CreateBuffer(
			&cb, nullptr, &m_pCBufferPerMesh )))
	{
		_ASSERT_EXPR( false, L"�ݽ����ޯ̧�쐬���s" );
		return E_FAIL;
	}

	// �R���X�^���g�o�b�t�@ �}�e���A���p.
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	//�ݽ����ޯ̧���w��.
	cb.ByteWidth = sizeof( CBUFFER_PER_MATERIAL );//�ݽ����ޯ̧�̻���.
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	//�������݂ű���.
	cb.MiscFlags = 0;	//���̑����׸�(���g�p).
	cb.StructureByteStride = 0;	//�\���̻̂���(���g�p).
	cb.Usage = D3D11_USAGE_DYNAMIC;	//�g�p���@:���ڏ�������.

									//�ݽ����ޯ̧�̍쐬.
	if( FAILED(
		m_pDevice11->CreateBuffer(
			&cb, nullptr, &m_pCBufferPerMaterial )))
	{
		_ASSERT_EXPR( false, L"�ݽ����ޯ̧�쐬���s" );
		return E_FAIL;
	}

	D3D11_BUFFER_DESC cbf;
	// �R���X�^���g�o�b�t�@ �}�e���A���p.
	cbf.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	//�ݽ����ޯ̧���w��.
	cbf.ByteWidth = sizeof( cbPerObject );//�ݽ����ޯ̧�̻���.
	cbf.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	//�������݂ű���.
	cbf.MiscFlags = 0;	//���̑����׸�(���g�p).
	cbf.StructureByteStride = 0;	//�\���̻̂���(���g�p).
	cbf.Usage = D3D11_USAGE_DYNAMIC;	//�g�p���@:���ڏ�������.

										//�ݽ����ޯ̧�̍쐬.
	if( FAILED(
		m_pDevice11->CreateBuffer(
			&cbf, nullptr, &m_pCBufferPerFrame ) ) ){
		_ASSERT_EXPR( false, L"�ݽ���F�ޯ̧�쐬���s" );
		return E_FAIL;
	}


	return S_OK;
}

// �����_�����O�p.
// ��DirectX���̃����_�����O�֐�.
//   �ŏI�I�ɉ�ʂɏo�͂���̂�Main�N���X�̃����_�����O�֐������.
void CDX9Mesh::Render( D3DXMATRIX& mView, D3DXMATRIX& mProj,
	Light& stLight, stCAMERA& stCamera, bool ChangeTexFlag )
{
	//�@���[���h�s��, �X�P�[���s��, ��]�s��, ���s�ړ��s��.
	D3DXMATRIX mWorld, mScale, mRot, mTran;
	D3DXMATRIX mYaw, mPitch, mRoll;

	//�g��k���s��쐬.
	D3DXMatrixScaling(
		&mScale, //(out)
		m_fScale, m_fScale, m_fScale );	// x, y, z.

	// Y����]�s��쐬.
	D3DXMatrixRotationY( &mYaw,		m_vRot.y );	// �P����Y����].
												// X����]�s��쐬.
	D3DXMatrixRotationX( &mPitch,	m_vRot.x );	// �P����X����].
												// Z����]�s��쐬.
	D3DXMatrixRotationZ( &mRoll,	m_vRot.z );	// �P����Z����].

												// ���s�ړ��s��.
	D3DXMatrixTranslation(
		&mTran,	//(out).
		m_vPos.x, m_vPos.y, m_vPos.z );	// x, y, z���W.

										// ��]�s����쐬.
										//mRot = mYaw * mPitch * mRoll;
	mRot = mYaw * mPitch * mRoll;
	// ���[���h�s��쐬.
	//    ��   �g�k * ��] * �ړ�	�����Ԃ��ƂĂ���� !!!.
	mWorld = mScale * mRot * mTran;

	//�g�p���鼪��ނ̾��.
	m_pContext11->VSSetShader( m_pVertexShader, nullptr, 0 );	//���_�����.
	m_pContext11->PSSetShader( m_pPixelShader, nullptr, 0 );	//�߸�ټ����.

	static float rng = 0.0f;
	if( GetAsyncKeyState('K') & 0x8000 ){
		rng += 0.001f;
	}

	//����ނ̺ݽ����ޯ̧�Ɋe���ް���n��.
	D3D11_MAPPED_SUBRESOURCE pData;
	//�ޯ̧�����ް��̏��������J�n����Map.
	if( SUCCEEDED( m_pContext11->Map(
		m_pCBufferPerFrame, 0,
		D3D11_MAP_WRITE_DISCARD,
		0, &pData ) ) )
	{
		cbPerObject cb;	//�ݽ����ޯ̧.
		//���[���h�s���n��.
		cb.World = mWorld;
		D3DXMatrixTranspose( &cb.World, &cb.World );

		//ܰ���,�ޭ�,��ۼު���ݍs���n��.
		D3DXMATRIX mWVP = mWorld * mView * mProj;
		D3DXMatrixTranspose( &mWVP, &mWVP );//�s���]�u����.
		//���s��̌v�Z���@��DirectX��GPU�ňقȂ邽�ߓ]�u���K�v.
		cb.WVP = mWVP;

		cb.light.Rot = stLight.Rot;
		D3DXMatrixTranspose( &cb.light.Rot, &cb.light.Rot );

		//cb.light.pos = D3DXVECTOR3(0.0f, 5.0f, 0.0f);
		cb.light.dir = D3DXVECTOR3( 0.0f, 0.0f, 1.0f );

		D3DXVec3TransformNormal( &cb.light.dir, &cb.light.dir, &cb.light.Rot );

		cb.light.range = stLight.range;
		cb.light.cone = stLight.cone;					//�l���ł����ƂȂ񂩏Ƃ炷�~���������Ȃ�܂�.
		cb.light.att = stLight.att;
		cb.light.ambient = stLight.ambient;
		cb.light.diffuse = stLight.diffuse;

		cb.light.pos.x = stLight.pos.x;
		cb.light.pos.y = stLight.pos.y;
		cb.light.pos.z = stLight.pos.z;

		cb.fAlpha = m_fAlpha;

		//cb.light.dir.x = 10 - stLight.pos.x;
		//cb.light.dir.y = 10 - stLight.pos.y;
		//cb.light.dir.z = 10 - stLight.pos.z;

		memcpy_s(
			pData.pData,	//��߰����ޯ̧.
			pData.RowPitch,	//��߰����ޯ̧����.
			(void*)( &cb ),	//��߰�����ޯ̧.
			sizeof( cb ) );	//��߰�����ޯ̧����.

		//�ޯ̧�����ް��̏��������I������Unmap.
		m_pContext11->Unmap( m_pCBufferPerFrame, 0 );
	}
	//���̺ݽ����ޯ̧���ǂ̼���ނŎg�p���邩�H.
	m_pContext11->VSSetConstantBuffers(
		0, 1, &m_pCBufferPerFrame );	//���_�����.
	m_pContext11->PSSetConstantBuffers(
		0, 1, &m_pCBufferPerFrame );	//�߸�ټ����.

										//���_���߯�ڲ��Ă��.
	m_pContext11->IASetInputLayout( m_pVertexLayout );

	//����è�ށE���ۼް���.
	m_pContext11->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	// ���_�o�b�t�@���Z�b�g.
	UINT stride = m_pMesh->GetNumBytesPerVertex();
	UINT offset = 0;
	m_pContext11->IASetVertexBuffers(
		0, 1, &m_pVertexBuffer, &stride, &offset );

	// �����̐������A���ꂼ��̑����̃C���f�b�N�X�o�b�t�@��`��.
	for( DWORD No = 0; No < m_NumAttr; No++ ){
		// �g�p����Ă��Ȃ��}�e���A���΍�.
		if( m_pMaterials[m_AttrID[No]].dwNumFace == 0 ){
			continue;
		}
		// �C���f�b�N�X�o�b�t�@���Z�b�g.
		m_pContext11->IASetIndexBuffer(
			m_ppIndexBuffer[No], DXGI_FORMAT_R32_UINT, 0 );
		// �}�e���A���̊e�v�f���V�F�[�_�ɓn��.
		D3D11_MAPPED_SUBRESOURCE pDataMat;
		if( SUCCEEDED(
			m_pContext11->Map( m_pCBufferPerMaterial, 
				0, D3D11_MAP_WRITE_DISCARD, 0, &pDataMat )))
		{
			CBUFFER_PER_MATERIAL cb;
			// �A���r�G���g,�f�B�t�[�Y,�X�y�L�������V�F�[�_�ɓn��.
			cb.vAmbient = m_pMaterials[m_AttrID[No]].Ambient;
			cb.vDiffuse = m_pMaterials[m_AttrID[No]].Diffuse;
			cb.vSpecular = m_pMaterials[m_AttrID[No]].Specular;

			//			cb.vColor.w = 0.1f;

			memcpy_s( pDataMat.pData, pDataMat.RowPitch,
				(void*)&cb, sizeof(cb) );
			m_pContext11->Unmap( m_pCBufferPerMaterial, 0 );
		}
		// ���̃R���X�^���g�o�b�t�@���ǂ̃V�F�[�_�Ŏg�����H.
		m_pContext11->VSSetConstantBuffers( 1, 1, &m_pCBufferPerMaterial );
		m_pContext11->PSSetConstantBuffers( 1, 1, &m_pCBufferPerMaterial );

		// �e�N�X�`�����V�F�[�_�ɓn��.
		if( m_pMaterials[m_AttrID[No]].pTexture != nullptr ){
			// �e�N�X�`��������Ƃ�.
			m_pContext11->PSSetSamplers( 0, 1, &m_pSampleLinear );
			if( ChangeTexFlag == false ){
				// ���ɓǂݍ���ł����e�N�X�`����\��.
				m_pContext11->PSSetShaderResources( 0, 1, &m_pMaterials[m_AttrID[No]].pTexture );
			} else {
				// �؂�ւ����e�N�X�`����\��.
				ChangeTextureRender( No );
			}
		} else {
			// �e�N�X�`�����Ȃ��Ƃ�.
			ID3D11ShaderResourceView* pNoThing[1] = {0};
			m_pContext11->PSSetShaderResources( 0, 1, pNoThing );
		}
		// �v���~�e�B�u(�|���S��)�������_�����O.
		m_pContext11->DrawIndexed( m_pMaterials[m_AttrID[No]].dwNumFace * 3, 0, 0 );
	}
}

void CDX9Mesh::ChangeTextureRender( int no )
{
	int str_length = lstrlen( m_pMaterials[m_AttrID[no]].szTexTureName );
	int str_no = 0;
	bool change_flag = false;
	for( str_no = str_length; 0 < str_no; str_no-- ){
		if( m_pMaterials[m_AttrID[no]].szTexTureName[str_no] == '\\' ){
			str_no++;	//�p�X�̋�؂�܂ŗ��Ă���̂ň�߂�.
			change_flag = true;
			break;
		}
	}
	if( change_flag == true ){
		//�Ώۃe�N�X�`�����Ȃ̂�����.
		if( strcmp(
			&m_pMaterials[m_AttrID[no]].szTexTureName[str_no],
			m_pChangeTextures->szTargetTextureName ) == 0 ){
			//�ʂ̃e�N�X�`����ݒ�.
			m_pContext11->PSSetShaderResources( 0, 1, &m_pChangeTextures->pTexture );
		} else{
			m_pContext11->PSSetShaderResources( 0, 1, &m_pMaterials[m_AttrID[no]].pTexture );
		}
	}
}

// �X�t�B�A�쐬.
HRESULT CDX9Mesh::InitSphere( CDX9Mesh* pMesh )
{
	LPDIRECT3DVERTEXBUFFER9 pVB = nullptr;	// ���_�o�b�t�@.
	void* pVertices = nullptr;	// ���_.
	D3DXVECTOR3 vCenter;		// ���S.
	float fRadius;

	// ���_�o�b�t�@���擾.
	if( FAILED(
		pMesh->GetMesh()->GetVertexBuffer( &pVB ) ) ){
		return E_FAIL;
	}
	// ���b�V���̒��_�o�b�t�@�����b�N����.
	if( FAILED(
		pVB->Lock( 0, 0, &pVertices, 0 ) ) ){
		SAFE_RELEASE( pVB );
		return E_FAIL;
	}
	// ���b�V���̊O�ډ~�̒��S�Ɣ��a���v�Z����.
	D3DXComputeBoundingSphere(
		static_cast<D3DXVECTOR3*>( pVertices ),
		pMesh->GetMesh()->GetNumVertices(),					// ���_�̐�.
		D3DXGetFVFVertexSize( pMesh->GetMesh()->GetFVF() ),	// ���_�̏��.
		&vCenter,	// (out)���S���W.
		&fRadius );	// (out)���a.

					// �A�����b�N.
	pVB->Unlock();
	SAFE_RELEASE( pVB );

	// ���S�Ɣ��a���\���̂ɐݒ�.
	pMesh->m_Sphere.vCenter = vCenter;
	pMesh->m_Sphere.fRadius = fRadius * 0.07f;

	return S_OK;
}

HRESULT CDX9Mesh::InitBlend()
{
	//��̧�����ޗp�����޽ðč\����.
	//pnģ�ٓ��ɱ�̧��񂪂���̂ŁA���߂���悤�������޽ðĂŐݒ肷��.
	D3D11_BLEND_DESC BlendDesc;
	ZeroMemory( &BlendDesc, sizeof( BlendDesc ) );	//������.

	BlendDesc.IndependentBlendEnable
		= false;	//false:RenderTarget[0]�����ް�̂ݎg�p����.
					//true:RenderTarget[0�`7]���g�p�ł���
					// (���ް���ޯĖ��ɓƗ����������ޏ���).
	BlendDesc.AlphaToCoverageEnable
		= false;		//true:��̧ĩ���ڰ�ނ��g�p����.
	BlendDesc.RenderTarget[0].BlendEnable
		= true;		//true:��̧�����ނ��g�p����.
	BlendDesc.RenderTarget[0].SrcBlend	//���f�ނɑ΂���ݒ�.
		= D3D11_BLEND_SRC_ALPHA;			//��̧�����ނ��w��.
	BlendDesc.RenderTarget[0].DestBlend	//�d�˂�f�ނɑ΂���ݒ�.
		= D3D11_BLEND_INV_SRC_ALPHA;		//��̧�����ނ̔��]���w��.
	BlendDesc.RenderTarget[0].BlendOp	//�����޵�߼��.
		= D3D11_BLEND_OP_ADD;				//ADD:���Z����.
	BlendDesc.RenderTarget[0].SrcBlendAlpha	//���f�ނ̱�̧�ɑ΂���w��.
		= D3D11_BLEND_ONE;						//���̂܂܎g�p.
	BlendDesc.RenderTarget[0].DestBlendAlpha	//�d�˂�f�ނ̱�̧�ɑ΂���ݒ�.
		= D3D11_BLEND_ZERO;							//�������Ȃ��B
	BlendDesc.RenderTarget[0].BlendOpAlpha	//��̧�������޵�߼��.
		= D3D11_BLEND_OP_ADD;					//ADD:���Z����.
	BlendDesc.RenderTarget[0].RenderTargetWriteMask	//�߸�ٖ��̏�������Ͻ�.
		= D3D11_COLOR_WRITE_ENABLE_ALL;			//�S�Ă̐���(RGBA)�ւ��ް��̊i�[��������.

	//�����޽ðč쐬.
	if( FAILED(
		m_pDevice11->CreateBlendState(
			&BlendDesc, &m_pAlphaBlend ) ) ){
		_ASSERT_EXPR( false, L"�����޽ðč쐬���s" );
		return E_FAIL;
	}

	BlendDesc.AlphaToCoverageEnable
		= true;		//true:��̧ĩ���ڰ�ނ��g�p����.
	if( FAILED(
		m_pDevice11->CreateBlendState(
			&BlendDesc, &m_CoverageBlend ) ) ){
		_ASSERT_EXPR( false, L"�����޽ðč쐬���s" );
		return E_FAIL;
	}

	//�����޽ðč쐬.
	BlendDesc.RenderTarget[0].BlendEnable
		= false;	//false:��̧�����ނ��g�p���Ȃ�.
	if( FAILED(
		m_pDevice11->CreateBlendState(
			&BlendDesc, &m_pNoAlphaBlend ) ) ){
		_ASSERT_EXPR( false, L"�����޽ðč쐬���s" );
		return E_FAIL;
	}

	return S_OK;
}

//���ߐݒ�̐؂�ւ�.
void CDX9Mesh::SetBlend( bool EnableAlpha )
{
	//�����޽ðĂ̐ݒ�.
	UINT mask = 0xffffffff;	//Ͻ��l.
	if( EnableAlpha == true ){
		m_pContext11->OMSetBlendState(
			m_pAlphaBlend, nullptr, mask );
	} else{
		m_pContext11->OMSetBlendState(
			m_pNoAlphaBlend, nullptr, mask );
	}
}

void CDX9Mesh::SetCoverage( bool EnableCoverage )
{
	UINT mask = 0xffffffff;	//Ͻ��l.
	if( EnableCoverage == true ){
		m_pContext11->OMSetBlendState(
			m_CoverageBlend, nullptr, mask );
	} else{
		m_pContext11->OMSetBlendState(
			m_pNoAlphaBlend, nullptr, mask );
	}
}

HRESULT CDX9Mesh::InitWtire()
{
	D3D11_RASTERIZER_DESC rdc;
	ZeroMemory( &rdc, sizeof( rdc ) );
	rdc.FillMode = D3D11_FILL_SOLID;//�h��Ԃ�(�د��).
	rdc.CullMode = D3D11_CULL_NONE;	//D3D11_CULL_BACK:�w�ʂ�`�悵�Ȃ�.
									//D3D11_CULL_FRONT:���ʂ�`�悵�Ȃ�.
									//D3D11_CULL_NONE:��ݸނ�؂�(���w�ʂ�`�悷��).
	rdc.FrontCounterClockwise = FALSE;//��غ�݂̕\�������肷���׸�.
									  //TRUE:�����Ȃ�O�����B�E���Ȃ�������B
									  //FALSE:�t�ɂȂ�.
	rdc.DepthClipEnable = FALSE;	//�����ɂ��Ă̸د��ݸޗL��.

	m_pDevice11->CreateRasterizerState( &rdc, &m_RasterizerWireOFF );
	rdc.FillMode = D3D11_FILL_WIREFRAME;
	m_pDevice11->CreateRasterizerState( &rdc, &m_RasterizerWireON );

	m_pContext11->RSSetState( m_RasterizerWireOFF );

	return S_OK;
}

void CDX9Mesh::SetWireFlag( bool flag )
{
	if( flag == true ){
		m_pContext11->RSSetState( m_RasterizerWireON );
	} else{
		m_pContext11->RSSetState( m_RasterizerWireOFF );
	}
}

HRESULT CDX9Mesh::InitCullBack()
{
	D3D11_RASTERIZER_DESC rdc;
	ZeroMemory( &rdc, sizeof( rdc ) );
	rdc.FillMode = D3D11_FILL_SOLID;//�h��Ԃ�(�د��).
	rdc.CullMode = D3D11_CULL_NONE;	//D3D11_CULL_BACK:�w�ʂ�`�悵�Ȃ�.
									//D3D11_CULL_FRONT:���ʂ�`�悵�Ȃ�.
									//D3D11_CULL_NONE:��ݸނ�؂�(���w�ʂ�`�悷��).
	rdc.FrontCounterClockwise = FALSE;//��غ�݂̕\�������肷���׸�.
									  //TRUE:�����Ȃ�O�����B�E���Ȃ�������B
									  //FALSE:�t�ɂȂ�.
	rdc.DepthClipEnable = FALSE;	//�����ɂ��Ă̸د��ݸޗL��.

	m_pDevice11->CreateRasterizerState( &rdc, &m_RasterizerCullBackOFF );
	rdc.CullMode = D3D11_CULL_BACK;
	m_pDevice11->CreateRasterizerState( &rdc, &m_RasterizerCullBackON );

	m_pContext11->RSSetState( m_RasterizerWireOFF );

	return S_OK;
}

void CDX9Mesh::SetCullBackFlag( bool flag )
{
	if( flag == true ){
		m_pContext11->RSSetState( m_RasterizerCullBackON );
	} else{
		m_pContext11->RSSetState( m_RasterizerCullBackOFF );
	}
}

D3DXVECTOR2 CDX9Mesh::WorldtoScreenPos( const D3DXMATRIX& mView,
	const D3DXMATRIX& mProj, const D3DXVECTOR3& pos )
{
	D3DXVECTOR3 screenOutPos;

	// ���[���h�ϊ��s��.
	D3DXMATRIX mWorld;
	D3DXMatrixIdentity( &mWorld );	// �P�ʍs��쐬.

	// �r���[�|�[�g�s��.
	D3DXMATRIX mViewport;
	D3DXMatrixIdentity( &mViewport );	// �P�ʍs��쐬.
	mViewport._11 = static_cast<float>( WND_W ) / 2.0f;
	mViewport._22 = static_cast<float>( WND_H ) /-2.0f;
	mViewport._41 = static_cast<float>( WND_W ) / 2.0f;
	mViewport._42 = static_cast<float>( WND_H ) / 2.0f;

	// ���[���h, �r���[, �v���W�F, �r���[�|�[�g�̌v�Z.
	D3DXMATRIX m = mWorld * mView * mProj * mViewport;
	// D3DXVECTOR3�Ɗ|�����킹.
	D3DXVec3TransformCoord( &screenOutPos, &pos, &m );

	// �v�Z���ʂ�x, y���W��Ԃ�. 
	return D3DXVECTOR2( screenOutPos.x, screenOutPos.y );
}