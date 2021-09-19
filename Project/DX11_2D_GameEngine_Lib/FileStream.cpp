#include "stdafx.h"
#include "FileStream.h"

FileStream::FileStream(const std::string& path, const UINT& flags)
{
	this->is_open = false;
	this->flags = flags;

	int ios_flags = std::ios::binary; //파일을 바이너리 형식으로 입출력 혹은 추가를 함
	ios_flags |= (this->flags & FILE_STREAM_WRITE) ? std::ios::out : 0U; //바이너리 형식 + 파일에 쓰기
	ios_flags |= (this->flags & FILE_STREAM_READ) ? std::ios::in : 0U; //바이너리 형식 + 파일데이터 읽기
	ios_flags |= (this->flags & FILE_STREAM_APPEND) ? std::ios::app : 0U; //바이너리 형식 + 파일에 추가

	if (this->flags & FILE_STREAM_WRITE)
	{
		f_out.open(path, ios_flags);
		if (f_out.fail())
		{
			return;
		}
	}
	else if (this->flags & FILE_STREAM_READ)
	{
		f_in.open(path, ios_flags);
		if (f_in.fail())
		{
			return;
		}
	}

	is_open = true;

}

FileStream::~FileStream()
{
	Close();
}

void FileStream::Close()
{
	if (this->flags & FILE_STREAM_WRITE)
	{
		f_out.flush();
		f_out.close();
	}
	else if (this->flags & FILE_STREAM_READ)
	{
		f_in.clear();
		f_in.close();
	}
}

void FileStream::Skip(const UINT& n)
{
	if (flags & FILE_STREAM_WRITE)
		f_out.seekp(n, std::ios::cur); //현재 스트림내 커서위치에서 n만큼 떨어진 곳에서 다시 쓰기
	else if (flags & FILE_STREAM_READ)
		f_in.ignore(n, std::ios::cur); //현재 스트림내 커서위치까지만 n개의 문자를 읽음
}

void FileStream::Write(const void* value, const UINT& size)
{
	f_out.write(reinterpret_cast<const char*>(value), size);
}

void FileStream::Write(const std::string& value)
{
	const auto length = static_cast<UINT>(value.length());
	Write(length);

	f_out.write(value.c_str(), length);
}

void FileStream::Write(const std::vector<std::byte>& value)
{
	const auto length = static_cast<UINT>(value.size());
	Write(length);

	f_out.write(reinterpret_cast<const char*>(value.data()), length);
}

void FileStream::Read(std::string& value)
{
	auto length = Read<UINT>();

	value.clear();
	value.shrink_to_fit();

	value.reserve(length);
	value.resize(length);

	f_in.read(value.data(), length);
}

void FileStream::Read(std::vector<std::byte>& value)
{
	auto length = Read<UINT>();

	value.clear();
	value.shrink_to_fit();

	value.reserve(length);
	value.resize(length);

	f_in.read(reinterpret_cast<char*>(value.data()), length);
}
