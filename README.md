# Exercício 5 AVA

Servidor HTTP com ESP32

Implementações para atividade avaliativa da UNISAL

# Funcionamento

Em complemento ao que foi realizado em sala de aula como última atividade prática da disciplina de IoT, IA e Robótica, vamos aprimorar o exemplo que desenvolvemos para interface HTTP com o ESP32. Neste exemplo, elaboramos as bases para trabalhar com requisições GET e POST a fim de permitir a criação de uma API para controle de dispositivos pela internet.

Dito isso, modifique o projeto de referência para a execução das seguintes funcionalidades:
•	Conecte um LED a um GPIO de sua preferência. Tanto faz a cor do LED e o GPIO es-colhido. Esse LED será controlado por uma requisição POST, e terá seu status reportado por requisições GET, descritas adiante.
•	POST: Use o endpoint /led_control para receber requisições POST com o seguinte cor-po de mensagem:
{
    "led_cmd": 0 ou 1, sendo que 0 desliga o LED, e 1 liga o LED.
}

Sobre o POST, mesmo que não vá ser devolvido um retorno à requisição, é importante retornar HTTP 200, mesmo que seja uma resposta vazia.

•	GET: Use o endpoint /led_status para responder a requisições GET, devolvendo o se-guinte corpo de mensagem:
{
    "led_status": 0 ou 1, sendo 0 p/ LED desligado, e 1 p/ LED ligado.
}

