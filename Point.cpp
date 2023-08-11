//
//  Point.cpp
//  2.1 Приближение в треугольнике степень 1
//
//  Created by Arsenii Klevakin on 11.08.2023.
//

#include "Point.hpp"
#include "inputFunction.hpp"

void Point::calculateF_z(){
    z=fn(x, y);
}
