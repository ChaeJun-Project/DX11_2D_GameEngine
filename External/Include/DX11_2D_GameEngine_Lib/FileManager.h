#pragma once

//������ �����ϱ� ���� Ŭ����(���� �� ����)
//�� Ŭ������ �Լ����� ���� Directory�� ��� ���ڿ� ������ ��ġ�� �־����
//�� Ŭ������ �Լ����� ���� File�� ��� ���ڿ� ������ Ȯ���ڱ��� �־����
class FileManager final
{
public: 
    static void Initialize();
	static std::string GetAbsoluteContentPath() { return absolute_content_path; }

public:
	static void OpenDirectoryWindow(const std::string& directory);
	static const bool Create_Directory(const std::string& path); //Ư�� ��ο� ������ ����
	static const bool Delete_Directory(const std::string& path); //Ư�� ����� ���� �Ǵ� ������ ����(���� ���� �����͸� ��� ����)
	static const bool Delete_File(const std::string& path); //Ư�� ����� ������ ����
	static const bool Copy_File(const std::string& src, const std::string& dst); //������ ���� src(source)�� ������ dst(destination)�� ����
	static const bool IsDirectory(const std::string& path); //Ư�� ��ΰ� �������� Ȯ��
	static const bool IsExistDirectory(const std::string& path); //Ư�� ��ο� ������ �����ϴ��� Ȯ��
	static const bool IsExistFile(const std::string& path); //Ư�� ������ ������ �����ϴ��� Ȯ��
	static const std::vector<std::string> GetFileNameVectorFromDirectory(const std::string& path); //Ư�� ����� ���丮 ������ ��� ������ �̸��� ���ͷ� �����Ͽ� ��ȯ

	static const void RenameFileName(const std::string& folder_name, const std::string& extension_name, const std::string& old_file_name, const std::string& new_file_name);
	static const std::string GetFileNameFromPath(const std::string& path); //Ư�� ��ο� �ִ� ������ �̸��� ��ȯ(Ȯ���� ����)
	static const std::string GetIntactFileNameFromPath(const std::string& path); //Ư�� ��ο� �ִ� ������ �̸����� ��ȯ(Ȯ���� ������)
	static const std::string GetDirectoryFromPath(const std::string& path); //Ư�� ��ο� �ִ� ������ �̸��� ��ȯ
	static const std::string GetExtensionFromPath(const std::string& path); //Ư�� ��ο� �ִ� ������ Ȯ���ڸ� ��ȯ(string)
	static const std::wstring GetExtensionFromPath(const std::wstring& path); //Ư�� ��ο� �ִ� ������ Ȯ���ڸ� ��ȯ(wstring)
	static const std::string GetPathWithoutExtension(const std::string& path); //Ư�� ��ο��� Ư�� ������ Ȯ���ڸ� ������ ������ ��θ� ��ȯ
	static const std::string GetRelativeFromPath(const std::string& absolute_path); //���->����η� ��ȯ
	static const std::string GetParentDirectory(const std::string& path); //Ư�� ����� �θ�����(���� �ֻ�ܿ� �ִ� ����)�� ��θ� ����
	static const std::string GetWorkingDirectory(); //���� �������� ��θ� ��ȯ
	static const std::string GetFileDataNameFromPath(const std::string& path); // Ư�� ��ο� �ִ� ������ �̸����� Ư¡���� �����ؼ� ��ȯ(�ִϸ��̼��� �����ϱ� ���� ���)

	static const std::vector<std::string> GetDirectoriesInDirectory(const std::string& directory);
	static const std::vector<std::string> GetFilesInDirectory(const std::string& directory);

	static const std::vector<std::string>& GetSupportedTextureFormats() { return supported_texture_formats; }
	static const std::vector<std::string>& GetSupportedModelFormats() { return supported_model_formats; }
	static const std::vector<std::string>& GetSupportedAudioFormats() { return supported_audio_formats; }

	//�̸� �����ߴ� �����Ǵ� Ȯ���ڸ� ���� ����� ������ Ȯ���ڿ� ���Ͽ� �����Ǵ��� ���θ� Ȯ��
	//Texture, Model, Audio
	static const bool IsSupportedTextureFile(const std::string& path);   //Texture
	static const bool IsSupportedModelFile(const std::string& path);     //Model
	static const bool IsSupportedAudioFile(const std::string& path);     //Audio

	static const std::string ToUppercase(const std::string& lower); //�ҹ��� -> �빮��
	static const std::string ToLowercase(const std::string& upper); //�빮�� -> �ҹ���
	static const std::string ConvertWStringToString(const std::wstring& wstr); //�����ڵ� -> ��Ƽ����Ʈ
	static const std::wstring ConvertStringToWString(const std::string& str); //��Ƽ����Ʈ -> �����ڵ�
	static const char* ConvertStringToChar(const std::string& message); //String -> Char ��ȯ
	static const std::string ConvertCharToString(const char* message); //Char -> String ��ȯ

	static void ReplaceAll(std::string& in_out_str, const std::string& from, const std::string& to);
	
	static void SaveStringToFile(const std::string& str, FILE* p_file);
	static void LoadStringFromFile(std::string& str, FILE* p_file);

	//FPrintf
	static void FPrintf_Vector2(const Vector2& vector2, FILE* p_file);
	static void FPrintf_Vector3(const Vector3& vector3, FILE* p_file);

	template <typename T, typename = typename std::enable_if <
		std::is_same<T, Vector4>::value ||
		std::is_same<T, Quaternion>::value > ::type >
		static void FPrintf_Vector4(const T& vector4, FILE* p_file)
	{
		fprintf(p_file, "%f ", vector4.x);
		fprintf(p_file, "%f ", vector4.y);
		fprintf(p_file, "%f ", vector4.z);
		fprintf(p_file, "%f\n", vector4.w);
	}

	//FScanf
	static void FScanf(char* p_buffer, FILE* p_file);
	static void FScanf_Vector2(Vector2& vector2, FILE* p_file);
	static void FScanf_Vector3(Vector3& vector3, FILE* p_file);

	template <typename T, typename = typename std::enable_if <
		std::is_same<T, Vector4>::value ||
		std::is_same<T, Quaternion>::value > ::type >
		static void FScanf_Vector4(T& vector4, FILE* p_file)
	{
		fscanf_s(p_file, "%f ", &vector4.x);
		fscanf_s(p_file, "%f ", &vector4.y);
		fscanf_s(p_file, "%f ", &vector4.z);
		fscanf_s(p_file, "%f\n", &vector4.w);
	}

private:
	static std::string absolute_content_path;

	static std::vector<std::string> supported_texture_formats;		//Texture�� �����Ǵ� Ȯ���ڵ�
	static std::vector<std::string> supported_model_formats;		//Model�� �����Ǵ� Ȯ���ڵ�
	static std::vector<std::string> supported_audio_formats;		//Audio�� �����Ǵ� Ȯ���ڵ�
};