#pragma once
#include "afxdialogex.h"


// Cuadro de diálogo de boton

class boton : public CDialogEx
{
	DECLARE_DYNAMIC(boton)

public:
	boton(CWnd* pParent = nullptr);   // Constructor estándar
	virtual ~boton();

	virtual void OnFinalRelease();

	void CMiClaseDeDialogo::OnBnClickedMiBoton()
	{
		AfxMessageBox(_T("¡Hola, mundo!"));
	}


// Datos del cuadro de diálogo
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Compatibilidad con DDX/DDV

	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton3();
	afx_msg void s(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton4();
};
