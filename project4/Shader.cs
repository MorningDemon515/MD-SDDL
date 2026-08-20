using System.Runtime.CompilerServices;
using System.Text;

using Silk.NET.Core.Native;

using Silk.NET.Input;
using Silk.NET.Maths;
using Silk.NET.Windowing;

using Silk.NET.Direct3D.Compilers;
using Silk.NET.Direct3D11;
using Silk.NET.DXGI;

namespace Project4
{
    class Shader
    {
        private unsafe ID3D11VertexShader* _vertexShader = default;
        private unsafe ID3D11PixelShader* _pixelShader = default;

        private D3DCompiler _compiler = default;
        
        public unsafe ID3D10Blob* vertexCode = default;
        
        public unsafe Shader(ID3D11Device* _device, string vShaderCode, string pShaderCode)
        {
            
            _compiler = D3DCompiler.GetApi();
            
            var vertexShaderBytes = Encoding.ASCII.GetBytes(vShaderCode);
            var pixelShaderBytes = Encoding.ASCII.GetBytes(pShaderCode);
            
            ID3D10Blob* vertexErrors = default;
            
            ID3D10Blob* pixelCode = default;
            ID3D10Blob* pixelErrors = default;
            
            HResult hr = _compiler.Compile
            (
                in vertexShaderBytes[0],
                (nuint) vertexShaderBytes.Length,
                nameof(vShaderCode),
                null,
                ref Unsafe.NullRef<ID3DInclude>(),
                "main",
                "vs_5_0",
                0,
                0,
                ref vertexCode,
                ref vertexErrors
            );

            if (hr.IsFailure)
            {
                if (vertexErrors != null)
                {
                    Console.WriteLine(SilkMarshal.PtrToString((nint) vertexErrors->GetBufferPointer()));
                }

                hr.Throw();
            }
            
            hr = _compiler.Compile
            (
                in pixelShaderBytes[0],
                (nuint)pixelShaderBytes.Length,
                nameof(pShaderCode),
                null,
                ref Unsafe.NullRef<ID3DInclude>(),
                "main",
                "ps_5_0",
                0,
                0,
                ref pixelCode,
                ref pixelErrors
            ); 

            if (hr.IsFailure)
            {
                if (pixelErrors != null)
                {
                    Console.WriteLine(SilkMarshal.PtrToString((nint) pixelErrors->GetBufferPointer()));
                }

                hr.Throw();
            }

            _device->CreateVertexShader
            (
                vertexCode->GetBufferPointer(),
                vertexCode->GetBufferSize(),
                ref Unsafe.NullRef<ID3D11ClassLinkage>(),
                ref _vertexShader
            );

            _device->CreatePixelShader
            (
                pixelCode->GetBufferPointer(),
                pixelCode->GetBufferSize(),
                ref Unsafe.NullRef<ID3D11ClassLinkage>(),
                ref _pixelShader
            );

            if (vertexCode != null) vertexCode->Release();
            if (vertexErrors != null) vertexErrors->Release();
            if (pixelCode != null) pixelCode->Release();
            if (pixelErrors != null) pixelErrors->Release();

        }

        unsafe ~Shader()
        {
            _compiler.Dispose();
            
            if(_vertexShader != null) _vertexShader->Release();
            if(_pixelShader != null) _pixelShader->Release();
        }

        public unsafe void SetShader(ID3D11DeviceContext* _context)
        {
            var classInstance = (ID3D11ClassInstance*)null;
            _context->VSSetShader(_vertexShader, ref classInstance, 0);
            _context->PSSetShader(_pixelShader, ref classInstance, 0);
        }
    }
}
