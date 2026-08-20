using System.Runtime.CompilerServices;

using Silk.NET.Core.Native;

using Silk.NET.Input;
using Silk.NET.Maths;
using Silk.NET.Windowing;

using Silk.NET.Direct3D.Compilers;
using Silk.NET.Direct3D11;
using Silk.NET.DXGI;

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
                    RefreshRate = new Rational(60, 1)
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
           
           _context->ClearRenderTargetView(_renderTarget, ref RGBA.BLACK[0]);
           
           _shader.SetShader(_context);
           _context->IASetPrimitiveTopology(D3DPrimitiveTopology.D3DPrimitiveTopologyTrianglelist);
           _context->Draw(3, 0);           

           _swapChain->Present(1, 0);
        }

        private static unsafe void OnClosing()
        {
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

