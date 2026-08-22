using System.ComponentModel;
using System.Runtime.CompilerServices;

using Silk.NET.Core.Native;

using Silk.NET.Input;
using Silk.NET.Maths;
using Silk.NET.Windowing;

using Silk.NET.Direct3D.Compilers;
using Silk.NET.Direct3D11;
using Silk.NET.DXGI;

using SixLabors.ImageSharp;
using SixLabors.ImageSharp.PixelFormats;

namespace Project4
{
    public static partial class Program
    {
        private static IInputContext? _input;
        private static IMouse? _mouse;
        
        //Direct3D 11
        private static D3D11? _d3d11;
        private static DXGI? _dxgi;

        private static unsafe ID3D11Device* _device = default;
        private static unsafe ID3D11DeviceContext* _context = default;
        private static unsafe IDXGISwapChain* _swapChain = default;
        private static unsafe ID3D11RenderTargetView* _renderTarget = default;
        
        private static Shader _shader;

        private static uint _vertexStride; 
        private static uint _vertexOffset = 0; 
        
        private static unsafe ID3D11Buffer* _vertexBuffer = default;
        private static unsafe ID3D11Buffer* _indexBuffer = default;
        
        private static unsafe ID3D11InputLayout* _inputLayout = default;

        private static unsafe ID3D11Texture2D* ninja_t = default;
        private static unsafe ID3D11SamplerState* t_Sampler = default;
        private static unsafe ID3D11ShaderResourceView* ninja_rV = default;
        
        private static unsafe void OnLoad()
        {
            _input = _window.CreateInput();
            _mouse = _input.Mice[0];
            
            var keyboard = _input.Keyboards[0];
            keyboard.KeyDown += (_, key, _) =>
            {
                if (key == Key.Escape)
                {
                    _window.Close();
                }
            };
            
            _d3d11 = D3D11.GetApi(_window);
            _dxgi = DXGI.GetApi(_window);

            var hwnd = _window.Native.Win32.Value.Hwnd;
            var swapChainDesc = new SwapChainDesc
            {
                BufferCount = 1,
                BufferDesc = new ModeDesc
                {
                    Width = (uint)_window.Size.X,
                    Height = (uint)_window.Size.Y,
                    Format = Format.FormatR8G8B8A8Unorm,
                    RefreshRate = new Silk.NET.DXGI.Rational(60, 1)
                },
                BufferUsage = DXGI.UsageRenderTargetOutput,
                OutputWindow = hwnd,
                SampleDesc = new SampleDesc(1, 0),
                Windowed = true,
                SwapEffect = SwapEffect.SwapEffectDiscard
            };

            _d3d11.CreateDeviceAndSwapChain(
                (IDXGIAdapter*)null,
                D3DDriverType.Hardware,
                nint.Zero,
                (uint)CreateDeviceFlag.None,
                null,
                0,
                D3D11.SdkVersion,
                &swapChainDesc,
                ref _swapChain,
                ref _device,
                (D3DFeatureLevel*)null,
                ref _context
                );
            
            using var framebuffer = _swapChain->GetBuffer<ID3D11Texture2D>(0);
            _device->CreateRenderTargetView(framebuffer, null, ref _renderTarget);
            
            _context->OMSetRenderTargets(1, ref _renderTarget, ref Unsafe.NullRef<ID3D11DepthStencilView>());
            
            _shader = new Shader(_device, Triangle.vertex, Triangle.pixel);

            _vertexStride = (uint)sizeof(Vertex);

            Vertex[] vertices = new Vertex[4]
            {
                new Vertex(-0.5f,  0.5f, 0.0f, RGB.RED[0], RGB.RED[1], RGB.RED[2], 0.0f, 1.0f),
                new Vertex( 0.5f,  0.5f, 0.0f, RGB.GREEN[0], RGB.GREEN[1], RGB.GREEN[2], 1.0f, 1.0f),
                new Vertex( 0.5f, -0.5f, 0.0f, RGB.BLUE[0], RGB.BLUE[1], RGB.BLUE[2], 1.0f, 0.0f),
                new Vertex(-0.5f, -0.5f, 0.0f, RGB.WHITE[0], RGB.WHITE[1], RGB.WHITE[2], 0.0f, 0.0f)
            };

            uint[] indices = new uint[6]
            {
                3, 1, 2,
                3, 0, 1
            };
            
            var bufferDesc = new BufferDesc
            {
                ByteWidth = (uint) (vertices.Length * _vertexStride),
                Usage = Usage.Default,
                BindFlags = (uint) BindFlag.VertexBuffer
            };

            fixed (Vertex* verticesPtr = &vertices[0])
            {
                var subresourceData = new SubresourceData
                {
                    PSysMem = verticesPtr
                };
                
                _device->CreateBuffer(in bufferDesc, in subresourceData, ref _vertexBuffer);
            }
            
            bufferDesc = new BufferDesc
            {
                ByteWidth = (uint) (indices.Length * sizeof(uint)),
                Usage = Usage.Default,
                BindFlags = (uint) BindFlag.IndexBuffer
            };

            fixed (uint* indicesPtr = &indices[0])
            {
                var subresourceData = new SubresourceData
                {
                    PSysMem = indicesPtr
                };
                
                _device->CreateBuffer(in bufferDesc, in subresourceData, ref _indexBuffer);
            }

            InputElementDesc[] inputDesc = new InputElementDesc[3]
            {
                new InputElementDesc((byte*)SilkMarshal.StringToMemory("POSITION"),
                    0,
                    Format.FormatR32G32B32Float,
                    0,
                    0,
                    InputClassification.PerVertexData,
                    0),

                new InputElementDesc((byte*)SilkMarshal.StringToMemory("COLOR"),
                    0,
                    Format.FormatR32G32B32Float,
                    0,
                    3 * sizeof(float),
                    InputClassification.PerVertexData,
                    0),

                new InputElementDesc((byte*)SilkMarshal.StringToMemory("TEXCOORD"),
                    0,
                    Format.FormatR32G32Float,
                    0,
                    6 * sizeof(float),
                    InputClassification.PerVertexData,
                    0)
            };

            fixed (InputElementDesc* inputDescRef = &inputDesc[0])
            {
                _device->CreateInputLayout
                (
                    inputDescRef,
                    3,
                    _shader.vertexCode->GetBufferPointer(),
                    _shader.vertexCode->GetBufferSize(),
                    ref _inputLayout
                );
            }

            using var imgBmp = Image.Load<Bgra32>("ninja.jpg");

            var textureDesc = new Texture2DDesc
            {
                Width = (uint) imgBmp.Width,
                Height = (uint) imgBmp.Height,
                Format = Format.FormatB8G8R8A8Unorm,
                MipLevels = 1,
                BindFlags = (uint) BindFlag.ShaderResource,
                Usage = Usage.Default,
                CPUAccessFlags = 0,
                MiscFlags = (uint) ResourceMiscFlag.None,
                SampleDesc = new SampleDesc(1, 0),
                ArraySize = 1
            };
    
            if (imgBmp.DangerousTryGetSinglePixelMemory(out var bmp))
            {
                using (var bitmapData = bmp.Pin())
                {
                    var subresourceData = new SubresourceData
                    {
                        PSysMem = bitmapData.Pointer,
                        SysMemPitch = (uint) imgBmp.Width * sizeof(int),
                        SysMemSlicePitch = (uint) (imgBmp.Width * sizeof(int) * imgBmp.Height)
                    };
    
                _device->CreateTexture2D
                (
                    in textureDesc,
                    in subresourceData,
                    ref ninja_t
                );
            
                }
            }
            else
            {
                // TODO: Copy pixel data row-by-row, as a contiguous block is not available.
            }

            var srvDesc = new ShaderResourceViewDesc
            {
                Format = textureDesc.Format,
                ViewDimension = D3DSrvDimension.D3DSrvDimensionTexture2D,
                Anonymous = new ShaderResourceViewDescUnion
                {
                    Texture2D =
                    {
                        MostDetailedMip = 0,
                        MipLevels = 1
                    }
                }
            };

            _device->CreateShaderResourceView
            (
                (ID3D11Resource*)ninja_t,
                in srvDesc,
                ref ninja_rV
            );

            var samplerDesc = new SamplerDesc
            {
                Filter = Filter.MinMagMipLinear,
                AddressU = TextureAddressMode.Clamp,
                AddressV = TextureAddressMode.Clamp,
                AddressW = TextureAddressMode.Clamp,
                MipLODBias = 0,
                MaxAnisotropy = 1,
                MinLOD = float.MinValue,
                MaxLOD = float.MaxValue,
            };

            samplerDesc.BorderColor[0] = 0.0f;
            samplerDesc.BorderColor[1] = 0.0f;
            samplerDesc.BorderColor[2] = 0.0f;
            samplerDesc.BorderColor[3] = 1.0f;
    
            _device->CreateSamplerState
            (
                in samplerDesc,
                ref t_Sampler
            );
    
        }

        private static unsafe void OnUpdate(double deltaTime)
        {
            var keyboard = _input.Keyboards[0];
            _mouse = _input.Mice[0];

            if (keyboard.IsKeyPressed(Key.A))
            {
                Console.WriteLine("You press A!");
            }
            
            //Console.WriteLine(_mouse.Position);
            //Console.WriteLine(width + ", " + height);
            

        }

        private static unsafe void OnRender(double deltaTime)
        {
            
            var viewport = new Viewport(
                0, 
                0, 
                _window.FramebufferSize.X, 
                _window.FramebufferSize.Y, 
                0, 
                1);
            
           _context->RSSetViewports(1, in viewport);
           
           _context->ClearRenderTargetView(_renderTarget, ref RGBA.WHITE[0]);
           
           _shader.SetShader(_context);
            _context->PSSetSamplers(0, 1, t_Sampler);
            _context->PSSetShaderResources(0, 1, ninja_rV);

           _context->IASetPrimitiveTopology(D3DPrimitiveTopology.D3DPrimitiveTopologyTrianglelist);
           _context->IASetInputLayout(_inputLayout);
           _context->IASetVertexBuffers(0, 1, ref _vertexBuffer,  in _vertexStride,  in _vertexOffset);
           _context->IASetIndexBuffer(_indexBuffer, Format.FormatR32Uint, 0);

           _context->DrawIndexed(6, 0, 0);

           _swapChain->Present(1, 0);
        }

        private static unsafe void OnClosing()
        {
            ninja_t->Release();
            ninja_rV->Release();
            t_Sampler->Release();

            _inputLayout->Release();
            
            _vertexBuffer->Release();
            _indexBuffer->Release();
            
            _device->Release();
            _context->Release();
            _swapChain->Release();
            _renderTarget->Release();
            
            _d3d11.Dispose();
            _dxgi.Dispose();
        }

        private static unsafe void OnResize(Vector2D<int> size)
        {
            ID3D11RenderTargetView* nullRtv = null;
            _context->OMSetRenderTargets(0, ref nullRtv, ref Unsafe.NullRef<ID3D11DepthStencilView>());
            _renderTarget->Release();
            
            _swapChain->ResizeBuffers(0, (uint) size.X, (uint) size.Y, Format.FormatB8G8R8A8Unorm, 0);
            
            using var framebuffer = _swapChain->GetBuffer<ID3D11Texture2D>(0);
            _device->CreateRenderTargetView(framebuffer, null, ref _renderTarget);
            _context->OMSetRenderTargets(1, ref _renderTarget, ref Unsafe.NullRef<ID3D11DepthStencilView>());
        }
    }
}

