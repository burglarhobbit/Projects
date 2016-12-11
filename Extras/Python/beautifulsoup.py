from bs4 import BeautifulSoup as BS
import urllib
import json
import os
import time
import platform

a = 1
b = 400

my_url = 'https://www.instagram.com/p/BJV-kAAhajJ/'

r = urllib.urlopen(my_url).read()

soup = BS(r,"html.parser")
js = soup.find_all('script',type="text/javascript")[4].string
jsonValue = '{%s}' % (js.split('{', 1)[1].rsplit('}', 1)[0],)
value = json.loads(jsonValue)

last_comment = value['entry_data']['PostPage'][0]['media']['comments']['nodes'][-1]['text']
#print last_comment
while True:
	r = urllib.urlopen(my_url).read()

	soup = BS(r,"html.parser")
	js = soup.find_all('script',type="text/javascript")[4].string

	jsonValue = '{%s}' % (js.split('{', 1)[1].rsplit('}', 1)[0],)
	value = json.loads(jsonValue)

	new_comment = value['entry_data']['PostPage'][0]['media']['comments']['nodes'][-1]['text']

	if new_comment != last_comment:
		for i in range(3):
			if platform.system() == 'Linux':
				os.system('play --no-show-progress --null --channels 1 synth %s sine %f' % (a, b))
			elif platform.system() == 'Windows':
				import winsound
				winsound.Beep(b,a*1000)
			time.sleep(1)
		last_comment = new_comment
		try:
			print new_comment
		except Exception as e:
			pass