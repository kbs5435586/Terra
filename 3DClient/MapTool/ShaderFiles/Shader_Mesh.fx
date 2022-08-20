matrix		g_matWorld, g_matView, g_matProj;
vector		g_vLightDir;
vector		g_vLightDiffuse;
vector		g_vLightSpecular;
vector		g_vLightAmbient;
vector		g_vMtrlSpecular;
vector		g_vMtrlAmbient;
float		g_fPower;
vector		g_vCamPosition;
texture		g_DiffuseTexture;
int			g_iPick;

sampler DiffuseSampler = sampler_state
{
	texture = g_DiffuseTexture;
	minfilter = linear;
	magfilter = linear;
	mipfilter = linear;
};

struct VS_IN
{
	float3	vPosition : POSITION;
	float3	vNormal : NORMAL;
	float2	vTexUV : TEXCOORD;
};

struct VS_OUT
{
	vector	vPosition : POSITION;
	vector	vNormal : NORMAL;
	float2	vTexUV : TEXCOORD0;
	vector	vProjPos : TEXCOORD1;
};


VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT			Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_matWorld));
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;
	return Out;
}

struct PS_IN
{
	vector	vPosition : POSITION;
	vector	vNormal : NORMAL;
	float2	vTexUV : TEXCOORD0;
	vector	vProjPos : TEXCOORD1;
};


struct PS_OUT
{
	vector vDiffuseTex			: COLOR0;
	vector vNormalTex			: COLOR1;
	vector vDepthTex			: COLOR2;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	if (!g_iPick)
	{
		Out.vDiffuseTex = tex2D(DiffuseSampler, In.vTexUV);
		Out.vDiffuseTex.a = 1.f;
	}
	else
	{
		Out.vDiffuseTex = vector(1.f,0.f,0.f,1.f);
	}

	Out.vNormalTex = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
	Out.vDepthTex = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 500.0f, 0.f, 0.f);


	return Out;
}


technique	DefaultDevice
{
	pass NoneAlpha
	{
		AlphaBlendEnable = false;
		SrcBlend = one;
		DestBlend = one;

		CullMode = none;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}

}
