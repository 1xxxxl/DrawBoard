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
    hbutton1 = CreateWindow("BUTTON", "�ļ�", WS_VISIBLE | WS_CHILD,
        0, 0, 100, 30, hwnd, NULL, GetModuleHandle(NULL), NULL);
    hbutton2 = CreateWindow("BUTTON", "�滭", WS_VISIBLE | WS_CHILD,
        100, 0, 100, 30, hwnd, NULL, GetModuleHandle(NULL), NULL);
}

void CreateMenu(HWND hwnd) {
    hPopupMenu = CreatePopupMenu();
    AppendMenu(hPopupMenu, MF_STRING, IDM_OPEN, "��");
    AppendMenu(hPopupMenu, MF_STRING, IDM_SAVE, "����");
    menu2 = CreatePopupMenu();
    AppendMenu(menu2, MF_STRING, DRAW_CIRCLE, "Բ");
    AppendMenu(menu2, MF_STRING, DRAW_SQUARE, "����");
    AppendMenu(menu2, MF_STRING, DRAW_LINE, "�߶�");
    AppendMenu(menu2, MF_STRING, DRAW_POLYGON, "�����");

    // �������˵���������ť
    SetMenu(hbutton1, hPopupMenu);
    SetMenu(hbutton2, menu2);
}

void OpenFile() {
    // ���ļ��Ի���

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
            int time = 0;//time = 0����ȡx���� 1����ȡy����
            int dottime = 0;// 0����ȡ���� 1����ȡ���� 2���洢
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
                            MessageBox(hwnd, TEXT("�ļ��𻵣�"), TEXT("����"), MB_ICONERROR);
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
            MessageBox(hwnd, TEXT("�޷����ļ���"), TEXT("����"), MB_ICONERROR);
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
    ofn.lpstrTitle = TEXT("ѡ�񱣴�·�����ļ���");
    ofn.Flags = OFN_OVERWRITEPROMPT;

    // ���ļ��Ի���
    if (GetSaveFileName(&ofn) == TRUE)
    {
        // �����ļ�
        HANDLE hFile = CreateFile(szFilePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile == INVALID_HANDLE_VALUE)
        {
            MessageBox(hwnd, TEXT("����ʧ�ܣ�"), TEXT("����!"), MB_ICONERROR);
        }


        //��ȡ��������
        std::stringstream content0 ;
        content0 << '[';
        for (Shape* ShapeToBeSaved : ShapeVector){
            content0 << ShapeToBeSaved->KindsOfShape << '.' << ShapeToBeSaved->point_num << '.';
            for (POINT* point : ShapeToBeSaved->points)
                content0 << point->x << ',' << point->y << ',';
            // ����ǰд��λ���ƶ��������ڶ����ַ�
            content0.seekp(-1, std::ios_base::end);

            // �����һ���ַ��滻Ϊ'.'
            content0.put('.');
        }
        content0 << ']';

        std::string content1 = content0.str();

        std::vector<char> content2(content1.begin(), content1.end());
        content2.push_back('\0');
        char* content = content2.data();
        // д������
        DWORD dwBytesWritten;
        
        if (!WriteFile(hFile, content, strlen(content), &dwBytesWritten, NULL))
        {
            MessageBox(hwnd, TEXT("����ʧ�ܣ�"), TEXT("����"), MB_ICONERROR);
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
        // �رմ���
        PostQuitMessage(0);
        return 0;

    case WM_COMMAND:
        // ����˵������Ϣ
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
        // ��������Ļ��С��ͬ�Ļ�����
        GetClientRect(hwnd, &clientRect);
        int width = clientRect.right - clientRect.left;
        int height = clientRect.bottom - clientRect.top;

        HDC memDCFront = CreateCompatibleDC(hdc);
        HBITMAP frontBuffer = CreateCompatibleBitmap(hdc, width, height);
        HBITMAP oldFrontBuffer = (HBITMAP)SelectObject(memDCFront, frontBuffer);

        HDC memDCBack = CreateCompatibleDC(hdc);
        HBITMAP backBuffer = CreateCompatibleBitmap(hdc, width, height);
        HBITMAP oldBackBuffer = (HBITMAP)SelectObject(memDCBack, backBuffer);

        // ����ͼ�ε���̨������
        HBRUSH hbr = (HBRUSH)(COLOR_WINDOW + 1);
        FillRect(memDCBack, &clientRect, hbr);
        for (Shape* ShapeToBeDrawed : ShapeVector) {
            ShapeToBeDrawed->draw(hwnd,memDCBack);
        }
        // ����̨���������ݸ��Ƶ�ǰ̨������
        BitBlt(memDCFront, 0, 0, width, height, memDCBack, 0, 0, SRCCOPY);

        // ��ǰ̨���������ݻ��Ƶ���Ļ��
        BitBlt(hdc, 0, 0, width, height, memDCFront, 0, 0, SRCCOPY);

        // ������Դ
        SelectObject(memDCFront, oldFrontBuffer);
        DeleteObject(frontBuffer);
        DeleteDC(memDCFront);

        SelectObject(memDCBack, oldBackBuffer);
        DeleteObject(backBuffer);
        DeleteDC(memDCBack);

        EndPaint(hwnd, &ps);
    }
    // ������Ϣ����ϵͳĬ�ϵĴ�����
    return DefWindowProc(hwnd, message, wParam, lParam);
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
    //����
    load();
    // ���崰����
    WNDCLASS wndClass = { 0 };
    wndClass.style = CS_HREDRAW | CS_VREDRAW; // ������ʽ
    wndClass.lpfnWndProc = WndProc; // ������Ϣ������
    wndClass.hInstance = hInstance; // ��ǰʵ�����
    wndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // ����ɫ
    wndClass.lpszClassName = "MyWindowClass"; // ����������

    // ע�ᴰ����
    if (!RegisterClass(&wndClass))
    {
        MessageBox(NULL, "������ע��ʧ�ܣ�", "Error", MB_ICONERROR);
        return 0;
    }

    // ��������
    hwnd = CreateWindow("MyWindowClass", "��ͼ", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, hInstance, NULL);
    if (hwnd == NULL)
    {
        MessageBox(NULL, "���ڴ���ʧ�ܣ�", "Error", MB_ICONERROR);
        return 0;
    }

    // ������ť

    CreateBottun(hwnd);

    // ���������˵�

    CreateMenu(hwnd);

    // ��ʾ����
    ShowWindow(hwnd, iCmdShow);

    // ���´���
    UpdateWindow(hwnd);

    // ��Ϣѭ��
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}