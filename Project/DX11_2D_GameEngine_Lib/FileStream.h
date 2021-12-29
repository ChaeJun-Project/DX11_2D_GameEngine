#pragma once

enum FILE_STREAM_MODE : UINT
{
	FILE_STREAM_READ = 1U << 0,
	FILE_STREAM_WRITE = 1U << 1,
	FILE_STREAM_APPEND = 1U << 2,
};

//파일 입출력 및 추가를 담당하는 클래스 (리소스를 읽고 저장하는데에 사용)
class FileStream final
{
public:
	FileStream(const std::string& path, const UINT& flags);
	~FileStream();

	const bool  IsOpen() const { return m_is_open; }
	void Close();
	void Skip(const UINT& n);

	//=================================================================================================================
	//[Write] (프로그램내 데이터를 바이너리 형식으로 파일에 저장하기)
	//=================================================================================================================
	template <typename T, typename = typename std::enable_if< //std::enable_if를 사용하여 true인 경우에만 템플릿 치환을 가능하도록 함
		std::is_same<T, bool>::value || //std::is_same을 사용하여 T가 특정 자료형으로 치환될 경우만 true
		std::is_same<T, char>::value ||
		std::is_same<T, byte>::value ||
		std::is_same<T, int>::value ||
		std::is_same<T, UINT>::value ||
		std::is_same<T, long>::value ||
		std::is_same<T, unsigned long>::value ||
		std::is_same<T, long long>::value ||
		std::is_same<T, unsigned long long>::value ||
		std::is_same<T, float>::value ||
		std::is_same<T, double>::value ||
		std::is_same<T, long double>::value ||
		std::is_same<T, std::byte>::value ||
		std::is_same<T, Vector2>::value ||
		std::is_same<T, Vector3>::value ||
		std::is_same<T, Vector4>::value ||
		std::is_same<T, Vector4>::value ||
		std::is_same<T, Quaternion>::value ||
		std::is_same<T, Matrix>::value>::type>
		void Write(const T& value)
	{
		m_f_out.write(reinterpret_cast<const char*>(&value), sizeof(T)); //인자로 받은 value값을 파일에 바이너리 형식으로 저장
	}

	//Write함수 오버로딩
	void Write(const void* value, const UINT& size);
	void Write(const std::string& value);
	void Write(const std::vector<std::byte>& value);

	//=================================================================================================================
	//[Read] (바이너리 형식으로 파일에 저장된 데이터를 프로그램에서 사용하기)
	//=================================================================================================================
	template <typename T, typename = typename std::enable_if< //std::enable_if를 사용하여 true인 경우에만 템플릿 치환을 가능하도록 함
		std::is_same<T, bool>::value || //std::is_same을 사용하여 T가 특정 자료형으로 치환될 경우만 true
		std::is_same<T, char>::value ||
		std::is_same<T, byte>::value ||
		std::is_same<T, int>::value ||
		std::is_same<T, UINT>::value ||
		std::is_same<T, long>::value ||
		std::is_same<T, unsigned long>::value ||
		std::is_same<T, long long>::value ||
		std::is_same<T, unsigned long long>::value ||
		std::is_same<T, float>::value ||
		std::is_same<T, double>::value ||
		std::is_same<T, long double>::value ||
		std::is_same<T, std::byte>::value ||
		std::is_same<T, Vector2>::value ||
		std::is_same<T, Vector3>::value ||
		std::is_same<T, Vector4>::value ||
		std::is_same<T, Vector4>::value ||
		std::is_same<T, Quaternion>::value ||
		std::is_same<T, Matrix>::value>::type>
		void Read(T& value)
	{
		m_f_in.read(reinterpret_cast<char*>(&value), sizeof(T)); //파일에 바이너리 형식으로 저장된 데이터를 인자로 받은 value에 저장
	}

	//Read함수 오버로딩
	void Read(std::string& value);
	void Read(std::vector<std::byte>& value);

	template <typename T, typename = typename std::enable_if<
		std::is_same<T, bool>::value ||
		std::is_same<T, char>::value ||
		std::is_same<T, byte>::value ||
		std::is_same<T, int>::value ||
		std::is_same<T, UINT>::value ||
		std::is_same<T, long>::value ||
		std::is_same<T, unsigned long>::value ||
		std::is_same<T, long long>::value ||
		std::is_same<T, unsigned long long>::value ||
		std::is_same<T, float>::value ||
		std::is_same<T, double>::value ||
		std::is_same<T, long double>::value ||
		std::is_same<T, std::byte>::value ||
		std::is_same<T, Vector2>::value ||
		std::is_same<T, Vector3>::value ||
		std::is_same<T, Vector4>::value ||
		std::is_same<T, Vector4>::value ||
		std::is_same<T, Quaternion>::value ||
		std::is_same<T, Matrix>::value>::type>
		const T Read()
	{
		T value;
		Read(value);

		return value;
	}

private:
	std::ofstream m_f_out; //프로그램 내의 데이터를 프로그램내 스트림 파일에 저장
	std::ifstream m_f_in;  //프로그램내 스트림 파일의 데이터를 프로그램 내에 사용

	UINT m_flags = 0; //파일 스트림을 어떤 용도로 사용하는지
	bool m_is_open = false; //파일이 성공적으로 열렸는지 확인
};