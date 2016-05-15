#ifndef INCLUDE_MESSAGE
#define INCLUDE_MESSAGE


//wParam: –¢’è    lParam: EditBox*
#define WM_NOTIFY_ENTERTEXT (WM_APP+1)

//wParam: code    lParam: void*
#define WM_NOTIFY_FILEVIEW (WM_APP+2)
#define KN_FILEVIEW_CHDIR 1
#define KN_FILEVIEW_RETURN 2
//wParam: HANDLE  lParam: id
#define KM_NOTIFY_DROPFILES (WM_APP+3)

//wParam: ID
#define WM_TASKTRAY			(WM_APP+4)


#endif
