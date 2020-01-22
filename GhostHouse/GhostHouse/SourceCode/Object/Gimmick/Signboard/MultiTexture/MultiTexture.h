#ifndef MULTI_TEXTURE_H
#define MULTI_TEXTURE_H

#include "..\..\..\..\Main\Global.h"

class CMultiTexture
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
		float			fFloorUV;
		int				FrameNum;	// �t���[����.
	};

	CMultiTexture( ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11,
		const char* fileName, const vector<SPRITE_STATE> pSs );	//�ݽ�׸�.
	~CMultiTexture();	//�޽�׸�.

	//�����ݸޗp.
	void Render( const bool isRender );

private:
	CMultiTexture();	//�ݽ�׸�.

	//������.
	HRESULT Init( ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11, 
		const char* fileName, const vector<SPRITE_STATE> pSs );

	//���.
	void Release();

	//����ލ쐬.
	HRESULT InitShader();
	//���ٍ쐬.
	HRESULT InitModel();
	// �e�N�X�`���쐬.
	HRESULT CreateTexture();

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
	// �[�x(Z)�e�X�gON/OFF�ؑւ�.
	void SetDeprh( bool flag );

	// �[�x(z)�e�X�g�ݒ�.
	ID3D11DepthStencilState*	m_pDepthStencilState;
	ID3D11DepthStencilState*	m_pDepthStencilStateOff;

	//�����؂ɂЂƂ�.
	ID3D11Device*			m_pDevice11;	//���޲���޼ު��.
	ID3D11DeviceContext*	m_pContext11;	//���޲���÷��.

	//�����ق̎�ނ��Ƃɗp��.
	ID3D11VertexShader*		m_pVertexShader1;	//���_�����.
	ID3D11VertexShader*		m_pVertexShader2;	//���_�����.
	ID3D11InputLayout*		m_pVertexLayout;	//���_ڲ���.
	ID3D11PixelShader*		m_pPixelShader;		//�߸�ټ����.
	ID3D11Buffer*			m_pConstantBuffer;	//�ݽ����ޯ̧.

												//�����ق��Ƃɗp��.
	ID3D11Buffer*			m_pVertexBuffer1;	//���_�ޯ̧.
	ID3D11Buffer*			m_pVertexBuffer2;	//���_�ޯ̧.

	ID3D11SamplerState*					m_pSampleLinear;	//�����:ø����Ɋe��̨����������.
	vector<ID3D11ShaderResourceView*>	m_pTextures;

	D3DXMATRIX m_mWorld;

	ID3D11BlendState*		m_pAlphaBlend;		//��̧�����ޗL��.
	ID3D11BlendState*		m_pNoAlphaBlend;	//��̧�����ޖ���.


	D3DXVECTOR3		m_vPos;		//�ʒu.
	float			m_Alpha;	// �A���t�@�l.

	vector<SPRITE_STATE>	m_SState;		// �X�v���C�g���.
};

#endif	// #ifndef MULTI_TEXTURE_H.