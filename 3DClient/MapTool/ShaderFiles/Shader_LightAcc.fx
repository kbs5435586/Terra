
vector			g_vLightDir;

vector			g_vLightPos;
float			g_fRange;

vector			g_vLightDiffuse;
vector			g_vLightAmbient;
vector			g_vLightSpecular;

vector			g_vMtrlDiffuse = (vector)1.f;
vector			g_vMtrlSpecular = (vector)1.f;
vector			g_vMtrlAmbient = (vector)1.f;
float			g_fPower = 30.f;

vector			g_vCamPosition;

matrix			g_matProjInv;
matrix			g_matViewInv;


texture			g_NormalTexture;

sampler	NormalSampler = sampler_state
{
	texture = g_NormalTexture;
};

texture			g_DepthTexture;

sampler	DepthSampler = sampler_state
{
	texture = g_DepthTexture;
};

struct PS_IN
{
	vector		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector		vShade : COLOR0;
	vector		vSpecular : COLOR1;
};

PS_OUT PS_MAIN_DIRECTIONAL(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	vector		vNormalInfo = tex2D(NormalSampler, In.vTexUV);
	vector		vDepthInfo = tex2D(DepthSampler, In.vTexUV);

	// 0 ~ 1 => -1 ~ 1
	vector		vNormal = vector(vNormalInfo.xyz * 2.f - 1.f, 0.f);

	Out.vShade = (max(dot(normalize(g_vLightDir) * -1.f, vNormal), 0.f) + (g_vLightAmbient * g_vMtrlAmbient)) * (g_vLightDiffuse * g_vMtrlDiffuse);
	Out.vShade.a = 1.f;

	vector		vReflect;
	vector		vLook;

	vReflect = reflect(normalize(g_vLightDir), normalize(vNormal));

	vector		vPosition;

	// 투영영역안에 있는 픽셀의 위치. ( /z)
	vPosition.x = In.vTexUV.x * 2.f - 1.f;
	vPosition.y = In.vTexUV.y * -2.f + 1.f;
	vPosition.z = vDepthInfo.r;
	vPosition.w = 1.f;

	// 투영행렬까지 곱해놓은 상태의 위치를 구하자.(* 투영행렬)
	float		fViewZ = vDepthInfo.g * 500.0f;

	vPosition = vPosition * fViewZ;

	// 뷰스페이스 영역상의 위치를 구한다.
	vPosition = mul(vPosition, g_matProjInv);

	// 월드 스페이스 영역상의 위치를 구한다.
	vPosition = mul(vPosition, g_matViewInv);

	vLook = vPosition - g_vCamPosition;

	Out.vSpecular = pow(max(dot(normalize(vLook) * -1.f, normalize(vReflect)), 0.f), 30.f) * (g_vLightSpecular * g_vMtrlSpecular);

	return Out;
}

PS_OUT PS_MAIN_POINT(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	vector		vNormalInfo = tex2D(NormalSampler, In.vTexUV);
	vector		vDepthInfo = tex2D(DepthSampler, In.vTexUV);

	// 0 ~ 1 => -1 ~ 1
	vector		vNormal = vector(vNormalInfo.xyz * 2.f - 1.f, 0.f);

	vector		vPosition;

	// 투영영역안에 있는 픽셀의 위치. ( /z)
	vPosition.x = In.vTexUV.x * 2.f - 1.f;
	vPosition.y = In.vTexUV.y * -2.f + 1.f;
	vPosition.z = vDepthInfo.r;
	vPosition.w = 1.f;

	// 투영행렬까지 곱해놓은 상태의 위치를 구하자.(* 투영행렬)
	float		fViewZ = vDepthInfo.g * 500.0f;

	vPosition = vPosition * fViewZ;

	// 뷰스페이스 영역상의 위치를 구한다.
	vPosition = mul(vPosition, g_matProjInv);

	// 월드 스페이스 영역상의 위치를 구한다.
	vPosition = mul(vPosition, g_matViewInv);

	vector		vLightDir = vPosition - g_vLightPos;



	float		fAtt = max((g_fRange - length(vLightDir)), 0.f) / g_fRange;

	Out.vShade = (max(dot(normalize(vLightDir) * -1.f, vNormal), 0.f) + (g_vLightAmbient * g_vMtrlAmbient)) * (g_vLightDiffuse * g_vMtrlDiffuse) * fAtt;
	Out.vShade.a = 1.f;

	vector		vReflect;
	vector		vLook;

	vReflect = reflect(normalize(vLightDir), normalize(vNormal));

	vLook = vPosition - g_vCamPosition;

	Out.vSpecular = pow(max(dot(normalize(vLook) * -1.f, normalize(vReflect)), 0.f), 30.f) * (g_vLightSpecular * g_vMtrlSpecular) * fAtt;
	Out.vSpecular.a = 0.f;

	return Out;
}

technique	DefaultDevice
{
	pass Render_Directional
	{
		AlphaBlendEnable = false;
		SrcBlend = one;
		DestBlend = one;


		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_MAIN_DIRECTIONAL();
	}

	pass Render_Point
	{
		ZEnable = false;
		ZWriteEnable = false;

		AlphaBlendEnable = true;
		SrcBlend = one;
		DestBlend = one;

		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_MAIN_POINT();
	}
}