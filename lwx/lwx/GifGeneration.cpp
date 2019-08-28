#pragma once
#include "UIlib.h"
#include"commdlg.h"
#include<fstream>
#include<tchar.h>
using namespace DuiLib;

class CDuiFrameWnd : public WindowImplBase
{
public:
	virtual LPCTSTR    GetWindowClassName() const   { return _T("DUIMainFrame"); }
	virtual CDuiString GetSkinFile()                { return _T("duilib.xml"); } 
	virtual CDuiString GetSkinFolder()              { return _T(""); }
	//��������
	virtual void Notify(TNotifyUI& msg)
	{
		CDuiString RespName = msg.pSender->GetName();  //��Ӧ��
		if (msg.sType == _T("click")) //�������Ĵ�����
		{
			if (RespName == _T("closebtn"))     //�˳�
			{
				Close();
				return;
			}
			else if (RespName == _T("minbtn"))  //��С��
			{
				SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
				return;
			}
			else if (RespName == _T("maxbtn"))  //���
			{
				SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
				return;
			}
			else if (RespName == _T("restorebtn"))  //��ԭ
			{
				SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
				return;
			}
			else if (RespName == _T("FileLoad"))  //�����ļ�·��
			{
				FileLoad();
				return;
			}
			else if (RespName == _T("FileResave"))  //�����ļ�·��
			{
				FileResave();
				return;
			}
			else if (RespName == _T("VideoCut")) //��ȡ��Ƶ
			{
				if (!CheckFileType(1))
				{
					MessageBox(NULL, _T("��ѡ����ȷ���ļ����ͣ�"), _T("��ʾ"), IDOK);
				}				
				else
				{
					VideoCut();
						//p_FilePath->SetWindowText
					MessageBox(NULL, strVideoEnd, _T("��ʾ"), IDOK);
				}
					
				return;
			}
			else if (RespName == _T("ffplay")) //��Ƶ����
			{
				if(!CheckFileType(1))
					MessageBox(NULL, _T("��ѡ����ȷ���ļ����ͣ�"), _T("��ʾ"), IDOK);
				else
					VideoPlay();
				return;
			}
			else if (RespName == _T("GifMade"))  //gif����
			{
				//����Ҫѡ����ͼƬ����gif����
				if (CheckFileType(0))
				{
					GenerateGifWithPic();
					MessageBox(NULL, _T("ͼƬ����gif�ɹ���"), _T("��ʾ"), IDOK);
				}
				if (CheckFileType(1))
				{
					GenerateGifWithVi();
					MessageBox(NULL, _T("��Ƶ����gif�ɹ���"), _T("��ʾ"), IDOK);
				}
				else
					MessageBox(NULL, _T("��ѡ����ȷ���ļ����ͣ�"), _T("��ʾ"), IDOK);
			}
			return;
		}
		else if (msg.sType == _T("windowinit"))
		{
			p_FilePath = (CEditUI*)m_PaintManager.FindControl(_T("FilePath"));
			p_FileResPath  = (CEditUI*)m_PaintManager.FindControl(_T("FileResPath"));
			p_FileType = (CComboUI*)m_PaintManager.FindControl(_T("FileChoose"));
			p_VideoStartH = (CEditUI*)m_PaintManager.FindControl(_T("ViStartH"));
			p_VideoStartH->SetMaxChar(2);
			p_VideoStartM = (CEditUI*)m_PaintManager.FindControl(_T("ViStartM"));
			p_VideoStartM->SetMaxChar(2);
			p_VideoStartS = (CEditUI*)m_PaintManager.FindControl(_T("ViStartS"));
			p_VideoStartS->SetMaxChar(2);
			p_VideoEndH = (CEditUI*)m_PaintManager.FindControl(_T("ViEndH"));
			p_VideoEndH->SetMaxChar(2);
			p_VideoEndM = (CEditUI*)m_PaintManager.FindControl(_T("ViEndM"));
			p_VideoEndM->SetMaxChar(2);
			p_VideoEndS = (CEditUI*)m_PaintManager.FindControl(_T("ViEndS"));
			p_VideoEndS->SetMaxChar(2);
			
		}
	}
	bool CheckFileType(int n)
	{
		if (p_FileType->GetCurSel()!= n)
		{
			return false;
		}
		else
			return true;
	}
	void FileResave()
	{
		TCHAR  szPeFileExt[100] = TEXT("file\*.*");  //���������͵��ļ�
		TCHAR szPathName[MAX_PATH];               //�ļ�·����С
		OPENFILENAME ofn = { sizeof(OPENFILENAME) };
		//ofn.hwndOwner = hWnd;// ��OR�����ļ��Ի���ĸ�����
		ofn.lpstrFilter = szPeFileExt;
		lstrcpy(szPathName, TEXT(""));
		ofn.lpstrFile = szPathName;
		ofn.nMaxFile = sizeof(szPathName);//����û�ѡ���ļ��� ·�����ļ��� ������
		ofn.lpstrTitle = TEXT("ѡ���ļ�");//ѡ���ļ��Ի������
		ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT;//�����Ҫѡ�����ļ� ��������  OFN_ALLOWMULTISELECT��־
		BOOL bOk = GetSaveFileName(&ofn);
		if (bOk)
		{
			p_FileResPath->SetText(szPathName);
		}
	}

	//ѡ����Ƶ����ͼƬ��������·�����ڱ༭����
	void FileLoad()  //����Ҫ�����ͼƬ������Ƶ
	{
		TCHAR  szPeFileExt[100] = TEXT("file\*.*");  //���������͵��ļ�
		TCHAR szPathName[MAX_PATH];               //�ļ�·����С
		OPENFILENAME ofn = { sizeof(OPENFILENAME) };
		//ofn.hwndOwner = hWnd;// ��OR�����ļ��Ի���ĸ�����
		ofn.lpstrFilter = szPeFileExt;
		lstrcpy(szPathName, TEXT(""));
		ofn.lpstrFile = szPathName;
		ofn.nMaxFile = sizeof(szPathName);//����û�ѡ���ļ��� ·�����ļ��� ������
		ofn.lpstrTitle = TEXT("ѡ���ļ�");//ѡ���ļ��Ի������
		ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT;//�����Ҫѡ�����ļ� ��������  OFN_ALLOWMULTISELECT��־
		BOOL bOk = GetOpenFileName(&ofn);
		if (bOk)
		{
			p_FilePath->SetText(szPathName);
		}
	}

	//��ѡ��·���µ���Ƶ���м��У�ʱ����ڣ�
	void VideoCut()
	{
		strVideoStart = p_VideoStartH->GetText() + _T(":") + p_VideoStartM->GetText() + _T(":") + p_VideoStartS->GetText();
		strVideoEnd = p_VideoEndH->GetText() + _T(":") + p_VideoEndM->GetText() + _T(":") + p_VideoEndS->GetText();
		CDuiString VideoCmdInit = _T(" -ss ");
		CDuiString VideoCutCmd = VideoCmdInit + strVideoStart + _T(" -to ") + strVideoEnd + _T(" -i ") + p_FilePath->GetText() + _T(" -vcodec copy -acodec copy ") + p_FileResPath->GetText();
		FFmpegCMD(VideoCutCmd);
	} 
	////ffmpeg����ǰ׺
	void FFmpegCMD(CDuiString strCMD)     
	{
		CDuiString strFFmpegPath = CPaintManagerUI::GetInstancePath() + _T("ffmpeg\\ffmpeg");
		CDuiString strInit(_T("/c"));
		SHELLEXECUTEINFO strSEInfo;
		memset(&strSEInfo, 0, sizeof(SHELLEXECUTEINFO));
		strSEInfo.cbSize = sizeof(SHELLEXECUTEINFO);
		strSEInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
		strSEInfo.lpFile = _T("C:/Windows/System32/cmd.exe");
		CDuiString finCMD = strInit + strFFmpegPath + strCMD;
		strSEInfo.lpParameters = finCMD;
		strSEInfo.nShow = SW_HIDE; 
		ShellExecuteEx(&strSEInfo);
		WaitForSingleObject(strSEInfo.hProcess, INFINITE);
	}


	//������Ƶ����gif�ɹ�
	void GenerateGifWithVi() {
	    //CDuiString gifOutPath = CPaintManagerUI::GetInstancePath() + _T("ffmpeg\\test21.gif");
		CDuiString PicCMD = _T(" -r 50 -i ");
		PicCMD += p_FilePath->GetText() + _T(" ") + p_FileResPath->GetText();
		FFmpegCMD(PicCMD);
	}

	//����ͼƬ����gif�ɹ�
	void GenerateGifWithPic()
	{
		CDuiString strPictruePath = p_FilePath->GetText() + _T("\\%d.jpg ");
		CDuiString PicCMD = _T(" -r 1 -i ");
		PicCMD += strPictruePath + p_FileResPath->GetText();;
		FFmpegCMD(PicCMD);
	}

	//������Ƶ
	void VideoPlay()
	{   
		CDuiString strVideoPath = p_FilePath->GetText();
		CDuiString strFFmpegPath = CPaintManagerUI::GetInstancePath() + _T("ffmpeg\\ffplay ");
		CDuiString strInit(_T("/c"));
		SHELLEXECUTEINFO strSEInfo;
		memset(&strSEInfo, 0, sizeof(SHELLEXECUTEINFO));
		strSEInfo.cbSize = sizeof(SHELLEXECUTEINFO);
		strSEInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
		strSEInfo.lpFile = _T("C:/Windows/System32/cmd.exe");
		CDuiString finCMD = strInit + strFFmpegPath + strVideoPath;
		strSEInfo.lpParameters = finCMD;
		strSEInfo.nShow = SW_HIDE;
		ShellExecuteEx(&strSEInfo);
		WaitForSingleObject(strSEInfo.hProcess, INFINITE);
	}
private:
	CComboUI* p_FileType;    //�ļ�����
	CDuiString strVideoStart;  //��Ƶ��ʼʱ��
	CDuiString strVideoEnd;   //��Ƶ����ʱ��
	//��ʼ
	CEditUI* p_VideoStartH; //ʱ  
	CEditUI* p_VideoStartM; //��
	CEditUI* p_VideoStartS; //��
	//����
	CEditUI* p_VideoEndH;
	CEditUI* p_VideoEndM;
	CEditUI* p_VideoEndS;
	CEditUI* p_FilePath;   //ѡ���ļ���·��
	CEditUI* p_FileResPath;  //Ҫ�����ļ���·��
};

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	CPaintManagerUI::SetInstance(hInstance);
	CDuiFrameWnd duiFrame;
	duiFrame.Create(NULL, _T("DUIWnd"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	duiFrame.CenterWindow();
	duiFrame.ShowModal();
	return 0;
}
 