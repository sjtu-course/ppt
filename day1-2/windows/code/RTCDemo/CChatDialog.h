#pragma once


// CChatDialog dialog

class CChatDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CChatDialog)

public:
	CChatDialog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CChatDialog();
	void Init(const CString& name, const CString& room);

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHATDIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	CString m_room;
	CString m_name;
public:
	virtual BOOL OnInitDialog();
	CEdit m_chat;
	CEdit m_input;
	afx_msg void OnBnClickedSend();
};
