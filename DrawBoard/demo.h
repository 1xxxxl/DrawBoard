#pragma once
#include"shape.h"

using namespace std;
HWND hbutton1;
HWND hbutton2;

HWND hwnd;

HMENU hPopupMenu;
HMENU menu2;

OPENFILENAME ofn;
TCHAR szFileName[MAX_PATH] = { 0 };
TCHAR szFilePath[MAX_PATH] = { 0 };

Shape* ShapeToBeMoved;

bool isDragging;

enum menu1 {
    IDM_OPEN = 1,
    IDM_SAVE
};
enum menu2 {
    DRAW_CIRCLE = 1,
    DRAW_SQUARE,
    DRAW_LINE,
    DRAW_POLYGON
};

RECT dragShapeRect = {};
RECT clientRect;

int dragShapeType = -1;   // 当前选中的图形类型

POINT lastCursorPosition = {}; // 上次鼠标位置

void CreateBottun(HWND hwnd);
void CreateMenu(HWND hwnd);
void OpenFile();
void SaveFile();
