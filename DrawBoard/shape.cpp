#define _CRT_SECURE_NO_WARNINGS
#include"shape.h"

std::vector<Shape*> ShapeVector = {};

//Shape类
Shape::Shape() {
    this->points = {};
    this->point_num = 0;
    this->KindsOfShape = 0;
    ShapeVector.push_back(this);
}
void Shape::reCreate(int dx, int dy) {
    for (POINT* point : this->points) {
        point->x += dx;
        point->y += dy;
    }
}

Circle::Circle(){
    this->x = 0;
    this->y = 0;
    this->point_num = 2;
    this->KindsOfShape = CircleType;
}
Circle::Circle(std::vector<POINT*> points, int point_num) {
    int x1=points[0]->x, y1=points[0]->y, x2=points[1]->x, y2=points[1]->y;
    this->setCenter(x1, y1);
    this->setRadius(x2, y2);
    this->point_num = 2;
    this->KindsOfShape = CircleType;
    POINT* pt = new POINT();
    pt->x = x1, pt->y = y1;
    this->points.push_back(pt);
    pt = new POINT();
    pt->x = x2, pt->y = y2;
    this->points.push_back(pt);
}
void Circle::draw(HWND hwnd, HDC hdc) {
    RECT clientRect;
    GetClientRect(hwnd, &clientRect);
    RECT drawRect = { 0, 30, clientRect.right, clientRect.bottom };
    IntersectClipRect(hdc, drawRect.left, drawRect.top, drawRect.right, drawRect.bottom);
    HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0)); // 创建一个颜色画刷
    SelectObject(hdc, hBrush); // 将画刷选入设备环境
    Ellipse(hdc, x - radius, y + radius, x + radius, y - radius); // 填充椭圆形内部
    ReleaseDC(hwnd, hdc);
}
bool Circle::ifin(int clickx, int clicky) {
    int det = sqrt(pow(clickx - x, 2) + pow(clicky - y, 2));
    return det <= radius;
}
void Circle::reCreate(int dx, int dy) {
    x = x + dx;
    y = y + dy;
    Shape::reCreate(dx, dy);
};
void Circle::setCenter(int x, int y) {
    this->x = x;
    this->y = y;
}
void Circle::setRadius(int x, int y) {
    this->radius = sqrt(pow(this->x - x, 2) + pow(this->y - y, 2));
}

Square::Square() {
    this->left = this->bottom = this->top = this->right = 0;
    this->point_num = 2;
    this->KindsOfShape = SquareType;
}
Square::Square(std::vector<POINT*> points, int point_num) {
    int x1 = points[0]->x, y1 = points[0]->y, x2 = points[1]->x, y2 = points[1]->y;
    this->setStart(x1, y1);
    this->setEnd(x2, y2);
    this->point_num = 2;
    this->KindsOfShape = SquareType;
    POINT* pt = new POINT();
    pt->x = x1, pt->y = y1;
    this->points.push_back(pt);
    pt = new POINT();
    pt->x = x2, pt->y = y2;
    this->points.push_back(pt);
}
void Square::draw(HWND hwnd, HDC hdc) {
    RECT clientRect;
    GetClientRect(hwnd, &clientRect);
    RECT drawRect = { 0, 30, clientRect.right, clientRect.bottom };
    IntersectClipRect(hdc, drawRect.left, drawRect.top, drawRect.right, drawRect.bottom);
    HBRUSH hBrush = CreateSolidBrush(RGB(255, 180,200)); // 创建一个颜色画刷
    SelectObject(hdc, hBrush); // 将画刷选入设备环境
    Rectangle(hdc, left, top, right, bottom);
    ReleaseDC(hwnd, hdc);
}
bool Square::ifin(int clickx, int clicky) {
    return clickx >= (left < right ? left : right) && clickx <= (left > right ? left : right) && clicky <= (bottom > top ? bottom : top) && clicky >= (bottom < top ? bottom : top);
}
void Square::reCreate(int dx, int dy) {
    left += dx, right += dx;
    top += dy, bottom += dy;
    Shape::reCreate(dx, dy);
};
void Square::setStart(int x, int y) {
    this->left = x;
    this->top = y;
}
void Square::setEnd(int x, int y) {
    this->bottom = y;
    this->right = x;
}

Line::Line() {
    this->left = this->bottom = this->top = this->right = 0;
    this->point_num = 2;
    this->KindsOfShape = LineType;
}
Line::Line(std::vector<POINT*> points, int point_num) {
    int x1 = points[0]->x, y1 = points[0]->y, x2 = points[1]->x, y2 = points[1]->y;
    this->setStart(x1, y1);
    this->setEnd(x2, y2);
    this->point_num = 2;
    this->KindsOfShape = LineType;
    POINT* pt = new POINT();
    pt->x = x1, pt->y = y1;
    this->points.push_back(pt);
    pt = new POINT();
    pt->x = x2, pt->y = y2;
    this->points.push_back(pt);
}
void Line::draw(HWND hwnd, HDC hdc) {
    RECT clientRect;
    GetClientRect(hwnd, &clientRect);
    RECT drawRect = { 0, 30, clientRect.right, clientRect.bottom };
    IntersectClipRect(hdc, drawRect.left, drawRect.top, drawRect.right, drawRect.bottom);
    MoveToEx(hdc, left, top, NULL);
    LineTo(hdc, right, bottom);
    ReleaseDC(hwnd, hdc);
}
bool Line::ifin(int clickx, int clicky) {
    double det = abs((bottom - top) * clickx + (left - right) * clicky + top * right - left * bottom) / sqrt(pow(top - bottom, 2) + pow(left - right, 2));
    return det <= 3;
}
void Line::reCreate(int dx, int dy) {
    left += dx, right += dx;
    top += dy, bottom += dy;
    Shape::reCreate(dx, dy);
};
void Line::setStart(int x, int y) {
    this->left = x;
    this->top = y;
}
void Line::setEnd(int x, int y) {
    this->right = x;
    this->bottom = y;
}

polygon::polygon() {
    this->point_num = 0;
    this->KindsOfShape = PolygonType;
}
polygon::polygon(std::vector<POINT*> points, int point_num) {
    this->points = points;
    this->point_num = point_num;
    this->KindsOfShape = PolygonType;
}
void polygon::draw(HWND hwnd, HDC hdc) {
    RECT clientRect;
    GetClientRect(hwnd, &clientRect);
    RECT drawRect = { 0, 30, clientRect.right, clientRect.bottom };
    IntersectClipRect(hdc, drawRect.left, drawRect.top, drawRect.right, drawRect.bottom);

    if (this->point_num >= 3) {
        HBRUSH hBrush = CreateSolidBrush(RGB(150, 255, 123)); // 绿色
        SelectObject(hdc, hBrush);
        // 定义多边形的顶点坐标数组
        POINT* polygon_points = (POINT*)malloc(sizeof(POINT) * this->point_num);
        int i = 0;
        for (POINT* point : points) {
            polygon_points[i].x = point->x;
            polygon_points[i++].y = point->y;
        }

        // 绘制填充多边形
        Polygon(hdc, polygon_points, this->point_num);

        // 释放画刷对象
        DeleteObject(hBrush);
    }
    else {
        MoveToEx(hdc, this->points[0]->x, this->points[0]->y, NULL);
        LineTo(hdc, this->points[1]->x, this->points[1]->y);
    }
    ReleaseDC(hwnd, hdc);
}
bool polygon::ifin(int clickx, int clicky) {
    int num = this->point_num;
    int intersectCount = 0;
    POINT point;
    point.x = clickx, point.y = clicky;
    for (int i = 0; i < num; i++)
    {
        POINT*p1 = points[i];
        POINT*p2 = points[(i + 1) % num];

        // 检查点P是否在边p1p2的上方或下方
        if ((p1->y <= point.y && p2->y > point.y) || (p1->y > point.y && p2->y <= point.y))
        {
            // 计算射线与边的交点的x坐标
            double intersectX = (point.y - p1->y) * (p2->x - p1->x) / (p2->y - p1->y) + p1->x;

            // 如果交点在点P的右侧
            if (intersectX > point.x)
            {
                intersectCount++;
            }
        }
    }

    // 相交次数为奇数表示点P在多边形内部
    return (intersectCount % 2 == 1);
}
void polygon::reCreate(int dx,int dy) {
    Shape::reCreate(dx, dy);
}

//ShapeFactory
Shape* CircleFactory::createShape() {
    return new Circle();
}

Shape* SquareFactory::createShape() {
    return new Square();
}

Shape* LineFactory::createShape() {
    return new Line();
}

Shape* PolygonFactory::createShape() {
    return new polygon();
}

//ShapePainter
void CirclePainter::draw(Shape* shape, HWND hwnd, HDC hdc) {
    Circle* circle = (Circle*)shape;
    MSG msg;
    int time = 0;
    bool button_down = false;
    bool moving = false;
    while (true) {
        GetMessage(&msg, NULL, 0, 0);
        POINT* pt = new POINT;
        GetCursorPos(pt);
        ScreenToClient(hwnd, pt);
        if (msg.message == WM_LBUTTONDOWN) {
            button_down = true;
            if (pt->y <= 30)
                continue;
            else if (time == 0) {
                circle->points.push_back(pt);
                circle->setCenter(pt->x, pt->y);
                time++;
            }
            else if (time == 1) {
                circle->points.push_back(pt);
                circle->setRadius(pt->x, pt->y);
                time++;
            }
        }
        else if (time == 1 && msg.message == WM_MOUSEMOVE) {
            if (pt->y <= 30)
                continue;
            circle->setRadius(pt->x, pt->y);
            RECT clientRect;
            GetClientRect(hwnd, &clientRect);
            RECT drawRect = { 0, 30, clientRect.right, clientRect.bottom };
            InvalidateRect(hwnd, &drawRect, TRUE);
            UpdateWindow(hwnd);
            circle->draw(hwnd, hdc);
            if (button_down)
                moving = true;
        }
        else if (msg.message == WM_LBUTTONUP) {
            button_down = false;
            if (moving) {
                circle->points.push_back(pt);
                circle->setRadius(pt->x, pt->y);
                time++;
                moving = false;
            }
        }
        else if (msg.message == WM_RBUTTONDOWN) {
            break;
        }
        if (time == 2)
            break;
    }
}

void SquarePainter::draw(Shape* shape,HWND hwnd,HDC hdc) {
    Square* square = (Square*)shape;
    MSG msg;
    int time = 0;
    bool button_down = false;
    bool moving = false;
    while (true) {
        GetMessage(&msg, NULL, 0, 0);
        POINT* pt = new POINT;
        GetCursorPos(pt);
        ScreenToClient(hwnd, pt);
        if (msg.message == WM_LBUTTONDOWN) {
            button_down = true;
            if (pt->y <= 30)
                continue;
            else if (time == 0) {
                square->setStart(pt->x, pt->y);
                square->points.push_back(pt);
                time++;
            }
            else if (time == 1) {
                square->setEnd(pt->x, pt->y);
                square->points.push_back(pt);
                time++;
            }
        }
        else if (time == 1 && msg.message == WM_MOUSEMOVE) {
            if (pt->y <= 30)
                continue;
            square->setEnd(pt->x, pt->y);
            RECT clientRect;
            GetClientRect(hwnd, &clientRect);
            RECT rect{ 0,30,clientRect.right,clientRect.bottom };
            InvalidateRect(hwnd, &rect, TRUE);
            UpdateWindow(hwnd);
            square->draw(hwnd, hdc);
            if (button_down) {
                moving = true;
            }
        }
        else if (msg.message == WM_LBUTTONUP) {
            if (pt->y <= 30)
                continue;
            button_down = false;
            if (moving) {
                square->setEnd(pt->x, pt->y);
                square->points.push_back(pt);
                time++;
                moving = false;
            }
        }
        else if (msg.message == WM_RBUTTONDOWN)
            break;
        if (time == 2)
            break;
    }
}

void LinePainter::draw(Shape* shape, HWND hwnd, HDC hdc) {
    Line* line = (Line*)shape;
    MSG msg;
    int time = 0;
    bool button_down = false;
    bool moving = false;
    while (true) {
        GetMessage(&msg, NULL, 0, 0);
        POINT* pt = new POINT;
        GetCursorPos(pt);
        ScreenToClient(hwnd, pt);
        if (msg.message == WM_LBUTTONDOWN) {
            button_down = true;
            if (pt->y <= 30)
                continue;
            else if (time == 0) {
                line->setStart(pt->x, pt->y);
                line->points.push_back(pt);
                time++;
            }
            else if (time == 1) {
                line->setEnd(pt->x, pt->y);
                line->points.push_back(pt);
                time++;
            }
        }
        else if (time == 1 && msg.message == WM_MOUSEMOVE) {
            if (pt->y <= 30)
                continue;
            line->setEnd(pt->x, pt->y);
            RECT clientRect;
            GetClientRect(hwnd, &clientRect);
            RECT rect{ 0,30,clientRect.right,clientRect.bottom };
            InvalidateRect(hwnd, &rect, TRUE);
            UpdateWindow(hwnd);
            line->draw(hwnd, hdc);
            if (button_down) {
                moving = true;
            }
        }
        else if (msg.message == WM_LBUTTONUP) {
            if (pt->y <= 30)
                continue;
            button_down = false;
            if (moving) {
                line->setEnd(pt->x, pt->y);
                line->points.push_back(pt);
                time++;
                moving = false;
            }
        }
        else if (msg.message == WM_RBUTTONDOWN)
            break;
        if (time == 2)
            break;
    }
}

void PolygonPainter::draw(Shape* shape,HWND hwnd,HDC hdc) {
    polygon* _polygon = (polygon*)shape;
    MSG msg;
    bool begin = true;
    bool moving = false;
    POINT* start_point = new POINT;
    while (true) {
        GetMessage(&msg, NULL, 0, 0);
        POINT* point = new POINT;
        GetCursorPos(point);
        ScreenToClient(hwnd, point);
        if (msg.message == WM_LBUTTONDOWN) {
            if (begin) {
                begin = false;
                start_point->x = point->x;
                start_point->y = point->y;
                _polygon->points.push_back(point);
                _polygon->point_num++;
            }
            else {
                if (moving) {
                    if (point->x != start_point->x || point->y != start_point->y) {
                        _polygon->points.pop_back();
                        _polygon->points.push_back(point);
                    }
                    else
                        break;
                }
                else
                    break;
            }
            moving = false;
        }
        else if (msg.message == WM_MOUSEMOVE) {
            if (!begin) {
                if (moving) {
                    _polygon->points.pop_back();
                    _polygon->points.push_back(point);
                    RECT clientRect;
                    GetClientRect(hwnd, &clientRect);
                    RECT rect{ 0,30,clientRect.right,clientRect.bottom };
                    InvalidateRect(hwnd, &rect, TRUE);
                    UpdateWindow(hwnd);
                    _polygon->draw(hwnd, hdc);
                }
                else {
                    _polygon->points.push_back(point);
                    _polygon->point_num++;
                    moving = true;
                }
            }
        }
        else if (msg.message == WM_RBUTTONDOWN)
            break;
    }
}

//ShapeFacade
ShapeFacade::ShapeFacade() {
    this->shape = NULL;
    this->painter = NULL;
    this->factory = NULL;
}
CircleFacade::CircleFacade() {
   this->shape = new Circle();
    this->painter = new CirclePainter();
    this->factory = new CircleFactory();
}
SquareFacade::SquareFacade() {
    this->shape = new Square();
    this->painter = new SquarePainter();
    this->factory = new SquareFactory();
}
LineFacade::LineFacade() {
    this->shape = new Line();
    this->painter = new LinePainter();
    this->factory = new LineFactory();
}
PolygonFacade::PolygonFacade() {
    this->shape = new polygon();
    this->painter = new PolygonPainter();
    this->factory = new PolygonFactory();
}