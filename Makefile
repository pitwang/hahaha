all: compile

compile: Project.cpp
	icc -pg -mkl -o project Project.cpp

clean:
	$(RM) a.out


