
namespace Project4
{
    public struct Vertex
    {
        public float X, Y, Z;
        public float R, G, B;

        public float u, v;

        public float n_x, n_y, n_z;//Normal Vector

        public Vertex(
            float x, float y, float z, 
            float r, float g, float b, 
            float u, float v,
            float n_x, float n_y, float n_z)
        {
            X = x; Y = y; Z = z;
            R = r; G = g; B = b;
            this.u = u; this.v = v;
            this.n_x = n_x; this.n_y = n_y; this.n_z = n_z;
        }
    }
}
