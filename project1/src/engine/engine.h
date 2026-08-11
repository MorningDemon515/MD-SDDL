#ifndef ENGINE_H
#define ENGINE_H

#ifdef ENGINE_DLL
    #if defined(_WIN32)
        #define ENGINE_API __declspec(dllexport)
    #else 
        #define ENGINE_API    
    #endif
#else
    #if defined(_WIN32)
        #define ENGINE_API __declspec(dllimport)
    #else
        #define ENGINE_API
    #endif
#endif

#include "TextureType.h"
#include "AABB.h"

typedef enum Scancode
{
    K_UNKNOWN = 0,

    K_A = 4,
    K_B = 5,
    K_C = 6,
    K_D = 7,
    K_E = 8,
    K_F = 9,
    K_G = 10,
    K_H = 11,
    K_I = 12,
    K_J = 13,
    K_K = 14,
    K_L = 15,
    K_M = 16,
    K_N = 17,
    K_O = 18,
    K_P = 19,
    K_Q = 20,
    K_R = 21,
    K_S = 22,
    K_T = 23,
    K_U = 24,
    K_V = 25,
    K_W = 26,
    K_X = 27,
    K_Y = 28,
    K_Z = 29,

    K_1 = 30,
    K_2 = 31,
    K_3 = 32,
    K_4 = 33,
    K_5 = 34,
    K_6 = 35,
    K_7 = 36,
    K_8 = 37,
    K_9 = 38,
    K_0 = 39,

    K_RETURN = 40,
    K_ESCAPE = 41,
    K_BACKSPACE = 42,
    K_TAB = 43,
    K_SPACE = 44,

    K_MINUS = 45,
    K_EQUALS = 46,
    K_LEFTBRACKET = 47,
    K_RIGHTBRACKET = 48,
    K_BACKSLASH = 49,
    K_NONUSHASH = 50,
    K_SEMICOLON = 51,
    K_APOSTROPHE = 52,
    K_GRAVE = 53,
    K_COMMA = 54,
    K_PERIOD = 55,
    K_SLASH = 56,

    K_CAPSLOCK = 57,

    K_F1 = 58,
    K_F2 = 59,
    K_F3 = 60,
    K_F4 = 61,
    K_F5 = 62,
    K_F6 = 63,
    K_F7 = 64,
    K_F8 = 65,
    K_F9 = 66,
    K_F10 = 67,
    K_F11 = 68,
    K_F12 = 69,

    K_PRINTSCREEN = 70,
    K_SCROLLLOCK = 71,
    K_PAUSE = 72,
    K_INSERT = 73,
    K_HOME = 74,
    K_PAGEUP = 75,
    K_DELETE = 76,
    K_END = 77,
    K_PAGEDOWN = 78,
    K_RIGHT = 79,
    K_LEFT = 80,
    K_DOWN = 81,
    K_UP = 82,

    K_NUMLOCKCLEAR = 83, 
    K_KP_DIVIDE = 84,
    K_KP_MULTIPLY = 85,
    K_KP_MINUS = 86,
    K_KP_PLUS = 87,
    K_KP_ENTER = 88,
    K_KP_1 = 89,
    K_KP_2 = 90,
    K_KP_3 = 91,
    K_KP_4 = 92,
    K_KP_5 = 93,
    K_KP_6 = 94,
    K_KP_7 = 95,
    K_KP_8 = 96,
    K_KP_9 = 97,
    K_KP_0 = 98,
    K_KP_PERIOD = 99,

    K_NONUSBACKSLASH = 100,
    K_APPLICATION = 101, 
    K_POWER = 102, 

    K_KP_EQUALS = 103,
    K_F13 = 104,
    K_F14 = 105,
    K_F15 = 106,
    K_F16 = 107,
    K_F17 = 108,
    K_F18 = 109,
    K_F19 = 110,
    K_F20 = 111,
    K_F21 = 112,
    K_F22 = 113,
    K_F23 = 114,
    K_F24 = 115,
    K_EXECUTE = 116,
    K_HELP = 117,   
    K_MENU = 118,   
    K_SELECT = 119,
    K_STOP = 120,   
    K_AGAIN = 121,  
    K_UNDO = 122, 
    K_CUT = 123,     
    K_COPY = 124,    
    K_PASTE = 125,   
    K_FIND = 126,    
    K_MUTE = 127,
    K_VOLUMEUP = 128,
    K_VOLUMEDOWN = 129,

    K_KP_COMMA = 133,
    K_KP_EQUALSAS400 = 134,

    K_INTERNATIONAL1 = 135, 
                                         
    K_INTERNATIONAL2 = 136,
    K_INTERNATIONAL3 = 137, 
    K_INTERNATIONAL4 = 138,
    K_INTERNATIONAL5 = 139,
    K_INTERNATIONAL6 = 140,
    K_INTERNATIONAL7 = 141,
    K_INTERNATIONAL8 = 142,
    K_INTERNATIONAL9 = 143,
    K_LANG1 = 144, 
    K_LANG2 = 145, 
    K_LANG3 = 146,
    K_LANG4 = 147,
    K_LANG5 = 148, 
    K_LANG6 = 149, 
    K_LANG7 = 150, 
    K_LANG8 = 151,
    K_LANG9 = 152, 

    K_ALTERASE = 153,    
    K_SYSREQ = 154,
    K_CANCEL = 155,     
    K_CLEAR = 156,
    K_PRIOR = 157,
    K_RETURN2 = 158,
    K_SEPARATOR = 159,
    K_OUT = 160,
    K_OPER = 161,
    K_CLEARAGAIN = 162,
    K_CRSEL = 163,
    K_EXSEL = 164,

    K_KP_00 = 176,
    K_KP_000 = 177,
    K_THOUSANDSSEPARATOR = 178,
    K_DECIMALSEPARATOR = 179,
    K_CURRENCYUNIT = 180,
    K_CURRENCYSUBUNIT = 181,
    K_KP_LEFTPAREN = 182,
    K_KP_RIGHTPAREN = 183,
    K_KP_LEFTBRACE = 184,
    K_KP_RIGHTBRACE = 185,
    K_KP_TAB = 186,
    K_KP_BACKSPACE = 187,
    K_KP_A = 188,
    K_KP_B = 189,
    K_KP_C = 190,
    K_KP_D = 191,
    K_KP_E = 192,
    K_KP_F = 193,
    K_KP_XOR = 194,
    K_KP_POWER = 195,
    K_KP_PERCENT = 196,
    K_KP_LESS = 197,
    K_KP_GREATER = 198,
    K_KP_AMPERSAND = 199,
    K_KP_DBLAMPERSAND = 200,
    K_KP_VERTICALBAR = 201,
    K_KP_DBLVERTICALBAR = 202,
    K_KP_COLON = 203,
    K_KP_HASH = 204,
    K_KP_SPACE = 205,
    K_KP_AT = 206,
    K_KP_EXCLAM = 207,
    K_KP_MEMSTORE = 208,
    K_KP_MEMRECALL = 209,
    K_KP_MEMCLEAR = 210,
    K_KP_MEMADD = 211,
    K_KP_MEMSUBTRACT = 212,
    K_KP_MEMMULTIPLY = 213,
    K_KP_MEMDIVIDE = 214,
    K_KP_PLUSMINUS = 215,
    K_KP_CLEAR = 216,
    K_KP_CLEARENTRY = 217,
    K_KP_BINARY = 218,
    K_KP_OCTAL = 219,
    K_KP_DECIMAL = 220,
    K_KP_HEXADECIMAL = 221,

    K_LCTRL = 224,
    K_LSHIFT = 225,
    K_LALT = 226,
    K_LGUI = 227, 
    K_RCTRL = 228,
    K_RSHIFT = 229,
    K_RALT = 230, 
    K_RGUI = 231,

    K_MODE = 257,    

    K_SLEEP = 258,                 
    K_WAKE = 259,                  

    K_CHANNEL_INCREMENT = 260,      
    K_CHANNEL_DECREMENT = 261,     

    K_MEDIA_PLAY = 262,          
    K_MEDIA_PAUSE = 263,        
    K_MEDIA_RECORD = 264,       
    K_MEDIA_FAST_FORWARD = 265, 
    K_MEDIA_REWIND = 266,       
    K_MEDIA_NEXT_TRACK = 267,   
    K_MEDIA_PREVIOUS_TRACK = 268,
    K_MEDIA_STOP = 269,         
    K_MEDIA_EJECT = 270,        
    K_MEDIA_PLAY_PAUSE = 271,   
    K_MEDIA_SELECT = 272,       

    K_AC_NEW = 273,             
    K_AC_OPEN = 274,          
    K_AC_CLOSE = 275,         
    K_AC_EXIT = 276,           
    K_AC_SAVE = 277,         
    K_AC_PRINT = 278,         
    K_AC_PROPERTIES = 279,   

    K_AC_SEARCH = 280,          
    K_AC_HOME = 281,          
    K_AC_BACK = 282,         
    K_AC_FORWARD = 283,         
    K_AC_STOP = 284,             
    K_AC_REFRESH = 285,        
    K_AC_BOOKMARKS = 286,      

    K_SOFTLEFT = 287, 
    K_SOFTRIGHT = 288, 

    K_CALL = 289,
    K_ENDCALL = 290, 

    K_RESERVED = 400,

    K_COUNT = 512
} Scancode;

typedef enum MouseButton
{
    BUTTON_LEFT = 1,
    BUTTON_MIDDLE = 2,
    BUTTON_RIGHT = 3,
    BUTTON_X1 = 4,
    BUTTON_X2 = 5
} MouseButton;

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct Window Window;
    typedef struct Renderer Renderer;
    typedef struct Key Key;
    typedef struct Manager Manager;
    typedef int Handle;
    typedef struct Mesh Mesh;
    typedef struct Texture Texture;
    typedef struct Camera Camera;
    typedef unsigned int id;

	ENGINE_API void InitEngine();
	ENGINE_API void QuitEngine();
    ENGINE_API float GetTime();
    ENGINE_API void Delay(unsigned int time);

    ENGINE_API Manager* M_Create();
    ENGINE_API void* M_Get(Manager* manager, Handle handle);
    ENGINE_API void M_Remove(Manager* manager, Handle handle);
    ENGINE_API void M_Release(Manager* manager);

	ENGINE_API Handle M_W_Create(Manager* manager, int width, int height);
	ENGINE_API int W_IsRun(Window* window);
	ENGINE_API void W_SetRun(Window* window, int run);
	ENGINE_API void W_PollEvents(Window* window);
	ENGINE_API void W_GetSize(Window* window, int *width, int *height);
	ENGINE_API void W_SetRelative(Window* window, int relative);
    ENGINE_API void W_SetTitle(Window* window, const char* title);
    ENGINE_API void W_SetSize(Window* window, int width, int height);

	ENGINE_API Handle M_K_Create(Manager* manager);
	ENGINE_API int K_Down(Key* key, int num);
    ENGINE_API int K_Up(Key* key, int num);
    ENGINE_API int K_Press(Key* key, int num);
    ENGINE_API void K_Update(Key* key);
    ENGINE_API void K_Event(Key* key);

    ENGINE_API void Mouse_GetPos(float *x, float *y);
    ENGINE_API void Mouse_SetPos(Window* window, float x,float y);
    ENGINE_API void Mouse_GetRelative(float *x, float *y);
    ENGINE_API float Mouse_GetWheel();
    ENGINE_API int Mouse_Press(MouseButton mb);
    ENGINE_API int Mouse_Down(MouseButton mb);
    ENGINE_API int Mouse_Up(MouseButton mb);
    ENGINE_API void Mouse_Update();

    ENGINE_API Handle M_R_Create(Manager* manager, Window* window);
    ENGINE_API void R_Present(Renderer* renderer);
    ENGINE_API void R_Clear(Renderer* renderer, float r, float g, float b);
    ENGINE_API void R_SetViewport(Renderer* renderer, int x, int y, int w,int h);
    ENGINE_API void R_Begin(Renderer* renderer);
    ENGINE_API void R_End(Renderer* renderer);
    ENGINE_API void R_IsLight(Renderer* renderer, int isLight);
    ENGINE_API void R_SetGamma(Renderer* renderer, float gamma);

    ENGINE_API Handle M_R_Mesh(Manager* manager, Renderer* renderer, float bright[],float pos[], float texCoords[], float color[], unsigned int indices[], int indexNum, int posNum, int isTex);
    ENGINE_API Handle M_R_Mesh_Plane(Manager* manager, Renderer* renderer, float bright[], float color[], int isTex);
    ENGINE_API Handle M_R_Mesh_Cube(Manager* manager, Renderer* renderer, float bright[], float color[], int isTex);
    ENGINE_API Handle M_R_Mesh_Sphere(Manager* manager, Renderer* renderer, int stacks, int sectors, float bright[], float color[], int isTex);
    ENGINE_API Handle M_R_Mesh_Cone(Manager* manager, Renderer* renderer, float rad, float height,int sectors, float bright[], float color[], int isTex);
    ENGINE_API Handle M_R_Mesh_Cylinder(Manager* manager, Renderer* renderer, float rad, float height, int sectors, float bright[], float color[], int isTex);
    ENGINE_API Handle M_R_Mesh_Torus(Manager* manager, Renderer* renderer, float mainRad, float tubeRad, int mainSec, int tubeSec, float bright[], float color[], int isTex);
    ENGINE_API void R_Mesh_Draw(Mesh* mesh);
    ENGINE_API void R_Mesh_Set_properties(Mesh* mesh, float metallic, float roughness);

    ENGINE_API Handle T_Create(Manager* manager, int width, int height, int nrChannels, unsigned char* data, TextureType type);
    ENGINE_API Handle T_CreateFromFile(Manager* manager, const char* file, TextureType type);
    ENGINE_API void T_Enable(Texture* texture, Mesh* mesh, unsigned int level);

    ENGINE_API Camera* Cam_Create(Renderer* renderer, float pos[3], float near, float far, float fov); //angle
    ENGINE_API void Cam_Update(Camera* camera);
    ENGINE_API void Cam_Move(Camera* camera, float x, float y, float z);
    ENGINE_API void Cam_Rotate(Camera* camera, float pitch, float yaw, float roll);

    ENGINE_API void Cam_GetPos(Camera* camera, float *pos);
    ENGINE_API void Cam_GetDir(Camera* camera, float *dir);

    ENGINE_API void R_Mesh_Translate(Mesh* mesh, float x, float y, float z);
    ENGINE_API void R_Mesh_Scale(Mesh* mesh, float x, float y, float z);
    ENGINE_API void R_Mesh_Rotate(Mesh* mesh, float axis[3], float angle);
    ENGINE_API void R_Mesh_Offset(Mesh* mesh, float x, float y, float z);

    ENGINE_API id R_Light_Paralle(Renderer* renderer, float color[3], float strength, float direction[3]);
    ENGINE_API id R_Light_Point(Renderer* renderer, float color[3], float strength, float position[3], float constant, float linear, float quadratic);
    ENGINE_API id R_Light_Spot(Renderer* renderer, float color[3], float strength, float position[3], float direction[3], float constant, float linear, float quadratic, float cutOff, float outerCutOff);

    ENGINE_API void R_Light_Set_position(Renderer* renderer, id lightid, float position[3]);
    ENGINE_API void R_Light_Set_direction(Renderer* renderer, id lightid, float direction[3]);
    ENGINE_API void R_Light_Set_attenuation(Renderer* renderer, id lightid, float constant, float linear, float quadratic);
    ENGINE_API void R_Light_Set_angle(Renderer* renderer, id lightid, float cutOff, float outerCutOff);
    ENGINE_API void R_Light_Set_color(Renderer* renderer, id lightid, float color[3]);
    ENGINE_API void R_Light_Set_strength(Renderer* renderer, id lightid, float strength);

    ENGINE_API void R_Init_SkyBox(Renderer* renderer, const char* back,
                                                        const char* bottom,
                                                        const char* front,
                                                        const char* left,
                                                        const char* right,
                                                        const char* top);

    ENGINE_API void R_Set_SkyBox(Renderer* renderer, int issb);        
    ENGINE_API void R_Set_SkyBox_Light(Renderer* renderer, int issb);

    ENGINE_API AABB R_Mesh_Get_AABB(Mesh* mesh);
    ENGINE_API AABB R_Mesh_Set_AABB(AABB aabb, Mesh* mesh);
    ENGINE_API AABB R_AABB_To_Screen(Camera* camera, AABB aabb);

	ENGINE_API void Test();


#ifdef __cplusplus
}
#endif

#endif