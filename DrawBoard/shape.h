#pragma once
#include<Windows.h>
#include<stdio.h>
#include <cstdio>
#include <tchar.h>  
#include<cmath>
#include<vector>
#include<iostream>
#include<string>
#include<sstream>

using namespace std;

class Shape;
class ShapePainter;
class ShapeFactory;
extern std::vector<Shape*> ShapeVector;
enum type {
	CircleType = 1,
	SquareType,
	LineType,
	PolygonType
};


class Shape{
public:
	std::vector<POINT*> points;
	int point_num;
	int KindsOfShape;

	Shape();

	virtual void draw(HWND hwnd, HDC hdc) = 0;
	virtual bool ifin(int clickx, int clicky) = 0;
	virtual void reCreate(int dx, int dy);
};
class Circle :public Shape {
public:
	int x, y, radius;
	Circle();
	Circle(std::vector<POINT*> points, int point_num);
	
	void setCenter(int x, int y);
	void setRadius(int x, int y);
	void draw(HWND hwnd, HDC hdc);
	bool ifin(int clickx, int clicky);
	void reCreate(int dx, int dy);
};
class Square :public Shape {
public:
	int left, right, top, bottom;
	Square();
	Square(std::vector<POINT*> points, int point_num);

	void setStart(int x, int y);
	void setEnd(int x, int y);
	void draw(HWND hwnd, HDC hdc);
	bool ifin(int clickx, int clicky);
	void reCreate(int dx, int dy);
};
class Line :public Shape {
public:
	int left, right, top, bottom;
	Line();
	Line(std::vector<POINT*> points, int point_num);

	void setStart(int x, int y);
	void setEnd(int x, int y);
	void draw(HWND hwnd, HDC hdc);
	bool ifin(int clickx, int clicky);
	void reCreate(int dx, int dy);
};
class polygon :public Shape {
public:
	polygon();
	polygon(std::vector<POINT*> points, int point_num);

	void draw(HWND hwnd, HDC hdc);
	bool ifin(int clickx, int clicky);
	void reCreate(int dx, int dy);
};

class ShapeFactory {
public:
	virtual Shape* createShape() = 0;
};
class CircleFactory :public ShapeFactory {

	Shape* createShape();
};
class SquareFactory :public ShapeFactory {

	Shape* createShape();
};
class LineFactory :public ShapeFactory {

	Shape* createShape();
};
class PolygonFactory :public ShapeFactory {

	Shape* createShape();
};

class ShapePainter {
public:
	virtual void draw(Shape* shape, HWND hwnd, HDC hdc) = 0;
};
class CirclePainter :public ShapePainter {
	void draw(Shape* shape, HWND hwnd, HDC hdc);
};
class SquarePainter :public ShapePainter {
	void draw(Shape* shape, HWND hwnd, HDC hdc);
};
class LinePainter :public ShapePainter {
	void draw(Shape* shape, HWND hwnd, HDC hdc);
};
class PolygonPainter :public ShapePainter {
	void draw(Shape* shape, HWND hwnd, HDC hdc);
};

class ShapeFacade {
public:
	ShapeFacade();

	Shape* shape;
	ShapePainter* painter;
	ShapeFactory* factory;
};
class CircleFacade :public ShapeFacade {
public:
	CircleFacade();
};
class SquareFacade :public ShapeFacade {
public:
	SquareFacade();
};
class LineFacade : public ShapeFacade {
public:
	LineFacade();
};
class PolygonFacade : public ShapeFacade {
public:
	PolygonFacade();
};