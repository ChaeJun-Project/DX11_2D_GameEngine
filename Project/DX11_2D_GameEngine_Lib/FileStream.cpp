#include "stdafx.h"
#include "FileStream.h"

FileStream::FileStream(const std::string& path, const UINT& flags)
{
	m_is_open = false;
	m_flags = flags;

	int ios_flags = std::ios::binary; //������ ���̳ʸ� �������� ����� Ȥ�� �߰��� ��
	ios_flags |= (flags & FILE_STREAM_WRITE) ? std::ios::out : 0U; //���̳ʸ� ���� + ���Ͽ� ����
	ios_flags |= (flags & FILE_STREAM_READ) ? std::ios::in : 0U; //���̳ʸ� ���� + ���ϵ����� �б�
	ios_flags |= (flags & FILE_STREAM_APPEND) ? std::ios::app : 0U; //���̳ʸ� ���� + ���Ͽ� �߰�

	if (flags & FILE_STREAM_WRITE)
	{
		m_f_out.open(path, ios_flags);
		if (m_f_out.fail())
		{
			return;
		}
	}
	else if (flags & FILE_STREAM_READ)
	{
		m_f_in.open(path, ios_flags);
		if (m_f_in.fail())
		{
			return;
		}
	}

	m_is_open = true;

}

FileStream::~FileStream()
{
	Close();
}

void FileStream::Close()
{
	if (m_flags & FILE_STREAM_WRITE)
	{
		m_f_out.flush();
		m_f_out.close();
	}
	else if (m_flags & FILE_STREAM_READ)
	{
		m_f_in.clear();
		m_f_in.close();
	}
}

void FileStream::Skip(const UINT& n)
{
	if (m_flags & FILE_STREAM_WRITE)
		m_f_out.seekp(n, std::ios::cur); //���� ��Ʈ���� Ŀ����ġ���� n��ŭ ������ ������ �ٽ� ����
	else if (m_flags & FILE_STREAM_READ)
		m_f_in.ignore(n, std::ios::cur); //���� ��Ʈ���� Ŀ����ġ������ n���� ���ڸ� ����
}

void FileStream::Write(const void* value, const UINT& size)
{
	m_f_out.write(reinterpret_cast<const char*>(value), size);
}

void FileStream::Write(const std::string& value)
{
	const auto length = static_cast<UINT>(value.length());
	Write(length);

	m_f_out.write(value.c_str(), length);
}

void FileStream::Write(const std::vector<std::byte>& value)
{
	const auto length = static_cast<UINT>(value.size());
	Write(length);

	m_f_out.write(reinterpret_cast<const char*>(value.data()), length);
}

void FileStream::Read(std::string& value)
{
	auto length = Read<UINT>();

	value.clear();
	value.shrink_to_fit();

	value.reserve(length);
	value.resize(length);

	m_f_in.read(value.data(), length);
}

void FileStream::Read(std::vector<std::byte>& value)
{
	auto length = Read<UINT>();

	value.clear();
	value.shrink_to_fit();

	value.reserve(length);
	value.resize(length);

	m_f_in.read(reinterpret_cast<char*>(value.data()), length);
}
