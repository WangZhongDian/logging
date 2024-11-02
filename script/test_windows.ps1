cmake build -B build . 
Set-Location -Path "./build"
cmake --build .
ctest
Set-Location -Path ".."
