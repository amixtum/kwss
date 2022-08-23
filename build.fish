mkdir -p build
cd build 
cmake ../
cmake --build .
cd ..

# for YCM C family completion
rm compile_commands.json
cp build/compile_commands.json .
