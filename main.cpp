//
//  main.cpp
//  2.1 Приближение в треугольнике степень 1
//
//  Created by Arsenii Klevakin on 26.07.2023.
//

/*
 Нужно написать класс треугольника, который по трем точкам (x,y) будет считать коэфиценты для апроксимации
 Там будет функция которая будет выдавать значение Pf, проверяя что заданная точка лежит внутри треугольника
 */

/*
 Алгоритм
 1 Задаются исходные значения функции в некоторых точках
 2 Задается фигура в которой должна будет построена интерполяция
 3 Производится треангуляция покрывающая исходную фигуру
 4 Покрываем сеткой исходный треугольник
 5 Вычисляем значение Pf в каждой точке сетки, проверяя что точка лежит внутри фигуры
 6 Вычисляем реальные значения функции из пункта 5
 7 Записываем точки в два файла
 */

#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
#include <math.h>

using namespace std;

double fn(double x, double y);

struct Point {
    double x;
    double y;
    double z=0;
};

class Triangle {
private:
    struct Point {
        double x;
        double y;
    };
    
    vector<double> alpha[3];
    
    Point points[3];
    
    // Функция для вычисления расстояния между двумя точками
    double distance(Point p1, Point p2) {
        double dx = p2.x - p1.x;
        double dy = p2.y - p1.y;
        return sqrt(pow(dx, 2) + pow(dy, 2));
    }

    // Функция для вычисления площади треугольника по трем вершинам
    double calculateTriangleArea(Point p1, Point p2, Point p3) {
        double a = distance(p1, p2);
        double b = distance(p2, p3);
        double c = distance(p3, p1);
        double s = (a + b + c) / 2.0;
        return sqrt(s * (s - a) * (s - b) * (s - c));
    }
    
    double calculateL(Point p1, Point p2, Point d){
        double res;
        res=((d.x-p1.x)*(p2.y-p1.y))-((d.y-p1.y)*(p2.x-p1.x));
        return res;
    }
    
    //Вычисляем Пси_1 Пси_2 Пси_3
    double calculatePsi_i(int i, Point d){
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
    double calculateFi(int i, Point d){
        Point p1, p2, p3;
        p1=points[0]; p2=points[1]; p3=points[2];
        double res;
        res = calculatePsi_i(i, d)/calculatePsi_i(i, points[i-1]);
        return res;
    }
    

public:
    Triangle(double x1, double y1, double x2, double y2, double x3, double y3) {
        points[0] = {x1, y1};
        points[1] = {x2, y2};
        points[2] = {x3, y3};
    }
    
    Triangle(){
        
    }
    
    // Функция для проверки, лежит ли заданная точка внутри треугольника
    bool isPointInside(double x, double y) {
        Point t = {x,y};

        // Вычисление площадей подтреугольников
        double areaTotal = calculateTriangleArea(points[0], points[1], points[2]);
        double area1 = calculateTriangleArea(points[0], points[1], t);
        double area2 = calculateTriangleArea(t, points[1], points[2]);
        double area3 = calculateTriangleArea(points[0], t, points[2]);

        // Если сумма площадей подтреугольников равна площади треугольника, то точка лежит внутри треугольника
        return abs(areaTotal - (area1 + area2 + area3)) < 1e-6;
    }
    
    double Pf(double x, double y){
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
    
};

double fn(double x, double y){
    return x*x+y*y;
}

vector<Point> MakeaNet(double x1, double y1, double x2, double y2, int n){
    vector<Point> Points;
    double dx = (x2 - x1) / n;
    double dy = (y2 - y1) / n;

    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= n; j++) {
            double x = x1 + i * dx;
            double y = y1 + j * dy;
            Point P = {x, y, 0};
            Points.push_back(P);
        }
    }
    
    return Points;
}

vector<Point> calculatePfValuesForNet(vector<Triangle> triangles, double x1, double y1, double x2, double y2, int n){
    vector<Point> Points = MakeaNet(x1, y1, x2, y2, n);
    for(int i=0; i<Points.size(); i++){
        for(int j=0; j<triangles.size(); j++){
            if(triangles[j].isPointInside(Points[i].x, Points[i].y))
                Points[i].z=triangles[j].Pf(Points[i].x, Points[i].y);
        }
    }
    
    return Points;
}

void write_points(vector<Point> Points, const string& filename) {
    ofstream file(filename);
    if (file.is_open()) {
        for (int i = 0; i < Points.size(); i++) {
            file << Points[i].x << " " << Points[i].y << " " << Points[i].z << endl;
        }
        file.close();
        cout << "Points written to file: " << filename << endl;
    } else {
        cerr << "Unable to open file: " << filename << endl;
    }
}

// Функция для разбиения прямоугольника на (n+1)*(n+1) маленьких прямоугольников
vector<vector<Point>> splitRectangle(double x1, double y1, double x2, double y2, int n) {
    vector<vector<Point>> rectangles;
    double dx = (x2 - x1) / n;
    double dy = (y2 - y1) / n;

    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= n; j++) {
            double x = x1 + i * dx;
            double y = y1 + j * dy;
            rectangles.push_back({{x, y}, {x + dx, y}, {x + dx, y + dy}, {x, y + dy}});
        }
    }

    return rectangles;
}

// Функция для разбиения прямоугольника на прямоугольные треугольники
vector<vector<Point>> triangulateRectangle(vector<Point> rectangle) {
    vector<vector<Point>> triangles;
    triangles.push_back({rectangle[0], rectangle[1], rectangle[3]});
    triangles.push_back({rectangle[1], rectangle[2], rectangle[3]});
    return triangles;
}


double calculateL(Point p1, Point p2, Point d){
    double res;
    res=((d.x-p1.x)*(p2.y-p1.y))-((d.y-p1.y)*(p2.x-p1.x));
    return res;
}

int main() {
    double x1 = -10, y1 = -10;
    double x2 = 10, y2 = 10;
    int n = 10; // Разбиение прямоугольника на n частей

    vector<vector<Point>> rectangles = splitRectangle(x1, y1, x2, y2, n);
    vector<Triangle> Triangles;
    
    for (const auto& rectangle : rectangles) {
        vector<vector<Point>> triangles = triangulateRectangle(rectangle);
        for (const auto& triangle : triangles) {
            cout << "Triangle: ";
            vector<double> a;
            for (const auto& point : triangle) {
                cout << "(" << point.x << ", " << point.y << ") ";
                a.push_back(point.x);
                a.push_back(point.y);
            }
            
            Triangle T(a[0], a[1], a[2], a[3], a[4], a[5]);
            Triangles.push_back(T);
            cout << endl;
        }
    }
    
    cout << Triangles[0].Pf(0, 0)<<  endl;
    
    //последний параметр отвечает за то насколько мелкой будет сетка
    vector<Point> AproxPoints = calculatePfValuesForNet(Triangles, x1, y1, x2, y2, 50);
    
    //считаем реальные значения в точках  (x,y)
    
    vector<Point> RealPoints;
    for(int i=0; i<AproxPoints.size(); i++){
        Point help;
        help.x = AproxPoints[i].x;
        help.y = AproxPoints[i].y;
        help.z = fn(help.x, help.y);
        RealPoints.push_back(help);
    }
    
    Point a = {1, 1};
    Point b = {4, 5};
    Point c = {2, 3};
    
    write_points(RealPoints, "original_points.txt");
    write_points(AproxPoints, "interpolated_points.txt");
    
    cout <<"="<<calculateL(a, b, c) << endl;
    return 0;
}

