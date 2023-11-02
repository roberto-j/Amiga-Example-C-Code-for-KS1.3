#include <exec/types.h>
#include <intuition/intuition.h>
#include <proto/exec.h>
#include <proto/intuition.h>
#include <stdio.h>

struct IntuitionBase *IntuitionBase;
struct Screen *FirstScreen;
struct Window *FirstWindow;
struct IntuiMessage *message;

struct TextAttr Font = {
    (STRPTR) "topaz.font", TOPAZ_EIGHTY, FS_NORMAL, FPF_ROMFONT
};

struct NewScreen FirstNewScreen = {
    0, 0,
    640, 256,
    3,
    0, 1,
    HIRES,
    CUSTOMSCREEN,
    &Font,
    (UBYTE *) "Screen Test",
    NULL,
    NULL
};

struct NewWindow FirstNewWindow = {
    0, 0,
    640, 256,
    0, 1,
    CLOSEWINDOW | MENUPICK | MOUSEBUTTONS | MOUSEMOVE,
    WINDOWDEPTH | WINDOWSIZING | WINDOWDRAG | WINDOWCLOSE | SMART_REFRESH | ACTIVATE | REPORTMOUSE,
    NULL,
    NULL,
    (UBYTE *) "New Window",
    NULL,
    NULL,
    100, 50,
    640, 200,
    CUSTOMSCREEN
};

int Open_All();
void Close_All();

int main() {
    ULONG MessageClass;
    USHORT code;

    // Open all necessary libraries and windows
    if (Open_All() != 0) {
        return 1; // Return with error code if initialization fails
    }

    while (TRUE) {
        WaitPort(FirstWindow->UserPort);
        while ((message = (struct IntuiMessage *)GetMsg(FirstWindow->UserPort))) {
            switch (message->Class) {
                case CLOSEWINDOW:
                    Close_All();
                    return 0; // Gracefully exit the program

                case MOUSEBUTTONS:
                    break;

                case MOUSEMOVE:
                    break;
            }
            ReplyMsg((struct Message *)message);
        }
    }
}

int Open_All() {
    if (!(IntuitionBase = (struct IntuitionBase *) OpenLibrary("intuition.library", 0L))) {
        printf("Intuition Library not found!\n");
        Close_All();
        return 1; // Return with error code
    }

    if (!(FirstScreen = (struct Screen *) OpenScreen(&FirstNewScreen))) {
        printf("No screen page!\n");
        Close_All();
        return 1; // Return with error code
    }

    FirstNewWindow.Screen = FirstScreen;

    if (!(FirstWindow = (struct Window *) OpenWindow(&FirstNewWindow))) {
        printf("Window will not open!\n");
        Close_All();
        return 1; // Return with error code
    }

    return 0; // Success
}

void Close_All() {
    if (FirstWindow) ClearMenuStrip(FirstWindow);
    if (FirstWindow) CloseWindow(FirstWindow);
    if (FirstScreen) CloseScreen(FirstScreen);
    if (IntuitionBase) CloseLibrary((struct Library *)IntuitionBase);
}
