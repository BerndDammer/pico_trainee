import network
import binascii

# WLAN-Interface initialisieren
wlan = network.WLAN(network.STA_IF)
wlan.active(True)

# Scan durchführen
print("Scanne nach WLAN-Netzwerken...")
networks = wlan.scan()

# Ergebnisse formatieren und ausgeben
# Ein Eintrag enthält: (SSID, BSSID, Kanal, RSSI, Sicherheit, Hidden)
print(f"{'SSID':<20} | {'RSSI':<5} | {'Kanal':<5} | {'Sicherheit'}")
print("-" * 50)

for ap in networks:
    ssid = ap[0].decode('utf-8')
    bssid = binascii.hexlify(ap[1]).decode('utf-8')
    channel = ap[2]
    rssi = ap[3]
    security = ap[4]
    
    print(f"{ssid:<20} | {rssi:<5} | {channel:<5} | {security}")
