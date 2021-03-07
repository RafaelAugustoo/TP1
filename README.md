# **CG-TP1**

##### **Trabalho de Computação Gráfica do Segundo Período de Engenharia de Computação do CEFET-MG**
---
>## Integrantes do grupo:
>####	Fernando Lorenzeto Rodrigues
>####	Rafael Assunção De Ladeira Torga
>####	Rafael Augusto de Souza
---
# Instruções para compilação e execução:

>## Importante ressaltar que é necessário verificar se seus programas seguem atualizados antes de prosseguir:
>	sudo apt-get update
>	sudo apt-get upgrade

## Bibliotecas utilizadas no desenvolvimento do Jogo

**As bibliotecas podem ser instaladas via comando _make install-libs__**

### SDL e SDL_mixer: (para músicas apenas)
>	sudo apt-get install libsdl1.2-dev libsdl-mixer1.2-dev libsdl-ttf2.0-dev 
### FreeGlut
>	sudo apt-get install freeglut3-dev
### Glew
>	sudo apt-get install libglew-dev
### SOIL: 
>	sudo apt-get install libsoil-dev

## Execução
>### Existem dois modos:
> Via Terminal manualmente onde você irá utilzar o comando **cd/diretório** e vai colocar o diretório/pasta onde foi baixado o jogo. **Ex: cd 'Área de Trabalho'/tp1**
> Após isso você deve digitar o comando gcc main.c -lglut -lGLU -lGL -lGLEW -lSOIL -lSDL -lSDLmain -lSDL_mixer para compilar o programa
e em seguida ./a.out para executar.

>### O outro modo é via terminal porém utilzando os comandos make:
### Instala todas as bibliotecas necessárias.
>	make install-libs 
### Compila o Game
>	make all
### Executa o Game
>	make run
### Limpa o arquivo compilado
>	make clean
---
# Lista de itens adicionais implementados:

	Relativas a texturas e gráficos:

        	1-Texturas animadas (até 12%): crie animações para as texturas dos
			inimigos e personagem. #dikentinha: busque por spritesheet no Google.

        	2-Jogador parado ou andando (2%)

        	3-Inimigos andando (2%)

        	4-Outros objetos ou cenário animados (2%)

		5-Fundo lindão e com movimento/animação (até 6%): em vez de um fundo com uma simples 
		cor, colocar imagens ou até umas estrelinhas piscando aleatoriamente.


	Relativas ao jogo:

		1.Fases infinitas (4%): gere uma nova fase toda vez que o jogador morrer ou que ele tenha êxito em aterrisar

	Relativas à nave:

        	1-Vidas (5%): em vez de morrer na primeira colisão, o jogador pode ter vidas
        		Deve haver uma head-up display (HUD) mostrando a vida atual do jogador.

        	2-Controle via mouse (5%): além do controle via teclado, torne possível jogar com o mouse.



	Outras funcionalidades:

         	1-Sons (até 12%). Colocar efeitos sonoros e música de fundo no seu jogo
		

         	2-Telas (até 12%): faça um jogo completo, ou seja, implemente telas de splash screen,
			menu inicial, instruções, créditos, opções, game over, etc
			
>### **__Algumas telas do Game Contem Easter Eggs, divirta-se.__**
			
>#####	*As imagens e sons utilzados no game podem ser encontradas nesses sites: (Tudo com free download).
>#####	Músicas e efeitos sonoros: https://mixkit.co/free-sound-effects
>#####	Imagens: https://www.wallpaperflare.com/music-stars-background-80s-neon-80-s-synth-retrowave-wallpaper-bdlfo

# **VIDEO**
>Link do vídeo https://youtu.be/LqFkw0xMnuM
[![IMAGE ALT TEXT HERE](http://img.youtube.com/vi/LqFkw0xMnuM/0.jpg)](https://youtu.be/LqFkw0xMnuM)

