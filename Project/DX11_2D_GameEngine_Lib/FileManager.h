#pragma once
//������ �����ϱ� ���� Ŭ����(���� �� ����)
//�� Ŭ������ �Լ����� ���� Directory�� ��� ���ڿ� ������ ��ġ�� �־����
//�� Ŭ������ �Լ����� ���� File�� ��� ���ڿ� ������ Ȯ���ڱ��� �־����
//���� = ���丮 ���� ���� �ǹ�
class FileManager final : public Singleton<FileManager>
{
	SINGLETON(FileManager);

	FileManager() = default;
	~FileManager();

public: 
    void Initialize();
	const std::string& GetAbsoluteContentPath() { return m_absolute_content_path; }

	const std::string& GetAbsoluteAnimationPath() { return m_absolute_animation_path; }
	const std::string& GetAbsoluteAudioPath()	  { return m_absolute_audio_path; }
	const std::string& GetAbsoluteMaterialPath()  { return m_absolute_material_path; }
	const std::string& GetAbsoluteMeshPath()	  { return m_absolute_mesh_path; }
	const std::string& GetAbsoluteScenePath()	  { return m_absolute_scene_path; }
	const std::string& GetAbsoluteTexturePath()	  { return m_absolute_texture_path; }
	const std::string& GetAbsoluteTileMapPath()	  { return m_absolute_tilemap_path; }
	const std::string& GetAbsolutePrefabPath()	  { return m_absolute_prefab_path; }
	
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

	void RenameFile(const std::string& folder_path, const std::string& extension_name, const std::string& old_file_name, const std::string& new_file_name);
	void RemoveFile(const std::string& file_path);
	const std::string GetFileNameFromPath(const std::string& path); //Ư�� ��ο� �ִ� ������ �̸��� ��ȯ(Ȯ���� ����)
	const std::string GetOriginFileNameFromPath(const std::string& path); //Ư�� ��ο� �ִ� ������ �̸����� ��ȯ(Ȯ���� ������)
	const std::string GetDirectoryPathFromPath(const std::string& path); //Ư�� ��ο� �ִ� ������ ���� ������ ��θ� ��ȯ
	const std::string GetDirectoryFromPath(const std::string& path); //Ư�� ��ο� �ִ� ������ �̸��� ��ȯ
	const std::string GetExtensionFromPath(const std::string& path); //Ư�� ��ο� �ִ� ������ Ȯ���ڸ� ��ȯ(string)
	const std::wstring GetExtensionFromPath(const std::wstring& path); //Ư�� ��ο� �ִ� ������ Ȯ���ڸ� ��ȯ(wstring)
	const std::string GetPathWithoutExtension(const std::string& path); //Ư�� ��ο��� Ư�� ������ Ȯ���ڸ� ������ ������ ��θ� ��ȯ
	const std::string GetRelativeResourcePathFromAbsolutePath_1(const std::string& absolute_path); //���� ��θ� ���� �۾� ���丮 ��ο� ������� ��θ� ���� (Content ������ ���� ��������) (wstring -> string ��ȯ�� �� ���ڿ��� ���ؼ� ó��)
	const std::string GetRelativeResourcePathFromAbsolutePath_2(const std::string& absolute_path); //���� ��θ� ���� �۾� ���丮 ��ο� ������� ��θ� ���� (Content ������ ���� ��������)
	const std::string GetRelativeFromPath(const std::string& absolute_path); //���->����η� ��ȯ
	const std::string GetParentDirectory(const std::string& path); //Ư�� ����� �θ�����(���� �ֻ�ܿ� �ִ� ����)�� ��θ� ����
	const std::string GetWorkingDirectory(); //���� �������� ��θ� ��ȯ
	const std::string GetFileDataNameFromPath(const std::string& path); // Ư�� ��ο� �ִ� ������ �̸����� Ư¡���� �����ؼ� ��ȯ(�ִϸ��̼��� �����ϱ� ���� ���)

	const std::vector<std::string> GetDirectoriesInDirectory(const std::string& directory);
	const std::vector<std::string> GetFilesInDirectory(const std::string& directory);

	//�̸� �����ߴ� �����Ǵ� Ȯ���ڸ� ���� ����� ������ Ȯ���ڿ� ���Ͽ� �����Ǵ��� ���θ� Ȯ��
	const bool IsSupportedAnimationFile(const std::string& path); //Animation
	const bool IsSupportedAudioFile(const std::string& path);     //Audio
	const bool IsSupportedMaterialFile(const std::string& path);  //Material
	const bool IsSupportedMeshFile(const std::string& path);      //Mesh
	const bool IsSupportedPrefabFile(const std::string& path);    //Prefab
	const bool IsSupportedSceneFile(const std::string& path);     //Scene
	const bool IsSupportedTileMapFile(const std::string& path);   //TileMap
	const bool IsSupportedTextureFile(const std::string& path);   //Texture

	const std::string ToUppercase(const std::string& lower); //�ҹ��� -> �빮��
	const std::string ToLowercase(const std::string& upper); //�빮�� -> �ҹ���
	const std::string ConvertWStringToString(const std::wstring& wstr); //�����ڵ� -> ��Ƽ����Ʈ
	const std::wstring ConvertStringToWString(const std::string& str); //��Ƽ����Ʈ -> �����ڵ�
	const char* ConvertStringToUTF8(const std::string& str); //��Ƽ����Ʈ -> UTF8
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
	std::string m_absolute_content_path;		//Content���� ������

	std::string m_absolute_animation_path;		//Animation���� ������
	std::string m_absolute_audio_path;			//Audio���� ������
	std::string m_absolute_material_path;		//Material���� ������
	std::string m_absolute_mesh_path;			//Mesh���� ������
	std::string m_absolute_scene_path;			//Scene���� ������
	std::string m_absolute_texture_path;		//Texture���� ������
	std::string m_absolute_tilemap_path;		//TileMap���� ������
	std::string m_absolute_prefab_path;			//Prefab���� ������

	std::vector<std::string> m_supported_animation_formats;	//Animation�� �����Ǵ� Ȯ���ڵ�
	std::vector<std::string> m_supported_audio_formats;		//Audio�� �����Ǵ� Ȯ���ڵ�
	std::vector<std::string> m_supported_material_formats;	//Material�� �����Ǵ� Ȯ���ڵ�
	std::vector<std::string> m_supported_mesh_formats;		//Mesh�� �����Ǵ� Ȯ���ڵ�
	std::vector<std::string> m_supported_prefab_formats;	//Prefab�� �����Ǵ� Ȯ���ڵ�
	std::vector<std::string> m_supported_scene_formats;		//Scene�� �����Ǵ� Ȯ���ڵ�
	std::vector<std::string> m_supported_tilemap_formats;	//TileMap�� �����Ǵ� Ȯ���ڵ�
	std::vector<std::string> m_supported_texture_formats;	//Texture�� �����Ǵ� Ȯ���ڵ�
};