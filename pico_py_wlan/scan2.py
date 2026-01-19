# Bibliotheken laden
import network

# Ländereinstellung
network.country('DE')

# Client-Betrieb
wlan = network.WLAN(network.STA_IF)

# WLAN-Interface aktivieren
wlan.active(True)

# WLAN-Verbindung herstellen
wlan.connect('gisnet', 'wilhelmine')

# WLAN-Verbindungsstatus prüfen
import time
print('Warten auf WLAN-Verbindung')
while not wlan.isconnected() and wlan.status() >= 0:
    time.sleep(1)
print('WLAN-Verbindung hergestellt / Status:', wlan.status())


