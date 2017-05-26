nasm -f win64 sub_long.asm -o sub_long.o
"E:/Program Files/SASM/MinGW64/bin/gcc.exe" sub_long.o -m64 -o sub.exe
nasm -f win64 mul_long.asm -o mul_long.o
"E:/Program Files/SASM/MinGW64/bin/gcc.exe" mul_long.o -m64 -o mul.exe