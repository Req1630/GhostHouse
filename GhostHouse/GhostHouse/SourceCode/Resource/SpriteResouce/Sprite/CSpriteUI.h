#ifndef CSPRITE_UI_H
#define CSPRITE_UI_H

#include "..\..\..\Main\Global.h"

//�x���ɂ��Ă̺��ޕ��͂𖳌��ɂ���.4005:�Ē�`.
#pragma warning(disable:4005)

#include <D3DX11.h>
#include <D3D11.h>
#include <D3DX10.h>	//�uD3DX�`�v�̒�`�g�p���ɕK�v.
#include <D3D10.h>


//ײ���ؓǂݍ���.
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx10.lib")	//�uD3DX�`�v�̒�`�g�p���ɕK�v.




/**************************************************
*	���ײ�UI�׽.
**/
class CSpriteUI
{
public:
	// �������\����(float�^).
	struct WHDIZE_FLOAT
	{
		float w;
		float h;
	};
	// �X�v���C�g�\����.
	struct SPRITE_STATE
	{
		WHDIZE_FLOAT	Disp;		// �\����,����.
		WHDIZE_FLOAT	Base;		// ���摜�̕�,����.
		WHDIZE_FLOAT	Stride;		// 1�R�}������̕�,����.
		D3DXVECTOR3		vPos;		// ���W.
		int				FrameNum;	// �t���[����.
	};

	CSpriteUI();	//�ݽ�׸�.
	CSpriteUI( ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11,
		const char* fileName, const SPRITE_STATE& pSs );	//�ݽ�׸�.
	
	~CSpriteUI();	//�޽�׸�.

	//������.
	HRESULT Init( ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11, 
		const char* fileName, const SPRITE_STATE& pSs );

	//���.
	void Release();

	//�����ݸޗp.
	void Render();

	//UV�ʒu�ύX�\�����_�����O�p.
	void Render(float UV_x, float UV_y);

	// �[�x�e�X�g�Z�b�g.
	void SetDepth( bool flag );


	//�ʒu����ݒ�.
	void SetPosition( float x, float y, float z )
	{
		m_SState.vPos.x = x;
		m_SState.vPos.y = y;
		m_SState.vPos.z = z;
	}
	void SetPosition( const D3DXVECTOR3& vPos )
	{
		m_SState.vPos = vPos;
	}
	D3DXVECTOR3 GetPosition(){ return m_SState.vPos; }

	void SetPattern( LONG x, LONG y )
	{
		m_PatternNo.x = x;
		m_PatternNo.y = y;
	}

	void SetAlpha( float fAlpha )
	{ 
		m_Alpha = fAlpha; 
	}
	float GetAlpha() const { return m_Alpha; }

	void SetScale( float fScale ){ m_fScale = fScale; }

	float GetSpriteWidth(){ return m_SState.Base.w; }

private:
	//����ލ쐬.
	HRESULT InitShader();
	//���ٍ쐬.
	HRESULT InitModel();
	// �e�N�X�`���쐬.
	HRESULT CreateTexture( const char* fileName,
		ID3D11ShaderResourceView** pTexture );

	HRESULT InitBlend();
	void SetBlend( bool EnableAlpha );
	//======================================
	//	�\����.
	//======================================
	//�ݽ����ޯ̧�̱��ؑ��̒�`.
	//������ޓ��̺ݽ����ޯ̧�ƈ�v���Ă���K�v����.
	struct SHADER_CONSTANT_BUFFER
	{
		ALIGN16 D3DMATRIX	mW;				// ���[���h�s��.
		ALIGN16 float		fViewPortWidth;	// �r���[�|�[�g��.
		ALIGN16 float		fViewPortHeight;// �r���[�|�[�g����.
		ALIGN16 float		fAlpha;			// �A���t�@�l(���߂Ŏg�p����).
		ALIGN16 D3DXVECTOR2 vUV;			// UV���W.
	};
	//���_�̍\����.
	struct VERTEX
	{
		D3DXVECTOR3 Pos;	//���_���W.
		D3DXVECTOR2	Tex;	//ø������W.
	};

	//�����؂ɂЂƂ�.
	ID3D11Device*			m_pDevice11;	//���޲���޼ު��.
	ID3D11DeviceContext*	m_pContext11;	//���޲���÷��.

	//�����ق̎�ނ��Ƃɗp��.
	ID3D11VertexShader*		m_pVertexShader;	//���_�����.
	ID3D11InputLayout*		m_pVertexLayout;	//���_ڲ���.
	ID3D11PixelShader*		m_pPixelShader;		//�߸�ټ����.
	ID3D11Buffer*			m_pConstantBuffer;	//�ݽ����ޯ̧.

	//�����ق��Ƃɗp��.
	ID3D11Buffer*			m_pVertexBuffer;	//���_�ޯ̧.

	ID3D11SamplerState*		m_pSampleLinear;	//�����:ø����Ɋe��̨����������.
	ID3D11ShaderResourceView*	m_pTexture;		//ø���.

	int			m_AnimCount;	//����.
	D3DXVECTOR2	m_UV;		//ø���UV���W.

	ID3D11BlendState*		m_pAlphaBlend;		//��̧�����ޗL��.
	ID3D11BlendState*		m_pNoAlphaBlend;	//��̧�����ޖ���.

	// �[�x(z)�e�X�g�ݒ�.
	ID3D11DepthStencilState*	m_pDepthStencilState;
	ID3D11DepthStencilState*	m_pDepthStencilStateOff;


	D3DXVECTOR3		m_vPos;		//�ʒu.
	float			m_fScale;	// �X�P�[��.
	float			m_Alpha;	// �A���t�@�l.

	SPRITE_STATE	m_SState;		// �X�v���C�g���.
	POINT			m_PatternNo;	// �p�^�[���ԍ�.
};


#endif	// #ifndef CSPRITE_UI_H.