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
As such, there are 5 philosophers dynamic objects TPhilosopher*, a thread for each. They have 4 states - Think, Eat, Idle, Die. There are 5 plates and 5 forks. A philosopher needs 1 plate and 2 nearby forks. 2 philosophers are prohibitted from using the same forks or plates. After eating, a philosopher returns them and continues thinking. If a philosopher goes too long without eating, he dies.

Problem solving algorithm: During problem solving, the possibility of using one fork with two philosophers at the same time must be excluded. I.e. a critical section needs to be made for each fork. Each philosopher has to take a fork before starting the meal two forks i.e. two critical sections must be built. Then the problem will be solved, but the possibility arises to block the work of the program (cross-locking). If all the philosophers enter the dining room at the same time and each takes one fork at a time, no one will be able to take the second fork and everyone will starve to death. This must be avoided
Do not allow all 5 philosophers into the dining room at the same time to avoid cross-locking. Use a queue.
The critical section of the fork can be implemented using a mutex semaphore. It's default value must be 4 to limit the number of philosophers who can enter the dining room at the same time.

Semaphore: use CreateSemaphore function to create a mutex semaphore, WaitForSingleObject and ReleaseSemaphore

TPhilosopher class is already created. Use it.
Create 5 TPhilosopher dynamic objects. A new thread for each. They have 4 states - Think(), Eat(), Idle(), Die(). They each have force, windom and voracity.
Immediately after the philosopher is created, a thread starts working inside the object, which changes the philosopher's life force and wisdom 10 times a second depending on its state and voracity. When philosopher's force reaches zero, the philosopher will die. The philosopher's states can be changed by the methods Eat() (transforms to Eat), Think() (transforms to Think) and Kill() (transforms to Die)
TPhilosopher has the following methods:
void Think(); // After calling this method, the philosopher starts thinking and generating wisdom. The philosopher is in a thinking state for a random time interval (0.5s -1s). Blocks the thread. Death due to starvation overrides blocking.
void Eat(); // The philosopher begins to eat and blocks the thread until the end of the meal. Philosopher is in a feeding state for a random time interval (0.5s – 1s)
void Kill(); // Kill the philosopher
int GetState(); // Find out the current state of the philosopher: psIdle, psThink, psEat, psDie.
int GetForce(); 
int GetWisdom(); 
int GetVoracity(); 
bool SetVoracity(); 

TPhilosopher.h:
#ifndef PhilosopherH
#define PhilosopherH

#undef UNICODE
#include <Windows.h>
#include <string>

#define psIdle  0
#define psThink 1
#define psEat   2
#define psDie   3

#define WM_PHILOSOPHER WM_USER

class TPhilosopher {
  friend DWORD WINAPI _PhilosopherThread(void* P);
  private:
    int FForce, FWisdom, FState;
    CRITICAL_SECTION CPhilosopher;
    HANDLE FLife;
    HANDLE FStop;
    int FVoracity;
    void SetState(int AState);
  public:
    TPhilosopher();
    TPhilosopher(TPhilosopher& P);
    ~TPhilosopher();

    void Think();
    void Eat();
    void Kill();
    int  GetState();
    int  GetForce();
    int  GetWisdom();
    int  GetVoracity();
    bool SetVoracity(int AVoracity);
};


#endif

TPhilosopher.cpp:
#pragma hdrstop

#include <time.h>
#include "Philosopher.h"

#pragma package(smart_init)

#define MAX_PHILOSOPHERS 5
#define random(x) (rand() % x)

#define INIT_FORCE  50
#define THINK_TIME  ((500 + random(501)))
#define EAT_TIME    ((500 + random(501)))
#define IDLE_FORCE  -2
#define THINK_FORCE -1
#define EAT_FORCE    3

static TPhilosopher * Philosophers[MAX_PHILOSOPHERS] = {NULL, NULL, NULL, NULL, NULL};

DWORD WINAPI _PhilosopherThread(void* P);

static void Stop(const char * Msg) {
  for (int i = 0; i < MAX_PHILOSOPHERS; i++) if (Philosophers[i]) Philosophers[i]->Kill();
  MessageBox(NULL, Msg, "Problem is not solved", MB_OK | MB_ICONERROR);
  TerminateProcess(GetCurrentProcess(), 1);
}

TPhilosopher::TPhilosopher()
{
  srand((unsigned)time(NULL));
  bool found = false;
  for (int i = 0; i < MAX_PHILOSOPHERS; i++) if (!Philosophers[i]) {
    found = true;
    Philosophers[i] = this;
    break;
  }
  if (!found) Stop("Population of the philosophers is overflowed");
  FForce  = INIT_FORCE;
  FWisdom = 0;
  FState  = psIdle;
  InitializeCriticalSection(&CPhilosopher);
  FStop = CreateEvent(NULL, TRUE, FALSE, NULL);
  FVoracity = 50;
  FLife = CreateThread(NULL, 0, &_PhilosopherThread, (void*)this, 0, NULL);
}

TPhilosopher::TPhilosopher(TPhilosopher& P)
{
  throw std::string("Can't make a copy of philosopher");
}

TPhilosopher::~TPhilosopher()
{
    Kill();

    Sleep(100);

    WaitForSingleObject(FLife, INFINITE);
    CloseHandle(FLife);

    DeleteCriticalSection(&CPhilosopher);

    for (int i = 0; i < MAX_PHILOSOPHERS; i++) if (Philosophers[i] == this) {
        Philosophers[i] = NULL;
        break;
    }

    CloseHandle(FStop);
}

DWORD WINAPI _PhilosopherThread(void* P)
{
    HWND wnd = FindWindow("#32770", NULL);
    DWORD id = -1;
    for (int i = 0; i < MAX_PHILOSOPHERS; i++) if (Philosophers[i] == P) { id = i; break; }
    TPhilosopher* Philosopher = (TPhilosopher*)P;
    bool die = false;
    while (!die && WaitForSingleObject(Philosopher->FStop, 100) == WAIT_TIMEOUT) {
        EnterCriticalSection(&Philosopher->CPhilosopher);
        switch (Philosopher->FState) {
        case psIdle: Philosopher->FForce += IDLE_FORCE; break;
        case psThink: Philosopher->FForce += THINK_FORCE; Philosopher->FWisdom++; break;
        case psEat: Philosopher->FForce += EAT_FORCE; break;
        }
        die = Philosopher->FState == psDie || Philosopher->FForce < 0;
        LeaveCriticalSection(&Philosopher->CPhilosopher);
        if (wnd) PostMessage(wnd, WM_PHILOSOPHER, id, (LPARAM)P);
    }
    Philosopher->Kill();
    if (wnd) PostMessage(wnd, WM_PHILOSOPHER, id, (LPARAM)P);
    return 0;
}

void TPhilosopher::Think()
{
  if (GetState() != psIdle) return;
  SetState(psThink);
  WaitForSingleObject(FStop, THINK_TIME * (100 - FVoracity) / 100);
  SetState(psIdle);
}

void TPhilosopher::Eat()
{
  if (GetState() != psIdle) return;
  SetState(psEat);
  WaitForSingleObject(FStop, EAT_TIME * FVoracity / 100);
  SetState(psIdle);
}

void TPhilosopher::SetState(int AState)
{
  int state = GetState();
  if (state == psDie || state == AState) return;

  if (AState == psEat) {
    int cnt = 0;
    for (int i = 0; i < MAX_PHILOSOPHERS; i++) if (Philosophers[i] && Philosophers[i]->GetState() == psEat) cnt++;
    if (cnt >= 2) Stop("Two philosophers tried to use one fork at the same time");
  }

  EnterCriticalSection(&CPhilosopher);
  FState = AState;
  LeaveCriticalSection(&CPhilosopher);
}

int TPhilosopher::GetState()
{
  EnterCriticalSection(&CPhilosopher);
  int state = FState;
  LeaveCriticalSection(&CPhilosopher);
  return state;
}

int TPhilosopher::GetForce()
{
  EnterCriticalSection(&CPhilosopher);
  int force = FForce;
  LeaveCriticalSection(&CPhilosopher);
  return force;
}

int TPhilosopher::GetWisdom()
{
  EnterCriticalSection(&CPhilosopher);
  int wisdom = FWisdom;
  LeaveCriticalSection(&CPhilosopher);
  return wisdom;
}

void TPhilosopher::Kill()
{
    SetEvent(FStop);
    SetState(psDie);
}

int  TPhilosopher::GetVoracity()
{
    EnterCriticalSection(&CPhilosopher);
    int voracity = FVoracity;
    LeaveCriticalSection(&CPhilosopher);
    return voracity;
}

bool TPhilosopher::SetVoracity(int AVoracity)
{
    if (AVoracity < 0 || AVoracity > 100) return false;
    EnterCriticalSection(&CPhilosopher);
    FVoracity = AVoracity;
    LeaveCriticalSection(&CPhilosopher);
    return true;
}



Use the following code template:
TPhilosopher * Philosopher = new TPhilosopher;
while (Philosopher->GetState() != psDie) {
// call methods
}
delete Philosopher.

The structure of the program:
In the program, for each of the five philosophers, create an object of the philosopher class and a thread that describes the philosopher's routine. The philosopher's methods Eat (for the philosopher to eat) and Think (for the philosopher to think) must be called sequentially in the thread (one after another). In addition, it is necessary to exclude the possibility of simultaneous use of forks and to exclude the possibility of mutual blocking. To be able to display the states of the philosophers on the window, the philosopher object will send a WM_PHOLOSOPHER message to the main window with the id number of the philosopher (wParam) and the address of the philosopher object (lParam) whenever its state changes. The main form should update the specified philosopher on the screen in response to these notifications - IDC_WISDOM1/2/3/4/5, IDC_STATE1/2/3/4/5, IDC_FORCE1/2/3/4/5

Task: Declare global variables:
Declare variable to hold philosopher objects and semaphores.
1) An array of philosopher class dynamic objects
2) An array of fork semaphore handles
3) Dining room semaphore handle

Task: Implement philosopher routine thread and make a thread for each of the five philosophers
1) Read the philosopher ID number from the wParam;
2) In the main loop, while the philosopher with the provided ID number GetState() != psDie;
a) Call Think();
b) Check the dining room semaphore and enter the dining room;
c) Check the fork semaphore with the ID number and take the left fork;
d) Check the fork semaphore with the number ((id+1)%5) and take the right fork;
e) Call Eat();
f) Put down the right fork (release the fork semaphore with the number (id+1)%5);
g) Put down the left fork (release the fork semaphore with id number);
h) Leave the dining room (release the dining room semaphore).

Task: Implement WM_INITDIALOG message
1) Create binary semaphores (mutex) for forks;
2) Create a semaphore for the dining room;
3) Initialize the global philosophers array with NULLs

Task: Implement WM_DESTROY message
1) For each philosopher object in the global array:
a) Kill it;
b) Delete the object;
2) Close all fork semaphore handles
3) Close dining room semaphore handle

Task: Implement switch (LOWORD(wParam)) IDC_START case
1) Create 5 dynamic philosopher objects and write pointers to them to the global array
2) Launch a thread for each philosopher, specifying their ID as a param and close the thread's handle
Should only be available when the simulation is not running. Use EnableWindow for this.

Task: Implement switch (LOWORD(wParam)) IDC_STOP case
1) For each philosopher in the array,
a) Kill it;
b) Allow the associated thread to complete workl
c) Delete the philosopher object and write NULL to their array position
Should only be available when the simulation is running. Use EnableWindow for this.

Task: Implement WM_PHILOSOPHER message
1) Get the ID number of the philosopher who sent the message (the number of the philosopher will be sent via wParam).
2) If the philosopher object exists (the pointer in the array is not equal to NULL), read the philosopher's state, force and wisdom from it and output them to the respective fields on the screen (IDC_STATE1/2/3/4/5, IDC_FORCE1/2/3/4/5, IDC_WISDOM1/2/3/4/5)
3) If the philosopher object does not exist (the pointer in the array is equal to NULL), clear the relevant fields on the screen. Set them to "". (IDC_STATE1/2/3/4/5, IDC_FORCE1/2/3/4/5, IDC_WISDOM1/2/3/4/5)

During simulation, change the philosopher's color depending of the state of the philosopher. If Die - Red. If Eat - Green. If Think - Blue. To do this, handle WM_CTCOLORSTATIC. When processing WM_CTCOLORSTATIC message, use SetTextColor. Use lParam and GetDlgCtrlID to find which object is sending the message by converting its handle to ID.

Use the following WINAPI functions:
CreateSemaphore
ReleaseSemaphore
CreateMutex
ReleaseMutex
WaitForSingleObject
WaitForMultipleObjects

Do not change Philosopher.cpp and .h
*/
