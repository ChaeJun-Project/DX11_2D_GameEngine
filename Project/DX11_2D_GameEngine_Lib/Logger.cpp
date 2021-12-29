#include "stdafx.h"
#include "Logger.h"

Logger::Logger(const std::string& log_file_name)
	:m_log_file_name(log_file_name)
{
	m_log_file_name = m_log_path + m_log_file_name;
}

Logger::~Logger()
{
	m_log_vector.clear();
	m_log_vector.shrink_to_fit();
}

void Logger::SetCallBack(const Log_CallBack& log_func)
{
	m_p_log_func = log_func;
}

void Logger::ExcuteCallBack(const LogType& type, const std::string& text)
{
	//string, vector, list와 같은 컨테이너가 포함된 구조체는 Zeromemory or memset을 사용하면 메모리 누수 이슈가 발생
	Log log;
	log.type = type;
	log.text = text;

	m_log_vector.emplace_back(log);
	m_p_log_func(log);
}

void Logger::SaveToFile()
{
	FILE* p_file = nullptr;
	fopen_s(&p_file, m_log_file_name.c_str(), "wb");
	assert(p_file);

	for (auto& log : m_log_vector)
	{
		fprintf(p_file, "%s\n", log.text.c_str());
	}

	fclose(p_file);
}