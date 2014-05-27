cbuffer MatrixBuffer {
  matrix world;
  matrix modelview;
  matrix projection;
};

struct VertexInput {
  float4 position : POSITION;
  float4 color : COLOR;
};

struct PixelInput {
  float4 position : SV_POSITION;
  float4 color : COLOR;
};

PixelInput VS(VertexInput input) {
  PixelInput output;
  input.position.w = 1.0f;
  output.position = mul(input.position, world);
  output.position = mul(output.position, modelview);
  output.position = mul(output.position, projection);
  output.color = input.color;
  return output;
}