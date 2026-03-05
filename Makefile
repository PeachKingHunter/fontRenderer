all: run

libFolder = ./libSrc/
fontSrc= $(libFolder)fontManager/
sub=$(fontSrc)FileReader.c $(fontSrc)FontLoader.c $(fontSrc)FontRender.c $(libFolder)Frame.c $(libFolder)TextLabel.c $(libFolder)Button.c $(libFolder)List.c $(libFolder)mainUOG.c $(libFolder)ImageLabel.c

./lib/libUOG.a: $(sub)
	gcc -fsanitize=leak -Wall -lglfw -lGL -c $(sub)
	mkdir tmp
	mv *.o tmp
	# gcc -shared ./tmp/*.o -o lib.shared
	ar rcs libUOG.a ./tmp/*.o
	rm tmp -r
	cp ./libSrc/*.h ./lib/
	cp ./libSrc/fontManager/*.h ./lib/fontManager/
	mv libUOG.a ./lib/

main: ./src/main.c ./lib/libUOG.a
	gcc -Wall ./src/main.c ./lib/libUOG.a -lglfw -lGL -o main

run: main
	LSAN_OPTIONS=suppressions=leak.sup ./main

