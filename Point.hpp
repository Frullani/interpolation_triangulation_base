//
//  Point.hpp
//  2.1 Приближение в треугольнике степень 1
//
//  Created by Arsenii Klevakin on 11.08.2023.
//

#ifndef Point_hpp
#define Point_hpp



class Point {
public:
    double x;
    double y;
    double z=0;
    
    void calculateF_z();
};

#endif /* Point_hpp */
