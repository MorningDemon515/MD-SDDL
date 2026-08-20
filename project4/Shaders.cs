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
        };

        struct VS_OUT
        {
            float4 position : SV_POSITION;
            float3 color : COLOR0;
        };

        #endif
        ";
        #endregion

        #region  vertex.hlsl
        public const string vertex = IO + @"
        VS_OUT main(VS_IN input, uint vertexID : SV_VertexID)
        {
            VS_OUT output;

            float2 positions[3] = {
                float2( 0.0,  0.5),
                float2( 0.5, -0.5),
                float2(-0.5, -0.5)
            };

            float3 colors[3] = {
                float3(1.0, 0.0, 0.0),
                float3(0.0, 1.0, 0.0),
                float3(0.0, 0.0, 1.0)
            };

            output.position = float4(positions[vertexID],0.0f,  1.0);
            output.color = colors[vertexID];

            return output;
        }
        ";
        #endregion

        #region  pixel.hlsl

        public const string pixel = IO + @"
        float4 main(VS_OUT input) : SV_TARGET
        {
            return float4(input.color, 1.0);
        }
        ";

        #endregion
    }
}
