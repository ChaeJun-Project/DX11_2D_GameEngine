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
#include "Math/Color4.h"
#include "Math/Quaternion.h"
#include "Math/Matrix.h"
#pragma endregion

#pragma region Define
#include "Define.h"
#pragma endregion

#pragma region Settings
#include "Settings.h"
#pragma endregion

#pragma region Manager
//FileManager
#include "FileManager.h"
#include "FileStream.h"

//GraphicsManager
#include "GraphicsManager.h"

//ShaderManager
#include "ShaderManager.h"

//InputManager
#include "InputManager.h"

//TimeManager
#include "TimeManager.h"
#pragma endregion

#pragma region DX11ObjectID
#include "GUID.h"
#include "DX11Object.h"
#pragma endregion

#pragma region  Core
//D3D11BufferStruct
#include "VertexStruct.h"
#include "ConstantStruct.h"

//IA
#include "VertexBuffer.h"
#include "InputLayout.h"
#include "IndexBuffer.h"

//RS

//OM

//Shaders
#include "VertexShader.h"
#include "PixelShader.h"

//ConstantBuffer
#include "ConstantBuffer.h"
#pragma endregion



