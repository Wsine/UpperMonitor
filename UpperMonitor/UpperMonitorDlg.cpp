
// UpperMonitorDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UpperMonitor.h"
#include "UpperMonitorDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx {
public:
	CAboutDlg();

	// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD) {}

void CAboutDlg::DoDataExchange(CDataExchange* pDX) {
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CUpperMonitorDlg 对话框



CUpperMonitorDlg::CUpperMonitorDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(CUpperMonitorDlg::IDD, pParent) {
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CUpperMonitorDlg::~CUpperMonitorDlg() {
	DeleteObject(m_MenuDebugger);
	DeleteObject(m_MenuAppdev);
	DeleteObject(m_MainMenu);
}

void CUpperMonitorDlg::DoDataExchange(CDataExchange* pDX) {
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB, m_MainMenu);
}

BEGIN_MESSAGE_MAP(CUpperMonitorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, &CUpperMonitorDlg::OnSelchangeTab)
END_MESSAGE_MAP()


// CUpperMonitorDlg 消息处理程序

BOOL CUpperMonitorDlg::OnInitDialog() {
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if(pSysMenu != NULL) {
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if(!strAboutMenu.IsEmpty()) {
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	// 1. 插入Tab选项卡标签
	TCITEM tcItemDebugger;
	tcItemDebugger.mask = TCIF_TEXT;
	tcItemDebugger.pszText = _T("调试助手");
	m_MainMenu.InsertItem(0, &tcItemDebugger);
	TCITEM tcItemAppdev;
	tcItemAppdev.mask = TCIF_TEXT;
	tcItemAppdev.pszText = _T("应用开发");
	m_MainMenu.InsertItem(1, &tcItemAppdev);
	// 2. 关联对话框，将TAB控件设为选项卡对应对话框的父窗口
	m_MenuDebugger.Create(IDD_DEBUGGER, GetDlgItem(IDC_TAB));
	m_MenuAppdev.Create(IDD_APPDEV, GetDlgItem(IDC_TAB));
	// 3. 获取TAB控件客户区大小，用于调整选项卡对话框在父窗口中的位置
	CRect rect;
	m_MainMenu.GetClientRect(&rect);
	rect.top += 22;
	rect.right -= 3;
	rect.bottom -= 2;
	rect.left += 1;
	// 4. 设置子对话框尺寸并移动到指定位置
	m_MenuDebugger.MoveWindow(&rect);
	m_MenuAppdev.MoveWindow(&rect);
	// 5. 设置默认选项卡，对选项卡对话框进行隐藏和显示
	m_MenuDebugger.ShowWindow(SW_SHOWNORMAL);
	m_MenuAppdev.ShowWindow(SW_HIDE);
	m_MainMenu.SetCurSel(0);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CUpperMonitorDlg::OnSysCommand(UINT nID, LPARAM lParam) {
	if((nID & 0xFFF0) == IDM_ABOUTBOX) {
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else {
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CUpperMonitorDlg::OnPaint() {
	if(IsIconic()) {
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
	else {
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CUpperMonitorDlg::OnQueryDragIcon() {
	return static_cast<HCURSOR>(m_hIcon);
}


// 6. 处理选项卡切换
void CUpperMonitorDlg::OnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult) {
	*pResult = 0;

	// 获取当前点击选项卡标签下标
	int cursel = m_MainMenu.GetCurSel();
	// 根据下标将相应的对话框显示，其余隐藏
	switch(cursel) {
		case 0:
			m_MenuDebugger.ShowWindow(SW_SHOWNORMAL);
			m_MenuAppdev.ShowWindow(SW_HIDE);
			break;
		case 1:
			m_MenuDebugger.ShowWindow(SW_HIDE);
			m_MenuAppdev.ShowWindow(SW_SHOWNORMAL);
			break;
		default:
			break;
	}

}
