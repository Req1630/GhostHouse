#include "DebugText.h"
#include <crtdbg.h>

//�����̧�ٖ�.
const char SHADER_NAME[] = "Data\\Shader\\DebugText.hlsl";

//�ݽ�׸�.
CDebugText::CDebugText()
	: m_pDevice11( nullptr )
	, m_pContext11( nullptr )
	, m_pVertexShader( nullptr )
	, m_pVertexLayout( nullptr )
	, m_pPixelShader( nullptr )
	, m_pConstantBuffer( nullptr )
	, m_pVertexBuffer()
	, m_pSampleLinear( nullptr )
	, m_pAsciiTexture( nullptr )
	, m_pAlphaBlend( nullptr )
	, m_pNoAlphaBlend( nullptr )
	, m_fKerning()
	, m_fAlpha( 0.0f )
	, m_fScale( 1.0f )
	, m_vColor( 0.0f, 0.0f, 0.0f, 0.0f )

	, m_vPos()
{
}

//�޽�׸�.
CDebugText::~CDebugText()
{
}

//�������֐�.
HRESULT CDebugText::Init(
	ID3D11DeviceContext* pContext11,
	float Scale, D3DXVECTOR4 vColor )
{
	GetInstance()->m_fAlpha = vColor.w;
	GetInstance()->m_vColor = vColor;
	GetInstance()->m_fScale = Scale / 25.0f;

	//95�������J��Ԃ�.
	for( int i = 0; i < 95; i++ ){
		GetInstance()->m_fKerning[i] = 10.0f;
	}
	//���޲���÷�Ă��߰.
	GetInstance()->m_pContext11 = pContext11;
	//���޲���÷�Ă������޲����擾.
	GetInstance()->m_pContext11->GetDevice( &GetInstance()->m_pDevice11 );

	//Windows����.
	GetInstance()->m_dwWindowHeight = WND_H;
	GetInstance()->m_dwWindowWidth = WND_W;

	//===̫�Ă��Ƃɸ����(��`)�쐬===.
	float left = 0.0f, top = 0.0f, right = 0.0f, bottom = 0.0f;
	int cnt = 0;
	//2�dٰ�߂�1�������w�肷��.
	for( int k = 0; k < 10; k++ ){
		for( int i = 0; i < 10; i++ ){
			left = k * GetInstance()->WDIMENSION;
			top = i * GetInstance()->DIMENSION;
			right = left + GetInstance()->m_fKerning[cnt];
			bottom = top + GetInstance()->DIMENSION;

			left /= GetInstance()->TEX_DIMENSION;
			top /= GetInstance()->TEX_DIMENSION;
			right /= GetInstance()->TEX_DIMENSION;
			bottom /= GetInstance()->TEX_DIMENSION;

			VERTEX vertices[] =
			{
				D3DXVECTOR3( 0.0f,       GetInstance()->DIMENSION, 0.0f ),	D3DXVECTOR2( left,  bottom ),
				D3DXVECTOR3( 0.0f,       0.0f,      0.0f ),	D3DXVECTOR2( left,  top ),
				D3DXVECTOR3( GetInstance()->WDIMENSION, GetInstance()->DIMENSION, 0.0f ),	D3DXVECTOR2( right, bottom ),
				D3DXVECTOR3( GetInstance()->WDIMENSION, 0.0f,      0.0f ),	D3DXVECTOR2( right, top )
			};
			//�ő�v�f�����Z�o����.
			UINT uVerMax = sizeof( vertices ) / sizeof( vertices[0] );

			//�ޯ̧�\����.
			D3D11_BUFFER_DESC bd;
			bd.Usage = D3D11_USAGE_DEFAULT;	//�g�p���@(��̫��).
			bd.ByteWidth = sizeof( VERTEX ) * uVerMax;//���_�̻���.
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;//���_�ޯ̧�Ƃ��Ĉ���.
			bd.CPUAccessFlags = 0;	//CPU����ͱ������Ȃ�.
			bd.MiscFlags = 0;		//���̑����׸�(���g�p).
			bd.StructureByteStride = 0;	//�\���̻̂���(���g�p).

										//���ؿ���ް��\����.
			D3D11_SUBRESOURCE_DATA InitData;
			InitData.pSysMem = vertices;	//����؂̒��_���.

											//���_�ޯ̧�̍쐬.
			if( FAILED( GetInstance()->m_pDevice11->CreateBuffer(
				&bd, &InitData, &GetInstance()->m_pVertexBuffer[cnt] ) ) ){
				_ASSERT_EXPR( false, L"���_�ޯ̧�쐬���s" );
				return E_FAIL;
			}
			cnt++;
		}
	}

	//ø����p�̻���׍\����.
	D3D11_SAMPLER_DESC samDesc;
	ZeroMemory( &samDesc, sizeof( samDesc ) );
	samDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;//POINT:���������e��.
	samDesc.AddressU
		= D3D11_TEXTURE_ADDRESS_WRAP;//ׯ��ݸ�Ӱ��(WRAP:�J��Ԃ�).
	samDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	//MIRROR: ���]�J��Ԃ�.
	//CLAMP : �[�̖͗l�������L�΂�.
	//BORDER: �ʓr���E�F�����߂�.
	//����׍쐬.
	if( FAILED( GetInstance()->m_pDevice11->CreateSamplerState(
		&samDesc, &GetInstance()->m_pSampleLinear ) ) )//(out)�����.
	{
		_ASSERT_EXPR( false, L"����׍쐬���s" );
		return E_FAIL;
	}

	//ø����쐬.
	if( FAILED( D3DX11CreateShaderResourceViewFromFile(
		GetInstance()->m_pDevice11,	//ؿ�����g�p�������޲����߲��.
		"Data\\DebugText\\ascii.png",	//̧�ٖ�.
		nullptr, nullptr,
		&GetInstance()->m_pAsciiTexture,	//(out)ø���.
		nullptr ) ) ){
		_ASSERT_EXPR( false, L"ø����쐬���s" );
		return E_FAIL;
	}

	//����ލ쐬.
	if( FAILED( GetInstance()->InitShader() ) ){
		return E_FAIL;
	}
	//�����޽ðč쐬.
	if( FAILED( GetInstance()->InitBlend() ) ){
		return E_FAIL;
	}

	return S_OK;
}


//===========================================================
//	HLSĻ�ق�ǂݍ��ݼ���ނ��쐬����.
//	HLSL: High Level Shading Language �̗�.
//===========================================================
HRESULT CDebugText::InitShader()
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
		},
		{
			"TEXCOORD",						//ø����ʒu.
			0,
			DXGI_FORMAT_R32G32_FLOAT,		//DXGI��̫�ϯ�(32bit float�^*2).
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,	//�ް��̊J�n�ʒu.
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
	cb.ByteWidth = sizeof( SHADER_CONSTANT_BUFFER );//�ݽ����ޯ̧�̻���.
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

HRESULT CDebugText::InitBlend()
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
void CDebugText::SetBlend( bool EnableAlpha )
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

//�����ݸފ֐�.
void CDebugText::Render( const string& stext, const float& fvalue )
{
//#ifdef _DEBUG
	string Text = stext + to_string(fvalue);

	//�ޭ���ݽ̫��.
	D3DXVECTOR3 vEye( 0.0f, 0.0f, -1.0f );
	D3DXVECTOR3 vLook( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 vUp( 0.0f, 1.0f, 0.0f );
	D3DXMatrixLookAtLH(
		&GetInstance()->m_mView, &vEye, &vLook, &vUp );
#if 0
	//��ۼު������ݽ̫��(�Œ�).
	D3DMATRIX mOtho = {
		2.0f / static_cast<float>( m_dwWindowWidth ), 0.0f,0.0f,0.0f,
		0.0f, -2.0f / static_cast<float>( m_dwWindowHeight ),0.0f,0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f, 1.0f
	};
#else
	D3DXMATRIX mOtho;
	D3DXMatrixIdentity( &mOtho );//�P�ʍs��쐬.
	mOtho._11 = 2.0f / static_cast<float>( GetInstance()->m_dwWindowWidth );
	mOtho._22 = -2.0f / static_cast<float>( GetInstance()->m_dwWindowHeight );
	mOtho._41 = -1.0f;
	mOtho._42 = 1.0f;
#endif
	GetInstance()->m_mProj = mOtho;

	//����è�ށE���ۼް.
	GetInstance()->m_pContext11->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );
	//���_���߯�ڲ��Ă��.
	GetInstance()->m_pContext11->IASetInputLayout( GetInstance()->m_pVertexLayout );

	//�g�p���鼪��ނ̓o�^.
	GetInstance()->m_pContext11->VSSetShader( GetInstance()->m_pVertexShader, nullptr, 0 );
	GetInstance()->m_pContext11->PSSetShader( GetInstance()->m_pPixelShader, nullptr, 0 );

	//�ݽ����ޯ̧��o�^.
	GetInstance()->m_pContext11->VSSetConstantBuffers( 0, 1, &GetInstance()->m_pConstantBuffer );
	GetInstance()->m_pContext11->PSSetConstantBuffers( 0, 1, &GetInstance()->m_pConstantBuffer );

	//����ׂ�ø������߸�ټ���ނɓn��.
	GetInstance()->m_pContext11->PSSetSamplers( 0, 1, &GetInstance()->m_pSampleLinear );
	GetInstance()->m_pContext11->PSSetShaderResources( 0, 1, &GetInstance()->m_pAsciiTexture );

	//��������ٰ��.
	for( int i = 0; i < static_cast<int>(strlen( Text.c_str() )); i++ ){
		char font = Text[i];
		int index = font - 32;	//̫�Ĳ��ޯ���쐬.

								//̫�������ݸ�.
		GetInstance()->RenderFont( index, GetInstance()->m_vPos.x, GetInstance()->m_vPos.y );
		GetInstance()->m_vPos.x += GetInstance()->m_fKerning[index]*GetInstance()->m_fScale;
	}
//#endif	// #ifdef _DEBUG.
}

void CDebugText::Render( const string& stext )
{
//#ifdef _DEBUG
	string Text = stext;

	//�ޭ���ݽ̫��.
	D3DXVECTOR3 vEye( 0.0f, 0.0f, -1.0f );
	D3DXVECTOR3 vLook( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 vUp( 0.0f, 1.0f, 0.0f );
	D3DXMatrixLookAtLH(
		&GetInstance()->m_mView, &vEye, &vLook, &vUp );
#if 0
	//��ۼު������ݽ̫��(�Œ�).
	D3DMATRIX mOtho = {
		2.0f / static_cast<float>( m_dwWindowWidth ), 0.0f,0.0f,0.0f,
		0.0f, -2.0f / static_cast<float>( m_dwWindowHeight ),0.0f,0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f, 1.0f
	};
#else
	D3DXMATRIX mOtho;
	D3DXMatrixIdentity( &mOtho );//�P�ʍs��쐬.
	mOtho._11 = 2.0f / static_cast<float>( GetInstance()->m_dwWindowWidth );
	mOtho._22 = -2.0f / static_cast<float>( GetInstance()->m_dwWindowHeight );
	mOtho._41 = -1.0f;
	mOtho._42 = 1.0f;
#endif
	GetInstance()->m_mProj = mOtho;

	//����è�ށE���ۼް.
	GetInstance()->m_pContext11->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );
	//���_���߯�ڲ��Ă��.
	GetInstance()->m_pContext11->IASetInputLayout( GetInstance()->m_pVertexLayout );

	//�g�p���鼪��ނ̓o�^.
	GetInstance()->m_pContext11->VSSetShader( GetInstance()->m_pVertexShader, nullptr, 0 );
	GetInstance()->m_pContext11->PSSetShader( GetInstance()->m_pPixelShader, nullptr, 0 );

	//�ݽ����ޯ̧��o�^.
	GetInstance()->m_pContext11->VSSetConstantBuffers( 0, 1, &GetInstance()->m_pConstantBuffer );
	GetInstance()->m_pContext11->PSSetConstantBuffers( 0, 1, &GetInstance()->m_pConstantBuffer );

	//����ׂ�ø������߸�ټ���ނɓn��.
	GetInstance()->m_pContext11->PSSetSamplers( 0, 1, &GetInstance()->m_pSampleLinear );
	GetInstance()->m_pContext11->PSSetShaderResources( 0, 1, &GetInstance()->m_pAsciiTexture );

	//��������ٰ��.
	for( int i = 0; i < static_cast<int>( strlen( Text.c_str() ) ); i++ ){
		char font = Text[i];
		int index = font - 32;	//̫�Ĳ��ޯ���쐬.

								//̫�������ݸ�.
		GetInstance()->RenderFont( index, GetInstance()->m_vPos.x, GetInstance()->m_vPos.y );
		GetInstance()->m_vPos.x += GetInstance()->m_fKerning[index] * GetInstance()->m_fScale;
	}
//#endif	// #ifdef _DEBUG.
}

//̫�������ݸފ֐�.
void CDebugText::RenderFont( int FontIndex, float x, float y )
{
	//ܰ��ޕϊ�.
	D3DXMATRIX mWorld, mScale, mTran;
	D3DXMatrixIdentity( &mWorld );
	D3DXMatrixTranslation( &mTran,
		x,  y, -100.0f );

	//�g��k���s��쐬.
	D3DXMatrixScaling(
		&mScale, //(out)
		m_fScale, m_fScale, m_fScale );	// x, y, z.

	mWorld = mScale * mTran;

	//����ނ̺ݽ����ޯ̧�Ɋe���ް���n��.
	D3D11_MAPPED_SUBRESOURCE pData;
	SHADER_CONSTANT_BUFFER cb;	//�ݽ����ޯ̧.
								//�ޯ̧�����ް��̏��������J�n����map.
	if( SUCCEEDED(
		m_pContext11->Map( m_pConstantBuffer,
			0, D3D11_MAP_WRITE_DISCARD, 0, &pData ) ) ){
		//ܰ���,�ޭ�,��ۼު���ݍs���n��.
		D3DXMATRIX m = mWorld * m_mView * m_mProj;
		D3DXMatrixTranspose( &m, &m );//�s���]�u����.
		cb.mWVP = m;

		//�װ.
		cb.vColor = m_vColor;

		//���ߒl.
		cb.fAlpha.x = m_fAlpha;

		memcpy_s( pData.pData, pData.RowPitch,
			(void*)( &cb ), sizeof( cb ) );

		m_pContext11->Unmap( m_pConstantBuffer, 0 );
	}

	//���_�ޯ̧���.
	UINT stride = sizeof( VERTEX );//�ް��̊Ԋu.
	UINT offset = 0;
	m_pContext11->IASetVertexBuffers( 0, 1,
		&m_pVertexBuffer[FontIndex], &stride, &offset );


	//�`��.
	SetBlend( true );
	m_pContext11->Draw( 4, 0 );
	SetBlend( false );
}