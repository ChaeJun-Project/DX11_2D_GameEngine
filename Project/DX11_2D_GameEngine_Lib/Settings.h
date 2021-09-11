#pragma once
#include "stdafx.h"

//������ �Ӽ� �� ���� �뵵 Ŭ����
class Settings final : public Singleton<Settings>
{
	//�̱��� ��ũ��
	SINGLETON(Settings);
	Settings() {};
	virtual ~Settings() {};

public:
	//Instance Setting
	auto GetProgramInstance() const -> HINSTANCE { return m_program_instance; }
	void SetProgramInstance(HINSTANCE program_instance) { this->m_program_instance = program_instance; }

	//Handle Setting
	auto GetWindowHandle() const -> HWND { return m_window_handle; }
	void SetWindowHandle(HWND window_handle) { this->m_window_handle = window_handle; }

	//Width Setting
	auto GetWindowWidth() const -> const float& { return m_window_width; }
	void SetWindowWidth(const float& window_width) { this->m_window_width = window_width; }

	//Height Setting
	auto GetWindowHeight() const -> const float& { return m_window_height; }
	void SetWindowHeight(const float& window_height) { this->m_window_height = window_height; }

	//��������ȭ Setting
	auto IsVsync() const -> const bool& { return m_is_vsync; }
	void SetVsync(const bool& is_vsync) { this->m_is_vsync = is_vsync; }

private:
	//HINSTANCE: ���α׷��� ���� �޸𸮸� ������ �ִ�
	//���α׷��� �ν��Ͻ� �ĺ���(�ڵ�)
	//�����찡 �������� ���α׷����� �����ϱ� ���� �ĺ���
	HINSTANCE m_program_instance = nullptr;

	//HWND: ���α׷� �������� ������ �ĺ���(�ڵ�)
	//�ϳ��� ���α׷��� ���� �����츦 ���� �� �ֱ� ������ �ʿ�
	HWND m_window_handle = nullptr;

	float m_window_width = 0.0f; //������ â�� �ʺ�
	float m_window_height = 0.0f; //������ â�� ����

	bool m_is_vsync = false;
};