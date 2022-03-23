#include "stdafx.h"
#include "PathManager.h"

void main()
{
	//절대 경로(Solution, Include, Content) 초기화
	PathManager::Initialize();

	//현재 Script_Lib에 존재하는 모든 스크립트 코드 헤더 파일을 보관하고 있는 폴더의 경로
	//DX11_2D_GameEngine/External/Include/Script_Lib/
	std::string script_code_include_path = PathManager::GetIncludePath() + "Script_Lib/";

	//스크립트 코드 헤더 파일 저장
	WIN32_FIND_DATA script_file = {};
	HANDLE handle = FindFirstFile(std::string(script_code_include_path + "/*.h").c_str(), &script_file);

	if (INVALID_HANDLE_VALUE == handle)
		return;

	//Script 설정 폴더 경로
	//DX11_2D_GameEngine/ExeFile/Content/Script/
	std::string script_setting_path = PathManager::GetContentPath() + "Script/"; 

	//예외 파일 목록 파일 열기
	std::string exclude_list_path = script_setting_path + "Exclude_List.txt";
	FILE* p_exclude_list_file = nullptr;
	fopen_s(&p_exclude_list_file, exclude_list_path.c_str(), "rb");

	std::vector<std::string> exclude_file_name_vector;

	if (p_exclude_list_file != nullptr)
	{
		char char_buffer[256] = {};

		while (true)
		{
			int iLen = fscanf_s(p_exclude_list_file, "%s\n", char_buffer, 255);
			if (iLen == -1)
				break;

			exclude_file_name_vector.emplace_back(std::string(char_buffer));
		}
		fclose(p_exclude_list_file);
	}

	std::vector<std::string> script_name_vector;

	while (true)
	{
		//예외 파일 리스트에 포함되지 않은 경우, 스크립트 코드 파일로 판단
		bool is_exclude = false;

		std::string script_file_name = std::string(script_file.cFileName);

		for (size_t i = 0; i < exclude_file_name_vector.size(); ++i)
		{
			if (script_file_name._Equal(exclude_file_name_vector[i]))
			{
				is_exclude = true;
				break;
			}
		}

		if (!is_exclude)
		{
			script_name_vector.emplace_back(std::string(script_file.cFileName).substr(0, (script_file_name.length() - 2)));
		}

		//다음 스크립트 코드 파일 찾기
		if (!FindNextFile(handle, &script_file))
			break;
	}

	FindClose(handle);

	//이전에 CodeGen이 실행될 때 체크해둔 스크립트 코드 파일 목록
	std::string script_code_list_path = script_setting_path + "Script_List.txt";

	FILE* p_script_code_list_file = nullptr;
	fopen_s(&p_script_code_list_file, script_code_list_path.c_str(), "rb");

	if (p_script_code_list_file != nullptr)
	{
		std::vector<std::string> script_code_file_name_vector;

		char char_buffer[256] = {};

		while (true)
		{
			int iLen = fscanf_s(p_script_code_list_file, "%s\n", char_buffer, 255);
			if (iLen == -1)
				break;

			script_code_file_name_vector.emplace_back(std::string(char_buffer));
		}
		fclose(p_script_code_list_file);

		if (script_name_vector.size() == script_code_file_name_vector.size())
		{
			bool is_same = true;
			for (UINT i = 0; i < script_name_vector.size(); ++i)
			{
				//이전 Script_List 파일에 저장된 스크립트 파일 목록과 하나라도 다를 경우
				if (!script_name_vector[i]._Equal(script_code_file_name_vector[i]))
				{
					is_same = false;
					break;
				}
			}

			//이전 Script_List에 저장된 스크립트 파일 목록과, 현재 스크립트 파일 목록이 완전 일치할 경우, 변경하지 않음
			if (is_same)
				return;
		}
	}

	//==========================
	// ScriptManager
	//==========================
	//DX11_2D_GameEngine/ExeFile/Content/Script/
	std::string script_lib_project_path = PathManager::GetProjectPath() + "Script_Lib/";

	//ScriptManager Header
	FILE* p_script_manager_header_file = nullptr;
	std::string script_manager_header_path = script_lib_project_path + "ScriptManager.h";
	fopen_s(&p_script_manager_header_file, script_manager_header_path.c_str(), "wb");

	if (p_script_manager_header_file != nullptr)
	{
		//Header
		fprintf(p_script_manager_header_file, "#pragma once\n\n");
		fprintf(p_script_manager_header_file, "#include <vector>\n");
		fprintf(p_script_manager_header_file, "#include <string>\n\n");

		//Script Enum Type
		fprintf(p_script_manager_header_file, "enum class Script_Type : UINT\n{\n");
		for (UINT i = 0; i < script_name_vector.size(); ++i)
		{
			fprintf(p_script_manager_header_file, "\t");
			fprintf(p_script_manager_header_file, script_name_vector[i].c_str());
			fprintf(p_script_manager_header_file, ",\n");
		}
		fprintf(p_script_manager_header_file, "};\n\n");

		//class Script 전방선언
		fprintf(p_script_manager_header_file, "class Script;\n\n");

		//class ScriptManager 선언
		fprintf(p_script_manager_header_file, "class ScriptManager final\n{\n");
		//ScriptManager static 멤버함수 선언
		fprintf(p_script_manager_header_file, "public:\n");
		fprintf(p_script_manager_header_file, "\tstatic void GetScriptInfo(std::vector<std::string>& script_vector);\n");
		fprintf(p_script_manager_header_file, "\tstatic Script* GetScript(const std::string& script_name);\n");
		fprintf(p_script_manager_header_file, "};\n");

		fclose(p_script_manager_header_file);
	}

	//ScriptManager Cpp
	FILE* p_script_manager_cpp_file = nullptr;
	std::string script_manager_cpp_path = script_lib_project_path + "ScriptManager.cpp";
	fopen_s(&p_script_manager_cpp_file, script_manager_cpp_path.c_str(), "wb");

	if (p_script_manager_cpp_file != nullptr)
	{
		//Header
		fprintf(p_script_manager_cpp_file, "#include \"stdafx.h\"\n");
		fprintf(p_script_manager_cpp_file, "#include \"ScriptManager.h\"\n\n");
		
		//Script Header
		for (UINT i = 0; i < script_name_vector.size(); ++i)
		{
			fprintf(p_script_manager_cpp_file, "#include \"");
			fprintf(p_script_manager_cpp_file, script_name_vector[i].c_str());
			fprintf(p_script_manager_cpp_file, ".h\"\n");
		}

		//GetScriptInfo
		fprintf(p_script_manager_cpp_file, "\nvoid ScriptManager::GetScriptInfo(std::vector<std::string>& script_vector)\n{\n");
		for (UINT i = 0; i < script_name_vector.size(); ++i)
		{
			fprintf(p_script_manager_cpp_file, "\tscript_vector.emplace_back(\"");
			fprintf(p_script_manager_cpp_file, script_name_vector[i].c_str());
			fprintf(p_script_manager_cpp_file, "\");\n");
		}
		fprintf(p_script_manager_cpp_file, "}\n\n");

		//GetScript(Script Name)
		fprintf(p_script_manager_cpp_file, "Script* ScriptManager::GetScript(const std::string& script_name)\n{\n");
		for (UINT i = 0; i < script_name_vector.size(); ++i)
		{
			fprintf(p_script_manager_cpp_file, "\tif(");
			fprintf(p_script_manager_cpp_file, "script_name._Equal(\"");
			fprintf(p_script_manager_cpp_file, script_name_vector[i].c_str());
			fprintf(p_script_manager_cpp_file, "\"))\n");
			fprintf(p_script_manager_cpp_file, "\t\treturn new ");
			fprintf(p_script_manager_cpp_file, script_name_vector[i].c_str());
			fprintf(p_script_manager_cpp_file, ";\n");
		}
		fprintf(p_script_manager_cpp_file, "\n");
		fprintf(p_script_manager_cpp_file, "\treturn nullptr;\n}\n\n");

		fclose(p_script_manager_cpp_file);
	}

	//다음 번 빌드시 비교하기위한 Script_List 갱신
	fopen_s(&p_script_code_list_file, script_code_list_path.c_str(), "wb");
	if (p_script_code_list_file != nullptr)
	{
		for (UINT i = 0; i < script_name_vector.size(); ++i)
		{
			fprintf(p_script_code_list_file, script_name_vector[i].c_str());
			fprintf(p_script_code_list_file, "\n");
		}

		fclose(p_script_code_list_file);
	}
}