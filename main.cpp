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
#include "Triangle.hpp"
#include "Point.hpp"
#include "inputFunction.hpp"
#include <random>

using namespace std;


//Создает сетку точек в прямоугольнике
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

//Функция считает интерпалиционные значения функции для сетки, которая создается внутри прямоугольника, параметр n влияет на маштаб сетки
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


vector<Triangle> Make_Triangulation_Delane(Triangle MasterTriangle, vector<Point> points){
    vector<Triangle> triangles;
    //Найти треугольник которому принадлежит точка
    //Созать 4 новых треугольника по 4м точкам
    //Добавить 4 треугольника в масив
    triangles.push_back(MasterTriangle);
    for(int i=0; i<points.size(); i++){
        for(int j=0; j<triangles.size(); j++){
            if(triangles[j].isPointInside(points[i].x,points[i].y)){
                Triangle T=triangles[j];
                Triangle A(T.getVertex1(), T.getVertex2(), points[i]);
                Triangle B(T.getVertex1(), points[i], T.getVertex3());
                Triangle C(points[i], T.getVertex2(), T.getVertex3());
                triangles.erase(triangles.begin() + j);
                triangles.push_back(A);
                triangles.push_back(B);
                triangles.push_back(C);
                break;
            }
        }
    }
    
    return triangles;
}

vector<Point> generateRandomPointsInsideTriangle(Triangle triangle, int n) {
    vector<Point> randomPoints;
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0.0, 1.0);

    for (int i = 0; i < n; ++i) {
        double r1 = dis(gen);
        double r2 = dis(gen);

        if (r1 + r2 <= 1.0) {
            double x = triangle.getVertex1().x + r1 * (triangle.getVertex2().x - triangle.getVertex1().x) +
                       r2 * (triangle.getVertex3().x - triangle.getVertex1().x);
            double y = triangle.getVertex1().y + r1 * (triangle.getVertex2().y - triangle.getVertex1().y) +
                       r2 * (triangle.getVertex3().y - triangle.getVertex1().y);
            randomPoints.push_back({x, y});
        }
        else i--;
    }
    
    return randomPoints;
}

void writeTriangulationToFile(vector<Triangle> Triangles, const string& filename){
    ofstream file(filename);
    if(file.is_open()){
        for(int i=0; i<Triangles.size(); i++){
            Point v1, v2, v3;
            v1=Triangles[i].getVertex1();
            v2=Triangles[i].getVertex2();
            v3=Triangles[i].getVertex3();
            file << v1.x << " " << v1.y << " " << v1.z << endl << v2.x << " " << v2.y << " " << v2.z << endl;
            file << endl;
            file << v2.x << " " << v2.y << " " << v2.z << endl << v3.x << " " << v3.y << " " << v3.z << endl;
            file << endl;
            file << v3.x << " " << v3.y << " " << v3.z << endl << v1.x << " " << v1.y << " " << v1.z << endl;
            file << endl;
        }
        cout << "Triangulation writen to file";
    }
    else cout << "Unable to open triangulation file";
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
    

    //Код случайного набора точек внтури мастер треугольника
    vector<Triangle> Triangles2;
    vector<Point> Points2;
    Triangle MasterTriangle(-10, -10, -10, 10, 10, 0);
    
    //Генерируем n случайных точек внутри Мастре треугольника
    Points2=generateRandomPointsInsideTriangle(MasterTriangle, 20000);
    
    //Делаем треангуляцию Делане
    Triangles2 = Make_Triangulation_Delane(MasterTriangle, Points2);
    
    vector<Point> AproxPoints = calculatePfValuesForNet(Triangles2, x1, y1, x2, y2, 70);
    
    //считаем реальные значения в точках  (x,y)
    
    vector<Point> RealPoints;
    for(int i=0; i<AproxPoints.size(); i++){
        Point help;
        help.x = AproxPoints[i].x;
        help.y = AproxPoints[i].y;
        help.z = fn(help.x, help.y);
        RealPoints.push_back(help);
    }
    
    write_points(RealPoints, "original_points.txt");
    write_points(AproxPoints, "interpolated_points.txt");
    
    
    for(int i=0; i<Points2.size(); i++){
        cout << i << " " << Points2[i].x << " " << Points2[i].y << " " << MasterTriangle.isPointInside(Points2[i].x, Points2[i].y) <<  endl;
    }
    
    vector<Triangle> helpMasterTriangle;
    helpMasterTriangle.push_back(MasterTriangle);
    
    writeTriangulationToFile(Triangles2, "triangulation_points.txt");
    writeTriangulationToFile(helpMasterTriangle, "Master_Triangle.txt");
    cout << endl << "Num of Triangles is=" << Triangles2.size() << endl;
    
    return 0;
}

