from bibliopixel import LEDStrip
from bibliopixel.drivers.serial_driver import DriverSerial, LEDTYPE, ChannelOrder
from BiblioPixelAnimations.matrix.bloom import Bloom
import bibliopixel.colors as colors
import bibliopixel.log as log
import time

log.setLogLevel(log.DEBUG)

driver = DriverSerial(LEDTYPE.WS2812B, 42, dev="", c_order=ChannelOrder.GRB)
led = LEDStrip(driver, threadedUpdate=False, masterBrightness=255)
led.setMasterBrightness(32)
try:
    while True:
        led.fill(colors.Red)
        led.update()
        time.sleep(1)
        led.fill(colors.Green)
        led.update()
        time.sleep(1)
        led.fill(colors.Blue)
        led.update()
        time.sleep(1)
        led.fill(colors.Black)
        led.update()
        time.sleep(1)
except:
    led.all_off()
    led.update()
    time.sleep(1)
