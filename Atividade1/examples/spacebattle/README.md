# Computação Gráfica - Atividade 1

Repositório da aplicação 2D interativa desenvolvida para a disciplina de Computação Gráfica da UFABC (QS 2021.1).

Aluno: Lucas Fernandes Muniz
RA: 11201720792

##  *Space Battle*

O jogo *Space Battle* é um jogo inspirado no clássico jogo *Space Invaders*, cujo o objetivo é eliminar a horda de inimigos à frente da nave do jogador. O jogador controla uma nave que é capaz de atirar em direção ao inimigo e de ativar um escudo (campo de força), capaz de proteger sua nave contra a ofensiva inimiga. A horda de inimigos ficará na parte superior da janela, enquanto que o jogador estará na parte inferior. Nesse jogo, os inimigos sempre estarão mirando no jogador e atirarão conjuntamente em direção ao jogador a cada 1,5 segundos. Cada inimigo possui uma certa quantidade de vida, sendo que a coloração da nave inimiga muda de acordo com sua quantidade de vida. O inimigo possuirá a cor vermelho caso tenha menos de $25\%$ de vida, laranja se tem menos que $50\%$, amarelo se tem menos de $75\%$ e verde se tem mais que $75\%$. Quando uma horda de inimigos for exterminada, o jogo reiniciará e os inimigos da nova horda terão uma vida maior que os inimigos da horda anterior.


## Instruções de uso

O jogo possui os seguintes comandos:

* Movimentação: a nave do jogador pode se mover para a esquerda e para a direita. Para mover para a esquerda, é necessário pressionar o botão <kbd>A</kbd> ou a tecla direcional esquerda <kbd>←</kbd>. Para mover para a direita, é necessário pressionar o botão <kbd>D</kbd> ou a tecla direcional direita <kbd>→</kbd>. 

* Tiros: para atirar, basta pressionar a tecla <kbd>Space</kbd> ou o botão direito do mouse. Entre cada tiro efeutuado, haverá um intervalo de 250 ms para que o próximo tiro possa ser lançado.

* Campo de força: ativado ao pressionar a tecla <kbd>Ctrl</kbd>, <kbd>F</kbd> ou o botão esquerdo do mouse. O campo de forção durará 3 segundos, e só poderá ser reativado após 5 segundos.

O jogo *Space Battle* pode ser acessado nesta [página](https://lucas-muniz.github.io/ComputacaoGrafica-abcgapps/spacebattle/).
