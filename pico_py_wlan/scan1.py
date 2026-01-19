# Bibliotheken laden
import network

# Client-Betrieb
wlan = network.WLAN(network.STA_IF)

# WLAN-Interface aktivieren
wlan.active(True)

# WLANs ausgeben
print(wlan.scan())
