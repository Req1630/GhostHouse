#ifndef CDX9MESH_H
#define CDX9MESH_H

// �x���ɂ��ẴR�[�h���͂𖳌��ɂ��� 4005:�Ē�`.
#pragma warning( disable:4005 )

#include <Windows.h>
#include <string>

#include "..\..\..\Main\Global.h"

#include <d3dx9.h>
#include <D3DX11.h>
#include <D3D11.h>
#include <D3DX10.h>	// �uD3DX�`�v�̒�`�g�p�ɕK�v.
#include <D3D10.h>

#pragma comment( lib, "d3d9.lib" )
#pragma comment( lib, "d3dx9.lib" )
#pragma comment( lib, "d3dx11.lib" )
#pragma comment( lib, "d3d11.lib" )
#pragma comment( lib, "d3dx10.lib" )

// �X�t�B�A�\����.
struct SPHERE
{
	D3DXVECTOR3 vCenter;	// ���S.
	float		fRadius;	// ���a.
};


// ���b�V���f�[�^���t�@�C��������o�����߂�����DirectX9���g�p����.
//�������_�����O(�`��)��DirectX11�ōs��.
class CDX9Mesh
{
public:
	CDX9Mesh(
		HWND hWnd,
		ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11,
		LPDIRECT3DDEVICE9 pDevice9,
		const char* fileName );
	~CDX9Mesh();// �f�X�g���N�^.

	// �����_�����O�p.
	virtual void Render( D3DXMATRIX& mView, D3DXMATRIX& mProj,
		Light& stLight, stCAMERA& stCamera, bool ChangeTexFlag = false );

	// �u�����h�̗L������.
	void SetBlend( bool EnableAlpha );
	void SetCoverage( bool EnableCoverage );

	// �ʒu����ݒ�.
	void SetPosition( const D3DXVECTOR3& vPos ){ m_vPos = vPos; }
	D3DXVECTOR3 GetPosition(){ return m_vPos; }

	// ��]����ݒ�.
	void SetRotation( const D3DXVECTOR3& vRot ){ m_vRot = vRot; }

	// ��]�����擾.
	D3DXVECTOR3 GetRotation(){ return m_vRot; }

	// �g�k����ݒ�.
	void SetScale( float fScale ) { m_fScale = fScale; }

	// �A���t�@�l�̐ݒ�.
	void SetAlpha( float fAlpha ) { m_fAlpha = fAlpha; }

	// ���C���[�t���[���ŕ\�����邩�ݒ�.
	void SetWireFlag( bool flag );

	// ���ʕ`��̐ݒ� : true�ŕ`�悵�Ȃ�.
	void SetCullBackFlag( bool flag );

	static D3DXVECTOR2 WorldtoScreenPos( const D3DXMATRIX& mView, 
		const D3DXMATRIX& mProj, const D3DXVECTOR3& pos );

	LPD3DXMESH GetMesh(){ return m_pMeshForRay; }

	SPHERE	m_Sphere;	// �X�t�B�A�\����.

	struct CHANGE_TEXTURES
	{
		CHAR szTextureName[512];				//�e�N�X�`���t�@�C����.
		ID3D11ShaderResourceView*	pTexture;	//�؂�ւ���e�N�X�`��.

		ID3D11ShaderResourceView*	pDefaultTexture;	//�֑ؑO�̃e�N�X�`���A�h���X.
		CHAR szTargetTextureName[512];					//�؂�ւ���Ώۂ̃e�N�X�`���t�@�C����.
		CHANGE_TEXTURES()
			: szTextureName			()
			, pTexture				( nullptr )
			, pDefaultTexture		( nullptr )
			, szTargetTextureName	()
		{}

	};

	// �O���œǂݍ��񂾃e�N�X�`�����Z�b�g����.
	void SetChangeTextures( shared_ptr<CHANGE_TEXTURES> pCT ){ m_pChangeTextures = pCT; }
protected:
	CDX9Mesh();	// �R���X�g���N�^.


	HRESULT Init(
		HWND hWnd,
		ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11,
		LPDIRECT3DDEVICE9 pDevice9,
		const char* fileName );
	
	// ���b�V���ǂݍ���.
	HRESULT LoadXMesh( const char* filename );
	// ����֐�.
	void Release();
	// ����ލ쐬.
	HRESULT InitShader();
	// �u�����h�쐬.
	HRESULT InitBlend();
	// ���C���[�t���[���`��p�̍쐬.
	HRESULT InitWtire();
	// ���ʕ`��̐ݒ�.
	HRESULT InitCullBack();

	HRESULT InitSphere( CDX9Mesh* pMesh );

	void ChangeTextureRender( int no );

	//======================================
	//	�\����.
	//======================================
	//�ݽ����ޯ̧�̱��ؑ��̒�`.
	//������ޓ��̺ݽ����ޯ̧�ƈ�v���Ă���K�v����.
	struct CBUFFER_PER_MESH
	{
		D3DXMATRIX	mW;			//���[���h�s��.
		D3DXMATRIX	mWVP;		//ܰ���,�ޭ�,��ۼު���݂̍����ϊ��s��.	
								//		D3DXVECTOR4 vLightDir;	//���C�g����.
								//		D3DXVECTOR4 vCamPos;	//�J�����ʒu(���_�ʒu).
								//		D3DXVECTOR4 vColor;		// �A���t�@�l(���߂Ŏg�p����).

	};

	struct cbPerObject
	{
		ALIGN16 D3DXMATRIX  WVP;
		ALIGN16 D3DXMATRIX	World;
		ALIGN16 float		fAlpha;
		ALIGN16 Light		light;
	};

	struct CBUFFER_PER_MATERIAL
	{
		D3DXVECTOR4 vAmbient;	//�A���r�G���g�F(���F).
		D3DXVECTOR4	vDiffuse;	//�f�B�t�[�Y�F(�g�U���ːF).
		D3DXVECTOR4 vSpecular;	//�X�y�L�����F(���ʔ��ːF).
								//		D3DXVECTOR4 vColor;		// �A���t�@�l(���߂Ŏg�p����).
	};
	//���_�̍\����.
	struct VERTEX
	{
		D3DXVECTOR3 Pos;	//���_���W.
		D3DXVECTOR3 Normal;	//�@��(�A�e�v�Z�ɕK�v).
		D3DXVECTOR2	Tex;	//ø������W.
	};
	// �}�e���A���\����.
	struct MY_MATERIAL
	{
		D3DXVECTOR4 Ambient;	// �A���r�G���g.
		D3DXVECTOR4 Diffuse;	// �f�B�t�[�Y.
		D3DXVECTOR4 Specular;	// �X�y�L����.
		char szTexTureName[64];	// �e�N�X�`���t�@�C����.
		ID3D11ShaderResourceView* pTexture;	// �e�N�X�`��.
		DWORD dwNumFace;		// ���̑��̃}�e���A���̃|���S����.
								// �R���X�g���N�^.
		MY_MATERIAL() 
			: Ambient		()
			, Diffuse		()
			, Specular		()
			, szTexTureName	()
			, pTexture		( nullptr )
			, dwNumFace		(0)
		{}
		// �f�X�g���N�^.
		~MY_MATERIAL()
		{
			SAFE_RELEASE( pTexture );
		}
	};

	HWND				m_hWnd;		// �E�B���h�E�n���h��.
	// Dx9
	LPDIRECT3DDEVICE9	m_pDevice9;	// Dx9�f�o�C�X�I�u�W�F�N�g.
	
	// Dx11
	ID3D11Device*			m_pDevice11;	//���޲���޼ު��.
	ID3D11DeviceContext*	m_pContext11;	//���޲���÷��.

											//�����ق̎�ނ��Ƃɗp��.
	ID3D11VertexShader*		m_pVertexShader;	//���_�����.
	ID3D11InputLayout*		m_pVertexLayout;	//���_ڲ���.
	ID3D11PixelShader*		m_pPixelShader;		//�߸�ټ����.
	ID3D11Buffer*			m_pCBufferPerMesh;		//�ݽ����ޯ̧(���b�V����).
	ID3D11Buffer*			m_pCBufferPerMaterial;	//�ݽ����ޯ̧(�}�e���A����).

	ID3D11Buffer*			m_pCBufferPerFrame;	//�ݽ����ޯ̧(�}�e���A����).

	ID3D11Buffer*			m_pVertexBuffer;	// ���_�o�b�t�@.
	ID3D11Buffer**			m_ppIndexBuffer;	// �C���f�b�N�X�o�b�t�@.
	ID3D11SamplerState*		m_pSampleLinear;	//�����:ø����Ɋe��̨����������.

	ID3D11BlendState*		m_pAlphaBlend;		//��̧�����ޗL��.
	ID3D11BlendState*		m_pNoAlphaBlend;	//��̧�����ޖ���.
	ID3D11BlendState*		m_CoverageBlend;	//��̧ĩ���ڰ�ޗL��.

	ID3D11RasterizerState*	m_RasterizerWireON;
	ID3D11RasterizerState*	m_RasterizerWireOFF;

	ID3D11RasterizerState*	m_RasterizerCullBackON;
	ID3D11RasterizerState*	m_RasterizerCullBackOFF;


	LPD3DXMESH	m_pMesh;			// ���b�V���I�u�W�F�N�g.
	LPD3DXMESH	m_pMeshForRay;		// ���C�ƃ��b�V���p.
	DWORD		m_NumMaterials;		// �}�e���A����.

	MY_MATERIAL*	m_pMaterials;	// �}�e���A���\����.
	DWORD			m_NumAttr;		// ������.
	DWORD			m_AttrID[300];	// ����ID ��300�����܂�.

	bool			m_EnableTexture;// �e�N�X�`������.

	shared_ptr<CHANGE_TEXTURES> m_pChangeTextures;	// �}���`�e�N�X�`���p.

private:
	Light light;

	float			m_fScale;		// �g�k�l( x, y, z���{ ).
	D3DXVECTOR3		m_vRot;			// ��]�l( x, y z ).
									// ��	 x=Pitch, y=Yaw, z=Roll.
	D3DXVECTOR3		m_vPos;			// �ʒu( x, y, z ).
	float			m_fAlpha;		// �A���t�@�l.


};
#endif // #ifndef CDX9MESH_H.