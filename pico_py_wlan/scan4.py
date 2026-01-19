import network
import time

# WLAN-Schnittstelle initialisieren
wlan = network.WLAN(network.STA_IF) # STA_IF für Station (Client) Modus
wlan.active(True) # WLAN aktivieren

print("Suche nach WLAN-Netzwerken...")
# WLANs scannen und Liste der Netzwerke (SSIDs) erhalten
networks = wlan.scan()

if networks:
    print(f"Gefundene Netzwerke ({len(networks)}):")
    for net in networks:
        ssid = net[0].decode('utf-8') # SSID dekodieren
        # Signalstärke in dBm, kann als negative Zahl interpretiert werden
        rssi = net[3]
        print(f"  - {ssid} (Signal: {rssi} dBm)")
else:
    print("Keine WLAN-Netzwerke gefunden.")

# Optional: WLAN wieder deaktivieren, um Strom zu sparen
# wlan.active(False)
