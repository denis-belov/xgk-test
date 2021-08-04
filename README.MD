# Generating makefile
## GCC x64
```
node /home/denis/reps/denis-belov/c-build/src/makefiler gcc-x64 /home/denis/reps/denis-belov/xgk-test/make.json
```
## MSVC x64
```
node /home/denis/reps/denis-belov/c-build/src/makefiler msvc-x64 /home/denis/reps/denis-belov/xgk-test/make.json
```

# Running makefile
## Linux
```
make -f /home/denis/reps/denis-belov/xgk-test/makefiles/gcc-x64/makefile
```
## WIndows
```
nmake /F /home/denis/reps/denis-belov/xgk-test/makefiles/msvc-x64/makefile
```

# Running app
## Linux
```
cd /home/denis/reps/denis-belov/xgk-test/build/gcc-x64/output/bin && ./xgk-test
```
## WIndows
```
cd /home/denis/reps/denis-belov/xgk-test/build/msvc-x64/output/bin && xgk-test.exe
```