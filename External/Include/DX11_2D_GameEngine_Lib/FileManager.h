#pragma once

//������ �����ϱ� ���� Ŭ����(���� �� ����)
//�� Ŭ������ �Լ����� ���� Directory�� ��� ���ڿ� ������ ��ġ�� �־����
//�� Ŭ������ �Լ����� ���� File�� ��� ���ڿ� ������ Ȯ���ڱ��� �־����
class FileManager final
{
public:
	static void OpenDirectoryWindow(const std::string& directory);
	//static const bool Create_Directory(const std::string& path); //Ư�� ��ο� ������ ����
	//static const bool Delete_Directory(const std::string& path); //Ư�� ����� ���� �Ǵ� ������ ����(���� ���� �����͸� ��� ����)
	//static const bool Delete_File(const std::string& path); //Ư�� ����� ������ ����
	//static const bool Copy_File(const std::string& src, const std::string& dst); //������ ���� src(source)�� ������ dst(destination)�� ����
	//static const bool IsDirectory(const std::string& path); //Ư�� ��ΰ� �������� Ȯ��
	//static const bool IsExistDirectory(const std::string& path); //Ư�� ��ο� ������ �����ϴ��� Ȯ��
	//static const bool IsExistFile(const std::string& path); //Ư�� ������ ������ �����ϴ��� Ȯ��

	static const std::string GetFileNameFromPath(const std::string& path); //Ư�� ��ο� �ִ� ������ �̸��� ��ȯ(Ȯ���� ����)
	static const std::string GetIntactFileNameFromPath(const std::string& path); //Ư�� ��ο� �ִ� ������ �̸����� ��ȯ(Ȯ���� ������)
	static const std::string GetDirectoryFromPath(const std::string& path); //Ư�� ��ο� �ִ� ������ �̸��� ��ȯ
	static const std::string GetExtensionFromPath(const std::string& path); //Ư�� ��ο� �ִ� ������ Ȯ���ڸ� ��ȯ
	static const std::string GetPathWithoutExtension(const std::string& path); //Ư�� ��ο��� Ư�� ������ Ȯ���ڸ� ������ ������ ��θ� ��ȯ
	static const std::string GetRelativeFromPath(const std::string& absolute_path); //���->����η� ��ȯ
	static const std::string GetParentDirectory(const std::string& path); //Ư�� ����� �θ�����(���� �ֻ�ܿ� �ִ� ����)�� ��θ� ����
	static const std::string GetWorkingDirectory(); //���� �������� ��θ� ��ȯ
	static const std::string GetFileDataNameFromPath(const std::string& path); // Ư�� ��ο� �ִ� ������ �̸����� Ư¡���� �����ؼ� ��ȯ(�ִϸ��̼��� �����ϱ� ���� ���)

	static const std::vector<std::string> GetDirectoriesInDirectory(const std::string& directory);
	static const std::vector<std::string> GetFilesInDirectory(const std::string& directory);

	static const std::vector<std::string>& GetSupportedTextureFormats() { return supported_texture_formats; }
	static const std::vector<std::string>& GetSupportedModelFormats() { return supported_model_formats; }
	static const std::vector<std::string>& GetSupportedScriptFormats() { return supported_script_formats; }
	static const std::vector<std::string>& GetSupportedAudioFormats() { return supported_audio_formats; }

	//�̸� �����ߴ� �����Ǵ� Ȯ���ڸ� ���� ����� ������ Ȯ���ڿ� ���Ͽ� �����Ǵ��� ���θ� Ȯ��
	//Texture, Model, Animation, Script, Audio
	static const bool IsSupportedTextureFile(const std::string& path);   //Texture
	static const bool IsSupportedModelFile(const std::string& path);     //Model
	static const bool IsSupportedScriptFile(const std::string& path);    //Script
	static const bool IsSupportedAudioFile(const std::string& path);     //Audio

	static const std::string ToUppercase(const std::string& lower); //�ҹ��� -> �빮��
	static const std::string ToLowercase(const std::string& upper); //�빮�� -> �ҹ���
	static const std::string ToString(const std::wstring& wstr); //�����ڵ� -> ��Ƽ����Ʈ
	static const std::wstring ToWString(const std::string& str); //��Ƽ����Ʈ -> �����ڵ�
	static const char* ConvertStringToChar(const std::string& message); //String -> Char ��ȯ
	static const std::string ConvertCharToString(const char* message); //Char -> String ��ȯ

	static void ReplaceAll(std::string& in_out_str, const std::string& from, const std::string& to);
	//static void ChangeFileName(const std::string& from_path, const std::string& to_path);

private:
	static std::vector<std::string> supported_texture_formats;		//Texture�� �����Ǵ� Ȯ���ڵ�
	static std::vector<std::string> supported_model_formats;		//Model�� �����Ǵ� Ȯ���ڵ�
	static std::vector<std::string> supported_script_formats;		//Script�� �����Ǵ� Ȯ���ڵ�
	static std::vector<std::string> supported_audio_formats;		//Audio�� �����Ǵ� Ȯ���ڵ�
};