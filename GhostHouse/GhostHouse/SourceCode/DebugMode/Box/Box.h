#ifndef BOX_H
#define BOX_H

#include "..\..\Main\Global.h"

class CBox
{
public:
	CBox();
	~CBox();

	// �������֐�.
	HRESULT Init( 
		ID3D11Device* pDevice11, 
		ID3D11DeviceContext* pContext11, 
		const D3DXVECTOR3& vMaxPoint,
		const D3DXVECTOR3& vMinPoint );
	// �`��֐�.
	void Render( D3DXMATRIX& mView, D3DXMATRIX& mProj );

	// ���W�Z�b�g�֐�.
	void SetPosition( const D3DXVECTOR3& vPos ){ m_vPosition = vPos; }
	// Y����]�Z�b�g�֐�.
	void SetRotationYaw( const float& fYaw ){ m_fYaw = fYaw; }

private:
	// �V�F�[�_�[�쐬.
	HRESULT InitShader();
	// �L���[�u,�{�b�N�X�̍쐬.
	HRESULT InitCube( const D3DXVECTOR3& vMaxPoint, const D3DXVECTOR3& vMinPoint );
	// �u�����h�쐬.
	HRESULT InitBlend();

	// �u�����h�̗L������.
	void SetBlend( bool EnableAlpha );

	//�ݽ����ޯ̧�̱��ؑ��̒�`.
	//������ޓ��̺ݽ����ޯ̧�ƈ�v���Ă���K�v����.
	struct C_BUFFER
	{
		D3DXMATRIX	mWVP;		//ܰ���,�ޭ�,��ۼު���݂̍����ϊ��s��.	
		D3DXVECTOR4	vColor;		//�װ(RGBA�̌^�ɍ��킹��).
	};
	//���_�̍\����.
	struct VERTEX
	{
		D3DXVECTOR3 Pos;	//���_���W.
	};

	ID3D11Device*			m_pDevice11;
	ID3D11DeviceContext*	m_pContext11;

	//�����ق̎�ނ��Ƃɗp��.
	ID3D11VertexShader*		m_pVertexShader;	//���_�����.
	ID3D11InputLayout*		m_pVertexLayout;	//���_ڲ���.
	ID3D11PixelShader*		m_pPixelShader;		//�߸�ټ����.
	ID3D11Buffer*			m_pConstantBuffer;	//�ݽ����ޯ̧.

	ID3D11BlendState*		m_pAlphaBlend;		//��̧�����ޗL��.
	ID3D11BlendState*		m_pNoAlphaBlend;	//��̧�����ޖ���.

	ID3D11Buffer*			g_pIndexBuffer;
												//�����ق��Ƃɗp��.
	ID3D11Buffer*			m_pVertexBuffer;	//���_�ޯ̧.

	D3DXVECTOR3		m_vPosition;
	float			m_fYaw;

};

#endif	// #ifndef BOX_H.