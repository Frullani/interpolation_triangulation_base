set datafile separator whitespace
set terminal qt size 800, 600
set title "3D Triangle Plot"
set xlabel "X"
set ylabel "Y"
set zlabel "Z"
set grid
set key off
set view equal xyz
set xrange [-15:15]
set yrange [-15:15]
set zrange [-2:10]

# Указываем наименование файла с точками
points_file1 = "Master_Triangle.txt"
points_file2 = "triangulation_points.txt"

# Указываем, что у нас трехмерные данные (x, y, z)
splot points_file1 using 1:2:3 with lines lc rgb "blue" lw 4 title "Triangle", points_file2 using 1:2:3 with lines lc rgb "red" lw 2 title "Triangles"

