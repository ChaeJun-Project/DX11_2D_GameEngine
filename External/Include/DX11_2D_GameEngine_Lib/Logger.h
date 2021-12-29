#pragma once

struct Log final
{
	LogType type; //어떤 종류를 출력할지
	std::string text; //출력할 메시지
};

typedef std::function<void(const Log&)> Log_CallBack;

class Logger final
{
public:
	Logger(const std::string& log_file_name);
	~Logger();
	
public:
	void SetCallBack(const Log_CallBack& log_func);
	void ExcuteCallBack(const LogType& type, const std::string& text);
	void SaveToFile();
	
private:
	std::string m_log_path = "Log/";
	std::string m_log_file_name;
	std::vector<Log> m_log_vector;

	Log_CallBack m_p_log_func = nullptr;

	friend class LogManager;
};

