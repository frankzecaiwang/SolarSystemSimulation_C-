#ifndef WINVER
#define WINVER 0x0501
#endif

#include <windows.h>
#include <wingdi.h>
#include "time.h"
#include "math.h"
#include "Object.h"
//#include "MP3Player.h"
#include "logfile.h"


void OnPaint(HWND);
void OnResize(HWND);
void OnTimer(HWND);
void OnMouseLBUPEvent(HWND hwnd, LPARAM lParam);

void initialization(HWND);

const char g_szClassName[] = "myWindowClass";

AstroObject g_objArray[MAX_OBJ];

CLOG g_logfile;

long g_count = 0;

//MP3Player g_player;

//	g_player.OpenFromFile(L"demo.mp3");

// Step 4: the Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
        case WM_CLOSE:
        	g_logfile.closefileIO();
            DestroyWindow(hwnd);
        	break;
        
		case WM_DESTROY:
			g_logfile.closefileIO();
            PostQuitMessage(0);
        	break;
        	
        case WM_PAINT:
        	OnPaint(hwnd);
            break;

        case WM_SIZE:
        	OnResize(hwnd);
            break;
            
         case WM_TIMER:
         	OnTimer(hwnd);
         	break;
        	
   	    case WM_LBUTTONDOWN:

			OnMouseLBUPEvent(hwnd, lParam);
			
        	break;
         	
   	    case WM_KEYDOWN:
        	switch (wParam)
        	{
        		case VK_ESCAPE:
            		PostQuitMessage(0);
            		break;
        	}
        	break;
        
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wc;
    HWND hwnd;
    MSG Msg;

    //Step 1: Registering the Window Class
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = CreateSolidBrush(RGB(0,0,0));//(HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = g_szClassName;
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

    if(!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "Window Registration Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Step 2: Creating the Window
    hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        g_szClassName,
        "Shooting Stars",
        WS_OVERLAPPEDWINDOW | WS_MAXIMIZE,
        CW_USEDEFAULT, CW_USEDEFAULT, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
        NULL, NULL, hInstance, NULL);

    if(hwnd == NULL)
    {
        MessageBox(NULL, "Window Creation Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
    
    g_logfile.OpenFileIO();
    
    initialization(hwnd);
    
    SetTimer(hwnd,             // handle to main window 
    	0,                         // timer identifier 
    	TIMER_LEN,                      // 25ms interval 
    	(TIMERPROC) NULL);         // no timer callback 

    // Step 3: The Message Loop
    while(GetMessage(&Msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return Msg.wParam;
}


void initialization(HWND hwnd)
{		
	RECT wndsize;
	GetWindowRect(hwnd, &wndsize);
	
	g_init_width      = wndsize.right-wndsize.left;
	g_init_height       = wndsize.bottom-wndsize.top;
	g_current_width   = g_init_width;
	g_current_height    = g_init_height;
	
	srand(time(NULL));
	
	g_objArray[0].set(1.989*pow(10,30),(double)0,(double)0,50,RGB(255,255,0),(const char *)"Sun");
	g_objArray[0].m_velocity.set(0,0);
	
	g_objArray[1].set(5.972*pow(10,27),sun_earth_d_pixle*Scalar_d, sun_earth_d_pixle*Scalar_d,15,RGB(50,100,255),(const char *)"Earth");
	g_objArray[1].m_velocity.set(60.78*100000,-60.78*100000);	

	g_objArray[2].set(7.34*pow(10,22),(sun_earth_d_pixle+12)*Scalar_d, (sun_earth_d_pixle+12)*Scalar_d,5,RGB(255,255,255),(const char *)"Moon");
	g_objArray[2].m_velocity.set(2000000,-2010000);
	g_objArray[2].m_velocity += g_objArray[1].m_velocity;
	
	g_objArray[3].set(3.285*pow(10,24),-sun_earth_d_pixle/4*Scalar_d, -sun_earth_d_pixle/4*Scalar_d,10,RGB(170,170,170),(const char *)"Mercury");
	g_objArray[3].m_velocity.set(100*100000,-100*100000);
	
	g_objArray[4].set(3.285*pow(10,20),-sun_earth_d_pixle/8*Scalar_d, -sun_earth_d_pixle/8*Scalar_d,10,RGB(255,255,255),(const char *)"Comet");
	g_objArray[4].m_velocity.set(1845*pow(10,4),-3000*pow(10,4));	
}

void OnTimer(HWND hwnd)
{
	
	for(int i=0; i < MAX_OBJ; i++)
	{
		g_objArray[i].update(g_objArray, MAX_OBJ);
	}
	
	g_count ++;
		
	RedrawWindow(hwnd,NULL,NULL,RDW_INVALIDATE | RDW_INTERNALPAINT);	
}

void OnPaint(HWND hwnd)
{

	PAINTSTRUCT ps;
 	HRGN bgRgn;
    HBRUSH hBrush_star;
    RECT clientRect;
    HDC hdc;
    HBRUSH hBrush;
    
    RECT rect_obj;
    
    float length_ratio_scale, width_ratio_scale;
    
    double dRadius;
    
 	hdc = BeginPaint(hwnd, &ps);
     
  	GetClientRect(hwnd, &clientRect);
   	//bgRgn = CreateRectRgnIndirect(&clientRect);
    hBrush = CreateSolidBrush(RGB(0,0,0));
    //FillRgn(hdc, bgRgn, hBrush);
    
    FillRect(hdc,&clientRect,hBrush);
    
    
    length_ratio_scale = g_current_width*1.0/g_init_width;
    width_ratio_scale  = g_current_height*1.0/g_init_height;
    
    SelectObject(hdc, GetStockObject(DC_BRUSH)); 
    
    if(g_count>100)
    {
    	g_count = 0;	
    }
    
    for(int i=0; i < MAX_OBJ; i++)
    {    	
    	int width, height;
    	
    	rect_obj.left = long((g_objArray[i].m_posx/Scalar_d - g_objArray[i].m_size) * length_ratio_scale + g_current_width / 2);	
    	rect_obj.top = long((g_objArray[i].m_posy/Scalar_d - g_objArray[i].m_size) * width_ratio_scale + g_current_height / 2);
    	
		rect_obj.right = long(rect_obj.left + g_objArray[i].m_size * length_ratio_scale);
    	rect_obj.bottom = long(rect_obj.top + g_objArray[i].m_size * width_ratio_scale);

    	
		SetDCBrushColor(hdc, g_objArray[i].m_color);
    	
    	Ellipse(hdc, (int)rect_obj.left,(int)rect_obj.top, (int)rect_obj.right, (int)rect_obj.bottom);

    }
    
    //SelectObject(hdc, hBrush_old); 
    
    SetDCBrushColor(hdc, RGB(255,255,255));
    
   	//DeleteObject(bgRgn);
    DeleteObject(hBrush);    
      
    EndPaint(hwnd, &ps);
    	
}


void OnResize(HWND hwnd)
{
	RECT wndsize;
	GetWindowRect(hwnd, &wndsize);

	g_current_width      = wndsize.right-wndsize.left;
	g_current_height       = wndsize.bottom-wndsize.top;
}


void OnMouseLBUPEvent(HWND hwnd, LPARAM lParam)
{
	//pt.x = GET_X_LPARAM(lParam);
	//pt.y = GET_Y_LPARAM(lParam);		
}

