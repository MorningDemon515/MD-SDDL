using Silk.NET.Input;
using Silk.NET.Maths;
using Silk.NET.Windowing;

namespace Project4
{
    public static partial class Program
    {
        private static IWindow? _window;
        
        public static unsafe void Main()
        {
            WindowOptions options = WindowOptions.Default;
            options.API = GraphicsAPI.None;
            options.IsVisible = true;
            options.Size = new Vector2D<int>(800, 600);
            options.Title = "Project 4";
            
            _window = Window.Create(options);
            
            _window.Load += OnLoad;
            _window.Update += OnUpdate;
            _window.Render += OnRender;
            _window.Closing += OnClosing;
            _window.Resize += OnResize;
            
            _window.Initialize();
            _window.Center();

            _window.Run();
        }
    }
}
