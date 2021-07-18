.PHONY:all
all:array_generator matrix_generator adjoint_list_generator

array_generator:array_generator.cpp
	clang++ -g -fcilkplus array_generator.cpp -o array_generator

matrix_generator:matrix_generator.cpp
	clang++ -g -fcilkplus matrix_generator.cpp -o matrix_generator

adjoint_list_generator:adjoint_list_generator.cpp adjoint_list.cpp
	clang++ -g -fcilkplus adjoint_list_generator.cpp adjoint_list.cpp -o adjoint_list_generator