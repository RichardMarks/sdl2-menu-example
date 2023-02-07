import os
buildEnv = Environment(ENV = {'PATH' : os.environ['PATH']}, CCFLAGS = '-g -Wall -std=c++17')
buildEnv.ParseConfig('pkg-config sdl2 sdl2_mixer sdl2_image sdl2_ttf --cflags --libs')
buildEnv.Program('./bin/gamebin', Split("""src/main.cpp"""), LIBS = Split("""SDL2_mixer SDL2_image SDL2_ttf""") + buildEnv['LIBS'])