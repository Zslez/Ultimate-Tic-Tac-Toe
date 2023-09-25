compile:
	@g++ -I src/include -L src/lib -o uttt uttt.cpp -lmingw32 -lcrypto -lSDL2main -lSDL2_image -lSDL2_ttf -lSDL2 -static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic -mwindows


compileconsole:
	@g++ -I src/include -L src/lib -o uttt uttt.cpp -lmingw32 -lcrypto -lSDL2main -lSDL2_image -lSDL2_ttf -lSDL2 -static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic


compileprofile:
	@g++ -I src/include -L src/lib -o uttt uttt.cpp -lmingw32 -lcrypto -lSDL2main -lSDL2_image -lSDL2_ttf -lSDL2 -static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic -g -pg


run:
	@.\uttt


all: compileconsole run


profile: compileprofile run
	gprof uttt.exe gmon.out > output.txt


distribute: compile
	@rmdir /S /Q dist
	@mkdir dist
	@COPY /Y uttt.exe dist
	@COPY /Y zlib1.dll dist
	@COPY /Y SDL2.dll dist
	@COPY /Y SDL2_ttf.dll dist
	@COPY /Y SDL2_image.dll dist
	@COPY /Y libpng16-16.dll dist
	@COPY /Y LICENSE dist
	@XCOPY /S /I /Y /Q data dist\data


cloc:
	@cloc --exclude-dir=build,src,saves,dist,.vscode . --exclude-lang=Python,Markdown,make,XML,HTML


l2i:
	@pyinstaller -F level2image.py --distpath .
