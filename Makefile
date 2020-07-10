build:
	g++ -shared -std=c++11 -undefined dynamic_lookup `python3 -m pybind11 --includes` universe.cpp -o astrosim`python3-config --extension-suffix` -D PYTHON

test:
	g++ universe.cpp -std=c++11 -o astrosim

accelerate:
	icpc universe.cpp -std=c++11 -O3 ${MKLROOT}/lib/libmkl_intel_ilp64.a ${MKLROOT}/lib/libmkl_intel_thread.a ${MKLROOT}/lib/libmkl_core.a -liomp5 -lpthread -lm -ldl -mavx -mfma -march=native -mfpmath=sse -qopenmp -fno-pic -qopt-prefetch -DMKL_ILP64 -I${MKLROOT}/include -D EIGEN_USE_MKL_ALL -D NDEBUG -o astrosim
