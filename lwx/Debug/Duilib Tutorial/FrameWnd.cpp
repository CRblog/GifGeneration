#include "resource.h"
#include "FrameWnd.h"
#include "MenuWnd.h"

CFrameWnd::CFrameWnd( LPCTSTR pszXMLPath )
: CXMLWnd(pszXMLPath)
{

}

void CFrameWnd::InitWindow()
{
//    SetIcon(IDR_MAINFRAME); // ����������ͼ��
    CenterWindow();

    // ��ʼ��CActiveXUI�ؼ�
    CActiveXUI* pActiveXUI = static_cast<CActiveXUI*>(m_PaintManager.FindControl(_T("ActiveXDemo1")));

    if( pActiveXUI ) 
    {
        IWebBrowser2* pWebBrowser = NULL;

        pActiveXUI->SetDelayCreate(false);              // �൱�ڽ�����������DelayCreate���Ը�ΪFALSE����duilib�Դ���FlashDemo����Կ���������ΪTRUE             
        pActiveXUI->CreateControl(CLSID_WebBrowser);    // �൱�ڽ�����������Clsid����������{8856F961-340A-11D0-A96B-00C04FD705A2}��������CLSID_WebBrowser������뿴��Ӧ��ֵ�����<ExDisp.h>
        pActiveXUI->GetControl(IID_IWebBrowser2, (void**)&pWebBrowser);

        if( pWebBrowser != NULL ) 
        {
            //pWebBrowser->Navigate(L"https://code.google.com/p/duilib/",NULL,NULL,NULL,NULL);  
            pWebBrowser->Navigate(L"http://www.baidu.com/",NULL,NULL,NULL,NULL);  // ���ڹȸ�ʱ��ʱ��ǽ�����Ի��ɷ�Ӧ�����վ
            pWebBrowser->Release();
        }
    }

    // ��ʼ��CProgressUI�ؼ�
    CProgressUI* pProgress = static_cast<CProgressUI*>(m_PaintManager.FindControl(_T("ProgressDemo1")));    
    pProgress->SetValue(100);

    // ��ʼ��CListUI�ؼ�
    CDuiString str;
    CListUI* pList = static_cast<CListUI*>(m_PaintManager.FindControl(_T("ListDemo1")));

    for (int i = 0; i < 100; i++)
    {
        CListTextElementUI* pListElement = new CListTextElementUI;
        pListElement->SetTag(i);
        pList->Add(pListElement);

        str.Format(_T("%d"), i);
        pListElement->SetText(0, str);
        pListElement->SetText(1, _T("haha"));
    }
}

void CFrameWnd::Notify( TNotifyUI& msg )
{
    if( msg.sType == _T("click") ) 
    {
        if( msg.pSender->GetName() == _T("btnMenu") ) 
        {
            POINT pt = {msg.ptMouse.x, msg.ptMouse.y};
            CMenuWnd *pMenu = new CMenuWnd(_T("Menu/menu.xml"));

            pMenu->Init(*this, pt);
            pMenu->ShowWindow(TRUE);
        }
    }
    else if(msg.sType == _T("selectchanged"))
    {
        CDuiString    strName     = msg.pSender->GetName();
        CTabLayoutUI* pControl = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("tabTest")));

        if(strName == _T("OptionDemo1"))
            pControl->SelectItem(0);
        else if(strName == _T("OptionDemo2"))
            pControl->SelectItem(1);
        else if(strName == _T("OptionDemo3"))
            pControl->SelectItem(2);
    }

    __super::Notify(msg);
}

CControlUI* CFrameWnd::CreateControl( LPCTSTR pstrClassName )
{
    if (_tcsicmp(pstrClassName, _T("Wnd")) == 0)
    {
        CWndUI  *pUI  = new CWndUI;            
        HWND    hWnd  = CreateWindow(_T("BUTTON"), _T("win32"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 0, 0, 0, 0, m_PaintManager.GetPaintWindow(), NULL, NULL, NULL);
        pUI->Attach(hWnd);  

        //// �����õ���win32�İ�ť�����������MFC�İ�ť
        //CButton *pBtn = new CButton;    // �ǵ��ͷ��ڴ�
        //pBtn->Create(_T("MFC"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, CRect(0, 0, 0, 0), CWnd::FromHandle(m_PaintManager.GetPaintWindow()), 0);
        //pUI->Attach(*pBtn);            
        return pUI;
    }

    return NULL;
}

