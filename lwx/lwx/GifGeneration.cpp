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
	//操作类型
	virtual void Notify(TNotifyUI& msg)
	{
		CDuiString RespName = msg.pSender->GetName();  //响应名
		if (msg.sType == _T("click")) //鼠标点击类的处理动作
		{
			if (RespName == _T("closebtn"))     //退出
			{
				Close();
				return;
			}
			else if (RespName == _T("minbtn"))  //最小化
			{
				SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
				return;
			}
			else if (RespName == _T("maxbtn"))  //最大化
			{
				SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
				return;
			}
			else if (RespName == _T("restorebtn"))  //还原
			{
				SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
				return;
			}
			else if (RespName == _T("FileLoad"))  //加载文件路径
			{
				FileLoad();
				return;
			}
			else if (RespName == _T("FileResave"))  //保存文件路径
			{
				FileResave();
				return;
			}
			else if (RespName == _T("VideoCut")) //截取视频
			{
				if (!CheckFileType(1))
				{
					MessageBox(NULL, _T("请选择正确的文件类型！"), _T("提示"), IDOK);
				}				
				else
				{
					VideoCut();
						//p_FilePath->SetWindowText
					MessageBox(NULL, strVideoEnd, _T("提示"), IDOK);
				}
					
				return;
			}
			else if (RespName == _T("ffplay")) //视频播放
			{
				if(!CheckFileType(1))
					MessageBox(NULL, _T("请选择正确的文件类型！"), _T("提示"), IDOK);
				else
					VideoPlay();
				return;
			}
			else if (RespName == _T("GifMade"))  //gif生成
			{
				//这里要选择用图片还是gif生成
				if (CheckFileType(0))
				{
					GenerateGifWithPic();
					MessageBox(NULL, _T("图片生成gif成功！"), _T("提示"), IDOK);
				}
				if (CheckFileType(1))
				{
					GenerateGifWithVi();
					MessageBox(NULL, _T("视频生成gif成功！"), _T("提示"), IDOK);
				}
				else
					MessageBox(NULL, _T("请选择正确的文件类型！"), _T("提示"), IDOK);
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
		TCHAR  szPeFileExt[100] = TEXT("file\*.*");  //打开任意类型的文件
		TCHAR szPathName[MAX_PATH];               //文件路径大小
		OPENFILENAME ofn = { sizeof(OPENFILENAME) };
		//ofn.hwndOwner = hWnd;// 打开OR保存文件对话框的父窗口
		ofn.lpstrFilter = szPeFileExt;
		lstrcpy(szPathName, TEXT(""));
		ofn.lpstrFile = szPathName;
		ofn.nMaxFile = sizeof(szPathName);//存放用户选择文件的 路径及文件名 缓冲区
		ofn.lpstrTitle = TEXT("选择文件");//选择文件对话框标题
		ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT;//如果需要选择多个文件 则必须带有  OFN_ALLOWMULTISELECT标志
		BOOL bOk = GetSaveFileName(&ofn);
		if (bOk)
		{
			p_FileResPath->SetText(szPathName);
		}
	}

	//选择视频或者图片，将所在路径放在编辑框内
	void FileLoad()  //加载要处理的图片或者视频
	{
		TCHAR  szPeFileExt[100] = TEXT("file\*.*");  //打开任意类型的文件
		TCHAR szPathName[MAX_PATH];               //文件路径大小
		OPENFILENAME ofn = { sizeof(OPENFILENAME) };
		//ofn.hwndOwner = hWnd;// 打开OR保存文件对话框的父窗口
		ofn.lpstrFilter = szPeFileExt;
		lstrcpy(szPathName, TEXT(""));
		ofn.lpstrFile = szPathName;
		ofn.nMaxFile = sizeof(szPathName);//存放用户选择文件的 路径及文件名 缓冲区
		ofn.lpstrTitle = TEXT("选择文件");//选择文件对话框标题
		ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT;//如果需要选择多个文件 则必须带有  OFN_ALLOWMULTISELECT标志
		BOOL bOk = GetOpenFileName(&ofn);
		if (bOk)
		{
			p_FilePath->SetText(szPathName);
		}
	}

	//对选定路径下的视频进行剪切（时间段内）
	void VideoCut()
	{
		strVideoStart = p_VideoStartH->GetText() + _T(":") + p_VideoStartM->GetText() + _T(":") + p_VideoStartS->GetText();
		strVideoEnd = p_VideoEndH->GetText() + _T(":") + p_VideoEndM->GetText() + _T(":") + p_VideoEndS->GetText();
		CDuiString VideoCmdInit = _T(" -ss ");
		CDuiString VideoCutCmd = VideoCmdInit + strVideoStart + _T(" -to ") + strVideoEnd + _T(" -i ") + p_FilePath->GetText() + _T(" -vcodec copy -acodec copy ") + p_FileResPath->GetText();
		FFmpegCMD(VideoCutCmd);
	} 
	////ffmpeg命令前缀
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


	//根据视频生成gif成功
	void GenerateGifWithVi() {
	    //CDuiString gifOutPath = CPaintManagerUI::GetInstancePath() + _T("ffmpeg\\test21.gif");
		CDuiString PicCMD = _T(" -r 50 -i ");
		PicCMD += p_FilePath->GetText() + _T(" ") + p_FileResPath->GetText();
		FFmpegCMD(PicCMD);
	}

	//根据图片生成gif成功
	void GenerateGifWithPic()
	{
		CDuiString strPictruePath = p_FilePath->GetText() + _T("\\%d.jpg ");
		CDuiString PicCMD = _T(" -r 1 -i ");
		PicCMD += strPictruePath + p_FileResPath->GetText();;
		FFmpegCMD(PicCMD);
	}

	//播放视频
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
	CComboUI* p_FileType;    //文件类型
	CDuiString strVideoStart;  //视频开始时间
	CDuiString strVideoEnd;   //视频结束时间
	//开始
	CEditUI* p_VideoStartH; //时  
	CEditUI* p_VideoStartM; //分
	CEditUI* p_VideoStartS; //秒
	//结束
	CEditUI* p_VideoEndH;
	CEditUI* p_VideoEndM;
	CEditUI* p_VideoEndS;
	CEditUI* p_FilePath;   //选择文件的路径
	CEditUI* p_FileResPath;  //要保存文件的路径
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
 