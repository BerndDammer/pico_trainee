import network

CYW43_TRACE_ASYNC_EV = const(0x0001)
CYW43_TRACE_ETH_TX = const(0x0002)
CYW43_TRACE_ETH_RX = const(0x0004)
CYW43_TRACE_ETH_FULL = const(0x0008)
CYW43_TRACE_MAC = const(0x0010)
x = CYW43_TRACE_ETH_RX | CYW43_TRACE_ETH_FULL

wlan = network.WLAN(network.STA_IF)
wlan.config(trace=x)
