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

	//가변인자(...) 사용함수들 -> scanf와 printf와 비슷하게 구현
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
	void SaveLogToLogger(const LogType& type, const char* text); //입력받은 Log 내용을 Logger에 입력
	void SaveLogToFile();										//입력받은 Log를 파일에 기록

private:
	std::string m_caller_name;
	std::mutex m_log_mutex; //Log 기록을 동기화하기 위한 뮤텍스
	std::weak_ptr<Logger> m_p_current_logger;
};

