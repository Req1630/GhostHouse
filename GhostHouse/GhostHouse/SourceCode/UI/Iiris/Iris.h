#ifndef CWIPE_EFFECT_UI_H
#define CWIPE_EFFECT_UI_H

#include "..\..\Main\Global.h"

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
class CWipeEffectUI
{
public:
	CWipeEffectUI();	//�ݽ�׸�.
	CWipeEffectUI( ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11 );	//�ݽ�׸�.

	~CWipeEffectUI();	//�޽�׸�.

	//������.
	HRESULT Init( ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11 );

	//���.
	void Release();

	//�����ݸޗp.
	void Render();

	// �~�̒��S�ɂȂ鑊��̍��W.
	void SetTargetPos( D3DXVECTOR2 tarpos ){ m_vTargetPos = tarpos; };

	// ���C�v�G�t�F�N�g�̊J�n�ƏI��.
	void SetStart(){ m_isStart = true; }
	void SetEnd(){ m_isEnd = true; }

	bool isActive(){ return m_isRender; }

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

	// �[�x�e�X�g�Z�b�g.
	void SetDepth( bool flag );

	void Update();
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
		ALIGN16 float		fRadius;		// �~�̔��a.
		ALIGN16 D3DXVECTOR2 vTargetPos;		// �W�I�̍��W.
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

	ID3D11BlendState*		m_pAlphaBlend;		//��̧�����ޗL��.
	ID3D11BlendState*		m_pNoAlphaBlend;	//��̧�����ޖ���.

	// �[�x(z)�e�X�g�ݒ�.
	ID3D11DepthStencilState*	m_pDepthStencilState;
	ID3D11DepthStencilState*	m_pDepthStencilStateOff;

	D3DXVECTOR3		m_vPos;		//�ʒu.
	D3DXVECTOR2		m_vTargetPos;
	float			m_fRadius;

	bool			m_isStart;
	bool			m_isEnd;
	bool			m_isRender;
};


#endif	// #ifndef CWIPE_EFFECT_UI_H.