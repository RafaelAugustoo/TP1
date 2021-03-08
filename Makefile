
install-libs:
	@echo "Instalando bibliotecas utilizadas no game..."
	sudo apt-get install libsdl1.2-dev libsdl-mixer1.2-dev libsdl-ttf2.0-dev 
	sudo apt-get install freeglut3-dev
	sudo apt-get install libglew-dev
	sudo apt-get install libsoil-dev


all:
	@echo "Compilando o jogo..."
	gcc retro.c -lglut -lGLU -lGL -lGLEW -lSOIL -lSDL -lSDLmain -lSDL_mixer


run:
	@echo "Executando Retro Universe"
	./a.out

clean:
	@echo "Obrigado por jogar Retro Universe"
	@echo "Removendo..."
	rm ./a.out
