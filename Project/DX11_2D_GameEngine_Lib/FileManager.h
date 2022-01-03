#pragma once

//파일을 관리하기 위한 클래스(생성 및 삭제)
//이 클래스의 함수명의 끝이 Directory인 경우 인자에 폴더의 위치만 넣어야함
//이 클래스의 함수명의 끝이 File인 경우 인자에 파일의 확장자까지 넣어야함
class FileManager final
{
public: 
    static void Initialize();
	static std::string GetAbsoluteContentPath() { return absolute_content_path; }

public:
	static void OpenDirectoryWindow(const std::string& directory);
	static const bool Create_Directory(const std::string& path); //특정 경로에 폴더를 생성
	static const bool Delete_Directory(const std::string& path); //특정 경로의 폴더 또는 파일을 삭제(폴더 내의 데이터를 모두 삭제)
	static const bool Delete_File(const std::string& path); //특정 경로의 파일을 삭제
	static const bool Copy_File(const std::string& src, const std::string& dst); //파일을 복사 src(source)의 내용을 dst(destination)에 복사
	static const bool IsDirectory(const std::string& path); //특정 경로가 폴더인지 확인
	static const bool IsExistDirectory(const std::string& path); //특정 경로에 폴더가 존재하는지 확인
	static const bool IsExistFile(const std::string& path); //특정 폴더에 파일이 존재하는지 확인
	static const std::vector<std::string> GetFileNameVectorFromDirectory(const std::string& path); //특정 경로의 디렉토리 내부의 모든 파일의 이름을 벡터로 저장하여 반환

	static const void RenameFileName(const std::string& folder_name, const std::string& extension_name, const std::string& old_file_name, const std::string& new_file_name);
	static const std::string GetFileNameFromPath(const std::string& path); //특정 경로에 있는 파일의 이름을 반환(확장자 포함)
	static const std::string GetIntactFileNameFromPath(const std::string& path); //특정 경로에 있는 파일의 이름만을 반환(확장자 미포함)
	static const std::string GetDirectoryFromPath(const std::string& path); //특정 경로에 있는 폴더의 이름을 반환
	static const std::string GetExtensionFromPath(const std::string& path); //특정 경로에 있는 파일의 확장자를 반환(string)
	static const std::wstring GetExtensionFromPath(const std::wstring& path); //특정 경로에 있는 파일의 확장자를 반환(wstring)
	static const std::string GetPathWithoutExtension(const std::string& path); //특정 경로에서 특정 파일의 확장자만 제외한 나머지 경로를 반환
	static const std::string GetRelativeFromPath(const std::string& absolute_path); //경로->상대경로로 변환
	static const std::string GetParentDirectory(const std::string& path); //특정 경로의 부모폴더(가장 최상단에 있는 폴더)의 경로를 구함
	static const std::string GetWorkingDirectory(); //현재 실행중인 경로를 반환
	static const std::string GetFileDataNameFromPath(const std::string& path); // 특정 경로에 있는 파일의 이름에서 특징만을 추출해서 반환(애니메이션을 구분하기 위해 사용)

	static const std::vector<std::string> GetDirectoriesInDirectory(const std::string& directory);
	static const std::vector<std::string> GetFilesInDirectory(const std::string& directory);

	static const std::vector<std::string>& GetSupportedTextureFormats() { return supported_texture_formats; }
	static const std::vector<std::string>& GetSupportedModelFormats() { return supported_model_formats; }
	static const std::vector<std::string>& GetSupportedAudioFormats() { return supported_audio_formats; }

	//미리 정의했던 지원되는 확장자를 현재 경로의 파일의 확장자와 비교하여 지원되는지 여부를 확인
	//Texture, Model, Audio
	static const bool IsSupportedTextureFile(const std::string& path);   //Texture
	static const bool IsSupportedModelFile(const std::string& path);     //Model
	static const bool IsSupportedAudioFile(const std::string& path);     //Audio

	static const std::string ToUppercase(const std::string& lower); //소문자 -> 대문자
	static const std::string ToLowercase(const std::string& upper); //대문자 -> 소문자
	static const std::string ConvertWStringToString(const std::wstring& wstr); //유니코드 -> 멀티바이트
	static const std::wstring ConvertStringToWString(const std::string& str); //멀티바이트 -> 유니코드
	static const char* ConvertStringToChar(const std::string& message); //String -> Char 변환
	static const std::string ConvertCharToString(const char* message); //Char -> String 변환

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

	static std::vector<std::string> supported_texture_formats;		//Texture에 지원되는 확장자들
	static std::vector<std::string> supported_model_formats;		//Model에 지원되는 확장자들
	static std::vector<std::string> supported_audio_formats;		//Audio에 지원되는 확장자들
};