@echo off

echo ----------Input File----------

set input=main
echo %input%.exe

echo ----------Compling----------
g++ -finput-charset=utf-8 -fexec-charset=gbk %input%.cpp -o %input%.exe^
    -lopengl32 -lglfw3 -lgdi32 -L. -lglfw_wrapper -lgl_wrapper

echo ----------Running----------
%input%.exe

echo ----------End----------
pause
