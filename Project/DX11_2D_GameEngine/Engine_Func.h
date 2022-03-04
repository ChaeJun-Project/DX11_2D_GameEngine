#pragma once

inline void UpdateScene()
{
	//Update
	EventStruct event_struct;
	ZeroMemory(&event_struct, sizeof(EventStruct));

	event_struct.event_type = EventType::Update;

	EVENT_MANAGER->AddEvent(event_struct);
}