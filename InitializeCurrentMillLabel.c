#include "MillChanger.h"
#include "KMotionDef.h"

#ifndef TMP
#define TMP 10 // which spare persist to use to transfer data
#include "KflopToKMotionCNCFunctions.c"
#endif

main()
{
    char s[80];
    // Updates current milling tool in use
    int currentTool;
    // Try to open file
    FILE *f = fopen(TOOL_DISK_FILE, "rt");
    if (f)  // did file open?
    {
        // read a line and convert it
        fscanf(f, "%d", &currentTool);
        fclose(f);
    }
    // Now compute and form result
	sprintf(s,"%d",currentTool);
    // Put it onto the Screen
	DROLabel(1000, PREVIOUS_TOOL_LABEL_VAR, s);
}