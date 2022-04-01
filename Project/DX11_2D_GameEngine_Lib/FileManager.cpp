#include "stdafx.h"
#include "FileManager.h"
#include <filesystem> //파일을 관리하기 위한 라이브러리 포함
#include <shellapi.h>
#include <codecvt>

using namespace std::filesystem;

//find()                : 주어진 문자열이 존재하는 위치-> 정방향
//rfind()               : 주어진 문자열이 존재하는 위치-> 역방향
//find_first_of()       : 주어진 문자 중에 하나라도 걸리는 첫번째 위치
//find_last_of()        : 주어진 문자 중에 하나라도 걸리는 마지막 위치
//find_first_not_of()   : 주어진 문자가 아닌 문자가 걸리는 첫번째 위치
//find_last_not_of()    : 주어진 문자가 아닌 문자가 걸리는 마지막 위치

FileManager::~FileManager()
{
	m_supported_animation_formats.clear();
	m_supported_animation_formats.shrink_to_fit();

	m_supported_audio_formats.clear();
	m_supported_audio_formats.shrink_to_fit();

	m_supported_material_formats.clear();
	m_supported_material_formats.shrink_to_fit();

	m_supported_mesh_formats.clear();
	m_supported_mesh_formats.shrink_to_fit();

	m_supported_prefab_formats.clear();
	m_supported_prefab_formats.shrink_to_fit();

	m_supported_scene_formats.clear();
	m_supported_scene_formats.shrink_to_fit();

	m_supported_tilemap_formats.clear();
	m_supported_tilemap_formats.shrink_to_fit();

	m_supported_texture_formats.clear();
	m_supported_texture_formats.shrink_to_fit();
}

void FileManager::Initialize()
{
	//GetWorkingDirectory()
	//Debug or Release => DX11_2D_GameEngine/ExeFile/Release/Content/
	//Exe
	//Debug: DX11_2D_GameEngine/ExeFile/Debug/
	//Release: DX11_2D_GameEngine/ExeFile/Release/
	m_absolute_content_path = GetWorkingDirectory();
	{
		std::string exe_file_str = "ExeFile";
		auto index = m_absolute_content_path.find(exe_file_str.c_str());
		std::string absolute_exe_file_path = m_absolute_content_path.substr(0, (index + exe_file_str.length()));
		absolute_exe_file_path += "/";
		m_absolute_content_path = absolute_exe_file_path + "Content/";
	}

	m_absolute_animation_path = m_absolute_content_path + "Asset/Animation/";	//Animation폴더 절대경로
	m_absolute_audio_path = m_absolute_content_path + "Asset/Audio/";			//Audio폴더 절대경로
	m_absolute_material_path = m_absolute_content_path + "Asset/Material/";		//Material폴더 절대경로
	m_absolute_mesh_path = m_absolute_content_path + "Asset/Mesh/";				//Mesh폴더 절대경로
	m_absolute_scene_path = m_absolute_content_path + "Asset/Scene/";			//Scene폴더 절대경로
	m_absolute_texture_path = m_absolute_content_path + "Asset/Texture/";		//Texture폴더 절대경로
	m_absolute_tilemap_path = m_absolute_content_path + "Asset/TileMap/";		//TileMap폴더 절대경로
	m_absolute_prefab_path = m_absolute_content_path + "Asset/Prefab/";			//Prefab폴더 절대경로
	m_absolute_particle_path = m_absolute_content_path + "Asset/Particle/";		//Particle폴더 절대경로

	//Animation에 지원되는 확장자들
	m_supported_animation_formats =
	{
		".anim"
	};

	//Audio에 지원되는 확장자들
	m_supported_audio_formats =
	{
		".mod", ".mp3", ".wav"
	};

	//Material에 지원되는 확장자들
	m_supported_material_formats =
	{
		".mat"
	};

	//Mesh에 지원되는 확장자들
	m_supported_mesh_formats =
	{
		".mesh"
	};

	//Scene에 지원되는 확장자들
	m_supported_scene_formats =
	{
		".scene"
	};

	//Texture에 지원되는 확장자들
	m_supported_texture_formats =
	{
		".jpg", ".png", ".bmp", ".tga",
		".dds", ".exr", ".raw", ".gif",
		".hdr", ".ico", ".jng", ".jpeg"
	};

	//TileMap에 지원되는 확장자들
	m_supported_tilemap_formats =
	{
		".tile"
	};

	//Prefab에 지원되는 확장자들
	m_supported_prefab_formats =
	{
		".prefab"
	};

	//Particle에 지원되는 확장자들
	m_supported_particle_formats =
	{
		".particle"
	};
}

void FileManager::OpenDirectoryWindow(const std::string& directory)
{
	ShellExecuteA(nullptr, nullptr, nullptr, nullptr, directory.c_str(), SW_SHOW);
}

const bool FileManager::Create_Directory(const std::string& path)
{
	try
	{
		return create_directories(path); //create_directory(path)는 path에 해당하는 폴더 하나만을 만들지만 create_directories(path)는 path에 해당하는 폴더 여러개를 모두 만듦
	}

	catch (filesystem_error& error)
	{
		return false;
	}
}

const bool FileManager::Delete_Directory(const std::string& path)
{
	try
	{
		return remove_all(path) > 0; //remove_all은 경로상에 삭제된 폴더 혹은 파일의 개수를 리턴하는데 이것이 1개 이상일 경우 정상적으로 삭제됨을 의미하기 때문에 비교연산을 함.
	}

	catch (filesystem_error& error)
	{
		return false;
	}
}

const bool FileManager::Delete_File(const std::string& path)
{
	if (is_directory(path)) //특정 경로가 폴더인지 확인
		return false;

	try
	{
		return remove(path); //특정 폴더의 파일을 삭제
	}

	catch (filesystem_error& error)
	{
		return false;
	}
}

const bool FileManager::Copy_File(const std::string& src, const std::string& dst)
{
	if (src == dst) //src(발신자), dst(수신자)가 서로 같은 경우는 반환
		return false;

	//데이터를 저장할 폴더가 없는 경우 생성
	if (!IsExistDirectory(GetDirectoryFromPath(dst)))
		Create_Directory(GetDirectoryFromPath(dst));

	try
	{
		return copy_file(src, dst, copy_options::overwrite_existing); //src폴더의 내용을 dst폴더의 내용에 복사. 복사옵션은 이미 존재하는 파일은 덮어씌우기
	}

	catch (filesystem_error& error)
	{
		return false;
	}
}

const bool FileManager::IsDirectory(const std::string& path)
{
	try
	{
		return is_directory(path); //특정 경로가 폴더인지 확인 후 bool값 형식으로 반환
	}

	catch (filesystem_error& error)
	{
		return false;
	}
}

const bool FileManager::IsExistDirectory(const std::string& path)
{
	try
	{
		return exists(path); //특정 경로에 폴더가 존재하는지 확인
	}

	catch (filesystem_error& error)
	{
		return false;
	}
}

const bool FileManager::IsExistFile(const std::string& path)
{
	try
	{
		return exists(path); //특정 폴더에 파일이 존재하는지 확인
	}

	catch (filesystem_error& error)
	{
		return false;
	}
}

const std::vector<std::string> FileManager::GetFileNameVectorFromDirectory(const std::string& path)
{
	std::vector<std::string> file_name_vector;

	//해당 디렉토리의 각 파일들의 절대 경로 값을 받음
	for (auto& file_absolute_path : directory_iterator(path))
	{
		//절대 경로 값으로 부터 파일의 이름(확장자 포함)을 구함
		//u8string: convert path to string
		std::string file_name = GetFileNameFromPath(file_absolute_path.path().u8string());
		file_name_vector.emplace_back(file_name);
	}

	return file_name_vector;
}

void FileManager::RenameFile(const std::string& folder_path, const std::string& extension_name, const std::string& old_file_name, const std::string& new_file_name)
{
	auto old_file_path = folder_path + old_file_name + extension_name;
	auto new_file_path = folder_path + new_file_name + extension_name;
	rename(old_file_path.c_str(), new_file_path.c_str());
}

void FileManager::RemoveFile(const std::string& file_path)
{
	remove(file_path.c_str());
}

const std::string FileManager::GetFileNameFromPath(const std::string& path)
{
	auto last_index = path.find_last_of("\\/");
	auto file_name = path.substr(last_index + 1, path.length());

	return file_name;
}

const std::string FileManager::GetOriginFileNameFromPath(const std::string& path)
{
	auto file_name = GetFileNameFromPath(path);
	auto last_index = file_name.find_last_of('.');
	auto intact_file_name = file_name.substr(0, last_index);

	return intact_file_name;
}

const std::string FileManager::GetDirectoryPathFromPath(const std::string& path)
{
	auto last_index = path.find_last_of("\\/");
	auto directory_path = path.substr(0, last_index + 1);

	return directory_path;
}

const std::string FileManager::GetDirectoryFromPath(const std::string& path)
{
	auto last_index = path.find_last_of("\\/");
	auto directory = path.substr(last_index + 1, path.length());

	return directory;
}

const std::string FileManager::GetExtensionFromPath(const std::string& path)
{
	if (path.empty())
		return std::string();

	auto last_index = path.find_last_of('.');

	if (last_index != std::string::npos)
		return path.substr(last_index, path.length());

	return std::string();
}

const std::wstring FileManager::GetExtensionFromPath(const std::wstring& path)
{
	if (path.empty())
		return std::wstring();

	auto last_index = path.find_last_of('.');

	if (last_index != std::string::npos)
		return path.substr(last_index, path.length());

	return std::wstring();
}

const std::string FileManager::GetPathWithoutExtension(const std::string& path)
{
	auto directory = GetDirectoryFromPath(path);
	auto origin_file_name = GetOriginFileNameFromPath(path);

	return directory + origin_file_name;
}

const std::string FileManager::GetRelativeResourcePathFromAbsolutePath(const std::string& absolute_path)
{
	std::string content_str = "Content/";
	auto last_index = absolute_path.find(content_str.c_str());
	auto relative_resource_path = absolute_path.substr((last_index + content_str.length()), absolute_path.length());

	return relative_resource_path;
}

//경로->상대경로로 변환
const std::string FileManager::GetRelativeFromPath(const std::string& absolute_path)
{
	//절대 경로를 만듬
	//path클래스는 파일 경로를 저장하기 위한 클래스임.
	//absolute함수는 절대 경로로 이름을 반환하는 역할을 함
	//ex) C:\users\document~ 이런 경로로 이름을 반환
	//함수 인자로 받은 경로를 절대 경로로 만들어 p에 저장
	path p = absolute(absolute_path);
	//현재 실행중인 경로를 절대 경로로 만들어 r에 저장
	path r = absolute(GetWorkingDirectory());

	//루트 경로가 다를경우 절대경로 반환
	//현재 실행 중인 절대경로와 인자로 받은 절대경로의 루트 경로가 다른 경우
	//인자로 받은 절대경로를 string으로 변환하여 반환
	//현재 실행 중인 절대경로와 변환할 경로가 애초에 뿌리부터 다르면 상대경로로 변환이 불가능하기 때문에 먼저 검사
	if (p.root_path() != r.root_path())
		return p.generic_string();

	//루트 경로가 같은 경우
	//결과를 담을 객체
	path result;

	//두 경로가 갈라지는 지점을 체크
	path::const_iterator iter_path = p.begin();
	path::const_iterator iter_relative = r.begin();

	//p와 r의 디렉토리 글자가 같고 각 p와 r의 end()가 아닐 때 까지 iterator 순환
	//여기서 const_iterator는 글자 하나하나가 아닌 하나의 디렉토리 글자 전체를 의미함.
	//즉, /를 기준으로 /뒤에 오는 디렉토리의 문자 혹은 문자열의 주소를 가지고 있음.
	//ex) C:\Users\User\Desktop\D3D_D3D11Framework\Day16에서 
	//const_iterator를 +하면 C, 두번 +하면 Users, 세번 +하면 User를 가지고 있음.
	while (
		*iter_path == *iter_relative &&
		iter_path != p.end() &&
		iter_relative != r.end())
	{
		iter_path++;
		iter_relative++;
	}

	//relative에 남은 각 토큰에대해 ..을 추가
	//여기서 상대 경로를 
	if (iter_relative != r.end())
	{
		iter_relative++;
		while (iter_relative != r.end())
		{
			//path 클래스에서 /연산자는 디렉토리 구분자로 두 경로를 연결하는 것을 의미.
			//즉 경로뒤에 /..을 붙인다는 의미
			result /= "..";
			iter_relative++;
		}
	}

	//남은 경로 추가
	while (iter_path != p.end())
	{
		//인자로 받은 절대 경로의 뒷부분을 result에 연결함.
		//여기서 뒷부분은 현재 실행중인 절대경로와 겹치지 않는 부분임
		result /= *iter_path;
		iter_path++;
	}

	//path->string형식으로 자료형을 변환하여 반환
	return result.generic_string();
}

//인자로 받은 경로로 부모디렉토리의 경로를 구하는 함수
const std::string FileManager::GetParentDirectory(const std::string& path)
{
	auto found = path.find_last_of("\\/");
	auto result = path;

	//탈출조건
	//found에서 '\\' or '/'이 없는 경우 값을 그냥 반환
	if (found == std::string::npos)
		return path;

	//만약 found에서 '\\' or '/'를 찾았는데 이것이 인자로 받은 path의 길이-1번째 인덱스와 같은 경우
	//순환호출을 이용하여 하위 디렉토리를 잘라서 자기자신의 함수를 호출함.
	//ex) C:\Users\User\Desktop\D3D_D3D11Framework\ -> 인자로 받은 path의 마지막 문자가 \일 경우 
	if (found == path.length() - 1)
	{
		//result에 마지막 /을 제외한 나머지 문자를 저장
		result = result.substr(0, found - 1);
		return GetParentDirectory(result);
	}

	//위의 두 조건에 충족하지 않은 경우
	//ex) C:\Users\User\Desktop\D3D_D3D11Framework -> 인자로 받은 path의 마지막 문자가 \가 아닌 경우
	//ex) C:\Users\User\Desktop\을 반환
	return result.substr(0, found) + "/";
}

const std::string FileManager::GetWorkingDirectory()
{
	return current_path().generic_string() + "/"; //path->string 으로 자료형 변환
}

const std::string FileManager::GetFileDataNameFromPath(const std::string& path)
{
	auto origin_file_name = GetOriginFileNameFromPath(path); //순수 파일이름을 추출
	auto first_index = origin_file_name.find_first_of('_'); //_으로 끝
	auto file_data_name = origin_file_name.substr(first_index + 1, origin_file_name.size());

	return file_data_name;
}

const std::vector<std::string> FileManager::GetDirectoriesInDirectory(const std::string& directory)
{
	std::vector<std::string> directories;

	directory_iterator end_iter;
	for (directory_iterator iter(directory); iter != end_iter; iter++)
	{
		if (!is_directory(iter->status()))
			continue;

		directories.emplace_back(iter->path().generic_string());
	}

	return directories;
}

const std::vector<std::string> FileManager::GetFilesInDirectory(const std::string& directory)
{
	std::vector<std::string> files;

	directory_iterator end_iter;
	for (directory_iterator iter(directory); iter != end_iter; iter++)
	{
		if (!is_regular_file(iter->status()))
			continue;

		files.emplace_back(iter->path().generic_string());
	}

	return files;
}

const bool FileManager::IsSupportedAnimationFile(const std::string& path)
{
	auto file_extension = GetExtensionFromPath(path);

	for (const auto& format : m_supported_animation_formats)
	{
		//대소문자 비교포함
		if (file_extension == format || file_extension == ToUppercase(format))
			return true;
	}

	return false;
}

const bool FileManager::IsSupportedAudioFile(const std::string& path)
{
	auto file_extension = GetExtensionFromPath(path);

	for (const auto& format : m_supported_audio_formats)
	{
		//대소문자 비교포함
		if (file_extension == format || file_extension == ToUppercase(format))
			return true;
	}

	return false;
}

const bool FileManager::IsSupportedMaterialFile(const std::string& path)
{
	auto file_extension = GetExtensionFromPath(path);

	for (const auto& format : m_supported_material_formats)
	{
		//대소문자 비교포함
		if (file_extension == format || file_extension == ToUppercase(format))
			return true;
	}

	return false;
}

const bool FileManager::IsSupportedMeshFile(const std::string& path)
{
	auto file_extension = GetExtensionFromPath(path);

	for (const auto& format : m_supported_mesh_formats)
	{
		//대소문자 비교포함
		if (file_extension == format || file_extension == ToUppercase(format))
			return true;
	}

	return false;
}

const bool FileManager::IsSupportedSceneFile(const std::string& path)
{
	auto file_extension = GetExtensionFromPath(path);

	for (const auto& format : m_supported_scene_formats)
	{
		//대소문자 비교포함
		if (file_extension == format || file_extension == ToUppercase(format))
			return true;
	}

	return false;
}

const bool FileManager::IsSupportedTextureFile(const std::string& path)
{
	auto file_extension = GetExtensionFromPath(path);

	for (const auto& format : m_supported_texture_formats)
	{
		//대소문자 비교포함
		if (file_extension == format || file_extension == ToUppercase(format))
			return true;
	}

	return false;
}

const bool FileManager::IsSupportedTileMapFile(const std::string& path)
{
	auto file_extension = GetExtensionFromPath(path);

	for (const auto& format : m_supported_tilemap_formats)
	{
		//대소문자 비교포함
		if (file_extension == format || file_extension == ToUppercase(format))
			return true;
	}

	return false;
}

const bool FileManager::IsSupportedPrefabFile(const std::string& path)
{
	auto file_extension = GetExtensionFromPath(path);

	for (const auto& format : m_supported_prefab_formats)
	{
		//대소문자 비교포함
		if (file_extension == format || file_extension == ToUppercase(format))
			return true;
	}

	return false;
}

const bool FileManager::IsSupportedParticleFile(const std::string& path)
{
	auto file_extension = GetExtensionFromPath(path);

	for (const auto& format : m_supported_particle_formats)
	{
		//대소문자 비교포함
		if (file_extension == format || file_extension == ToUppercase(format))
			return true;
	}

	return false;
}

const std::string FileManager::ToUppercase(const std::string& lower)
{
	std::string upper;

	for (const auto& character : lower)
		upper += toupper(character);

	return upper;
}

const std::string FileManager::ToLowercase(const std::string& upper)
{
	std::string lower;

	for (const auto& character : upper)
		lower += tolower(character);

	return lower;
}

const std::string FileManager::ConvertWStringToString(const std::wstring& wstr)
{
	int len;
	//WideCharToMultiByte은 유니코드를 멀티바이트 코드로 바꿔줌
	//동시에 문자열의 길이 값을 반환
	//함수의 중간인자에 -1은 처음부터 끝까지 복사한다는 것을 의미
	len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);

	char buf[1024] = {};

	//유니코드에서 멀티바이트 코드로 변환한 문자열을 buf에 len값 만큼 복사
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, buf, len, NULL, NULL);

	//복사한 문자열을 가지고 있는 buf의 데이터로 string객체 생성
	std::string result(buf);

	//string 객체 반환
	return result;
}

const std::wstring FileManager::ConvertStringToWString(const std::string& str)
{
	int len = 0;
	//MultiByteToWideChar은 멀티바이트 코드를 유니코드로 바꿔줌
	//동시에 문자열의 길이 값을 반환
	//함수의 중간인자에 -1은 처음부터 끝까지 복사한다는 것을 의미
	len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);

	wchar_t buf[1024] = {};

	//멀티바이트 코드에서 유니코드로 변환한 문자열을 buf에 len값 만큼 복사
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, buf, len);

	//복사한 문자열을 가지고 있는 buf의 데이터로 string객체 생성
	std::wstring result(buf);

	return result;
}

const char* FileManager::ConvertStringToUTF8(const std::string& str)
{
	int len = 0;
	//멀티바이트 -> 유니코드
	auto wstring = ConvertStringToWString(str);

	char buf[1024] = {};

	//유니코드 -> UTF8
	len = WideCharToMultiByte(CP_UTF8, 0, wstring.c_str(), -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_UTF8, 0, wstring.c_str(), -1, buf, len, NULL, NULL);

	return buf;
}

const char* FileManager::ConvertStringToChar(const std::string& message)
{
	char buf[BUFSIZ + 1];

	strcpy_s(buf, message.c_str());

	return buf;
}

const std::string FileManager::ConvertCharToString(const char* message)
{
	std::string str(message);

	return str;
}

void FileManager::ReplaceAll(std::string& in_out_str, const std::string& from, const std::string& to)
{
	size_t start_pos = 0;

	while ((start_pos = in_out_str.find(from, start_pos)) != std::string::npos)
	{
		in_out_str.replace(start_pos, from.length(), to);
		start_pos += to.length();
	}
}

void FileManager::SaveStringToFile(const std::string& str, FILE* p_file)
{
	size_t iLen = str.length();
	fwrite(&iLen, sizeof(size_t), 1, p_file);
	fwrite(str.c_str(), sizeof(char), iLen, p_file);
}

void FileManager::LoadStringFromFile(std::string& str, FILE* p_file)
{
	size_t iLen = 0;
	fread(&iLen, sizeof(size_t), 1, p_file);

	char szBuffer[256] = "";
	fread(szBuffer, sizeof(wchar_t), iLen, p_file);
	str = szBuffer;
}

void FileManager::FPrintf_Vector2(const Vector2& vector2, FILE* p_file)
{
	fprintf(p_file, "%f ", vector2.x);
	fprintf(p_file, "%f\n", vector2.y);
}

void FileManager::FPrintf_Vector3(const Vector3& vector3, FILE* p_file)
{
	fprintf(p_file, "%f ", vector3.x);
	fprintf(p_file, "%f ", vector3.y);
	fprintf(p_file, "%f\n", vector3.z);
}

void FileManager::FScanf(char* p_buffer, FILE* p_file)
{
	int i = 0;
	while (true)
	{
		char c = (char)getc(p_file);
		if (c == '\n')
		{
			p_buffer[i++] = '\0';
			break;
		}

		p_buffer[i++] = c;
	}
}

void FileManager::FScanf_Vector2(Vector2& vector2, FILE* p_file)
{
	fscanf_s(p_file, "%f", &vector2.x);
	fscanf_s(p_file, "%f\n", &vector2.y);
}

void FileManager::FScanf_Vector3(Vector3& vector3, FILE* p_file)
{
	fscanf_s(p_file, "%f", &vector3.x);
	fscanf_s(p_file, "%f", &vector3.y);
	fscanf_s(p_file, "%f\n", &vector3.z);
}