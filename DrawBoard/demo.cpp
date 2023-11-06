#define _CRT_SECURE_NO_WARNINGS
#include"demo.h"

ShapeFacade* facade[5];
void load() {
    facade[1] = new CircleFacade;
    ShapeVector.pop_back();
    facade[2] = new SquareFacade;
    ShapeVector.pop_back();
    facade[3] = new LineFacade;
    ShapeVector.pop_back();
    facade[4] = new PolygonFacade;
    ShapeVector.pop_back();
}

void CreateBottun(HWND hwnd) {
    hbutton1 = CreateWindow("BUTTON", "文件", WS_VISIBLE | WS_CHILD,
        0, 0, 100, 30, hwnd, NULL, GetModuleHandle(NULL), NULL);
    hbutton2 = CreateWindow("BUTTON", "绘画", WS_VISIBLE | WS_CHILD,
        100, 0, 100, 30, hwnd, NULL, GetModuleHandle(NULL), NULL);
}

void CreateMenu(HWND hwnd) {
    hPopupMenu = CreatePopupMenu();
    AppendMenu(hPopupMenu, MF_STRING, IDM_OPEN, "打开");
    AppendMenu(hPopupMenu, MF_STRING, IDM_SAVE, "保存");
    menu2 = CreatePopupMenu();
    AppendMenu(menu2, MF_STRING, DRAW_CIRCLE, "圆");
    AppendMenu(menu2, MF_STRING, DRAW_SQUARE, "矩形");
    AppendMenu(menu2, MF_STRING, DRAW_LINE, "线段");
    AppendMenu(menu2, MF_STRING, DRAW_POLYGON, "多边形");

    // 将弹出菜单关联到按钮
    SetMenu(hbutton1, hPopupMenu);
    SetMenu(hbutton2, menu2);
}

void OpenFile() {
    // 打开文件对话框

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFilter = TEXT("Text files (*.txt)\0*.txt\0All files (*.*)\0*.*\0 Batch Files (*.bat)\0*.bat\0All Files\0*.*\0");
    ofn.lpstrFile = szFileName;
    ofn.nMaxFile = sizeof(szFileName) / sizeof(*szFileName);
    ofn.Flags = OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn))
    {
        FILE* file = _tfopen(szFileName, TEXT("r"));
        if (file != NULL)
        {
            char c;
            int type = 0;
            int time = 0;//time = 0：读取x坐标 1：读取y坐标
            int dottime = 0;// 0：读取类型 1：读取点数 2：存储
            int point_num_read = 0;
            int point_num_theoretical = 0;
            int value = 0;
            POINT* point = new POINT;
            std::vector<POINT*> points;

            ShapeVector.clear();

            while ((c = fgetc(file)) != EOF) {
                switch (c) {
                case('['):
                    break;
                case('0'):
                case('1'):
                case('2'):
                case('3'):
                case('4'):
                case('5'):
                case('6'):
                case('7'):
                case('8'):
                case('9'):
                    value = value * 10 + c - '0';
                    break;
                case(','):
                    switch (time) {
                    case(0):
                        point->x = value;
                        value = 0;
                        time++;
                        break;
                    case(1):
                        point->y = value;
                        points.push_back(point);
                        point = new POINT;
                        point_num_read++;
                        value = 0;
                        time = 0;
                        break;
                    }
                    break;
                case('.'):
                    switch (dottime) {
                    case(0):
                        type = value;
                        value = 0;
                        dottime++;
                        break;
                    case(1):
                        point_num_theoretical = value;
                        value = 0;
                        dottime++;
                        break;
                    case(2):
                    {
                        point->y = value;
                        points.push_back(point);
                        point_num_read++;
                        time = 0;
                        value = 0;
                        dottime = 0;
                        if (point_num_read != point_num_theoretical)
                            MessageBox(hwnd, TEXT("文件损坏！"), TEXT("错误"), MB_ICONERROR);
                        Shape* ShapeToBeRead = NULL;
                            switch (type) {
                            case(CircleType):
                                ShapeToBeRead = new Circle(points, 2);
                                break;
                            case(SquareType):
                                ShapeToBeRead = new Square(points, 2);
                                break;
                            case(LineType):
                                ShapeToBeRead = new Line(points, 2);
                                break;
                            case(PolygonType):
                                ShapeToBeRead = new polygon(points, point_num_theoretical);
                            }
                        ShapeVector.push_back(ShapeToBeRead);
                        point_num_read = 0;
                        point_num_theoretical = 0;
                        points.clear();
                    }
                    }
                }
            }
            std::fclose(file);
            GetClientRect(hwnd, &clientRect);
            RECT rect={ 0,30,clientRect.right,clientRect.bottom };
            InvalidateRect(hwnd, &rect, TRUE);

        }
        else
        {
            MessageBox(hwnd, TEXT("无法打开文件！"), TEXT("错误"), MB_ICONERROR);
        }
    }
}

void SaveFile() {
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = TEXT("Text files (*.txt)\0*.txt\0All files (*.*)\0*.*\0 Batch Files (*.bat)\0*.bat\0All Files\0*.*\0");
    ofn.lpstrFile = szFilePath;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFilePath);
    ofn.lpstrTitle = TEXT("选择保存路径和文件名");
    ofn.Flags = OFN_OVERWRITEPROMPT;

    // 打开文件对话框
    if (GetSaveFileName(&ofn) == TRUE)
    {
        // 创建文件
        HANDLE hFile = CreateFile(szFilePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile == INVALID_HANDLE_VALUE)
        {
            MessageBox(hwnd, TEXT("创建失败！"), TEXT("错误!"), MB_ICONERROR);
        }


        //获取保存内容
        std::stringstream content0 ;
        content0 << '[';
        for (Shape* ShapeToBeSaved : ShapeVector){
            content0 << ShapeToBeSaved->KindsOfShape << '.' << ShapeToBeSaved->point_num << '.';
            for (POINT* point : ShapeToBeSaved->points)
                content0 << point->x << ',' << point->y << ',';
            // 将当前写入位置移动到倒数第二个字符
            content0.seekp(-1, std::ios_base::end);

            // 将最后一个字符替换为'.'
            content0.put('.');
        }
        content0 << ']';

        std::string content1 = content0.str();

        std::vector<char> content2(content1.begin(), content1.end());
        content2.push_back('\0');
        char* content = content2.data();
        // 写入内容
        DWORD dwBytesWritten;
        
        if (!WriteFile(hFile, content, strlen(content), &dwBytesWritten, NULL))
        {
            MessageBox(hwnd, TEXT("保存失败！"), TEXT("错误"), MB_ICONERROR);
            CloseHandle(hFile);
        }

        CloseHandle(hFile);
    }
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int x, y;
    HDC hdc = GetDC(hwnd);
    PAINTSTRUCT ps;

    switch (message)
    {

    case WM_DESTROY:
        // 关闭窗口
        PostQuitMessage(0);
        return 0;

    case WM_COMMAND:
        // 处理菜单项的消息
        if (wParam == BN_CLICKED && (HWND)lParam == hbutton1) {
            POINT ptPopup;
            GetCursorPos(&ptPopup);
            TrackPopupMenu(hPopupMenu, TPM_LEFTALIGN | TPM_TOPALIGN, ptPopup.x, ptPopup.y, 0, hwnd, NULL);
            MSG msg;
            GetMessage(&msg, NULL, 0, 0);
            switch (msg.wParam)
            {
            case IDM_OPEN:
                OpenFile();
                break;

            case IDM_SAVE:
                SaveFile();
                break;
            }
        }
        else if (wParam == BN_CLICKED && (HWND)lParam == hbutton2) {
            POINT ptPopup;
            GetCursorPos(&ptPopup);
            TrackPopupMenu(menu2, TPM_LEFTALIGN | TPM_TOPALIGN, ptPopup.x, ptPopup.y, 0, hwnd, NULL);
            MSG msg;
            GetMessage(&msg, NULL, 0, 0);
            int ShapeType = msg.wParam;
            HDC hdc = GetDC(hwnd);
            facade[ShapeType]->shape = facade[ShapeType]->factory->createShape();
            facade[ShapeType]->painter->draw(facade[ShapeType]->shape, hwnd, hdc);
            if (facade[ShapeType]->shape != NULL)
                facade[ShapeType]->shape->draw(hwnd, hdc);
        }
        break;
    case WM_LBUTTONDOWN:
        lastCursorPosition.x = LOWORD(lParam);
        lastCursorPosition.y = HIWORD(lParam);
        
        for (Shape* ShapeToBeCatched : ShapeVector) {
            if (ShapeToBeCatched->ifin(lastCursorPosition.x, lastCursorPosition.y)) {
                isDragging = true;
                ShapeToBeMoved = ShapeToBeCatched;
                break;
            }
        }
        break;
    case WM_MOUSEMOVE:
        x = LOWORD(lParam);
        y = HIWORD(lParam);
        if (isDragging)
        {
            int dx = x - lastCursorPosition.x;
            int dy = y - lastCursorPosition.y;
            ShapeToBeMoved->reCreate(dx, dy);
            lastCursorPosition.x = x;
            lastCursorPosition.y = y;
            GetClientRect(hwnd, &clientRect);
            RECT rect{ 0,30,clientRect.right,clientRect.bottom };
            InvalidateRect(hwnd, &rect, TRUE);
        }
        break;
    case WM_SIZE:
    {
        GetClientRect(hwnd, &clientRect);
        RECT rect{ 0,30,clientRect.right,clientRect.bottom };
        InvalidateRect(hwnd, &rect, TRUE);
        break;
    }
    case WM_LBUTTONUP:
        if (isDragging)
        {
            isDragging = false;
            dragShapeType = -1;
        }
        break;
    case WM_PAINT:
        hdc = BeginPaint(hwnd, &ps);
        // 创建与屏幕大小相同的缓冲区
        GetClientRect(hwnd, &clientRect);
        int width = clientRect.right - clientRect.left;
        int height = clientRect.bottom - clientRect.top;

        HDC memDCFront = CreateCompatibleDC(hdc);
        HBITMAP frontBuffer = CreateCompatibleBitmap(hdc, width, height);
        HBITMAP oldFrontBuffer = (HBITMAP)SelectObject(memDCFront, frontBuffer);

        HDC memDCBack = CreateCompatibleDC(hdc);
        HBITMAP backBuffer = CreateCompatibleBitmap(hdc, width, height);
        HBITMAP oldBackBuffer = (HBITMAP)SelectObject(memDCBack, backBuffer);

        // 绘制图形到后台缓冲区
        HBRUSH hbr = (HBRUSH)(COLOR_WINDOW + 1);
        FillRect(memDCBack, &clientRect, hbr);
        for (Shape* ShapeToBeDrawed : ShapeVector) {
            ShapeToBeDrawed->draw(hwnd,memDCBack);
        }
        // 将后台缓冲区内容复制到前台缓冲区
        BitBlt(memDCFront, 0, 0, width, height, memDCBack, 0, 0, SRCCOPY);

        // 将前台缓冲区内容绘制到屏幕上
        BitBlt(hdc, 0, 0, width, height, memDCFront, 0, 0, SRCCOPY);

        // 清理资源
        SelectObject(memDCFront, oldFrontBuffer);
        DeleteObject(frontBuffer);
        DeleteDC(memDCFront);

        SelectObject(memDCBack, oldBackBuffer);
        DeleteObject(backBuffer);
        DeleteDC(memDCBack);

        EndPaint(hwnd, &ps);
    }
    // 其他消息交给系统默认的处理函数
    return DefWindowProc(hwnd, message, wParam, lParam);
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
    //加载
    load();
    // 定义窗口类
    WNDCLASS wndClass = { 0 };
    wndClass.style = CS_HREDRAW | CS_VREDRAW; // 窗口样式
    wndClass.lpfnWndProc = WndProc; // 窗口消息处理函数
    wndClass.hInstance = hInstance; // 当前实例句柄
    wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // 背景色
    wndClass.lpszClassName = "MyWindowClass"; // 窗口类名称

    // 注册窗口类
    if (!RegisterClass(&wndClass))
    {
        MessageBox(NULL, "窗口类注册失败！", "Error", MB_ICONERROR);
        return 0;
    }

    // 创建窗口
    hwnd = CreateWindow("MyWindowClass", "画图", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, hInstance, NULL);
    if (hwnd == NULL)
    {
        MessageBox(NULL, "窗口创建失败！", "Error", MB_ICONERROR);
        return 0;
    }

    // 创建按钮

    CreateBottun(hwnd);

    // 创建弹出菜单

    CreateMenu(hwnd);

    // 显示窗口
    ShowWindow(hwnd, iCmdShow);

    // 更新窗口
    UpdateWindow(hwnd);

    // 消息循环
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}