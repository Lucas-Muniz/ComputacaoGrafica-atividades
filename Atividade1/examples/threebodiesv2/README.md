# Computação Gráfica - Atividade 3

Repositório da aplicação 3D interativa com iluminação e texturização, desenvolvida para a disciplina de Computação Gráfica da UFABC (QS 2021.1).

Aluno: Lucas Fernandes Muniz
RA: 11201720792

##  *Visualizador de três corpos - Versão 2* 

Esta aplicação 3D, inspirada no clássico [problema dos três corpos](https://pt.wikipedia.org/wiki/Problema_dos_tr%C3%AAs_corpos), tem o objetivo de promover uma visualização tridimensional das órbitas de três corpos que estão sujeitos às atrações gravitacionais entre eles. O problema dos três corpos é conhecido por ser um sistema caótico que não possui solução analítica, sendo necessário o uso de métodos numéricos para o cálculo do movimento dos corpos. Essa aplicação permite que o usuário movimente a câmera pelo cenário, e também permite a modificação das massas de cada um dos corpos, possibilitando a visualização da mudança das órbitas gerada pela alteração da massa. O usuário pode escolher entre os *shaders* **texture** e **blinnphong**. O primeiro apresenta corpos com texturizações do Sol, da Terra e de Júpiter, sendo a fonte de luz representada pelo corpo texturizado como o Sol. Já o segundo mostra os corpos sem texturização, mas com iluminação, sendo que neste *shader* a fonte de luz pode ser movimentada pelo usuário. Em ambos os *shaders*, utilizou-se o modelo de Blinn–Phong com sombreamento de Phong. O usuário também pode escolher duas configurações de corpos:

* *8 Figure*: configuração baseada em uma solução estável do problema de três corpos. As órbitas dos corpos formam a figura de um número 8 deitado. 

* *Chaotic*: configuração que inicia em um movimento estável, mas que adquire uma movimentação caótica rapidamente.

## Instruções de uso

A aplicação possui os seguintes movimentos de câmera:

* *Dolly*: as setas pra cima/pra baixo (botões <kbd>↑</kbd>/<kbd>↓</kbd>) permitem movimentar a câmera pra frente e pra trás ao longo da direção de visão, respectivamente.

* *Truck*: as setas pra esquerda/pra direita (botões <kbd>←</kbd>/<kbd>→</kbd>)  permitem movimentar a câmera pra esquerda e pra direita, respectivamente.

* *Pan*: as teclas <kbd>A</kbd> e <kbd>D</kbd> fazem a câmera girar em torno de seu eixo **y**.

* *Tilt*: as teclas <kbd>W</kbd> e <kbd>S</kbd> fazem a câmera girar em torno de seu eixo **x**.

A aplicação também possui os controles de simulação:

* *Shader*: o *combo box* <kbd>Shader</kbd> permite escolher o shader que o usuário deseja utilizar na geração da imagem.

* Sistema: o *combo box* <kbd>System</kbd> permite escolher a configuração de corpos que o usuário quer visualizar (*8 Figure* ou *Chaotic*).

* Modificação das massas: os *sliders* permitem a modificação de cada um dos corpos. A massa inicial de todos os corpos é 1, mas pode ser modificada em valores de 0 a 10. Ao modificar a massa, é possível observar a mudança no tamanho do corpo.

* Posição inicial da câmera: o botão <kbd>Initial Position</kbd> permite retorna a câmera à posição inicial. 

* Reiniciar: o botão <kbd>Restart</kbd> permite reiniciar a simulação da movimentação dos corpos. 

Se o usuário estiver utilzando o shader **blinnphong**, é possível movimentar a fonte de luz:

* Movimentação da luz: basta mover o mouse com o botão esquerdo pressionado.


### Informações adicionais

1. A configuração *8 Figure* é uma solução estável do problema dos três corpos, mas devido ao acúmulo de erros do método numérico utilizado nessa aplicação, as órbitas dos corpos perdem a estabilidade com o tempo, podendo adquirir uma movimentação caótica.

2. O método numérico utilizado para a resolução da equação diferecial ordinária que define o movimento dos corpos foi baseado no [método de Runge–Kutta](https://pt.wikipedia.org/wiki/M%C3%A9todo_de_Runge-Kutta).

3. Não foi implementado a detecção de colisão dos corpos, logo, um corpo pode acabar "entrando" no outro.

A aplicação *Three Bodies Viewer (Version 2)* pode ser acessada nesta [página](https://lucas-muniz.github.io/ComputacaoGrafica-abcgapps/threebodiesv2/).
