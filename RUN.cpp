#undef  UNICODE

/*================================================================================*/

#include <windows.h>

/*================================================================================*/

#define  IB_DIALOG  100

#define  IE  100

#define  IB  200

/*================================================================================*/

long __stdcall WndProc( HWND hWnd , unsigned mes , unsigned wParam , long lParam ) ;

BOOL __stdcall DlgProc( HWND hDlg , unsigned mes , unsigned wParam , long lParam ) ;

/*================================================================================*/

WNDCLASS  wc = { 0 , WndProc , 0 , 0 ,
                 0 , 0 , 0 , (HBRUSH)(COLOR_WINDOW+1) , 0 , "MainWndClass" } ;

MSG  msg ;

HWND  hMainWnd ;

HKEY  hKey ;

char  * s[] = { "Clear" , "Set" , "Delete" } ;

char  aux[7] ;

char  sDRT[] = "DisableRegistryTools" ;

unsigned  i ;

/*================================================================================*/

int __stdcall WinMain( HINSTANCE hInstance , HINSTANCE hPrevInstance , char * CmdLine , int ShowCmd )

{  wc.hInstance = hInstance ;

   wc.hIcon = LoadIcon( 0 , IDI_APPLICATION ) ;

   wc.hCursor = LoadCursor( 0 , IDC_ARROW ) ;

   RegisterClass(&wc) ;

   hMainWnd = CreateWindowEx( WS_EX_CLIENTEDGE , "MainWndClass" , sDRT , WS_SYSMENU ,
                              300 , 300 , 400 , 400 , 0 , 0 , wc.hInstance , 0 ) ;

   ShowWindow( hMainWnd , 1 ) ;

   while ( GetMessage( &msg , 0 , 0 , 0 ) != 0 )

   {  TranslateMessage(&msg) ;

      DispatchMessage(&msg) ;
   }

   return 0 ;
}

/*================================================================================*/

long __stdcall WndProc( HWND hWnd , unsigned mes , unsigned wParam , long lParam )

{  switch ( mes )

   {  case WM_CREATE : CreateWindowEx( 0 , "BUTTON" , "Dialog" , WS_CHILD | WS_VISIBLE ,
                                       10 , 10 , 75 , 25 , hWnd , (HMENU)IB_DIALOG , wc.hInstance , 0 ) ;

                       return 0 ;

      case WM_COMMAND : if ( ( wParam & 0xffff ) == IB_DIALOG )

                        DialogBoxParam( wc.hInstance , "DlgRsrc" , hWnd , DlgProc , 0 ) ;

                        return 0 ;

      case WM_DESTROY : PostQuitMessage(0) ;

                        return 0 ;

              default : DefWindowProc( hWnd , mes , wParam , lParam ) ;
   }
}

/*================================================================================*/

BOOL __stdcall DlgProc( HWND hDlg , unsigned mes , unsigned wParam , long lParam )

{  switch ( mes )

   {  case WM_INITDIALOG : return TRUE ;

         case WM_COMMAND : if ( ( wParam & 0xffff ) == IE ) return FALSE ;

			   SendDlgItemMessage( hDlg , IE , WM_GETTEXT , 7 , (long)aux ) ;

                           for ( i=0 ; i <= 2 ; i++ )

                           if ( lstrcmp( s[i] , aux ) == 0 ) break ;

                           if ( i == 3 )

                           {  MessageBox( 0 , "Incorrect String" , "Error" , MB_ICONERROR ) ;

                              return TRUE ;
                           }

                           RegCreateKeyEx( HKEY_CURRENT_USER ,
                                           "Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System" ,
                                           0 , 0 , REG_OPTION_NON_VOLATILE , KEY_ALL_ACCESS , 0 , &hKey , 0 ) ;

                           switch ( i )

                           {  case 0 :

                              case 1 : RegSetValueEx( hKey , sDRT , 0 , REG_DWORD , (BYTE *)&i , 4 ) ;

                                       break ;

                              case 2 : RegDeleteValue( hKey , sDRT ) ;

                                       break ;
                           }

                           RegCloseKey(hKey) ;

                           return TRUE ;

           case WM_CLOSE : EndDialog( hDlg , 0 ) ;

                           return TRUE ;

                 default : return FALSE ;
   }
}