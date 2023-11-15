#undef UNICODE // Pārslēgt projektu ASCII režīmā
#include <windows.h>
#include <stdio.h>
#include "resource.h"
#pragma warning(disable : 4996)
// Ziņojumu apstrādātāja prototips
BOOL CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);
// Galvenā funkcija
// ja galvenā loga šablonam ir piešķirts resursa identifikators IDD_MAINDIALOG
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int) {
    DialogBox(
        hInstance,
        MAKEINTRESOURCE(IDD_MAINDIALOG),
        NULL,
        DLGPROC(MainWndProc)
    );
    return 0;
}
// Ziņojumu apstrādātājs
BOOL CALLBACK MainWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
    switch (Msg) { // pārbaudīt ziņojuma identifikatoru
    case WM_INITDIALOG: // ja tas ir inicializācijas paziņojums
        // izpildīt inicializācijas darbības
        return TRUE;
    case WM_COMMAND: // ja tas ir paziņojums no vadības elementa
        switch (LOWORD(wParam)) { // pārbaudīt vadības elementa ID
        case IDOK: // ja bija uzspiesta OK poga
            DestroyWindow(hWnd); // aizvērt logu
            return TRUE;
        }
        return FALSE;
    case WM_DESTROY: // ja tas ir pabeigšanas pieprasījums
        PostQuitMessage(0); // pabeigt programmu
        return TRUE;
    }
    return FALSE;
}

/*
I have C++ WINAPI Code:

And resource:
Dialog IDD_MAINDIALOG, Button IDC_START, Button IDC_STOP, Button IDC_EXIT, Static Text IDC_WISDOM1, Static Text IDC_WISDOM2, Static Text IDC_WISDOM3, Static Text IDC_WISDOM4, Static Text IDC_WISDOM5, Static Text IDC_FORCE1, Static Text IDC_FORCE2, Static Text IDC_FORCE3, Static Text IDC_FORCE4, Static Text IDC_FORCE5, Static Text IDC_STATE1, Static Text IDC_STATE2, Static Text IDC_STATE3, Static Text IDC_STATE4, Static Text IDC_STATE5
*/