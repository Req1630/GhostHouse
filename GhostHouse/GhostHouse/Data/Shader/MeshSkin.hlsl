/***************************************************************************************************
*	SkinMeshCode Version 1.10
*	LastUpdate	: 2016/06/29
**/
// ��`.
#define MAX_BONE_MATRICES 255

// �O���[�o��.
Texture2D		g_texColor	: register( t0 );	// �e�N�X�`���[�� ���W�X�^�[t(n)
SamplerState	g_samLinear : register( s0 );	// �T���v���[�̓��W�X�^�[s(n)

struct Light
{
	float3 pos;
	float  range;
	float3 dir;
	float cone;
	float3 att;
	float4 ambient;
	float4 diffuse;
	matrix rot;
	float4 g_vColor;
};

cbuffer cbPerObject	: register( b0 )
{
	matrix g_mWor;		// ���[���h�s��.
	matrix g_mWVP2;		// ���[���h����ˉe�܂ł̕ϊ��s��.
	Light light			/*: packoffset(c3)*/;
};

cbuffer global_1	: register( b1 )
{
	matrix g_mW;		// ���[���h�s��.
	matrix g_mWVP;		// ���[���h����ˉe�܂ł̕ϊ��s��.
	float4 g_Ambient = float4( 0, 0, 0, 0 );	// �A���r�G���g��.
	float4 g_Diffuse = float4( 1, 0, 0, 0 );	// �g�U����(�F�j
	float4 g_Specular = float4( 1, 1, 1, 1 );	// ���ʔ���.
};

cbuffer global_bones: register( b2 )//�{�[���̃|�[�Y�s�񂪓���.
{
	matrix g_mConstBoneWorld[MAX_BONE_MATRICES];
};

// �X�L�j���O��̒��_�E�@��������.
struct Skin
{
	float4 Pos;
	float3 Norm;
};
// �o�[�e�b�N�X�o�b�t�@�[�̓���.
struct VSSkinIn
{
	float3 Pos		: POSITION;		// �ʒu.  
	float3 Norm		: NORMAL;		// ���_�@��.
	float2 Tex		: TEXCOORD;		// �e�N�X�`���[���W.
	uint4  Bones	: BONE_INDEX;	// �{�[���̃C���f�b�N�X.
	float4 Weights	: BONE_WEIGHT;	// �{�[���̏d��.
};

//�s�N�Z���V�F�[�_�[�̓��́i�o�[�e�b�N�X�o�b�t�@�[�̏o�́j�@
struct PSSkinIn
{
	float4 Pos	: SV_Position;	// �ʒu.
	float3 Norm : NORMAL;		// ���_�@��.
	float2 Tex	: TEXCOORD;		// �e�N�X�`���[���W.

	float4 worldPos : POSITION;

	float4 Color: COLOR0;		// �ŏI�J���[�i���_�V�F�[�_�[�ɂ����Ắj
};


//�w�肵���ԍ��̃{�[���̃|�[�Y�s���Ԃ��@�T�u�֐��i�o�[�e�b�N�X�V�F�[�_�[�Ŏg�p�j
matrix FetchBoneMatrix( uint iBone )
{
	return g_mConstBoneWorld[iBone];
}


//���_���X�L�j���O�i�{�[���ɂ��ړ��j����B�T�u�֐��i�o�[�e�b�N�X�V�F�[�_�[�Ŏg�p�j
Skin SkinVert( VSSkinIn Input )
{
	Skin Output = (Skin)0;

	float4 Pos = float4( Input.Pos, 1 );
	float3 Norm = Input.Norm;
	// �{�[��0
	uint iBone = Input.Bones.x;
	float fWeight = Input.Weights.x;
	matrix m = FetchBoneMatrix( iBone );
	Output.Pos += fWeight * mul( Pos, m );
	Output.Norm += fWeight * mul( Norm, ( float3x3 )m );
	// �{�[��1
	iBone = Input.Bones.y;
	fWeight = Input.Weights.y;
	m = FetchBoneMatrix( iBone );
	Output.Pos += fWeight * mul( Pos, m );
	Output.Norm += fWeight * mul( Norm, ( float3x3 )m );
	// �{�[��2
	iBone = Input.Bones.z;
	fWeight = Input.Weights.z;
	m = FetchBoneMatrix( iBone );
	Output.Pos += fWeight * mul( Pos, m );
	Output.Norm += fWeight * mul( Norm, ( float3x3 )m );
	// �{�[��3
	iBone = Input.Bones.w;
	fWeight = Input.Weights.w;
	m = FetchBoneMatrix( iBone );
	Output.Pos += fWeight * mul( Pos, m );
	Output.Norm += fWeight * mul( Norm, ( float3x3 )m );

	return Output;
}

// �o�[�e�b�N�X�V�F�[�_.
PSSkinIn VSSkin( VSSkinIn input )
{
	PSSkinIn output = (PSSkinIn)0;
	Skin vSkinned = SkinVert( input );

	output.Pos = mul( vSkinned.Pos, g_mWVP );
	output.Norm = normalize( mul( vSkinned.Norm, ( float3x3 )g_mW ) );
	output.Tex = input.Tex;

	output.worldPos = mul( vSkinned.Pos, g_mW );

	//float3 LightDir	= normalize( g_vLight );
	//float3 PosWorld	= mul( vSkinned.Pos, g_mW );
	//float3 ViewDir	= normalize( g_vEye - PosWorld );
	//float3 Normal	= normalize( output.Norm );
	//float4 NL		= saturate( dot( Normal, LightDir ) );

	//float3 Reflect	= normalize( 2 * NL * Normal - LightDir );
	//float4 specular = pow( saturate( dot( Reflect, ViewDir ) ), 4 ); 

	//output.Color	= g_Diffuse * NL + specular * g_Specular;

	return output;
}


// �s�N�Z���V�F�[�_.
float4 PSSkin( PSSkinIn input ) : SV_Target
{
	input.Norm = normalize( input.Norm );

	float4 diffuse = g_texColor.Sample( g_samLinear, input.Tex );

	float3 finalColor = float3( 0.0f, 0.0f, 0.0f );

	//Create the vector between light position and pixels position
	float3 lightToPixelVec = light.pos - input.worldPos;

	//Find the distance between the light pos and pixel pos
	float d = length( lightToPixelVec );

	//Add the ambient light
	float3 finalAmbient = diffuse * light.ambient;

	//If pixel is too far, return pixel color with ambient light
	if( d > light.range )
		return float4( finalAmbient, diffuse.a );

	//Turn lightToPixelVec into a unit length vector describing
	//the pixels direction from the lights position
	lightToPixelVec /= d;

	//Calculate how much light the pixel gets by the angle
	//in which the light strikes the pixels surface
	float howMuchLight = dot( lightToPixelVec, input.Norm );

	//If light is striking the front side of the pixel
	if( howMuchLight > 0.0f ){
		//Add light to the finalColor of the pixel
		finalColor += diffuse * light.diffuse;

		//Calculate Light's Distance Falloff factor
		finalColor /= ( light.att[0] + ( light.att[1] * d ) ) + ( light.att[2] * ( d*d ) );

		//Calculate falloff from center to edge of pointlight cone
		//finalColor *= pow(max(dot(-lightToPixelVec, light.dir), 0.0f), light.cone);
		finalColor *= pow( max( dot( -lightToPixelVec, light.dir ), 0.0f ), light.cone );
	}

	//make sure the values are between 1 and 0, and add the ambient
	finalColor = saturate( ( finalColor / 5 ) + finalAmbient );

	//Return Final Color
	return float4( finalColor, diffuse.a );



	//// �e�N�X�`������s�N�Z���F�����o��.
	//float4 TexDiffuse = g_texColor.Sample( g_samLinear, input.Tex );
	//// �o�͗p���쐬.
	//float4 RetColor = input.Color/2 + TexDiffuse/2;

	//// �e�N�X�`���̃��l�����̂܂܎g�p�B
	//RetColor.a = TexDiffuse.a;

	//return RetColor;
}

float4 D2D_PS( PSSkinIn input ) : SV_Target
{
	float4 diffuse = g_texColor.Sample( g_samLinear, input.Tex );

	return diffuse;
}