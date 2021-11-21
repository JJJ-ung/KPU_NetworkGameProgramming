matrix		g_matWorld, g_matView, g_matProj;

float3		g_vCloth, g_vBody;

texture		g_BaseTexture;
sampler BaseSampler = sampler_state
{
	minfilter = none;
	magfilter = none;
	mipfilter = none;

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

float BlendMode_Overlay(float base, float blend)
{
	return (base <= 0.5) ? 2 * base * blend : 1 - 2 * (1 - base) * (1 - blend);
}

float3 BlendMode_Overlay(float3 base, float3 blend)
{
	return float3(BlendMode_Overlay(base.r, blend.r),
		BlendMode_Overlay(base.g, blend.g),
		BlendMode_Overlay(base.b, blend.b));
}

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	float4 vColor = tex2D(BaseSampler, In.vTexUV);

	float3 vBase = float3(vColor.r, vColor.r, vColor.r) * vColor.a;

	float3 vCloth = float3(vColor.g, vColor.g, vColor.g) * vColor.g * g_vCloth;

	float3 vBody = float3(vColor.b, vColor.b, vColor.b) * vColor.b * g_vBody;

	vCloth = BlendMode_Overlay(vBase, vCloth) * vColor.g;

	vBody = BlendMode_Overlay(vBase, vBody) * vColor.b;

	float3 vResult = vBase * (1 - vColor.g) * (1 - vColor.b) + vCloth + vBody;

	Out.vColor = vector(vResult.r, vResult.g, vResult.b, vColor.a);

	return Out;
}

technique	Default_Device
{
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
