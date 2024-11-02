cmake build -B build . 
Set-Location -Path "./build"
make 
ctest
Set-Location -Path ".."
