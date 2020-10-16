// CChatDialog.cpp : implementation file
//

#include "pch.h"
#include "RTCDemo.h"
#include "CChatDialog.h"
#include "afxdialogex.h"


// CChatDialog dialog

IMPLEMENT_DYNAMIC(CChatDialog, CDialogEx)

CChatDialog::CChatDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHATDIALOG, pParent)
{

}

CChatDialog::~CChatDialog()
{
}

void CChatDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHAT, m_chat);
	DDX_Control(pDX, IDC_INPUT, m_input);
}

void CChatDialog::Init(const CString& name, const CString& room)
{
	m_name = name;
	m_room = room;
}

BEGIN_MESSAGE_MAP(CChatDialog, CDialogEx)
	ON_BN_CLICKED(IDC_SEND, &CChatDialog::OnBnClickedSend)
END_MESSAGE_MAP()


// CChatDialog message handlers


BOOL CChatDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	CString winTitle;
	winTitle.Format(L"ChatBox - %s", m_room);
	SetWindowText(winTitle);

	CString helloText;
	helloText.Format(L"Hello, %s\r\n", m_name);
	m_chat.SetWindowText(helloText);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CChatDialog::OnBnClickedSend()
{
	CString strText;
	m_chat.GetWindowText(strText);
	strText += "\r\n";

	CString strInput;
	m_input.GetWindowText(strInput);
	strText += strInput;

	m_input.SetWindowText(L"");
	m_chat.SetWindowText(strText);
}
