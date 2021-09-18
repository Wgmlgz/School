build:
	@echo "Running emcc..."
	emcc -o ../Frontend/build.js GraphsExport.cpp -O3 -std=c++17 -s WASM=1 -s NO_EXIT_RUNTIME=1 -s "EXPORTED_RUNTIME_METHODS=['ccall']" -s EXPORTED_RUNTIME_METHODS='["cwrap"]' 
