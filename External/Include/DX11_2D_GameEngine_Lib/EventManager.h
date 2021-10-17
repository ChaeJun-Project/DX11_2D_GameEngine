#pragma once

struct EventStruct
{
	EventType event_type;
	DWORD_PTR object_address;
	DWORD_PTR layer_index;
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

private:
     std::queue<EventStruct> m_event_struct_queue;
	 std::queue<GameObject*> m_p_dead_game_object_queue;
};

template<typename T>
inline void EventManager::ClearQueue(std::queue<T>& queue)
{
    std::queue<T> empty_queue;
	std::swap(queue, empty_queue);
}
