#include "KMotionDef.h"
#include "Magazine.h"

void MagazineWatch();
/* void OpenMagazine();
void CloseMagazine(); */
void OpenMagazineNoWait();
void CloseMagazineNoWait();
void SetMagazineStatus(int status);
int GetMagazineStatus();

enum magazineStatus
{
    Initial,
    Opened,
    Opening,
    Closed,
    Closing
};

void MagazineWatch()
{
    enum magazineStatus status = persist.UserData[MAGAZINE_STATUS_VAR];
    switch (status)
    {
    case Initial:
        if (ReadBit(MAGAZINE_CLOSED_INPUT))
        {
            status = Closed;
        }
        else
        {
            status = Opened;
        }

        ClearBit(OPEN_MAGAZINE_OUTPUT);
        ClearBit(CLOSE_MAGAZINE_OUTPUT);
        break;

    case Opening:
        if (ReadBit(MAGAZINE_OPENED_INPUT))
        {
            ClearBit(OPEN_MAGAZINE_OUTPUT);
            status = Opened;
        }

        break;
    
    case Closing:
        if (ReadBit(MAGAZINE_CLOSED_INPUT))
        {
            ClearBit(CLOSE_MAGAZINE_OUTPUT);
            status = Closed;
        }

        break;

    default:
        break;
    }
}

/* // Opens the tool magazine.
void OpenMagazine()
{
    SetBit(OPEN_MAGAZINE_OUTPUT);
    while(!ReadBit(MAGAZINE_OPENED_INPUT));
    //Delay_sec(4);
    ClearBit(OPEN_MAGAZINE_OUTPUT);
}

// Closes the tool magazine.
void CloseMagazine()
{
    SetBit(CLOSE_MAGAZINE_OUTPUT);
    while(!ReadBit(MAGAZINE_CLOSED_INPUT));
    //Delay_sec(4);
    ClearBit(CLOSE_MAGAZINE_OUTPUT);
} */

// Send an open command to the tool magazine.
void OpenMagazineNoWait()
{
    SetBit(OPEN_MAGAZINE_OUTPUT);
    SetMagazineStatus(Opening);
}

// Send an open command to the tool magazine.
void CloseMagazineNoWait()
{
    SetBit(CLOSE_MAGAZINE_OUTPUT);
    SetMagazineStatus(Closing);
}

void SetMagazineStatus(int status)
{
    persist.UserData[MAGAZINE_STATUS_VAR] = status;
}

int GetMagazineStatus()
{
    return persist.UserData[MAGAZINE_STATUS_VAR];
}