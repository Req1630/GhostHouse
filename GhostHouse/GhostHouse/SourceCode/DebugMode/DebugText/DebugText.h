#ifndef TEXT_SPRITE_H
#define TEXT_SPRITE_H

//�x���ɂ��Ă̺��ޕ��͂𖳌��ɂ���.4005:�Ē�`.
#pragma warning(disable:4005)
#include <Windows.h>

#include <D3DX11.h>
#include <D3D11.h>
#include <D3DX10.h>	//�uD3DX�`�v�̒�`�g�p���ɕK�v.
#include <D3D10.h>

#include "..\..\Main\Global.h"
#include "..\..\SingletonBase\SingletonBase.h"

//ײ���ؓǂݍ���.
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx10.lib")	//�uD3DX�`�v�̒�`�g�p���ɕK�v.


// �f�o�b�O�e�L�X�g�N���X.
class CDebugText : CSingletonBase<CDebugText>
{
public:
	const float DIMENSION		= 12.0f;
	const float WDIMENSION		= 10.0f;
	const float TEX_DIMENSION	= 128.0f;

	//======================================
	//	�\����.
	//======================================
	//�ݽ����ޯ̧�̱��ؑ��̒�`.
	//������ޓ��̺ݽ����ޯ̧�ƈ�v���Ă���K�v����.
	struct SHADER_CONSTANT_BUFFER
	{
		D3DXMATRIX	mWVP;		//ܰ���,�ޭ�,��ۼު���݂̍����ϊ��s��.
		D3DXVECTOR4 vColor;		// �J���[(RGBA�̌^�ɍ��킹��).
		D3DXVECTOR4 fAlpha;		// �A���t�@�l(x �̂ݎg�p).
	};
	//���_�̍\����.
	struct VERTEX
	{
		D3DXVECTOR3 Pos;	//���_���W.
		D3DXVECTOR2	Tex;	//ø������W.
	};

	CDebugText();	// �R���X�g���N�^
	~CDebugText();	// �f�X�g���N�^.

					// �������֐�.
	static HRESULT Init( 
		ID3D11DeviceContext* pContext11,
		float Scale, 
		D3DXVECTOR4 vColor );
	static void SetPosition( const D3DXVECTOR3& vPos ){ GetInstance()->m_vPos = vPos; }

	// �����_�����O�֐�.
	static void Render( const string& stext, const float& fvalue );
	static void Render( const string& stext );
	
	static float GetScale(){ return GetInstance()->m_fScale*10.0f; };
private:
	//����ލ쐬.
	HRESULT InitShader();

	// �u�����h�X�e�[�g�쐬.
	HRESULT InitBlend();
	// �A���t�@�u�����h�ؑ֐ݒ�.
	void SetBlend( bool EnableAlpha );

	// �t�H���g�����_�����O�֐�.
	void RenderFont( int FontIndex, float x, float y );

	ID3D11Device*			m_pDevice11;	//���޲���޼ު��.
	ID3D11DeviceContext*	m_pContext11;	//���޲���÷��.

	ID3D11VertexShader*		m_pVertexShader;	//���_�����.
	ID3D11InputLayout*		m_pVertexLayout;	//���_ڲ���.
	ID3D11PixelShader*		m_pPixelShader;		//�߸�ټ����.
	ID3D11Buffer*			m_pConstantBuffer;	//�ݽ����ޯ̧.

	ID3D11Buffer*			m_pVertexBuffer[100];	//���_�ޯ̧.

	ID3D11SamplerState*			m_pSampleLinear;	//�����:ø����Ɋe��̨����������.
	ID3D11ShaderResourceView*	m_pAsciiTexture;	//ø���.


	ID3D11BlendState *m_pAlphaBlend;	// �A���t�@�u�����h�L��.
	ID3D11BlendState *m_pNoAlphaBlend;	// �A���t�@�u�����h����.

	DWORD	m_dwWindowWidth;	// �E�B���h�E��.	
	DWORD	m_dwWindowHeight;	// �E�B���h�E����.

	float		m_fKerning[100];	// �J�[�j���O.
	float		m_fAlpha;			// ���ߒl.
	float		m_fScale;			// �g�k�l(25pixel���25pixel = 1.0f).
	D3DXVECTOR4	m_vColor;			// �F.

	D3DXVECTOR3 m_vPos;	// �ʒu.
	D3DXMATRIX	m_mView;// �r���[�s��.
	D3DXMATRIX	m_mProj;// �v���W�F�N�V�����s��.

};

#endif // #ifndef CDEBUG_TEXT_H.