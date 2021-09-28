#include "stdafx.h"
#include "FileManager.h"
#include <filesystem> //������ �����ϱ� ���� ���̺귯�� ����
#include <shellapi.h>

//find()                : �־��� ���ڿ��� �����ϴ� ��ġ-> ������
//rfind()               : �־��� ���ڿ��� �����ϴ� ��ġ-> ������
//find_first_of()       : �־��� ���� �߿� �ϳ��� �ɸ��� ù��° ��ġ
//find_last_of()        : �־��� ���� �߿� �ϳ��� �ɸ��� ������ ��ġ
//find_first_not_of()   : �־��� ���ڰ� �ƴ� ���ڰ� �ɸ��� ù��° ��ġ
//find_last_not_of()    : �־��� ���ڰ� �ƴ� ���ڰ� �ɸ��� ������ ��ġ

using namespace std::filesystem;

//Texture�� �����Ǵ� Ȯ���ڵ�
std::vector<std::string> FileManager::supported_texture_formats
{
	".jpg",
	".png",
	".bmp",
	".tga",
	".dds",
	".exr",
	".raw",
	".gif",
	".hdr",
	".ico",
	".iff",
	".jng",
	".jpeg",
	".koala",
	".kodak",
	".mng",
	".pcx",
	".pbm",
	".pgm",
	".ppm",
	".pfm",
	".pict",
	".psd",
	".raw",
	".sgi",
	".targa",
	".tiff",
	".tif",
	".wbmp",
	".webp",
	".xbm",
	".xpm"
};

//Model�� �����Ǵ� Ȯ���ڵ�
std::vector<std::string> FileManager::supported_model_formats
{
	".3ds",
	".obj",
	".fbx",
	".blend",
	".dae",
	".lwo",
	".c4d",
	".ase",
	".dxf",
	".hmp",
	".md2",
	".md3",
	".md5",
	".mdc",
	".mdl",
	".nff",
	".ply",
	".stl",
	".x",
	".smd",
	".lxo",
	".lws",
	".ter",
	".ac3d",
	".ms3d",
	".cob",
	".q3bsp",
	".xgl",
	".csm",
	".bvh",
	".b3d",
	".ndo"
};

//Script�� �����Ǵ� Ȯ���ڵ�
std::vector<std::string> FileManager::supported_script_formats
{
	".as",
};

//Audio�� �����Ǵ� Ȯ���ڵ�
std::vector<std::string> FileManager::supported_audio_formats
{
	".aiff",
	".asf",
	".asx",
	".dls",
	".flac",
	".fsb",
	".it",
	".m3u",
	".midi",
	".mod",
	".mp2",
	".mp3",
	".ogg",
	".pls",
	".s3m",
	".vag", // PS2/PSP
	".wav",
	".wax",
	".wma",
	".xm",
	".xma" // XBOX 360
};

void FileManager::OpenDirectoryWindow(const std::string& directory)
{
	ShellExecuteA(nullptr, nullptr, nullptr, nullptr, directory.c_str(), SW_SHOW);
}

//const bool FileManager::Create_Directory(const std::string& path)
//{
//	try
//	{
//		return create_directories(path); //create_directory(path)�� path�� �ش��ϴ� ���� �ϳ����� �������� create_directories(path)�� path�� �ش��ϴ� ���� �������� ��� ����
//	}
//
//	catch (filesystem_error& error)
//	{
//		//LOG_ERROR_F("%s,%s", error.what(), path.c_str());
//		return false;
//	}
//}
//
//const bool FileManager::Delete_Directory(const std::string& path)
//{
//	try
//	{
//		return remove_all(path) > 0; //remove_all�� ��λ� ������ ���� Ȥ�� ������ ������ �����ϴµ� �̰��� 1�� �̻��� ��� ���������� �������� �ǹ��ϱ� ������ �񱳿����� ��.
//	}
//
//	catch (filesystem_error& error)
//	{
//		//LOG_ERROR_F("%s,%s", error.what(), path.c_str());
//		return false;
//	}
//}
//
//const bool FileManager::Delete_File(const std::string& path)
//{
//	if (is_directory(path)) //Ư�� ��ΰ� �������� Ȯ��
//		return false;
//
//	try
//	{
//		return remove(path); //Ư�� ������ ������ ����
//	}
//
//	catch (filesystem_error& error)
//	{
//		//LOG_ERROR_F("%s, %s", error.what(), path.c_str());
//		return false;
//	}
//}
//
//const bool FileManager::Copy_File(const std::string& src, const std::string& dst)
//{
//	if (src == dst) //src(�߽���), dst(������)�� ���� ���� ���� ��ȯ
//		return false;
//
//	//�����͸� ������ ������ ���� ��� ����
//	if (!IsExistDirectory(GetDirectoryFromPath(dst)))
//		Create_Directory(GetDirectoryFromPath(dst));
//
//	try
//	{
//		return copy_file(src, dst, copy_options::overwrite_existing); //src������ ������ dst������ ���뿡 ����. ����ɼ��� �̹� �����ϴ� ������ ������
//	}
//
//	catch (filesystem_error& error)
//	{
//		//LOG_ERROR("Could not copy\"" + src + "\", " + std::string(error.what()));
//		return false;
//	}
//}
//
//const bool FileManager::IsDirectory(const std::string& path)
//{
//	try
//	{
//		return is_directory(path); //Ư�� ��ΰ� �������� Ȯ�� �� bool�� �������� ��ȯ
//	}
//
//	catch (filesystem_error& error)
//	{
//		//LOG_ERROR_F("%s,%s", error.what(), path.c_str());
//		return false;
//	}
//}
//
//const bool FileManager::IsExistDirectory(const std::string& path)
//{
//	try
//	{
//		return exists(path); //Ư�� ��ο� ������ �����ϴ��� Ȯ��
//	}
//
//	catch (filesystem_error& error)
//	{
//		//LOG_ERROR_F("%s,%s", error.what(), path.c_str());
//		return false;
//	}
//}
//
//const bool FileManager::IsExistFile(const std::string& path)
//{
//	try
//	{
//		return exists(path); //Ư�� ������ ������ �����ϴ��� Ȯ��
//	}
//
//	catch (filesystem_error& error)
//	{
//		//LOG_ERROR_F("%s,%s", error.what(), path.c_str());
//		return false;
//	}
//}

const std::string FileManager::GetFileNameFromPath(const std::string& path)
{
	auto last_index = path.find_last_of("\\/");
	auto file_name = path.substr(last_index + 1, path.length());

	return file_name;
}

const std::string FileManager::GetIntactFileNameFromPath(const std::string& path)
{
	auto file_name = GetFileNameFromPath(path);
	auto last_index = file_name.find_last_of('.');
	auto intact_file_name = file_name.substr(0, last_index);

	return intact_file_name;
}

const std::string FileManager::GetDirectoryFromPath(const std::string& path)
{
	auto last_index = path.find_last_of("\\/");
	auto directory = path.substr(0, last_index + 1);

	return directory;
}

const std::string FileManager::GetExtensionFromPath(const std::string& path)
{
	if (path.empty())
		return "NONE";

	auto last_index = path.find_last_of('.');

	if (last_index != std::string::npos)
		return path.substr(last_index, path.length());

	return "NONE";
}

const std::string FileManager::GetPathWithoutExtension(const std::string& path)
{
	auto directory = GetDirectoryFromPath(path);
	auto intact_file_name = GetIntactFileNameFromPath(path);

	return directory + intact_file_name;
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
	auto file_name = GetIntactFileNameFromPath(path); //���� �����̸��� ����
	auto first_index = file_name.find_first_of('_'); //_���� ��
	auto file_data_name = file_name.substr(first_index + 1, file_name.size());

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

//Texture
const bool FileManager::IsSupportedTextureFile(const std::string& path)
{

	auto file_extension = GetExtensionFromPath(path);
	auto supported_formats = GetSupportedTextureFormats();

	for (const auto& format : supported_formats)
	{
		//��ҹ��� ������
		if (file_extension == format || file_extension == ToUppercase(format))
			return true;
	}

	return false;
}

//Model
const bool FileManager::IsSupportedModelFile(const std::string& path)
{
	auto file_extension = GetExtensionFromPath(path);
	auto supported_formats = GetSupportedModelFormats();

	for (const auto& format : supported_formats)
	{
		//��ҹ��� ������
		if (file_extension == format || file_extension == ToUppercase(format))
			return true;
	}

	return false;
}

//Script
const bool FileManager::IsSupportedScriptFile(const std::string& path)
{
	auto file_extension = GetExtensionFromPath(path);
	auto supported_formats = GetSupportedScriptFormats();

	for (const auto& format : supported_formats)
	{
		//��ҹ��� ������
		if (file_extension == format || file_extension == ToUppercase(format))
			return true;
	}

	return false;
}

//Audio
const bool FileManager::IsSupportedAudioFile(const std::string& path)
{
	auto file_extension = GetExtensionFromPath(path);
	auto supported_formats = GetSupportedAudioFormats();

	for (const auto& format : supported_formats)
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

const std::string FileManager::ToString(const std::wstring& wstr)
{
	int len;
	//WideCharToMultiByte�� �����ڵ带 ��Ƽ����Ʈ �ڵ�� �ٲ���
	//���ÿ� ���ڿ��� ���� ���� ��ȯ
	//�Լ��� �߰����ڿ� -1�� ó������ ������ �����Ѵٴ� ���� �ǹ�
	len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);

	//���ڿ��� ���̸�ŭ char������ �����Ҵ�
	char* buf = new char[len];

	//�����ڵ忡�� ��Ƽ����Ʈ �ڵ�� ��ȯ�� ���ڿ��� buf�� len�� ��ŭ ����
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, buf, len, NULL, NULL);

	//������ ���ڿ��� ������ �ִ� buf�� �����ͷ� string��ü ����
	std::string result(buf);

	//�ʿ䰡 ������ ���� ����
	delete[] buf;

	//string ��ü ��ȯ
	return result;
}

const std::wstring FileManager::ToWString(const std::string& str)
{
	std::wstring result;
	result.assign(str.begin(), str.end());

	return result;
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

//void FileManager::ChangeFileName(const std::string& from_path, const std::string& to_path)
//{
//	if (!IsExistDirectory(from_path))
//		return;
//
//	rename(from_path.c_str(), to_path.c_str());
//}