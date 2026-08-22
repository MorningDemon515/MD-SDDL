using System.Numerics;

namespace Project4
{
    public struct Matrix
    {
        public float _11, _12, _13, _14;  
        public float _21, _22, _23, _24;
        public float _31, _32, _33, _34;
        public float _41, _42, _43, _44;

        public Matrix(
            float _11, float _12, float _13, float _14,
            float _21, float _22, float _23, float _24,
            float _31, float _32, float _33, float _34,
            float _41, float _42, float _43, float _44)
        {
            this._11 = _11; this._12 = _12; this._13 = _13; this._14 = _14;
            this._21 = _21; this._22 = _22; this._23 = _23; this._24 = _24;
            this._31 = _31; this._32 = _32; this._33 = _33; this._34 = _34;
            this._41 = _41; this._42 = _42; this._43 = _43; this._44 = _44;
        }

        public static Matrix operator * (Matrix mat1, Matrix mat2)
        {
            Matrix result;

            Vector4 row1 = new Vector4(mat2._11, mat2._12, mat2._13, mat2._14);
            Vector4 row2 = new Vector4(mat2._21, mat2._22, mat2._23, mat2._24);
            Vector4 row3 = new Vector4(mat2._31, mat2._32, mat2._33, mat2._34);
            Vector4 row4 = new Vector4(mat2._41, mat2._42, mat2._43, mat2._44);

            Vector4 vResult1 = row1 * mat1._11 + row2 * mat1._12 + row3 * mat1._13 + row4 * mat1._14;
            result._11 = vResult1.X; result._12 = vResult1.Y; result._13 = vResult1.Z; result._14 = vResult1.W;

            Vector4 vResult2 = row1 * mat1._21 + row2 * mat1._22 + row3 * mat1._23 + row4 * mat1._24;
            result._21 = vResult2.X; result._22 = vResult2.Y; result._23 = vResult2.Z; result._24 = vResult2.W;

            Vector4 vResult3 = row1 * mat1._31 + row2 * mat1._32 + row3 * mat1._33 + row4 * mat1._34;
            result._31 = vResult3.X; result._32 = vResult3.Y; result._33 = vResult3.Z; result._34 = vResult3.W;

            Vector4 vResult4 = row1 * mat1._41 + row2 * mat1._42 + row3 * mat1._43 + row4 * mat1._44;
            result._41 = vResult4.X; result._42 = vResult4.Y; result._43 = vResult4.Z; result._44 = vResult4.W;

            return result;
        }
    }
}