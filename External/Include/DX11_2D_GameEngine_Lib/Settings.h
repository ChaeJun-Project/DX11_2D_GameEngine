#pragma once
#include "stdafx.h"

//������ �Ӽ� �� ���� �뵵 Ŭ����
class Settings final
{
public:
	Settings() = default;
	~Settings() = default;

public:
	//Instance Setting
	HINSTANCE GetProgramInstance() const { return m_program_instance; }
	void SetProgramInstance(HINSTANCE program_instance) { m_program_instance = program_instance; }

	//Handle Setting
	HWND GetWindowHandle() const { return m_window_handle; }
	void SetWindowHandle(HWND window_handle) { m_window_handle = window_handle; }

	//Width Setting
	const UINT GetWindowWidth() const { return m_window_width; }
	void SetWindowWidth(const UINT& window_width) { m_window_width = window_width; }

	//Height Setting
	const UINT GetWindowHeight() const { return m_window_height; }
	void SetWindowHeight(const UINT& window_height) { m_window_height = window_height; }

	//Game Resolution
	const Vector2 GetGameResolution() const { return m_game_resolution; }
	void SetGameResolution(const Vector2 game_resolution) { m_game_resolution = game_resolution; }

	//��üȭ�� Setting
	const bool IsFullScreen() const { return m_is_full_screen; }
	void SetFullScreen(const bool& is_full_screen) { m_is_full_screen = is_full_screen; }

	//��������ȭ Setting
	const bool IsVsync() const { return m_is_vsync; }
	void SetVsync(const bool& is_vsync) { m_is_vsync = is_vsync; }

private:
	//HINSTANCE: ���α׷��� ���� �޸𸮸� ������ �ִ�
	//���α׷��� �ν��Ͻ� �ĺ���(�ڵ�)
	//�����찡 �������� ���α׷����� �����ϱ� ���� �ĺ���
	HINSTANCE m_program_instance = nullptr;

	//HWND: ���α׷� �������� ������ �ĺ���(�ڵ�)
	//�ϳ��� ���α׷��� ���� �����츦 ���� �� �ֱ� ������ �ʿ�
	HWND m_window_handle = nullptr;

	UINT m_window_width = 0; //������ â�� �ʺ�
	UINT m_window_height = 0; //������ â�� ����

	Vector2 m_game_resolution = Vector2::Zero; //���� �ػ�

	bool m_is_full_screen = false; //��üȭ�� ����
	bool m_is_vsync = false; //���� ����ȭ ����
};