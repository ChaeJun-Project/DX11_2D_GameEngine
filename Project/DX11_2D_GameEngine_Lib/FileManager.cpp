#include "stdafx.h"
#include "FileManager.h"
#include <filesystem> //������ �����ϱ� ���� ���̺귯�� ����
#include <shellapi.h>
#include <codecvt>

using namespace std::filesystem;

//find()                : �־��� ���ڿ��� �����ϴ� ��ġ-> ������
//rfind()               : �־��� ���ڿ��� �����ϴ� ��ġ-> ������
//find_first_of()       : �־��� ���� �߿� �ϳ��� �ɸ��� ù��° ��ġ
//find_last_of()        : �־��� ���� �߿� �ϳ��� �ɸ��� ������ ��ġ
//find_first_not_of()   : �־��� ���ڰ� �ƴ� ���ڰ� �ɸ��� ù��° ��ġ
//find_last_not_of()    : �־��� ���ڰ� �ƴ� ���ڰ� �ɸ��� ������ ��ġ

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

	m_absolute_animation_path = m_absolute_content_path + "Asset/Animation/";	//Animation���� ������
	m_absolute_audio_path = m_absolute_content_path + "Asset/Audio/";			//Audio���� ������
	m_absolute_material_path = m_absolute_content_path + "Asset/Material/";		//Material���� ������
	m_absolute_mesh_path = m_absolute_content_path + "Asset/Mesh/";				//Mesh���� ������
	m_absolute_scene_path = m_absolute_content_path + "Asset/Scene/";			//Scene���� ������
	m_absolute_texture_path = m_absolute_content_path + "Asset/Texture/";		//Texture���� ������
	m_absolute_tilemap_path = m_absolute_content_path + "Asset/TileMap/";		//TileMap���� ������
	m_absolute_prefab_path = m_absolute_content_path + "Asset/Prefab/";			//Prefab���� ������
	m_absolute_particle_path = m_absolute_content_path + "Asset/Particle/";		//Particle���� ������

	//Animation�� �����Ǵ� Ȯ���ڵ�
	m_supported_animation_formats =
	{
		".anim"
	};

	//Audio�� �����Ǵ� Ȯ���ڵ�
	m_supported_audio_formats =
	{
		".mod", ".mp3", ".wav"
	};

	//Material�� �����Ǵ� Ȯ���ڵ�
	m_supported_material_formats =
	{
		".mat"
	};

	//Mesh�� �����Ǵ� Ȯ���ڵ�
	m_supported_mesh_formats =
	{
		".mesh"
	};

	//Scene�� �����Ǵ� Ȯ���ڵ�
	m_supported_scene_formats =
	{
		".scene"
	};

	//Texture�� �����Ǵ� Ȯ���ڵ�
	m_supported_texture_formats =
	{
		".jpg", ".png", ".bmp", ".tga",
		".dds", ".exr", ".raw", ".gif",
		".hdr", ".ico", ".jng", ".jpeg"
	};

	//TileMap�� �����Ǵ� Ȯ���ڵ�
	m_supported_tilemap_formats =
	{
		".tile"
	};

	//Prefab�� �����Ǵ� Ȯ���ڵ�
	m_supported_prefab_formats =
	{
		".prefab"
	};

	//Particle�� �����Ǵ� Ȯ���ڵ�
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
		return create_directories(path); //create_directory(path)�� path�� �ش��ϴ� ���� �ϳ����� �������� create_directories(path)�� path�� �ش��ϴ� ���� �������� ��� ����
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
		return remove_all(path) > 0; //remove_all�� ��λ� ������ ���� Ȥ�� ������ ������ �����ϴµ� �̰��� 1�� �̻��� ��� ���������� �������� �ǹ��ϱ� ������ �񱳿����� ��.
	}

	catch (filesystem_error& error)
	{
		return false;
	}
}

const bool FileManager::Delete_File(const std::string& path)
{
	if (is_directory(path)) //Ư�� ��ΰ� �������� Ȯ��
		return false;

	try
	{
		return remove(path); //Ư�� ������ ������ ����
	}

	catch (filesystem_error& error)
	{
		return false;
	}
}

const bool FileManager::Copy_File(const std::string& src, const std::string& dst)
{
	if (src == dst) //src(�߽���), dst(������)�� ���� ���� ���� ��ȯ
		return false;

	//�����͸� ������ ������ ���� ��� ����
	if (!IsExistDirectory(GetDirectoryFromPath(dst)))
		Create_Directory(GetDirectoryFromPath(dst));

	try
	{
		return copy_file(src, dst, copy_options::overwrite_existing); //src������ ������ dst������ ���뿡 ����. ����ɼ��� �̹� �����ϴ� ������ ������
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
		return is_directory(path); //Ư�� ��ΰ� �������� Ȯ�� �� bool�� �������� ��ȯ
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
		return exists(path); //Ư�� ��ο� ������ �����ϴ��� Ȯ��
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
		return exists(path); //Ư�� ������ ������ �����ϴ��� Ȯ��
	}

	catch (filesystem_error& error)
	{
		return false;
	}
}

const std::vector<std::string> FileManager::GetFileNameVectorFromDirectory(const std::string& path)
{
	std::vector<std::string> file_name_vector;

	//�ش� ���丮�� �� ���ϵ��� ���� ��� ���� ����
	for (auto& file_absolute_path : directory_iterator(path))
	{
		//���� ��� ������ ���� ������ �̸�(Ȯ���� ����)�� ����
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

//���->����η� ��ȯ
const std::string FileManager::GetRelativeFromPath(const std::string& absolute_path)
{
	//���� ��θ� ����
	//pathŬ������ ���� ��θ� �����ϱ� ���� Ŭ������.
	//absolute�Լ��� ���� ��η� �̸��� ��ȯ�ϴ� ������ ��
	//ex) C:\users\document~ �̷� ��η� �̸��� ��ȯ
	//�Լ� ���ڷ� ���� ��θ� ���� ��η� ����� p�� ����
	path p = absolute(absolute_path);
	//���� �������� ��θ� ���� ��η� ����� r�� ����
	path r = absolute(GetWorkingDirectory());

	//��Ʈ ��ΰ� �ٸ���� ������ ��ȯ
	//���� ���� ���� �����ο� ���ڷ� ���� �������� ��Ʈ ��ΰ� �ٸ� ���
	//���ڷ� ���� �����θ� string���� ��ȯ�Ͽ� ��ȯ
	//���� ���� ���� �����ο� ��ȯ�� ��ΰ� ���ʿ� �Ѹ����� �ٸ��� ����η� ��ȯ�� �Ұ����ϱ� ������ ���� �˻�
	if (p.root_path() != r.root_path())
		return p.generic_string();

	//��Ʈ ��ΰ� ���� ���
	//����� ���� ��ü
	path result;

	//�� ��ΰ� �������� ������ üũ
	path::const_iterator iter_path = p.begin();
	path::const_iterator iter_relative = r.begin();

	//p�� r�� ���丮 ���ڰ� ���� �� p�� r�� end()�� �ƴ� �� ���� iterator ��ȯ
	//���⼭ const_iterator�� ���� �ϳ��ϳ��� �ƴ� �ϳ��� ���丮 ���� ��ü�� �ǹ���.
	//��, /�� �������� /�ڿ� ���� ���丮�� ���� Ȥ�� ���ڿ��� �ּҸ� ������ ����.
	//ex) C:\Users\User\Desktop\D3D_D3D11Framework\Day16���� 
	//const_iterator�� +�ϸ� C, �ι� +�ϸ� Users, ���� +�ϸ� User�� ������ ����.
	while (
		*iter_path == *iter_relative &&
		iter_path != p.end() &&
		iter_relative != r.end())
	{
		iter_path++;
		iter_relative++;
	}

	//relative�� ���� �� ��ū������ ..�� �߰�
	//���⼭ ��� ��θ� 
	if (iter_relative != r.end())
	{
		iter_relative++;
		while (iter_relative != r.end())
		{
			//path Ŭ�������� /�����ڴ� ���丮 �����ڷ� �� ��θ� �����ϴ� ���� �ǹ�.
			//�� ��εڿ� /..�� ���δٴ� �ǹ�
			result /= "..";
			iter_relative++;
		}
	}

	//���� ��� �߰�
	while (iter_path != p.end())
	{
		//���ڷ� ���� ���� ����� �޺κ��� result�� ������.
		//���⼭ �޺κ��� ���� �������� �����ο� ��ġ�� �ʴ� �κ���
		result /= *iter_path;
		iter_path++;
	}

	//path->string�������� �ڷ����� ��ȯ�Ͽ� ��ȯ
	return result.generic_string();
}

//���ڷ� ���� ��η� �θ���丮�� ��θ� ���ϴ� �Լ�
const std::string FileManager::GetParentDirectory(const std::string& path)
{
	auto found = path.find_last_of("\\/");
	auto result = path;

	//Ż������
	//found���� '\\' or '/'�� ���� ��� ���� �׳� ��ȯ
	if (found == std::string::npos)
		return path;

	//���� found���� '\\' or '/'�� ã�Ҵµ� �̰��� ���ڷ� ���� path�� ����-1��° �ε����� ���� ���
	//��ȯȣ���� �̿��Ͽ� ���� ���丮�� �߶� �ڱ��ڽ��� �Լ��� ȣ����.
	//ex) C:\Users\User\Desktop\D3D_D3D11Framework\ -> ���ڷ� ���� path�� ������ ���ڰ� \�� ��� 
	if (found == path.length() - 1)
	{
		//result�� ������ /�� ������ ������ ���ڸ� ����
		result = result.substr(0, found - 1);
		return GetParentDirectory(result);
	}

	//���� �� ���ǿ� �������� ���� ���
	//ex) C:\Users\User\Desktop\D3D_D3D11Framework -> ���ڷ� ���� path�� ������ ���ڰ� \�� �ƴ� ���
	//ex) C:\Users\User\Desktop\�� ��ȯ
	return result.substr(0, found) + "/";
}

const std::string FileManager::GetWorkingDirectory()
{
	return current_path().generic_string() + "/"; //path->string ���� �ڷ��� ��ȯ
}

const std::string FileManager::GetFileDataNameFromPath(const std::string& path)
{
	auto origin_file_name = GetOriginFileNameFromPath(path); //���� �����̸��� ����
	auto first_index = origin_file_name.find_first_of('_'); //_���� ��
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
		//��ҹ��� ������
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
		//��ҹ��� ������
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
		//��ҹ��� ������
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
		//��ҹ��� ������
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
		//��ҹ��� ������
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
		//��ҹ��� ������
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
		//��ҹ��� ������
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
		//��ҹ��� ������
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
		//��ҹ��� ������
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
	//WideCharToMultiByte�� �����ڵ带 ��Ƽ����Ʈ �ڵ�� �ٲ���
	//���ÿ� ���ڿ��� ���� ���� ��ȯ
	//�Լ��� �߰����ڿ� -1�� ó������ ������ �����Ѵٴ� ���� �ǹ�
	len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);

	char buf[1024] = {};

	//�����ڵ忡�� ��Ƽ����Ʈ �ڵ�� ��ȯ�� ���ڿ��� buf�� len�� ��ŭ ����
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, buf, len, NULL, NULL);

	//������ ���ڿ��� ������ �ִ� buf�� �����ͷ� string��ü ����
	std::string result(buf);

	//string ��ü ��ȯ
	return result;
}

const std::wstring FileManager::ConvertStringToWString(const std::string& str)
{
	int len = 0;
	//MultiByteToWideChar�� ��Ƽ����Ʈ �ڵ带 �����ڵ�� �ٲ���
	//���ÿ� ���ڿ��� ���� ���� ��ȯ
	//�Լ��� �߰����ڿ� -1�� ó������ ������ �����Ѵٴ� ���� �ǹ�
	len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);

	wchar_t buf[1024] = {};

	//��Ƽ����Ʈ �ڵ忡�� �����ڵ�� ��ȯ�� ���ڿ��� buf�� len�� ��ŭ ����
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, buf, len);

	//������ ���ڿ��� ������ �ִ� buf�� �����ͷ� string��ü ����
	std::wstring result(buf);

	return result;
}

const char* FileManager::ConvertStringToUTF8(const std::string& str)
{
	int len = 0;
	//��Ƽ����Ʈ -> �����ڵ�
	auto wstring = ConvertStringToWString(str);

	char buf[1024] = {};

	//�����ڵ� -> UTF8
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