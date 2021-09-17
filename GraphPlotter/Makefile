build:
	@echo "Running emcc..."
	emcc -o Web/build.js GraphsExport.cpp -O3 -s WASM=1 -s NO_EXIT_RUNTIME=1 -s "EXTRA_EXPORTED_RUNTIME_METHODS=['ccall']" -s EXTRA_EXPORTED_RUNTIME_METHODS='["cwrap"]' -std=c++17
