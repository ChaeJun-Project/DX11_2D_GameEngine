#pragma once

struct EventStruct
{
	EventType event_type;
	DWORD_PTR object_address_1; //�̺�Ʈ ó���� �ش��ϴ� GameObject�� �ּ�
	DWORD_PTR object_address_2; //�ڽ� GameObject�� ������ �� �ʿ��� �ڽ� GameObject�� �ּ�
	UINT layer_index;
};

class GameObject;

class EventManager final :public Singleton<EventManager>
{
	SINGLETON(EventManager);
	EventManager() = default;
	~EventManager();

	template<typename T>
	void ClearQueue(std::queue<T>& queue);

public:
     void Update();

private:
	void Excute(const EventStruct& event_struct);

public:
     void AddEvent(const EventStruct& event_struct);

	 const bool IsUpdate() { return  m_is_update; }

private:
     bool m_is_update = false;

     std::queue<EventStruct> m_event_struct_queue;
	 std::queue<GameObject*> m_p_dead_game_object_queue;
};

template<typename T>
inline void EventManager::ClearQueue(std::queue<T>& queue)
{
    std::queue<T> empty_queue;
	std::swap(queue, empty_queue);
}
