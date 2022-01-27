#pragma once

#pragma region STL(Standard Template Library)
#include <string>
#include <vector>
#include <list>
#include <map>
#include <deque>
#include <queue>
#include <unordered_map>
#include <thread>
#include <process.h>
#include <mutex>
#include <functional>
#include <variant>
#pragma endregion

#pragma region ComPtr
#include <wrl/client.h>
using namespace Microsoft::WRL;
#pragma endregion

#pragma region DirectX11 
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>

using namespace DirectX;
using namespace DirectX::PackedVector;

//DirectX Library
#pragma comment(lib, "dxgi")
#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")
#pragma comment(lib, "dxguid")
#pragma endregion

#pragma region Module
#include "Singleton.h"
#pragma endregion

#pragma region Math
#include "Math/Math.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "Math/Quaternion.h"
#include "Math/Matrix.h"
//#include "Math/SimpleMath.h"
//using namespace SimpleMath;
#pragma endregion

#pragma region Enum & Define & Function
#include "Enum.h"
#include "Define.h"
#include "Function.h"
#pragma endregion

#pragma region D3D11BufferStruct
#include "VertexStruct.h"
#include "ConstantStruct.h"
#include "StructuredStruct.h"
#pragma endregion

#pragma region Manager
//FileManager
#include "FileManager.h"
//Audio
#include "AudioManager.h"
//CollisionManager
#include "CollisionManager.h"
//EventManager
#include "EventManager.h"
//Font
#include "FontManager.h"
//InputManager
#include "InputManager.h"
//Log
#include "LogManager.h"
//RenderManger
#include "RenderManager.h"
//ResourceManager
#include "ResourceManager.h"
//SceneManager
#include "SceneManager.h"
//TimeManager
#include "TimeManager.h"
//GraphicsManager
#include "GraphicsManager.h"
#pragma endregion

#pragma region Log
#include "Logger.h"
#pragma endregion

#pragma region Settings
#include "Settings.h"
#pragma endregion

#pragma region DX11ObjectID
#include "DX11Object.h"
#pragma endregion