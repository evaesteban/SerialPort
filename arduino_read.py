import serial
import time
arduino = serial.Serial('COM6', 2000000, timeout=.1)
while True:
	data = arduino.readline()[:-2] 
	if data == b'W':
		print (time.time())
		break