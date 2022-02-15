#pragma once

#include "stdafx.h"

class PathManager final
{
public:
   static void Initialize();

public:
   static std::string& GetSolutionPath();
   static std::string& GetIncludePath();
   static std::string& GetProjectPath();
   static std::string& GetContentPath();

private:
	static std::string m_solution_path;
	static std::string m_include_path;
	static std::string m_project_path;
	static std::string m_content_path;
};

