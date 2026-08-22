namespace Project4
{
    public static class Triangle
    {
        #region IO.hlsl
        private const string IO = @"
        #ifndef IO_HLSL
        #define IO_HLSL

        struct VS_IN
        {
            float3 position : POSITION;
            float3 color : COLOR;
            float2 uv : TEXCOORD;
            float3 normal : NORMAL;
        };

        struct VS_OUT
        {
            float4 position : SV_POSITION;
            float3 color : COLOR;
            float2 uv : TEXCOORD;
            float3 normal : NORMAL;

            float3 fragPos : POSITION1;
        };

        #endif
        ";
        #endregion

        #region  vertex.hlsl
        public const string vertex = IO + @"
        cbuffer Buffer0 : register(b0)
        {
            row_major float4x4 projection;
            row_major float4x4 view;
            row_major float4x4 model;
        };

        VS_OUT main(VS_IN input)
        {
            VS_OUT output;

            float4 worldPos = mul(model, float4(input.position,  1.0));

            output.fragPos = worldPos.xyz;

            output.position = mul(projection, mul(view, worldPos));
            output.color = input.color;
            output.uv = float2(input.uv.x, 1.0f - input.uv.y);

            float3x3 normalMat = (float3x3)transpose(model);
            output.normal = mul(normalMat, input.normal);

            return output;
        }
        ";
        #endregion

        #region  pixel.hlsl

        public const string pixel = IO + @"
        Texture2D ninja_t : register(t0);

        SamplerState t_Sampler : register(s0);

        float4 main(VS_OUT input) : SV_TARGET
        {
            float3 obj_color = (float3)ninja_t.Sample(t_Sampler, input.uv);

            float3 N = normalize(input.normal);

            float3 lightColor = float3(1.0f, 1.0f, 1.0f);
            float3 lightPos = float3(1.0f, 1.0f, -1.0f);

            float3 lightDir = normalize(input.fragPos - lightPos);

            float3 ambient = float3(0.05, 0.05, 0.05) * lightColor * obj_color;

            float3 diffuse = max(dot(N, lightDir), 0.0f) * lightColor * obj_color;

            return float4(ambient + diffuse, 1.0f);
        }
        ";

        #endregion
    }
}
