import bluetooth
import sys
import speech_recognition as sr #Speech Recognition
import pyttsx #Text to speech

# Global Variables
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

target_name = "HC-05"
target_address = None

nearby_devices = bluetooth.discover_devices()
#print nearby_devices


for bdaddr in nearby_devices:
#	print bluetooth.lookup_name( bdaddr )
	if target_name == bluetooth.lookup_name( bdaddr ):
		target_address = bdaddr
		break

# Function to recognize audio
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

	if target_address is not None:
		#Instantiating bluetooth socket
		sock=bluetooth.BluetoothSocket(bluetooth.RFCOMM)
		print ">>Trying connection to HC-05..."
		port = 1
		
		#Connecting to the predefined Bluetooth Address of our HC-05 module
		sock.connect(('98:D3:32:20:50:9F', port))
		print ">>CONNECTED!"
#	print "found target bluetooth device with address ", target_address

	rec = ''
	flag = True #Flag for re-asking the user whether to continue or not
	rec = recognize_audio(say)
	while 1:
		if rec!=notUnderstood and flag:
			#rec = 'hello'
			rec = rec.encode('utf-8') #encoding in 8-bit character format

			if target_address is not None:
				print ">>Trying sending: %s" % rec
				sock.send(rec + '\r') #Sending recognized string over bluetooth
				print ">>Finished sending!"
		
		rec1 = recognize_audio(continueOrNot)

		if rec1 == "no": #Exit condition
			if target_address is not None:
				sock.close()
			exit()
		elif rec1 == "yes": #Continue condition
			rec = recognize_audio(say)
			flag = True
		else:
			flag = False