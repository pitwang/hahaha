all: compile

compile: Project.cpp
	sudo icc -mkl -o project Project.cpp

clean:
	$(RM) a.out


