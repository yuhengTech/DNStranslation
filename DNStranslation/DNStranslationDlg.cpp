
#include "stdafx.h"
#include "DNStranslation.h"
#include "DNStranslationDlg.h"
#include "afxdialogex.h"
#include <sstream>
#pragma comment(lib, "Ws2_32.lib") 
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CAboutDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CDNStranslationDlg 对话框



CDNStranslationDlg::CDNStranslationDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DNSTRANSLATION_DIALOG, pParent)
	, DNS_input(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDNStranslationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, DNS_show);
	DDX_Text(pDX, IDC_EDIT1, DNS_input);
}

BEGIN_MESSAGE_MAP(CDNStranslationDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CDNStranslationDlg::OnBnClickedButton1)
	ON_MESSAGE(WM_MYMESSAGE, &CDNStranslationDlg::OnVmMymessage)
	ON_BN_CLICKED(IDC_BUTTON2, &CDNStranslationDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CDNStranslationDlg 消息处理程序

BOOL CDNStranslationDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDNStranslationDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDNStranslationDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CDNStranslationDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDNStranslationDlg::OnBnClickedButton1()
{
	UpdateData(true);
	if (!DNS_input.IsEmpty())
	{
		DNS_show.AddString(CString("-----------解析结果-----------"));
		DNS_show.AddString(CString("解析的域名:")+DNS_input);
		WSADATA wsaData;
		WORD wVersionRequested = MAKEWORD(2, 2);
		if (WSAStartup(wVersionRequested, &wsaData) != 0)
		{
			MessageBox("加载 WinSock DLL 失败", "错误");
			WSACleanup();
			return;
		}
		hostent *host = gethostbyname(DNS_input);
		if (host == NULL)
		{
			MessageBox("获取DNS失败", "错误");
			WSACleanup();
			return;
		}
		char **pptr = host->h_addr_list;
		DNS_show.AddString("解析到的ip地址:");
		for (int i=1; *pptr != NULL;i++)
		{
			CString x;
			DNS_show.AddString(CString("第") + CString(char(i + '0')) + CString("个IP地址:") +(inet_ntoa(*((struct in_addr *)*pptr))));
			pptr++;
		}
		WSACleanup();
	}
	PostMessage(WM_MYMESSAGE);
}


afx_msg LRESULT CDNStranslationDlg::OnVmMymessage(WPARAM wParam, LPARAM lParam)
{
	DNS_input.Delete(0, DNS_input.GetLength());
	UpdateData(false);
	return 0;
}


void CAboutDlg::OnBnClickedOk()
{
	CDialogEx::OnOK();
}


void CDNStranslationDlg::OnBnClickedButton2()
{
	DNS_show.ResetContent();
}
