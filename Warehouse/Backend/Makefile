build:
	em++ --no-entry ./main.cpp -o ../Frontend/warehouse/src/build.mjs  \
	  -s ENVIRONMENT='web' \
	  -s SINGLE_FILE=1  \
	  -s EXPORT_NAME='WWasmModule'  \
	  -s USE_ES6_IMPORT_META=0  \
	  -s TOTAL_MEMORY=500MB \
	  --std=c++20 \
	  -s EXPORTED_FUNCTIONS='["_malloc", "_free"]'  \
	  -s EXPORTED_RUNTIME_METHODS='["ccall", "cwrap"]'  \
	  -O3