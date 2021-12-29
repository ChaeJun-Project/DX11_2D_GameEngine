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

	//lock_guard �� �Լ��� �ϳ��� �����常 log_mutex�� ��� ������ �������� ����
	//�Ӱ迵��(�� ���� �ϳ��� �����常 ��밡��)�� ����� ���� ���
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

	//ȣ���� �Լ��� ��ϵǾ� �ִٸ�
	const auto& formatted_text = m_caller_name.empty() ? CURRENT_TIME + log_type_str + std::string(text) : CURRENT_TIME + log_type_str + m_caller_name + " : " + std::string(text);

	//�α׸� �ΰſ� ���
	SaveLogToLogger(type, formatted_text.c_str());
	SaveLogToFile();
	
	//ȣ���� �Լ��̸� �ʱ�ȭ
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
	va_list args; //�����μ� ����
	va_start(args, text); //���� ���� ù �����μ��� ��ġ�� ���ؼ� args�� ����. text�� ���� �μ��ε� �����μ��� ���۹���(�����μ��� ���� ����)�� ã�� ���� �ʿ�.
	vsnprintf(buffer, sizeof(buffer), text, args); //sprintf�� ����� ������ ��(%d, %s, %c ���� ������ �о� ��ȯ���ִ� ����).
	va_end(args); //�����μ��� �� ���� ��, �Լ��� ���������� ���ϵǵ��� ��. args�� ����ǰ� �����μ��� �ٽ� ���� �� ������ ��.

	Write(LogType::Info, buffer);
}

void LogManager::Warning_Formatted(const char* text, ...)
{
	char buffer[1024];
	va_list args; //�����μ� ����
	va_start(args, text); //���� ���� ù �����μ��� ��ġ�� ���ؼ� args�� ����. text�� ���� �μ��ε� �����μ��� ���۹���(�����μ��� ���� ����)�� ã�� ���� �ʿ�.
	vsnprintf(buffer, sizeof(buffer), text, args); //sprintf�� ����� ������ ��(%d, %s, %c ���� ������ �о� ��ȯ���ִ� ����).
	va_end(args); //�����μ��� �� ���� ��, �Լ��� ���������� ���ϵǵ��� ��. args�� ����ǰ� �����μ��� �ٽ� ���� �� ������ ��.

	Write(LogType::Warning, buffer);
}

void LogManager::Error_Formatted(const char* text, ...)
{
	char buffer[1024];
	va_list args; //�����μ� ����
	va_start(args, text); //���� ���� ù �����μ��� ��ġ�� ���ؼ� args�� ����. text�� ���� �μ��ε� �����μ��� ���۹���(�����μ��� ���� ����)�� ã�� ���� �ʿ�.
	vsnprintf(buffer, sizeof(buffer), text, args); //sprintf�� ����� ������ ��(%d, %s, %c ���� ������ �о� ��ȯ���ִ� ����).
	va_end(args); //�����μ��� �� ���� ��, �Լ��� ���������� ���ϵǵ��� ��. args�� ����ǰ� �����μ��� �ٽ� ���� �� ������ ��.

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