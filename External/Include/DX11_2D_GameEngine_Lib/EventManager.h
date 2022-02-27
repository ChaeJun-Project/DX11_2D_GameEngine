#pragma once

class Scene;
class GameObject;

typedef std::variant<GameObject*, std::shared_ptr<Scene>> EventData;

struct EventStruct
{
	EventType event_type;
	EventData object_address_1; //�̺�Ʈ ó���� �ش��ϴ� GameObject�� �ּ�
	EventData object_address_2; //�ڽ� GameObject�� ������ �� �ʿ��� �ڽ� GameObject�� �ּ�
};

class EventManager final :public Singleton<EventManager>
{
	SINGLETON(EventManager);

	EventManager() = default;
	~EventManager();

public:
     void Update();

private:
	void Excute(const EventStruct& event_struct);

	void CreateGameObject(const EventStruct& event_struct);
	void DeleteGameObject(const EventStruct& event_struct);
	void AddChildGameObject(const EventStruct& event_struct);
	void ChangeScene(const EventStruct& event_struct);

public:
     void AddEvent(const EventStruct& event_struct);

	 const bool IsUpdate() { return  m_is_update; }

private:
     bool m_is_update = false;

     std::queue<EventStruct> m_event_struct_queue;
	 std::queue<GameObject*> m_p_dead_game_object_queue;
};