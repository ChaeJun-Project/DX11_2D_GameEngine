#pragma once
//파일을 관리하기 위한 클래스(생성 및 삭제)
//이 클래스의 함수명의 끝이 Directory인 경우 인자에 폴더의 위치만 넣어야함
//이 클래스의 함수명의 끝이 File인 경우 인자에 파일의 확장자까지 넣어야함
//폴더 = 디렉토리 서로 같은 의미
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
	const bool Create_Directory(const std::string& path); //특정 경로에 폴더를 생성
	const bool Delete_Directory(const std::string& path); //특정 경로의 폴더 또는 파일을 삭제(폴더 내의 데이터를 모두 삭제)
	const bool Delete_File(const std::string& path); //특정 경로의 파일을 삭제
	const bool Copy_File(const std::string& src, const std::string& dst); //파일을 복사 src(source)의 내용을 dst(destination)에 복사
	const bool IsDirectory(const std::string& path); //특정 경로가 폴더인지 확인
	const bool IsExistDirectory(const std::string& path); //특정 경로에 폴더가 존재하는지 확인
	const bool IsExistFile(const std::string& path); //특정 폴더에 파일이 존재하는지 확인
	const std::vector<std::string> GetFileNameVectorFromDirectory(const std::string& path); //특정 경로의 디렉토리 내부의 모든 파일의 이름을 벡터로 저장하여 반환

	void RenameFile(const std::string& folder_path, const std::string& extension_name, const std::string& old_file_name, const std::string& new_file_name);
	void RemoveFile(const std::string& file_path);
	const std::string GetFileNameFromPath(const std::string& path); //특정 경로에 있는 파일의 이름을 반환(확장자 포함)
	const std::string GetOriginFileNameFromPath(const std::string& path); //특정 경로에 있는 파일의 이름만을 반환(확장자 미포함)
	const std::string GetDirectoryPathFromPath(const std::string& path); //특정 경로에 있는 파일이 속한 폴더의 경로를 반환
	const std::string GetDirectoryFromPath(const std::string& path); //특정 경로에 있는 폴더의 이름을 반환
	const std::string GetExtensionFromPath(const std::string& path); //특정 경로에 있는 파일의 확장자를 반환(string)
	const std::wstring GetExtensionFromPath(const std::wstring& path); //특정 경로에 있는 파일의 확장자를 반환(wstring)
	const std::string GetPathWithoutExtension(const std::string& path); //특정 경로에서 특정 파일의 확장자만 제외한 나머지 경로를 반환
	const std::string GetRelativeResourcePathFromAbsolutePath_1(const std::string& absolute_path); //절대 경로를 현재 작업 디렉토리 경로에 상대적인 경로를 구함 (Content 폴더의 하위 폴더부터) (wstring -> string 변환을 한 문자열에 대해서 처리)
	const std::string GetRelativeResourcePathFromAbsolutePath_2(const std::string& absolute_path); //절대 경로를 현재 작업 디렉토리 경로에 상대적인 경로를 구함 (Content 폴더의 하위 폴더부터)
	const std::string GetRelativeFromPath(const std::string& absolute_path); //경로->상대경로로 변환
	const std::string GetParentDirectory(const std::string& path); //특정 경로의 부모폴더(가장 최상단에 있는 폴더)의 경로를 구함
	const std::string GetWorkingDirectory(); //현재 실행중인 경로를 반환
	const std::string GetFileDataNameFromPath(const std::string& path); // 특정 경로에 있는 파일의 이름에서 특징만을 추출해서 반환(애니메이션을 구분하기 위해 사용)

	const std::vector<std::string> GetDirectoriesInDirectory(const std::string& directory);
	const std::vector<std::string> GetFilesInDirectory(const std::string& directory);

	//미리 정의했던 지원되는 확장자를 현재 경로의 파일의 확장자와 비교하여 지원되는지 여부를 확인
	const bool IsSupportedAnimationFile(const std::string& path); //Animation
	const bool IsSupportedAudioFile(const std::string& path);     //Audio
	const bool IsSupportedMaterialFile(const std::string& path);  //Material
	const bool IsSupportedMeshFile(const std::string& path);      //Mesh
	const bool IsSupportedPrefabFile(const std::string& path);    //Prefab
	const bool IsSupportedSceneFile(const std::string& path);     //Scene
	const bool IsSupportedTileMapFile(const std::string& path);   //TileMap
	const bool IsSupportedTextureFile(const std::string& path);   //Texture

	const std::string ToUppercase(const std::string& lower); //소문자 -> 대문자
	const std::string ToLowercase(const std::string& upper); //대문자 -> 소문자
	const std::string ConvertWStringToString(const std::wstring& wstr); //유니코드 -> 멀티바이트
	const std::wstring ConvertStringToWString(const std::string& str); //멀티바이트 -> 유니코드
	const char* ConvertStringToUTF8(const std::string& str); //멀티바이트 -> UTF8
	const char* ConvertStringToChar(const std::string& message); //String -> Char 변환
	const std::string ConvertCharToString(const char* message); //Char -> String 변환

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
	std::string m_absolute_content_path;		//Content폴더 절대경로

	std::string m_absolute_animation_path;		//Animation폴더 절대경로
	std::string m_absolute_audio_path;			//Audio폴더 절대경로
	std::string m_absolute_material_path;		//Material폴더 절대경로
	std::string m_absolute_mesh_path;			//Mesh폴더 절대경로
	std::string m_absolute_scene_path;			//Scene폴더 절대경로
	std::string m_absolute_texture_path;		//Texture폴더 절대경로
	std::string m_absolute_tilemap_path;		//TileMap폴더 절대경로
	std::string m_absolute_prefab_path;			//Prefab폴더 절대경로

	std::vector<std::string> m_supported_animation_formats;	//Animation에 지원되는 확장자들
	std::vector<std::string> m_supported_audio_formats;		//Audio에 지원되는 확장자들
	std::vector<std::string> m_supported_material_formats;	//Material에 지원되는 확장자들
	std::vector<std::string> m_supported_mesh_formats;		//Mesh에 지원되는 확장자들
	std::vector<std::string> m_supported_prefab_formats;	//Prefab에 지원되는 확장자들
	std::vector<std::string> m_supported_scene_formats;		//Scene에 지원되는 확장자들
	std::vector<std::string> m_supported_tilemap_formats;	//TileMap에 지원되는 확장자들
	std::vector<std::string> m_supported_texture_formats;	//Texture에 지원되는 확장자들
};