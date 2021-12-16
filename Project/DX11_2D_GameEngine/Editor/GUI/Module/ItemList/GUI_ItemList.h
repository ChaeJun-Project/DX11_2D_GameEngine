#pragma once

class GUI_ItemList final
{
public: 
	GUI_ItemList() = default;
	~GUI_ItemList();

public:
    void AddItem(const std::string& item_str) { m_item_list_vector.emplace_back(item_str); }
	void ClearItemList() { m_item_list_vector.clear(); m_item_list_vector.shrink_to_fit(); }

	std::vector<std::string>& GetItemList() { return m_item_list_vector; }

	int* GetCurrentListID() { return &m_current_index; }
	void SetCurrentListID(const int& current_index) { m_current_index = current_index; }
	void SetCurrentListID(const std::string& item_str);

private:
    std::vector<std::string> m_item_list_vector;

	int m_current_index = -1;
};

