#!/usr/bin/env python
# -*- coding: utf-8 -*-

import pcsclite
import time
import subprocess
import time
from splinter import Browser

browser = Browser('firefox') # création d'une instance du navigateur
#browser.visit('file:///home/pi/Documents/Api-ADE/html/index.html')
browser.visit('http://www-etu-info.iut2.upmf-grenoble.fr/~munierto/Api-ADE/html/') # chargement de la page de connexion

#subprocess.call("sudo usr/local/sbin/pcscd &", shell=True)

context = pcsclite.Context() # connexion au pcsc daemon (pcscd)
readers = context.list_readers() # liste des lecteurs de carte que pcscd reconnait
reader = readers[0]
print "Using reader : ", reader

while True:
	connected = False
	while connected==False:
		try :
			card = context.connect(reader) # tentative de connexion à une carte
			print "Carte connectee" # affichage dans la console pour test
			button = browser.find_by_id('b-connect') # Récupération de l'élément bouton de connexion d'id "b-connect"
			button.click() # Simulation d'un clic sur le bouton
			time.sleep(1) # délai d'attente avant déconnexion de la carte et tentative de lecture d'une autre 
			card.disconnect() # déconnexion de la carte lue
			print "Carte deconnectee" # affichage dans la console pour test
			connected = True
			del card # suppression de la carte
			
		except Exception as e:
			print e # affichage de l'exception dans la console pour test
			time.sleep(0.5) # délai d'attente avant retentative de lecture d'une carte
			connected = False

del context
