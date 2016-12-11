import time
import serial
import sys
import glob
import speech_recognition as sr #Speech Recognition
import pyttsx #Text to speech

googleRec = ">>Google Speech Recognition thinks you said: "
say = ">>Say Something"
notUnderstood = ">>Google Speech Recognition could not understand what you said."
continueOrNot = ">>Would you like to continue or not? yes or no."

#Text-to-Speech
engine = pyttsx.init()
rate = engine.getProperty('rate') #default = 200
engine.setProperty('rate', rate-20)
voices = engine.getProperty('voices')
engine.setProperty('voice', voices[1].id)
	
# configure the serial connections (the parameters differs on the device you are connecting to)
def serial_ports():
	""" Lists serial port names

		:raises EnvironmentError:
			On unsupported or unknown platforms
		:returns:
			A list of the serial ports available on the system
	"""
	if sys.platform.startswith('win'):
		ports = ['COM%s' % (i + 1) for i in range(256)]
	elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
		# this excludes your current terminal "/dev/tty"
		ports = glob.glob('/dev/tty[A-Za-z]*')
	elif sys.platform.startswith('darwin'):
		ports = glob.glob('/dev/tty.*')
	else:
		raise EnvironmentError('Unsupported platform')

	result = []
	for port in ports:
		try:
			s = serial.Serial(port)
			s.close()
			result.append(port)
		except (OSError, serial.SerialException):
			pass
	return result

def recognize_audio(printString):
	r = sr.Recognizer()
	with sr.Microphone() as source:
			r.adjust_for_ambient_noise(source)
			engine.say(printString)
			engine.runAndWait()
			print printString
			
			audio = r.listen(source)
	try:
		rec = r.recognize_google(audio,None,"en-IN",False)
		print googleRec + rec
		return rec
		#print rec
	except sr.UnknownValueError:
		print notUnderstood
		return notUnderstood
	except sr.RequestError as e:
		print "Could not request results from Google Speech Recognition Service; {0}".format(e)
		return notUnderstood
	
if __name__ == '__main__':
	ports = serial_ports()
	if ports!=[]:
		ser = serial.Serial(
			port=ports[0],
			baudrate=9600,
			parity=serial.PARITY_NONE,
			stopbits=serial.STOPBITS_ONE,
			bytesize=serial.EIGHTBITS
		)
		#RxTx
		ser.isOpen()
		ser.reset_output_buffer()
		ser.reset_input_buffer()
		print ports
	else:
		ser = None
		print "No COM Ports available."
	out = ''
	rec = ''
	rec = recognize_audio(say)
	while 1:
		if rec!=notUnderstood:
			rec = 'hello'
			rec = rec.encode('utf-8')

			if ser is not None:
				print "\n>>Transmitting %s..." %rec
				ser.write(rec + '\r')
				print ">>Transmit Complete!"
				time.sleep(1)
				out = ''
				print ">>Receiving..."
				while ser.inWaiting() > 0:
					out += ser.read(1) #1 = no. of bytes
				if out != '':
					print ">>UART Received String: " + out
		
		rec1 = recognize_audio(continueOrNot)
		
		if rec1 == "no":
			if ser is not None:
				ser.close()
			exit()
		elif rec1 == "yes":
			rec = recognize_audio(say)