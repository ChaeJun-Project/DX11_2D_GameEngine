#pragma once
//������ �����ϱ� ���� Ŭ����(���� �� ����)
//�� Ŭ������ �Լ����� ���� Directory�� ��� ���ڿ� ������ ��ġ�� �־����
//�� Ŭ������ �Լ����� ���� File�� ��� ���ڿ� ������ Ȯ���ڱ��� �־����
class FileManager final : public Singleton<FileManager>
{
	SINGLETON(FileManager);

	FileManager() = default;
	~FileManager();

public: 
    void Initialize();
	const std::string& GetAbsoluteContentPath() { return absolute_content_path; }

public:
	void OpenDirectoryWindow(const std::string& directory);
	const bool Create_Directory(const std::string& path); //Ư�� ��ο� ������ ����
	const bool Delete_Directory(const std::string& path); //Ư�� ����� ���� �Ǵ� ������ ����(���� ���� �����͸� ��� ����)
	const bool Delete_File(const std::string& path); //Ư�� ����� ������ ����
	const bool Copy_File(const std::string& src, const std::string& dst); //������ ���� src(source)�� ������ dst(destination)�� ����
	const bool IsDirectory(const std::string& path); //Ư�� ��ΰ� �������� Ȯ��
	const bool IsExistDirectory(const std::string& path); //Ư�� ��ο� ������ �����ϴ��� Ȯ��
	const bool IsExistFile(const std::string& path); //Ư�� ������ ������ �����ϴ��� Ȯ��
	const std::vector<std::string> GetFileNameVectorFromDirectory(const std::string& path); //Ư�� ����� ���丮 ������ ��� ������ �̸��� ���ͷ� �����Ͽ� ��ȯ

	const void RenameFileName(const std::string& folder_path, const std::string& extension_name, const std::string& old_file_name, const std::string& new_file_name);
	const std::string GetFileNameFromPath(const std::string& path); //Ư�� ��ο� �ִ� ������ �̸��� ��ȯ(Ȯ���� ����)
	const std::string GetOriginFileNameFromPath(const std::string& path); //Ư�� ��ο� �ִ� ������ �̸����� ��ȯ(Ȯ���� ������)
	const std::string GetDirectoryFromPath(const std::string& path); //Ư�� ��ο� �ִ� ������ �̸��� ��ȯ
	const std::string GetExtensionFromPath(const std::string& path); //Ư�� ��ο� �ִ� ������ Ȯ���ڸ� ��ȯ(string)
	const std::wstring GetExtensionFromPath(const std::wstring& path); //Ư�� ��ο� �ִ� ������ Ȯ���ڸ� ��ȯ(wstring)
	const std::string GetPathWithoutExtension(const std::string& path); //Ư�� ��ο��� Ư�� ������ Ȯ���ڸ� ������ ������ ��θ� ��ȯ
	const std::string GetRelativeFromPath(const std::string& absolute_path); //���->����η� ��ȯ
	const std::string GetParentDirectory(const std::string& path); //Ư�� ����� �θ�����(���� �ֻ�ܿ� �ִ� ����)�� ��θ� ����
	const std::string GetWorkingDirectory(); //���� �������� ��θ� ��ȯ
	const std::string GetFileDataNameFromPath(const std::string& path); // Ư�� ��ο� �ִ� ������ �̸����� Ư¡���� �����ؼ� ��ȯ(�ִϸ��̼��� �����ϱ� ���� ���)

	const std::vector<std::string> GetDirectoriesInDirectory(const std::string& directory);
	const std::vector<std::string> GetFilesInDirectory(const std::string& directory);

	const std::vector<std::string>& GetSupportedTextureFormats() { return supported_texture_formats; }
	const std::vector<std::string>& GetSupportedModelFormats() { return supported_model_formats; }
	const std::vector<std::string>& GetSupportedAudioFormats() { return supported_audio_formats; }

	//�̸� �����ߴ� �����Ǵ� Ȯ���ڸ� ���� ����� ������ Ȯ���ڿ� ���Ͽ� �����Ǵ��� ���θ� Ȯ��
	//Texture, Model, Audio
	const bool IsSupportedTextureFile(const std::string& path);   //Texture
	const bool IsSupportedModelFile(const std::string& path);     //Model
	const bool IsSupportedAudioFile(const std::string& path);     //Audio

	const std::string ToUppercase(const std::string& lower); //�ҹ��� -> �빮��
	const std::string ToLowercase(const std::string& upper); //�빮�� -> �ҹ���
	const std::string ConvertWStringToString(const std::wstring& wstr); //�����ڵ� -> ��Ƽ����Ʈ
	const std::wstring ConvertStringToWString(const std::string& str); //��Ƽ����Ʈ -> �����ڵ�
	const char* ConvertStringToChar(const std::string& message); //String -> Char ��ȯ
	const std::string ConvertCharToString(const char* message); //Char -> String ��ȯ

	void ReplaceAll(std::string& in_out_str, const std::string& from, const std::string& to);
	
	void SaveStringToFile(const std::string& str, FILE* p_file);
	void LoadStringFromFile(std::string& str, FILE* p_file);

public:
	//FPrintf
	void FPrintf_Vector2(const Vector2& vector2, FILE* p_file);
	void FPrintf_Vector3(const Vector3& vector3, FILE* p_file);

	template <typename T, typename = typename std::enable_if <
		std::is_same<T, Vector4>::value ||
		std::is_same<T, Quaternion>::value > ::type >
	void FPrintf_Vector4(const T& vector4, FILE* p_file)
	{
		fprintf(p_file, "%f ", vector4.x);
		fprintf(p_file, "%f ", vector4.y);
		fprintf(p_file, "%f ", vector4.z);
		fprintf(p_file, "%f\n", vector4.w);
	}

	//FScanf
	void FScanf(char* p_buffer, FILE* p_file);
	void FScanf_Vector2(Vector2& vector2, FILE* p_file);
	void FScanf_Vector3(Vector3& vector3, FILE* p_file);

	template <typename T, typename = typename std::enable_if <
		std::is_same<T, Vector4>::value ||
		std::is_same<T, Quaternion>::value > ::type >
    void FScanf_Vector4(T& vector4, FILE* p_file)
	{
		fscanf_s(p_file, "%f ", &vector4.x);
		fscanf_s(p_file, "%f ", &vector4.y);
		fscanf_s(p_file, "%f ", &vector4.z);
		fscanf_s(p_file, "%f\n", &vector4.w);
	}

private:
	std::string absolute_content_path;					//���� �۾� �����丮 ���

	std::vector<std::string> supported_texture_formats;	//Texture�� �����Ǵ� Ȯ���ڵ�
	std::vector<std::string> supported_model_formats;	//Model�� �����Ǵ� Ȯ���ڵ�
	std::vector<std::string> supported_audio_formats;	//Audio�� �����Ǵ� Ȯ���ڵ�
};