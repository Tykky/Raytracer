del compile_commands.json
mkdir build-debug
cd build-debug
vcvarsall.bat x64 && cmake -G "Ninja" -DCMAKE_EXPORT_COMPILE_COMMANDS=TRUE .. && cd .. && copy .\build-debug\compile_commands.json .
