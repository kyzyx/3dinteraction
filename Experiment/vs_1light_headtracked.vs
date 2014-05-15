cbuffer MatrixBuffer {
  matrix world;
  matrix modelview;
  matrix projection;
};

cbuffer PointLightBuffer {
	float4 lightpos;
	float4 diffuse;
	float4 ambient;
}

cbuffer HeadBuffer {
	float4 headpos;
}

struct VertexInput {
  float4 position : POSITION;
  float4 color : COLOR;
  float3 normal : NORMAL;
};

struct PixelInput {
  float4 position : SV_POSITION;
  float4 color : COLOR;
  float3 normal : NORMAL;
  float3 lightpos : TEXCOORD1;
};

PixelInput VS(VertexInput input) {
  PixelInput output;
  float4 worldpos;
  float4 objectpos;
  float4 w;
  float4 wr = 2./59.75;
  float4 hr = 2./33.6;

  input.position.w = 1.0f;
  output.position = mul(input.position, world);
  output.position = mul(output.position, modelview);
  output.position = mul(output.position, projection);
  objectpos = mul(input.position, world);

  w = 1 - objectpos.z/headpos.z;

  output.position.x = wr*(headpos.z*objectpos.x - objectpos.z*headpos.x)/(headpos.z - objectpos.z);
  output.position.y = hr*(headpos.z*objectpos.y - objectpos.z*headpos.y)/(headpos.z - objectpos.z);
  output.position.z = output.position.z/output.position.w;
  output.position.w = 1;
  output.position = output.position*w;

  output.color = input.color;
  output.normal = mul(input.normal, (float3x3)world);
  output.normal = normalize(output.normal);
  worldpos = mul(input.position, world);

  output.lightpos.xyz = lightpos.xyz - worldpos.xyz;
  output.lightpos = normalize(output.lightpos);

  return output;
}
