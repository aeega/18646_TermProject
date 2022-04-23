compile : 
	gcc conv_kernel_2_stages.c -std=c99 -mavx -mavx2 -mfma -lm -O3 -o conv_kernel_2_stages.x
	g++ image_read.cpp `pkg-config --cflags --libs opencv` -o image_read.x
	g++ image_write.cpp `pkg-config --cflags --libs opencv` -o image_write.x

run :
	./image_read.x $(image)
	./conv_kernel_2_stages.x
	./image_write.x $(image)
clean :
	rm -rf *.x *.csv

all : clean compile run
