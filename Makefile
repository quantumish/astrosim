build:
	g++ -shared -std=c++11 -undefined dynamic_lookup `python3 -m pybind11 --includes` matter.cpp -o astrosim`python3-config --extension-suffix` -D PYTHON

test:
	g++ matter.cpp -std=c++11 -o astrosim
