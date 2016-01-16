all: compile

compile: Project.cpp
	icc -mkl -o project Project.cpp

clean:
	$(RM) a.out


