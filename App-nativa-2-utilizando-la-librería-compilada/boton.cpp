#include "pch.h"
#include "MFCApplication1.h"
#include "afxdialogex.h"
#include "boton.h"
#include <string>

// Cuadro de diálogo de boton

IMPLEMENT_DYNAMIC(boton, CDialogEx)

boton::boton(CWnd* pParent /*= nullptr*/)
    : CDialogEx(IDD_DIALOG1, pParent)
{
    EnableAutomation();
}

boton::~boton()
{
}

void boton::OnFinalRelease()
{
    CDialogEx::OnFinalRelease();
}

void boton::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    // Asocia los controles de la interfaz con variables miembro
    DDX_Control(pDX, IDC_EDIT1, PrimerNumero);
    DDX_Control(pDX, IDC_EDIT2, SegundoNumero);
    DDX_Control(pDX, IDC_STATIC_RESULTADO, Resultado);
}

BEGIN_MESSAGE_MAP(boton, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON1, &boton::OnBnClickedButton1)  // Botón dividir
    ON_BN_CLICKED(IDC_BUTTON2, &boton::OnBnClickedButton2)  // Botón sumar
    ON_BN_CLICKED(IDC_BUTTON3, &boton::OnBnClickedButton3)  // Botón multiplicar
    ON_BN_CLICKED(IDC_BUTTON4, &boton::OnBnClickedButton4)  // Botón restar
END_MESSAGE_MAP()

void boton::OnBnClickedButton1()
{
    bool isValidNum1, isValidNum2;
    double num1 = ObtenerValorDeControl(PrimerNumero, isValidNum1);
    double num2 = ObtenerValorDeControl(SegundoNumero, isValidNum2);

    if (isValidNum1 && isValidNum2)
    {
        if (num2 != 0)
        {
            double res = num1 / num2;
            MostrarResultado(res);
        }
        else
        {
            AfxMessageBox(_T("Error: División por cero."));
        }
    }
}

void boton::OnBnClickedButton2()
{
    bool isValidNum1, isValidNum2;
    double num1 = ObtenerValorDeControl(PrimerNumero, isValidNum1);
    double num2 = ObtenerValorDeControl(SegundoNumero, isValidNum2);

    if (isValidNum1 && isValidNum2)
    {
        double res = num1 + num2;
        MostrarResultado(res);
    }
}

void boton::OnBnClickedButton3()
{
    bool isValidNum1, isValidNum2;
    double num1 = ObtenerValorDeControl(PrimerNumero, isValidNum1);
    double num2 = ObtenerValorDeControl(SegundoNumero, isValidNum2);

    if (isValidNum1 && isValidNum2)
    {
        double res = num1 * num2;
        MostrarResultado(res);
    }
}

void boton::OnBnClickedButton4()
{
    bool isValidNum1, isValidNum2;
    double num1 = ObtenerValorDeControl(PrimerNumero, isValidNum1);
    double num2 = ObtenerValorDeControl(SegundoNumero, isValidNum2);

    if (isValidNum1 && isValidNum2)
    {
        double res = num1 - num2;
        MostrarResultado(res);
    }
}

// Función para obtener el valor de un control de edición con validación
double boton::ObtenerValorDeControl(CEdit& control, bool& isValid)
{
    CString texto;
    control.GetWindowTextW(texto);

    if (texto.IsEmpty())
    {
        AfxMessageBox(_T("Error: Campo vacío."));
        isValid = false;
        return 0.0;
    }

    TCHAR* end;
    double valor = _tcstod(texto, &end);

    if (*end != 0) // Verifica si hay caracteres no válidos
    {
        AfxMessageBox(_T("Error: Entrada no válida."));
        isValid = false;
        return 0.0;
    }

    isValid = true;
    return valor;
}

// Función para mostrar el resultado en el control estático
void boton::MostrarResultado(double resultado)
{
    CString textoResultado;
    textoResultado.Format(_T("%.2f"), resultado);  // Formatea el resultado a 2 decimales
    Resultado.SetWindowTextW(textoResultado);
}