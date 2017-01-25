#!/usr/bin/env python
# -*- coding: utf-8 -*-

# Le Raspbery Pi communique avec l'Arduino

import serial  # bibliothèque permettant la communication série
import time    # pour le délai d'attente entre les messages
import subprocess # module permettant le lancement de nouveaux processus

ser = serial.Serial('/dev/ttyACM0', 9600) # définit le nom du port USB occupé par l'Arduino et le taux de données transmises en bit par seconde

#subprocess.call("sudo -E firefox-esr &", shell=True)

# boucle répétée jusqu'à l'interruption du programme
while True:
	# fonction bloquante de lecture d'un message de l'Arduino
	presence = ser.read()
	# si l'Arduino a détécté une présence devant la borne
	if (presence=="1"):
		# appel système de reveil de l'économiseur d'écran 
		subprocess.call(["xset", "s", "reset"])
