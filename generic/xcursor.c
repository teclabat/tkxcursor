#include "../config.h"
#include "xcursor.h"
#include <X11/Xcursor/Xcursor.h>
#include <X11/X.h>
#include <string.h>
#include <stdlib.h>
#include "tk.h"

// keep a list of already loaded cursors
typedef struct XcursorCacheNode {
    char* fileName;
    Cursor cursor;
    struct XcursorCacheNode* next;
} XcursorCacheNode;
static XcursorCacheNode* xcursorCache = NULL;

// Cleanup function to free all cursors when command is deleted
static void Xcursor_Cleanup(ClientData clientData);

extern int Xcursor_Init(Tcl_Interp *interp) {
    // initialize stubs
    if (Tcl_InitStubs(interp, "8.6", 0) == NULL)
        return TCL_ERROR;

    if (Tk_InitStubs(interp, "8.6", 0) == NULL)
        return TCL_ERROR;

    // provide package information
    if (Tcl_PkgProvide(interp, PACKAGE_NAME, PACKAGE_VERSION) != TCL_OK)
        return TCL_ERROR;

    // Install "xcursor" command.
    Tcl_CreateObjCommand(interp, "xcursor", Tk_xcursor_cmd,
        (ClientData)NULL,
        (Tcl_CmdDeleteProc*)Xcursor_Cleanup);
  
    // fini
    return (TCL_OK);
}

static void Xcursor_Cleanup(ClientData clientData) {
    XcursorCacheNode* current = xcursorCache;
    XcursorCacheNode* next;
    Display* display = (Display*)clientData;

    // Walk through the linked list and free all resources
    while (current != NULL) {
        next = current->next;

        // Free the X11 cursor resource
        if (display != NULL && current->cursor != None) {
            XFreeCursor(display, current->cursor);
        }

        // Free the filename string
        if (current->fileName != NULL) {
            free(current->fileName);
        }

        // Free the structure itself
        free(current);

        current = next;
    }

    // Reset the global list pointer
    xcursorCache = NULL;
}

int Tk_xcursor_cmd(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]) {
    char* fileName = NULL;
    char* tkWinName = NULL;
    Tk_Window tkWin;
    XcursorCacheNode* cacheNode = NULL;
    XcursorCacheNode* newNode = NULL;
    XcursorCacheNode* last = NULL;

    // reset result
    Tcl_ResetResult(interp);

    // check argc
    if (objc != 3) {
        Tcl_AppendResult(interp, "Invalid command count, use: xcursor <tkwin> <cursorfile>", 0);
        return(TCL_ERROR);
    }

    // get the window name
    tkWinName = Tcl_GetString(objv[1]);
    // get the window object
    tkWin = Tk_NameToWindow(interp, tkWinName, (ClientData)Tk_MainWindow(interp));
    if (tkWin == NULL) {return(TCL_ERROR);}
    // check
    if (None == Tk_WindowId(tkWin)) Tk_MakeWindowExist(tkWin);

    // get the cursor name
    fileName = Tcl_GetString(objv[2]);
    if (fileName[0] == 0) {
        XUndefineCursor(Tk_Display(tkWin), Tk_WindowId(tkWin));
        return(TCL_OK);
    }

    // search existing cursors
    cacheNode = xcursorCache;
    while (cacheNode != NULL) {
        if (strcmp(fileName, cacheNode->fileName) == 0) break;
        cacheNode = cacheNode->next;
    }

    // if we did not find a valid cursor then create a new one from scratch
    if (cacheNode == NULL) {
        Cursor cursor = XcursorFilenameLoadCursor(Tk_Display(Tk_MainWindow(interp)), fileName);
        // basic check
        if (cursor == 0) {
            Tcl_AppendResult(interp, "invalid cursor file '", fileName, "'", 0);
            return(TCL_ERROR);
        }

        // allocate new node
        newNode = calloc(1, sizeof(XcursorCacheNode));
        if (newNode == NULL) {
            XFreeCursor(Tk_Display(Tk_MainWindow(interp)), cursor);
            Tcl_AppendResult(interp, "out of memory", NULL);
            return TCL_ERROR;
        }

        // allocate filename string
        newNode->fileName = calloc(strlen(fileName) + 1, sizeof(char));
        if (newNode->fileName == NULL) {
            free(newNode);
            XFreeCursor(Tk_Display(Tk_MainWindow(interp)), cursor);
            Tcl_AppendResult(interp, "out of memory", NULL);
            return TCL_ERROR;
        }

        // fill the new node with data
        strcpy(newNode->fileName, fileName);
        newNode->cursor = cursor;
        newNode->next = NULL;

        // add to list
        if (xcursorCache == NULL) {
            xcursorCache = newNode;
        } else {
            last = xcursorCache;
            while (last->next != NULL) {last = last->next;}
            last->next = newNode;
        }

        cacheNode = newNode;
    }

    // assign new cursor
    //XUndefineCursor(Tk_Display(tkWin), Tk_WindowId(tkWin));
    XDefineCursor(Tk_Display(tkWin), Tk_WindowId(tkWin), cacheNode->cursor);
    //XFreeCursor (Display *display, Cursor cursor)

    // fini
    return(TCL_OK);
}


