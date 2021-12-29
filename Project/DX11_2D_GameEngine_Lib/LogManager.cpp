#include "stdafx.h"
#include "LogManager.h"

#include "Logger.h"

LogManager::~LogManager()
{
	m_p_current_logger.reset();
}

void LogManager::Write(const LogType& type, const char* text)
{
	if (text == nullptr)
		return;

	//lock_guard 이 함수는 하나의 스레드만 log_mutex의 사용 권한을 가지도록 해줌
	//임계영역(한 번에 하나의 스레드만 사용가능)을 만들기 위해 사용
	std::lock_guard<std::mutex> guard(m_log_mutex);

	std::string log_type_str;

	switch (type)
	{
	case LogType::Info:
		log_type_str = "[Info] ";
		break;
	case LogType::Warning:
		log_type_str = "[Warning] ";
		break;
	case LogType::Error:
		log_type_str = "[Error] ";
		break;
	}

	//호출한 함수가 기록되어 있다면
	const auto& formatted_text = m_caller_name.empty() ? CURRENT_TIME + log_type_str + std::string(text) : CURRENT_TIME + log_type_str + m_caller_name + " : " + std::string(text);

	//로그를 로거에 등록
	SaveLogToLogger(type, formatted_text.c_str());
	SaveLogToFile();
	
	//호출한 함수이름 초기화
	m_caller_name.clear();
}

void LogManager::Write(const LogType& type, const std::string& text)
{
	if (text.empty())
		return;

	Write(type, text.c_str());
}

void LogManager::Info_Formatted(const char* text, ...)
{
	char buffer[1024];
	va_list args; //가변인수 선언
	va_start(args, text); //스택 상의 첫 가변인수의 위치를 구해서 args에 대입. text는 고정 인수인데 가변인수의 시작번지(가변인수의 다음 번지)를 찾기 위해 필요.
	vsnprintf(buffer, sizeof(buffer), text, args); //sprintf와 비슷한 역할을 함(%d, %s, %c 등의 형식을 읽어 변환해주는 역할).
	va_end(args); //가변인수를 다 읽은 후, 함수가 정상적으로 리턴되도록 함. args가 변경되고 가변인수를 다시 읽을 수 없도록 함.

	Write(LogType::Info, buffer);
}

void LogManager::Warning_Formatted(const char* text, ...)
{
	char buffer[1024];
	va_list args; //가변인수 선언
	va_start(args, text); //스택 상의 첫 가변인수의 위치를 구해서 args에 대입. text는 고정 인수인데 가변인수의 시작번지(가변인수의 다음 번지)를 찾기 위해 필요.
	vsnprintf(buffer, sizeof(buffer), text, args); //sprintf와 비슷한 역할을 함(%d, %s, %c 등의 형식을 읽어 변환해주는 역할).
	va_end(args); //가변인수를 다 읽은 후, 함수가 정상적으로 리턴되도록 함. args가 변경되고 가변인수를 다시 읽을 수 없도록 함.

	Write(LogType::Warning, buffer);
}

void LogManager::Error_Formatted(const char* text, ...)
{
	char buffer[1024];
	va_list args; //가변인수 선언
	va_start(args, text); //스택 상의 첫 가변인수의 위치를 구해서 args에 대입. text는 고정 인수인데 가변인수의 시작번지(가변인수의 다음 번지)를 찾기 위해 필요.
	vsnprintf(buffer, sizeof(buffer), text, args); //sprintf와 비슷한 역할을 함(%d, %s, %c 등의 형식을 읽어 변환해주는 역할).
	va_end(args); //가변인수를 다 읽은 후, 함수가 정상적으로 리턴되도록 함. args가 변경되고 가변인수를 다시 읽을 수 없도록 함.

	Write(LogType::Error, buffer);
}

void LogManager::SaveLogToLogger(const LogType& type, const char* text)
{
	if (text == nullptr)
		return;

	m_p_current_logger.lock()->ExcuteCallBack(type, std::string(text));
}

void LogManager::SaveLogToFile()
{
	m_p_current_logger.lock()->SaveToFile();
}