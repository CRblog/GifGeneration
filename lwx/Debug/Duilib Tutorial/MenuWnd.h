#pragma once
#include "duilib.h"

class CMenuWnd: public CXMLWnd
{
public:
    explicit CMenuWnd(LPCTSTR pszXMLPath);

protected:
    virtual ~CMenuWnd();   // ˽�л����������������˶���ֻ��ͨ��new�����ɣ�������ֱ�Ӷ���������ͱ�֤��delete this�������

public:
    void Init(HWND hWndParent, POINT ptPos);
    virtual void    OnFinalMessage(HWND hWnd);
    virtual LRESULT HandleMessage (UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual LRESULT OnKillFocus   (UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};
