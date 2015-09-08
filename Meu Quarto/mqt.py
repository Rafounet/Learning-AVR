# -*- coding: utf-8 -*-
#	Esse programa é o protótipo do protótipo.
#	A versão futura vai rodar em um ARM sem precisar de nenhum outro microcontrolador.
#	E tem muita coisa gambiarrada no código.
#	Pedro Vincius - 09/2015
#

import tweepy, time, sys, serial

# Autorizações para o Tweepy.
# Insira as suas aqui.
consumer_key=""
consumer_secret=""

access_token=""
access_token_secret=""

auth = tweepy.OAuthHandler(consumer_key, consumer_secret)
auth.secure = True
auth.set_access_token(access_token, access_token_secret)

api = tweepy.API(auth)

# Mostra o nome da conta do Twitter conectado.
print(api.me().name)

# Inicia a comunicação serial.
# Troque para a porta e velocidade em que o AVR está trabalhando.
comport = serial.Serial('/dev/ttyACM0', 9600)
time.sleep(1) # Coloquei esse delay por precaução. Não sei se é necessário.

adch = '' # Variavel que vai receber os 'high' bits do ADC.
adcl = '' # Variavel que vai receber os 'low' bits do ADC.
i = 1 # Será usada para ver por quantas leituras seguidas e iguais de temperatura tivemos.
		# Também evita que Tweets iguais sejam enviados.
prev_c = 0.000 # Para comparar os valores de temperatura.

frases = open('frases.txt', 'r') # Abre o seu arquivo com as falas.

print ('OK!') # Um OK.

# Aqui eu acho que começa a gambiarra. Preciso melhorar essa comunicação.
# Está assim só para testes
while 1:
	a = comport.readline(); # Lê a linha na serial.
	if a != '': # Garante que não é 'nada'.
		# A gambiarra começou.
		# Lembrando que o AVR está enviando os dados da seguinte maneira e ordem:
		# 1 caractere para os 'high' bits
		# 1 caractere para os 'low' bits
		# Quebra a linha.
		adch = a[0]  #Aqui eu leio o caracter equivalente aos 2 'high' bits
		adcl = a[1]  #Aqui eu leio o caracter equivalente aos 8 'low' bits
		adch = ord(adch) # Vejo o valor equivalente ao 1° caractere.
		adcl = ord(adcl) # O mesmo para o 2°.
		adch = adch << 8 # Desloco os 'high' bits para o devido lugar.
	
		adcr = adch + adcl # Somo para ter um valor de 10 bits.
		
		c = adcr * 0.4887 # Multiplico lido pelo ADC pela constante do LM35 para ter a 
								# temperatura em °C
		print(adcr) # Um print para saber o valor do ADC.
		print(c) # E a temperatura em °C
		if i == 1: # Para executar somente uma vez.
			prev_c = c # prev_c é a variável que vai receber o valor anterior de temperatura
						  # para comparar depois.
			print('teste') # Dá um teste pra ver se está tudo OK.
			frase = frases.readline() # Lê a primeira linha do aquivo que contém as frases.
			api.update_status(status=frase) # Posta no Twitter.
			print (frase) # Mostra a frase no terminal.
		i += 1 # Incrementa 1 a variável.
		
		if (i > 2): # Aqui o programa segue...
			frase = frases.readline()
			api.update_status(status=frase)
			print (frase)
			if prev_c != c: # Só posta a temperatura se ela for diferente da anterior.
				print('Tweet Postado')
				api.update_status(status='A temperatura do quarto é: %s' %(c)+'°C .' + '- %s' %(i))
				prev_c = c # Armazena o atual valor de temperatura para comparar depois.
	
	
	
	
	
	
	
	
	
