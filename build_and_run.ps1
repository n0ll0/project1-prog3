Remove-Item build/* -Recurse;
cmake -G Ninja -S . -B build -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=C:/msys64/ucrt64/bin/gcc.exe -DCMAKE_CXX_COMPILER=C:/msys64/ucrt64/bin/g++.exe ;
cd build;
ninja;
.\main.exe;
cd ..