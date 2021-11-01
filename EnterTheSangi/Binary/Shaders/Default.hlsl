matrix		g_matWorld, g_matView, g_matProj;

texture		g_BaseTexture;

sampler BaseSampler = sampler_state
{
	AddressU = wrap;
	AddressV = wrap;
	texture = g_BaseTexture;
};

struct VS_IN
{
	float3	vPosition : POSITION;
	float2	vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4	vPosition : POSITION;
	float2	vTexUV : TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	// Out.vPosition = vector(In.vPosition, 1.f);
	Out.vTexUV = In.vTexUV;

	return Out;
}

struct PS_IN
{
	float4	vPosition : POSITION;
	float2	vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector	vColor : COLOR0;
};

// 픽셀 셰이더
// 1.픽셀의 색을 채운다.(결정한다)
// vector PS_MAIN(PS_IN In) : COLOR0
PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	// tex2D(색이들어있는 샘플러, 샘플러를 참조할 텍스쳐uv좌표);

	Out.vColor = tex2D(BaseSampler, In.vTexUV);

	/*if (Out.vColor.r < 0.5f &&
		Out.vColor.g < 0.5f &&
		Out.vColor.b < 0.5f)
	{
		Out.vColor = (vector)0.f;
		Out.vColor.a = 1.f;
	}*/



	return Out;
}


// technique : 기본 디바이스에서 빌드가 가능한 셰이더 버젼을 선택해주는 기능.
// : 사용하고자하는 패스의 지정.
// : 빌드하고자하는 셰이더의 버젼을 지정.
technique	Default_Device
{
	// 기능의 캡슐화.(명암 + 림라이트 + 그림자)
	pass Default_Rendering
	{
		AlphaBlendEnable = true;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		ZEnable = false;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}

	pass Default_Rendering
	{
		AlphaBlendEnable = false;
		ZEnable = false;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}
}
