#pragma once

class GUI_TreeItem;

class GUI_Tree final
{
public:
    GUI_Tree() = default;
    ~GUI_Tree();

public:
    void Update();
  
public:
    GUI_TreeItem* AddItem(GUI_TreeItem* p_parent, const std::string& item_name, const PayLoad& pay_load, const bool& use_drag_and_drop);
    void Clear();

    void SetClickedEmptySpace(Event_CallBack p_clicked_empty_space) { m_p_clicked_empty_space = p_clicked_empty_space; }
    void ExcuteClickedEmptySpaceCallBack();

    void SetClickedCallBack1(Clicked_CallBack1 p_clicked_func_1) { m_p_clicked_func_1 = p_clicked_func_1; }
    void SetClickedCallBack2(Clicked_CallBack2 p_clicked_func_2) { m_p_clicked_func_2 = p_clicked_func_2; }
    void ExcuteClickedCallBack(GUI_TreeItem* p_item);
    GUI_TreeItem* GetSelectedItem() { SAFE_GET_POINTER(m_p_selected_item); }
    void SetSelectedItem(const std::string& directory_path);

    void SetDragDropCallBack(DragDrop_CallBack p_p_drag_drop_func) { m_p_drag_drop_func = p_p_drag_drop_func; }
    void SetDragStartItem(GUI_TreeItem* p_item) { m_p_drag_start_item = p_item; }
    GUI_TreeItem* GetDragStartItem() { SAFE_GET_POINTER(m_p_drag_start_item); }
    void SetDroppedItem(GUI_TreeItem* p_item);

    void SetIsVisibleRoot(const bool& is_visible_root) { m_is_visible_root = is_visible_root; }

private:
    GUI_TreeItem* m_p_root_item = nullptr;

    Event_CallBack m_p_clicked_empty_space = nullptr;

    Clicked_CallBack1 m_p_clicked_func_1 = nullptr;
    Clicked_CallBack2 m_p_clicked_func_2 = nullptr;

    GUI_TreeItem* m_p_selected_item = nullptr;

    DragDrop_CallBack m_p_drag_drop_func = nullptr;

    GUI_TreeItem* m_p_drag_start_item = nullptr;
    GUI_TreeItem* m_p_dropped_item = nullptr;

    bool m_is_visible_root = false;

    friend class GUI_TreeItem;
};

