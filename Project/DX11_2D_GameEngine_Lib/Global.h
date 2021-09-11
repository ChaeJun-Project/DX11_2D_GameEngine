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

//Header
#include "Define.h"
#include "Struct.h"
#include "Extern.h"

//Module
#include "Singleton.h"

//Settings
#include "Settings.h"

//DirectX11 Header
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>

using namespace DirectX;
using namespace DirectX::PackedVector;

//DirectX Library
#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")
#pragma comment(lib, "dxguid")
