#include "Box.h"

// �V�F�[�_�t�@�C����(�f�B���N�g�����܂�).
const char SHADER_NAME[] = "Data\\Shader\\Ray.hlsl";

CBox::CBox()
	: m_pDevice11	 	( nullptr )
	, m_pContext11	 	( nullptr )
	, m_pVertexShader	( nullptr )
	, m_pVertexLayout	( nullptr )
	, m_pPixelShader	( nullptr )	
	, m_pConstantBuffer	( nullptr )
	, g_pIndexBuffer	( nullptr )
	, m_pVertexBuffer	( nullptr )
	, m_pAlphaBlend		( nullptr )
	, m_pNoAlphaBlend	( nullptr )
	, m_vPosition		()
	, m_fYaw			( 0.0f )
{
}

CBox::~CBox()
{
	SAFE_RELEASE( m_pNoAlphaBlend );
	SAFE_RELEASE( m_pAlphaBlend );
	SAFE_RELEASE( g_pIndexBuffer );
	SAFE_RELEASE( m_pConstantBuffer );
	SAFE_RELEASE( m_pPixelShader );
	SAFE_RELEASE( m_pVertexBuffer );
	SAFE_RELEASE( m_pVertexLayout );
	SAFE_RELEASE( m_pVertexShader );

	m_pContext11	= nullptr;
	m_pDevice11		= nullptr;
}

// �������֐�.
HRESULT CBox::Init(
	ID3D11Device* pDevice11,
	ID3D11DeviceContext* pContext11,
	const D3DXVECTOR3& vMaxPoint,
	const D3DXVECTOR3& vMinPoint )
{
	m_pDevice11 = pDevice11;
	m_pContext11 = pContext11;

	if( FAILED( InitShader() )){
		return E_FAIL;
	}
	if( FAILED( InitCube( vMaxPoint, vMinPoint ) )){
		return E_FAIL;
	}
	if( FAILED( InitBlend() )){
		return E_FAIL;
	}
	return S_OK;
}

//----------------------.
// �V�F�[�_�[�쐬.
//----------------------.
HRESULT CBox::InitShader()
{
	ID3DBlob* pCompiledShader = nullptr;
	ID3DBlob* pErrors = nullptr;
	UINT uCompileFlag = 0;
#ifdef _DEBUG
	uCompileFlag =
		D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION;
#endif//#ifdef _DEBUG

	//HLSL�����ްï������ނ�����ނ��쐬.
	if( FAILED(
		D3DX11CompileFromFile(
			SHADER_NAME,	//�����̧�ٖ�(HLSĻ��).
			nullptr,		//ϸے�`�̔z��ւ��߲��(���g�p).
			nullptr,		//�ݸٰ��̧�ق���������̪���ւ��߲��(���g�p).
			"VS_Main",		//����޴��ذ�߲�Ċ֐��̖��O.
			"vs_5_0",		//����ނ����ق��w�肷�镶����(���̧��).
			uCompileFlag,	//����޺��߲��׸�.
			0,				//�̪�ĺ��߲��׸�(���g�p).
			nullptr,		//�گ�� ����� ����̪���ւ��߲��(���g�p).
			&pCompiledShader,//����ނ��i�[������؂ւ��߲��.
			&pErrors,		//�װ�ƌx���ꗗ���i�[������؂ւ��߲��.
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
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{
			"POSITION",						//�ʒu.
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,	//DXGI��̫�ϯ�(32bit float�^*3).
			0,
			0,								//�ް��̊J�n�ʒu.
			D3D11_INPUT_PER_VERTEX_DATA, 0
		}
	};
	//���_���߯�ڲ��Ă̔z��v�f�����Z�o.
	UINT numElements = sizeof( layout ) / sizeof( layout[0] );

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

	//HLSL�����߸�ټ���ނ�����ނ��쐬.
	if( FAILED(
		D3DX11CompileFromFile(
			SHADER_NAME,	//�����̧�ٖ�(HLSĻ��).
			nullptr,		//ϸے�`�̔z��ւ��߲��(���g�p).
			nullptr,		//�ݸٰ��̧�ق���������̪���ւ��߲��(���g�p).
			"PS_Main",		//����޴��ذ�߲�Ċ֐��̖��O.
			"ps_5_0",		//����ނ����ق��w�肷�镶����(���̧��).
			uCompileFlag,	//����޺��߲��׸�.
			0,				//�̪�ĺ��߲��׸�(���g�p).
			nullptr,		//�گ�� ����� ����̪���ւ��߲��(���g�p).
			&pCompiledShader,//����ނ��i�[������؂ւ��߲��.
			&pErrors,		//�װ�ƌx���ꗗ���i�[������؂ւ��߲��.
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
	cb.ByteWidth = sizeof( C_BUFFER );//�ݽ����ޯ̧�̻���.
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	//�������݂ű���.
	cb.MiscFlags = 0;	//���̑����׸�(���g�p).
	cb.StructureByteStride = 0;	//�\���̻̂���(���g�p).
	cb.Usage = D3D11_USAGE_DYNAMIC;	//�g�p���@:���ڏ�������.

									//�ݽ����ޯ̧�̍쐬.
	if( FAILED(
		m_pDevice11->CreateBuffer(
			&cb,
			nullptr,
			&m_pConstantBuffer ) ) ){
		_ASSERT_EXPR( false, L"�ݽ����ޯ̧�쐬���s" );
		return E_FAIL;
	}


	return S_OK;
}

//----------------------------.
// �L���[�u,�{�b�N�X�̍쐬.
//----------------------------.
HRESULT CBox::InitCube( const D3DXVECTOR3& vMaxPoint, const D3DXVECTOR3& vMinPoint )
{
	//�����(�l�p�`)�̒��_���쐬.
	VERTEX vertices[]=
	{
		//���_���W(x,y,z)				 
		D3DXVECTOR3(vMinPoint.x,  vMaxPoint.y, vMinPoint.z), //���_�P(���㉜).
		D3DXVECTOR3( vMaxPoint.x,  vMaxPoint.y, vMinPoint.z), //���_�Q(�E�㉜).
		D3DXVECTOR3(vMinPoint.x, vMinPoint.y, vMinPoint.z), //���_�R(������).
		D3DXVECTOR3( vMaxPoint.x, vMinPoint.y, vMinPoint.z), //���_�S(�E����).

		D3DXVECTOR3(vMinPoint.x,  vMaxPoint.y,  vMaxPoint.z ), //���_�P(�����O).
		D3DXVECTOR3(vMinPoint.x, vMinPoint.y,  vMaxPoint.z ), //���_�Q(������O).
		D3DXVECTOR3( vMaxPoint.x,  vMaxPoint.y,  vMaxPoint.z ), //���_�R(�E���O).
		D3DXVECTOR3( vMaxPoint.x, vMinPoint.y,  vMaxPoint.z ), //���_�S(�E����O).

		D3DXVECTOR3(vMinPoint.x,  vMaxPoint.y,  vMaxPoint.z ), //���_�P(�����O).
		D3DXVECTOR3(vMinPoint.x,  vMaxPoint.y, vMinPoint.z ), //���_�Q(���㉜).
		D3DXVECTOR3(vMinPoint.x, vMinPoint.y,  vMaxPoint.z ), //���_�R(������O).
		D3DXVECTOR3(vMinPoint.x, vMinPoint.y, vMinPoint.z ), //���_�S(������).

		D3DXVECTOR3( vMaxPoint.x,  vMaxPoint.y,  vMaxPoint.z ), //���_�P(�E���O).
		D3DXVECTOR3( vMaxPoint.x, vMinPoint.y,  vMaxPoint.z ), //���_�Q(�E����O).
		D3DXVECTOR3( vMaxPoint.x,  vMaxPoint.y, vMinPoint.z ), //���_�R(�E�㉜).
		D3DXVECTOR3( vMaxPoint.x, vMinPoint.y, vMinPoint.z ), //���_�S(�E����).

		D3DXVECTOR3(vMinPoint.x,  vMaxPoint.y,  vMaxPoint.z ), //���_�P(����).
		D3DXVECTOR3( vMaxPoint.x,  vMaxPoint.y,  vMaxPoint.z ), //���_�Q(����).
		D3DXVECTOR3(vMinPoint.x,  vMaxPoint.y, vMinPoint.z ), //���_�R(�E��).
		D3DXVECTOR3( vMaxPoint.x,  vMaxPoint.y, vMinPoint.z ), //���_�S(�E��).

		D3DXVECTOR3(vMinPoint.x, vMinPoint.y,  vMaxPoint.z ), //���_�P(����).
		D3DXVECTOR3(vMinPoint.x, vMinPoint.y, vMinPoint.z ), //���_�Q(����).
		D3DXVECTOR3( vMaxPoint.x, vMinPoint.y,  vMaxPoint.z ), //���_�R(�E��).
		D3DXVECTOR3( vMaxPoint.x, vMinPoint.y, vMinPoint.z )  //���_�S(�E��).
	};
	WORD g_IndexList[]{
		0,  1,  2,     3,  2,  1,
		4,  5,  6,     7,  6,  5,
		8,  9,  10,    11, 10, 9,
		12, 13, 14,    15, 14, 13,
		16, 17, 18,    19, 18, 17,
		20, 21, 22,    23, 22, 21,
	};

	//�ő�v�f�����Z�o����.
	UINT uVerMax = sizeof(vertices) / sizeof(vertices[0]);

	//�ޯ̧�\����.
	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;	//�g�p���@(��̫��).
	bd.ByteWidth = sizeof(VERTEX) * uVerMax;//���_�̻���.
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;//���_�ޯ̧�Ƃ��Ĉ���.
	bd.CPUAccessFlags = 0;	//CPU����ͱ������Ȃ�.
	bd.MiscFlags = 0;		//���̑����׸�(���g�p).
	bd.StructureByteStride = 0;	//�\���̻̂���(���g�p).

	//���ؿ���ް��\����.
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;	//����؂̒��_���.

	//���_�ޯ̧�̍쐬.
	if (FAILED(m_pDevice11->CreateBuffer(
		&bd, &InitData, &m_pVertexBuffer)))
	{
		_ASSERT_EXPR(false, L"���_�ޯ̧�쐬���s");
		return E_FAIL;
	}

	D3D11_BUFFER_DESC id;
	id.Usage = D3D11_USAGE_DEFAULT;
	id.ByteWidth = sizeof( WORD ) * 36;
	id.BindFlags = D3D11_BIND_INDEX_BUFFER;
	id.CPUAccessFlags = 0;	//CPU����ͱ������Ȃ�.
	id.MiscFlags = 0;		//���̑����׸�(���g�p).
	id.StructureByteStride = 0;	//�\���̻̂���(���g�p).

	D3D11_SUBRESOURCE_DATA IndexData;
	IndexData.pSysMem = g_IndexList;
	IndexData.SysMemPitch = 0;
	IndexData.SysMemSlicePitch = 0;
	if (FAILED( m_pDevice11->CreateBuffer(
		&id, &IndexData, &g_pIndexBuffer ) ))
	{
		_ASSERT_EXPR( false, L"�C���f�b�N�X�ޯ̧�쐬���s" );
		return E_FAIL;
	}

	return S_OK;
}

//-------------------.
// �`��֐�.
//-------------------.
void CBox::Render( D3DXMATRIX& mView, D3DXMATRIX& mProj )
{
	if( m_pVertexShader == nullptr ) return;

	//ܰ��ލs��.
	D3DXMATRIX	mWorld;

	// ��]�s��,�@�ړ��s��.
	D3DXMATRIX mRot, mTran, mScale;

	// ��]�s��.
	D3DXMatrixRotationY( &mRot, m_fYaw );

	//���s�ړ�.
	D3DXMatrixTranslation( &mTran,
		m_vPosition.x, m_vPosition.y, m_vPosition.z );

	//�g��k���s��쐬.
	D3DXMatrixScaling(
		&mScale, //(out)
		1.0f, 1.0f, 1.0f );	// x, y, z.

	//�g�p���鼪��ނ̾��.
	m_pContext11->VSSetShader( m_pVertexShader, nullptr, 0 );//���_�����.
	m_pContext11->PSSetShader( m_pPixelShader, nullptr, 0 );//�߸�ټ����.

	// ���[���h�s��
	mWorld = mScale * mRot * mTran;

	//����ނ̺ݽ����ޯ̧�Ɋe���ް���n��.
	D3D11_MAPPED_SUBRESOURCE pData;
	C_BUFFER cb;	//�ݽ����ޯ̧.
	//�ޯ̧�����ް��̏��������J�n����Map.
	if (SUCCEEDED(m_pContext11->Map(
		m_pConstantBuffer, 0,
		D3D11_MAP_WRITE_DISCARD,
		0, &pData)))
	{
		//ܰ���,�ޭ�,��ۼު���ݍs���n��.
		D3DXMATRIX mWVP = mWorld * mView * mProj;
		D3DXMatrixTranspose(&mWVP, &mWVP);//�s���]�u����.
										  //���s��̌v�Z���@��DirectX��GPU�ňقȂ邽�ߓ]�u���K�v.

		cb.mWVP = mWVP;

		//�F�̐ݒ�.
		cb.vColor = D3DXVECTOR4( 0.5f, 0.0f, 0.5f, 0.4f );


		memcpy_s(
			pData.pData,	//��߰����ޯ̧.
			pData.RowPitch,	//��߰����ޯ̧����.
			(void*)(&cb),	//��߰�����ޯ̧.
			sizeof(cb));	//��߰�����ޯ̧����.

							//�ޯ̧�����ް��̏��������I������Unmap.
		m_pContext11->Unmap( m_pConstantBuffer, 0 );
	}

	//���̺ݽ����ޯ̧���ǂ̼���ނŎg�p���邩�H.
	m_pContext11->VSSetConstantBuffers( 0, 1, &m_pConstantBuffer );	//���_�����.
	m_pContext11->PSSetConstantBuffers( 0, 1, &m_pConstantBuffer );	//�߸�ټ����.

	//���_�ޯ̧���.
	UINT stride = sizeof( VERTEX );//�ް��̊Ԋu.
	UINT offset = 0;
	m_pContext11->IASetVertexBuffers( 0, 1,
		&m_pVertexBuffer, &stride, &offset );

	//���_���߯�ڲ��Ă��.
	m_pContext11->IASetInputLayout( m_pVertexLayout );
	m_pContext11->IASetIndexBuffer( g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0 );

	//����è�ށE���ۼް���.
	m_pContext11->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	SetBlend( true );
	//����è�ނ������ݸ�(�`��).
	m_pContext11->DrawIndexed( 36, 0, 0 );
	SetBlend( false );
}

HRESULT CBox::InitBlend()
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
		= false;	//true:��̧ĩ���ڰ�ނ��g�p����.
	BlendDesc.RenderTarget[0].BlendEnable
		= true;	//true:��̧�����ނ��g�p����.
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
void CBox::SetBlend( bool EnableAlpha )
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