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

            output.position = float4(input.position,  1.0);
            output.color = input.color;

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
