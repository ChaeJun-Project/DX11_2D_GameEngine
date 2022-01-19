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

#pragma region Log
#include "Logger.h"
#pragma endregion

#pragma region Settings
#include "Settings.h"
#pragma endregion

#pragma region D3D11BufferStruct
#include "VertexStruct.h"
#include "ConstantStruct.h"
#include "StructuredStruct.h"
#pragma endregion

#pragma region Manager
//FileManager
#include "FileManager.h"

//Log
#include "LogManager.h"

//InputManager
#include "InputManager.h"

//TimeManager
#include "TimeManager.h"

//GraphicsManager
#include "GraphicsManager.h"

//SceneManager
#include "SceneManager.h"

//EventManager
#include "EventManager.h"

//ResourceManager
#include "ResourceManager.h"

//RenderManger
#include "RenderManager.h"

//CollisionManager
#include "CollisionManager.h"

//Font
#include "FontManager.h"

//Audio
#include "AudioManager.h"
#pragma endregion

#pragma region DX11ObjectID
#include "DX11Object.h"
#pragma endregion

#pragma region  Core
//ConstantBuffer
#include "ConstantBuffer.h"

//IA
#include "VertexBuffer.h"
#include "InputLayout.h"
#include "IndexBuffer.h"

//RS
#include "RasterizerState.h"

//OM
#include "DepthStencilState.h"
#include "BlendState.h"

//Shaders
#include "VertexShader.h"
#include "GeometryShader.h"
#include "PixelShader.h"
#include "ComputeShader.h"
#include "SamplerState.h"
#pragma endregion

#pragma region Resource
//Mesh
#include "Mesh.h"

//Material
#include "Material.h"

//Shader
#include "Shader.h"

//Texture
#include "Texture.h"

//Animation
#include "Animation.h"

//Prefab
#include "Prefab.h"
#pragma endregion

#pragma region  Component
//Transform
#include "Transform.h"

//Camera
#include "Camera.h"

//SpriteRenderer
#include "SpriteRenderer.h"

//Script
#include "Script.h"

//Collider2D
#include "Collider2D.h"

//Animator
#include "Animator.h"
#pragma endregion

//Light2D
#include "Light2D.h"

//Particle
#include "ParticleSystem.h"
#pragma endregion

#pragma region GameObject
#include "GameObject.h"
#pragma endregion

