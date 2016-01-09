from bibliopixel import LEDMatrix
from bibliopixel.drivers.serial_driver import DriverSerial, LEDTYPE, ChannelOrder
from BiblioPixelAnimations.matrix.bloom import Bloom
from BiblioPixelAnimations.matrix.Text import ScrollText

import bibliopixel.colors as colors
import bibliopixel.log as log
import time

log.setLogLevel(log.DEBUG)

w = NUM_LEDS_PER_STRIP = 64
h = NUM_STRIPS = 16

driver = DriverSerial(LEDTYPE.GENERIC, NUM_LEDS_PER_STRIP*NUM_STRIPS, dev="", c_order=ChannelOrder.RGB,  hardwareID = "16C0:0483")
led = LEDMatrix(driver, width=NUM_LEDS_PER_STRIP, height=NUM_STRIPS, serpentine=False, threadedUpdate=False, masterBrightness=255)
led.setMasterBrightness(32)
testcolors = [colors.Red, colors.Green, colors.Blue, colors.White, colors.Off]
try:
    print "test"
    # anim = ScrollText(led, "WyoManiacal", xPos=NUM_LEDS_PER_STRIP/2, yPos=0, color=colors.White)
    while True:
        # anim = Bloom(led, dir=True)
        # anim.run(fps=30, amt=6)#, max_steps =100)
        # led.setTexture([[colors.hue_helper(y, h, 0)]*w for y in range(h)])
        # anim = ScrollText(led, "WyoManiacal", xPos=NUM_LEDS_PER_STRIP/2, yPos=0, color=None, size=2)
        # anim.run(fps=15, amt=1, untilComplete = True)
        for c in testcolors:
            led.fillScreen(c)
            led.update()
            time.sleep(1)

    print "done"
except KeyboardInterrupt, e:
    print e
    print "exce"
    led.all_off()
    led.update()
    time.sleep(1)

print "all off"
led.all_off()
led.update()
time.sleep(1)
