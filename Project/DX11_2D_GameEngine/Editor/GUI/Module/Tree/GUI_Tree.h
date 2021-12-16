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
    GUI_TreeItem* AddItem(GUI_TreeItem* p_parent, const std::string& item_name, const PayLoadType& pay_load_type, DWORD_PTR object_address);
    void Clear();

    void SetClickedCallBack(Clicked_CallBack p_clicked_func) { m_p_clicked_func = p_clicked_func; }
    void ExcuteClickedCallBack(GUI_TreeItem* p_item);
    GUI_TreeItem* GetSelectedItem() { SAFE_GET_POINTER(m_p_selected_item); }

    void SetDragDropCallBack(DragDrop_CallBack p_p_drag_drop_func) { m_p_drag_drop_func = p_p_drag_drop_func; }
    void SetDragStartItem(GUI_TreeItem* p_item) { m_p_drag_start_item = p_item; }
    GUI_TreeItem* GetDragStartItem() { SAFE_GET_POINTER(m_p_drag_start_item); }
    void SetDroppedItem(GUI_TreeItem* p_item);

    void SetIsVisibleRoot(const bool& is_visible_root) { m_is_visible_root = is_visible_root; }

private:
    GUI_TreeItem* m_p_root_item = nullptr;

    Clicked_CallBack m_p_clicked_func = nullptr;

    GUI_TreeItem* m_p_selected_item = nullptr;

    DragDrop_CallBack m_p_drag_drop_func = nullptr;

    GUI_TreeItem* m_p_drag_start_item = nullptr;
    GUI_TreeItem* m_p_dropped_item = nullptr;

    bool m_is_visible_root = false;

    friend class GUI_TreeItem;
};

