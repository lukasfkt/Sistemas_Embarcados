<h1>Perguntas Lab4</h1>

Quais são as configurações para:
<ol>
<li><b>– OS_TICK_FREQ? </li></b> Resposta: 1000
<li><b>– OS_THREAD_NUM?</li></b> Resposta: 1
<li><b>– OS_THREAD_DEF_STACK_NUM?</li></b> Resposta: 1
</ol>

<b>Qual é o efeito dessa alteração na execução do programa do projeto “tarefas”? </b></br>
Com a diminuição da TICK_FREQ, os LEDS acendem e apagam com um tempo maior (Troca de estados dos LEDS ocorrem de forma mais lenta).

<b>Quais são os períodos de ativação das tarefas thread1 e thread2?</b></br>
Thread1 = 100*(1/TICK_FREQ) + Tempo do job.</br>
Thread2 = 100*(1/TICK_FREQ).
