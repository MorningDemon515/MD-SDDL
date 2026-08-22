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
            float3 position : POSITION0;
            float3 color : COLOR0;
            float2 uv : TEXCOORD0;
        };

        struct VS_OUT
        {
            float4 position : SV_POSITION;
            float3 color : COLOR0;
            float2 uv : TEXCOORD0;
        };

        #endif
        ";
        #endregion

        #region  vertex.hlsl
        public const string vertex = IO + @"
        VS_OUT main(VS_IN input, uint vertexID : SV_VertexID)
        {
            VS_OUT output;

            output.position = float4(input.position,  1.0);
            output.color = input.color;
            output.uv = float2(input.uv.x, 1.0f - input.uv.y);

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
            return ninja_t.Sample(t_Sampler, input.uv);
        }
        ";

        #endregion
    }
}
