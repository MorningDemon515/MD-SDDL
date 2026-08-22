
namespace Project4
{
    public struct Vertex
    {
        public float X, Y, Z;
        public float R, G, B;

        public float u, v;

        public Vertex(float x, float y, float z, float r, float g, float b, float u, float v)
        {
            X = x; Y = y; Z = z;
            R = r; G = g; B = b;
            this.u = u; this.v = v;
        }
    }
}
