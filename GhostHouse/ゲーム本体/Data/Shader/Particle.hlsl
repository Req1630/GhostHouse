//��۰���.

// �e�N�X�`��.
Texture2D		g_Texture	: register(t0);
// �T���v��.
SamplerState	g_Sampler	: register(s0);

//�ݽ����ޯ̧.
cbuffer global
{
	matrix	g_WVP;	//ܰ���,�ޭ�,��ۼު���݂̕ϊ��s��.
};

//�\����.
struct VS_OUTPUT
{
	float4	Pos	: SV_Position;
};

struct GS_OUTPUT
{
	float4	Pos : SV_Position;
	float2	UV	: TEXCOORD;
};

//���_�����.
VS_OUTPUT VS_Main(float4 Pos : POSITION)
{
	VS_OUTPUT output;
	output.Pos = Pos;//�����ł͒��_��ϊ����Ȃ��ł��̂܂ܼ޵��ؼ���ނɓn��.

	return output;
}

//�޵��ؼ����.
//����غ�݂𕪊�.
[maxvertexcount(4)]//���_�̍ő吔.
void GS_Point(point VS_OUTPUT Input[1],
	inout TriangleStream<GS_OUTPUT> TriStream)
	//TriangleStream:�޵��ؼ���ނō쐬�����O�p�`.
{
	float w = 0;//4�̒��_�S�Ă�w�l��1�Ԗ�[0]�̒��_��w�ɍ��킹��.
				//���킹��͉̂��Ԗڂł������B�S�Ă������Ȃ�.
				//��غ�݂��߰����|�����Ă��܂��̂�h�~.

	// �l�p�`�̒��_.
	GS_OUTPUT output = (GS_OUTPUT)0;
	output.Pos = mul(Input[0].Pos, g_WVP);
	w = output.Pos.w;
	output.UV = float2( 0.0f, 0.0f );
	TriStream.Append(output);//Append():���_�̒ǉ�.


	output.Pos = Input[0].Pos + float4(0.5f, 0.0f, 0.0f, 0.0f);
	output.Pos = mul(output.Pos, g_WVP);
	output.Pos.w = w;
	output.UV = float2( 1.0f, 0.0f );
	TriStream.Append(output);

	output.Pos = Input[0].Pos + float4(0.0f, -0.5f, 0.0f, 0.0f);
	output.Pos = mul(output.Pos, g_WVP);
	output.Pos.w = w;
	output.UV = float2( 0.0f, 1.0f );
	TriStream.Append(output);

	output.Pos = Input[0].Pos + float4(0.5f, -0.5f, 0.0f, 0.0f);
	output.Pos = mul(output.Pos, g_WVP);
	output.Pos.w = w;
	output.UV = float2( 1.0f, 1.0f );
	TriStream.Append(output);

	TriStream.RestartStrip();//3�̒��_��ǉ�������A�Ăяo���Đݒ�.
}

//�߸�ټ����.
float4 PS_Main(GS_OUTPUT Input) : SV_Target
{
	// �e�N�X�`����\��.
	return g_Texture.Sample( g_Sampler, Input.UV );
}