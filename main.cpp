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

/*
Task: Write a program that solves the dining philosophers problem. It is required to 1) keep track of the 5 philosopher's life state and 2) solve the cross-exclusions, which may occur during the use of forks and plates, using semaphores.

Description of the problem: Five philosophers live in one house. Their lives are spent in thought, interrupted by meals. A starving philosopher enters the dining room, sits down at the round table, picks up two forks and starts to eat. On the table are five plates and five forks between them. The philosopher can only pick up the two nearest forks. When the philosopher has finished his meal, he puts down his forks, leaves the dining room and continues thought.
As such, there are 5 philosophers dynamic objects TPhilosopher*, a thread for each. They have 3 states - Think(), Eat(), Idle(). There are 5 plates and 5 forks. A philosopher needs 1 plate and 2 nearby forks. 2 philosophers are prohibitted from using the same forks or plates. After eating, a philosopher returns them and continues thinking. If a philosopher goes too long without eating, he dies.

Problem solving algorithm: During problem solving, the possibility of using one fork with two philosophers at the same time must be excluded. I.e. a critical section needs to be made for each fork. Each philosopher has to take a fork before starting the meal two forks i.e. two critical sections must be built. Then the problem will be solved, but the possibility arises to block the work of the program (cross-locking). If all the philosophers enter the dining room at the same time and each takes one fork at a time, no one will be able to take the second fork and everyone will starve to death. This must be avoided
Do not allow all 5 philosophers into the dining room at the same time to avoid cross-locking. Use a queue.
The critical section of the fork can be implemented using a mutex. It's default value must be 4 to limit the number of philosophers who can enter the dining room at the same time.
*/