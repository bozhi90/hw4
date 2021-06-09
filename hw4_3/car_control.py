import pyb, sensor, image, time, math

RED_LED_PIN = 1
BLUE_LED_PIN = 3
pyb.LED(RED_LED_PIN).on()

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QQVGA) # we run out of memory if the resolution is much bigger...
sensor.skip_frames(time = 2000)
sensor.set_auto_gain(False)  # must turn this off to prevent image washout...
sensor.set_auto_whitebal(False)  # must turn this off to prevent image washout...
clock = time.clock()

f_x = (2.8 / 3.984) * 160 # find_apriltags defaults to this if not set
f_y = (2.8 / 2.952) * 120 # find_apriltags defaults to this if not set
c_x = 160 * 0.5 # find_apriltags defaults to this if not set (the image.w * 0.5)
c_y = 120 * 0.5 # find_apriltags defaults to this if not set (the image.h * 0.5)

def degrees(radians):
   return (180 * radians) / math.pi

uart = pyb.UART(3,9600,timeout_char=1000)
uart.init(9600,bits=8,parity = None, stop=1, timeout_char=1000)

while(True):
   clock.tick()
   img = sensor.snapshot()
   pyb.LED(BLUE_LED_PIN).off()
   pyb.LED(RED_LED_PIN).on()
   for tag in img.find_apriltags(fx=f_x, fy=f_y, cx=c_x, cy=c_y): # defaults to TAG36H11
      img.draw_rectangle(tag.rect(), color = (255, 0, 0))
      img.draw_cross(tag.cx(), tag.cy(), color = (0, 255, 0))

      printx = tag.x_translation()
      if printx > 0.5:
        print(1)
        uart.write(("1").encode())
      elif printx < -0.5:
        print(-1)
        uart.write(("2").encode())
      else :
        print(0)
        uart.write(("0").encode())

      #print(tag.x_translation())
      pyb.LED(RED_LED_PIN).off()
      pyb.LED(BLUE_LED_PIN).on()
