//
//  Triangle.hpp
//  2.1 Приближение в треугольнике степень 1
//
//  Created by Arsenii Klevakin on 11.08.2023.
//

#ifndef Triangle_hpp
#define Triangle_hpp

#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
#include <math.h>
#include "Point.hpp"
#include "inputFunction.hpp"

using namespace std;

class Triangle {
private:
    vector<double> alpha[3];
    
    Point points[3];
    
    // Функция для вычисления расстояния между двумя точками
    double distance(Point p1, Point p2);

    // Функция для вычисления площади треугольника по трем вершинам
    double calculateTriangleArea(Point p1, Point p2, Point p3);
    
    double calculateL(Point p1, Point p2, Point d);
    
    //Вычисляем Пси_1 Пси_2 Пси_3
    double calculatePsi_i(int i, Point d);
    
    //Вычисляем  Фи_1 Фи_2 Фи_3
    double calculateFi(int i, Point d);
    

public:
    Triangle(double x1, double y1, double x2, double y2, double x3, double y3);
    
    Triangle();
    
    Triangle(Point a, Point b, Point c);
    
    
    // Функция для проверки, лежит ли заданная точка внутри треугольника
    bool isPointInside(double x, double y);
    
    double Pf(double x, double y);
    
    Point getVertex1();
    Point getVertex2();
    Point getVertex3();
    
};

#endif /* Triangle_hpp */
