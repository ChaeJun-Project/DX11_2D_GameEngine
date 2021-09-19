#pragma once

//STL(Standard Template Library)
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
#include <chrono>
#include <variant>

//ComPtr Header
#include <wrl/client.h>
using namespace Microsoft::WRL;

//DirectX11 Header
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

//Module
#include "Singleton.h"

//Math
#include "Math/Math.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "Math/Color4.h"
#include "Math/Quaternion.h"
#include "Math/Matrix.h"

//Define
#include "Define.h"

//Settings
#include "Settings.h"

//Core
#include "Graphics.h"

//Shaders
#include "VertexShader.h"

//Manager
//FileManager
#include "FileManager.h"
#include "FileStream.h"
//ShaderManager
#include "ShaderManager.h"


