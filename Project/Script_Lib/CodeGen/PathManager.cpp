#include "stdafx.h"
#include "PathManager.h"

std::string PathManager::m_solution_path;
std::string PathManager::m_include_path;
std::string PathManager::m_project_path;
std::string PathManager::m_content_path;

void PathManager::Initialize()
{
	char char_buffer[256] = {};

    //Current Path
	//Exe ÆÄÀÏ: DX11_2D_GameEngine\ExeFile\Debug
	std::string current_path = std::filesystem::current_path().generic_string();

	//Solution Path
	{
		std::string solution_str = "DX11_2D_GameEngine";
		auto index = current_path.find(solution_str.c_str());
		m_solution_path = current_path.substr(0, (index + solution_str.length()));
		m_solution_path += "/";
	}
	
	//Include Path
	{
		m_include_path = m_solution_path + "External/Include/";
	}

	//Project Path
	{
		m_project_path = m_solution_path + "Project/";
	}

	//Content Path
	{
		m_content_path = m_solution_path + "ExeFile/Content/";
	}
}

std::string& PathManager::GetSolutionPath()
{
	return m_solution_path;
}

std::string& PathManager::GetIncludePath()
{
	return m_include_path;
}

std::string& PathManager::GetProjectPath()
{
	return m_project_path;
}

std::string& PathManager::GetContentPath()
{
	return m_content_path;
}
