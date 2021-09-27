build:
	@echo "Running emcc..."
	emcc -o ../Frontend/build.js Build.cpp -O3 -std=c++20 -s WASM=1 -s NO_EXIT_RUNTIME=1 -s "EXPORTED_RUNTIME_METHODS=['ccall']" -s EXPORTED_RUNTIME_METHODS='["cwrap"]' 
