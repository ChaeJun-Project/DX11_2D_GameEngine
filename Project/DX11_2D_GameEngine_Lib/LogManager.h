#pragma once

class Logger;

class LogManager final : public Singleton<LogManager>
{
	SINGLETON(LogManager);

	LogManager() = default;
	~LogManager();

public:
	void Write(const LogType& type, const char* text);
	void Write(const LogType& type, const std::string& text);

	//��������(...) ����Լ��� -> scanf�� printf�� ����ϰ� ����
	void Info_Formatted(const char* text, ...);
	void Warning_Formatted(const char* text, ...);
	void Error_Formatted(const char* text, ...);

	void SetLogger(const std::weak_ptr<class Logger>& logger)
	{
		m_p_current_logger = logger;
	}

	void SetCallerName(const std::string& caller_name) 
	{ 
		m_caller_name = caller_name;
	}

private: 
	void SaveLogToLogger(const LogType& type, const char* text); //�Է¹��� Log ������ Logger�� �Է�
	void SaveLogToFile();										//�Է¹��� Log�� ���Ͽ� ���

private:
	std::string m_caller_name;
	std::mutex m_log_mutex; //Log ����� ����ȭ�ϱ� ���� ���ؽ�
	std::weak_ptr<Logger> m_p_current_logger;
};

