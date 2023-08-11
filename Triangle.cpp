//
//  Triangle.cpp
//  2.1 Приближение в треугольнике степень 1
//
//  Created by Arsenii Klevakin on 11.08.2023.
//

#include "Triangle.hpp"

// Функция для вычисления расстояния между двумя точками
double Triangle::distance(Point p1, Point p2) {
    double dx = p2.x - p1.x;
    double dy = p2.y - p1.y;
    return sqrt(pow(dx, 2) + pow(dy, 2));
}

// Функция для вычисления площади треугольника по трем вершинам
double Triangle::calculateTriangleArea(Point p1, Point p2, Point p3) {
    double a = distance(p1, p2);
    double b = distance(p2, p3);
    double c = distance(p3, p1);
    double s = (a + b + c) / 2.0;
    return sqrt(s * (s - a) * (s - b) * (s - c));
}

double Triangle::calculateL(Point p1, Point p2, Point d){
    double res;
    res=((d.x-p1.x)*(p2.y-p1.y))-((d.y-p1.y)*(p2.x-p1.x));
    return res;
}

//Вычисляем Пси_1 Пси_2 Пси_3
double Triangle::calculatePsi_i(int i, Point d){
    Point p1, p2, p3;
    p1=points[0]; p2=points[1]; p3=points[2];
    double res;
    i--;
    if (i==0) res=calculateL(p2, p3, d);
    if (i==1) res=calculateL(p1, p3, d);
    if (i==2) res=calculateL(p1, p2, d);
    return res;
}

//Вычисляем  Фи_1 Фи_2 Фи_3
double Triangle::calculateFi(int i, Point d){
    Point p1, p2, p3;
    p1=points[0]; p2=points[1]; p3=points[2];
    double res;
    res = calculatePsi_i(i, d)/calculatePsi_i(i, points[i-1]);
    return res;
}

Triangle::Triangle(double x1, double y1, double x2, double y2, double x3, double y3) {
    points[0] = {x1, y1};
    points[1] = {x2, y2};
    points[2] = {x3, y3};
}

Triangle::Triangle(){
    
}

Triangle::Triangle(Point a, Point b, Point c){
    points[0] = a;
    points[1] = b;
    points[2] = c;
}


// Функция для проверки, лежит ли заданная точка внутри треугольника
bool Triangle::isPointInside(double x, double y) {
    Point t = {x,y};

    // Вычисление площадей подтреугольников
    double areaTotal = calculateTriangleArea(points[0], points[1], points[2]);
    double area1 = calculateTriangleArea(points[0], points[1], t);
    double area2 = calculateTriangleArea(t, points[1], points[2]);
    double area3 = calculateTriangleArea(points[0], t, points[2]);

    // Если сумма площадей подтреугольников равна площади треугольника, то точка лежит внутри треугольника
    return abs(areaTotal - (area1 + area2 + area3)) < 1e-6;
}

double Triangle::Pf(double x, double y){
    Point d;
    d.x=x;
    d.y=y;
    double res=0;
    for(int i=1; i<=3; i++){
        //double fi_i=calculateFi(i, d);
        //double fn_znch=fn(points[i-1].x, points[i-1].y);
        res=res+calculateFi(i, d)*fn(points[i-1].x, points[i-1].y);
    }
    return res;
}

Point Triangle::getVertex1() { return points[0]; }
Point Triangle::getVertex2() { return points[1]; }
Point Triangle::getVertex3() { return points[2]; }
