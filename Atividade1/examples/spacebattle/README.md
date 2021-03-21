# Computação Gráfica - Atividade 1

Repositório da aplicação 2D interativa desenvolvida para a disciplina de Computação Gráfica da UFABC (QS 2021.1).

Aluno: Lucas Fernandes Muniz

RA: 11201720792

##  *Space Battle*

O *Space Battle*, inspirado no clássico *Space Invaders*, é um jogo cujo o objetivo é eliminar a horda de inimigos em frente a nave do jogador. O jogador controla uma nave que é capaz de atirar em direção ao inimigo e de ativar um escudo (campo de força) capaz de proteger a nave contra a ofensiva inimiga. A horda de inimigos ficará na parte superior da janela da aplicação, enquanto que o jogador estará na parte inferior. Nesse jogo, os inimigos sempre estarão mirando no jogador e atirarão conjuntamente em direção ao jogador em intervalos de 1 segundo. Cada inimigo possui uma certa quantidade de vida, sendo que a coloração da nave inimiga muda de acordo com sua vida. O inimigo possuirá a cor vermelha caso tenha menos de 25% de vida, laranja se tem mais de 25% e menos que 50%, amarelo se tem mais que 50% e menos de 75% e verde se tem mais que 75%. Quando uma horda de inimigos for exterminada, o jogo reiniciará (após 5 segundos) e os inimigos da nova horda terão uma quantidade de vida maior do que os inimigos da horda anterior.


### Instruções de uso

O jogo possui os seguintes comandos:

* Movimentação: a nave do jogador pode se mover para a esquerda e para a direita. Para mover para a esquerda, é necessário pressionar a tecla <kbd>A</kbd> ou a tecla direcional esquerda <kbd>←</kbd>. Para mover para a direita, é necessário pressionar o botão <kbd>D</kbd> ou a tecla direcional direita <kbd>→</kbd>. 

* Tiros: para atirar, basta pressionar a tecla <kbd>Space</kbd> ou o botão direito do mouse. Após o lançamento de um tiro, haverá um intervalo de 250 ms para que o próximo tiro possa ser lançado.

* Campo de força: ativado ao pressionar a tecla <kbd>Ctrl</kbd> ou o botão esquerdo do mouse. O campo de forção durará 3 segundos, e só poderá ser reativado após 10 segundos da última ativação.

O jogo *Space Battle* pode ser acessado nesta [página](https://lucas-muniz.github.io/ComputacaoGrafica-abcgapps/spacebattle/).
