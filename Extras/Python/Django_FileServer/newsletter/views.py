#from django.core.mail import send_mail
from django.conf import settings
from django.http import HttpResponse, HttpResponseNotFound, StreamingHttpResponse
from django.shortcuts import render
from django.template.defaulttags import register

from .forms import SignUpForm
from .forms import ContactForm
from .forms import Download

from sendfile import sendfile
from wsgiref.util import FileWrapper

# Create your views here.
def home(request):
	title = 'Welcome'
	#if request.user.is_authenticated():
	#	title = "My Title %s" %(request.user)
	#if request.method == "POST":
	#	print request.POST
	form = SignUpForm(request.POST or None)
	
	context = {
		"title": title,
		"form": form,
	}

	if form.is_valid():
		instance = form.save(commit=False)
		
		full_name = form.cleaned_data.get("full_name")
		if not full_name:
			full_name = "New full name"
		instance.full_name = full_name
		instance.save()
	#	print instance.email
	#	form.save()
		print instance.timestamp
		context = {
			"title": "ThankYou",
		}
	return render(request,"base.html",context)

def contact(request):
	form = ContactForm(request.POST or None)
	if form.is_valid():
		for key, value in form.cleaned_data.iteritems():
			print key, value
	context = {
		"form": form,
	}
	
	return render(request,"forms.html",context)
	
def download_file(request):
	form = Download(request.POST or None)
	message = "Welcome"
	context = {
		"title": message,
		"form": form,
	}
	if form.is_valid():
		for key, value in form.cleaned_data.iteritems():
			print key, value
			message = "Error"
			context = {
				"title": message,
			}
		if '\\' in value:
			name = value.split('\\')
		if '/' in value:
			name = value.split('/')
		try:
			return sendfile(request,value,attachment=True,attachment_filename=name[-1])
		except Exception as e:
			return render(request, "mytitle.html",context)

	return render(request,"myform.html",context)

def update_index(request):
	import os 
	#global dirList
	#global fileList
	#global initPath
	dirList = dict()
	fileList = dict()
	initPath = 'E:\\'
	request.session['dirList'] = dirList
	request.session['fileList'] = fileList
	request.session['initPath'] = initPath
	for (path,dir,files) in os.walk('E:\\'):
		if dir!=[]:
			dirList[path] = dir
		if files!=[]:
			fileList[path] = files
	context = {
		"title": "Indexing Successful!"
	}
	#print dirList[initPath]
	return render(request, "mytitle.html",context)

def view_index(request):
	#import os
	#path = request.GET.get('path','')
	#global dirList,fileList,initPath
	
	#for key, value in dirList.iteritems():
	#	print key, value
	#	break
	dirList = request.session.get('dirList')
	fileList = request.session.get('fileList')
	initPath = request.session.get('initPath')
	try: 
		indexDict = {
			"initial": initPath,
			"directory":dirList,
			"file":fileList,
		}
		print dirList[initPath]
	except KeyError as e:
		return HttpResponseNotFound("<h1>Key Error, Index Not Found.</h1>")
	except NameError as e:
		return HttpResponseNotFound("<h1>Error: Index Again</h1>")
	return render(request,"singleindexview.html",indexDict)

def view_real_time_index(request,urlPath=''):
	import os
	from mimetypes import guess_type
	import pythoncom
	import sys
	import win32com.client

	#current_url = request.resolver_match.url_name
	
	initPath = None
	dirList = []
	fileList = []
	indexDict = dict()
	context = dict()
	finalDirPath = None
	name = None # Dir or File name
	base_path = [('C:\\','C:\\'),('E:\\','E:\\')] # (name, path)
	additional_homepage_dirs = ['C:\\Users\\User\\Desktop','C:\\Users\\User\\Downloads']

	# For Buttons
	for key,value in request.GET.iteritems():
		# print key,value
		initPath = key
		name = value
		# key = path
		# value = name
		# Will zip folder contents and download accordingly
		if value == "Download this folder":
			if name != "Home":
				filename = r"%s.zip" % os.path.split(initPath)[-1]
				return zip_file_optimised(initPath, filename)
			else:
				return HttpResponseNotFound("<h1>Sorry, but not possible!</h1>")
		elif key == "search":
			dirList, fileList = lev(value)
		elif initPath.endswith('.lnk'):
			pythoncom.CoInitialize()
			shell = win32com.client.Dispatch("WScript.Shell")
			shortcut = shell.CreateShortCut(initPath)
			initPath = shortcut.Targetpath
			#print "initPath: %s" %initPath
	#print urlPath
	#For Urls
	if urlPath.endswith('.lnk'):
		pythoncom.CoInitialize()
		shell = win32com.client.Dispatch("WScript.Shell")
		shortcut = shell.CreateShortCut(urlPath)
		urlPath = shortcut.Targetpath
	if os.path.isdir(urlPath):
		initPath = urlPath
	elif os.path.isfile(urlPath):
		initPath = urlPath
	
	if (name == "Home") or (initPath is None):
		initPath = "Home"
		templist = []

		#Will index the additional_homepage_dirs into proper format as base_path
		for i in additional_homepage_dirs:
			tempPath,dirName = os.path.split(i)
			templist.append(dirName)

		base_path += zip(templist,additional_homepage_dirs)
		dirList = base_path
		name = ''

#	if settings.DEBUG:
#		print dirList
	finalDirPath = initPath
	path_list = list()
	split_path = list()

	#Will Index the files & dirs into proper format: (name,path)
	if os.path.isdir(initPath):
		print "is Dir"
		for (path,dir,files) in os.walk(finalDirPath):

			if dir!=[]:
				for i in dir:
					dirList.append((i,os.path.join(path,i)))
			if files!=[]:
				for i in files:
					fileList.append((i,os.path.join(path,i)))
			break

	#Will Download the file
	elif os.path.isfile(initPath):
		print "not Dir"
		try:
			return sendfile(request,initPath,attachment=True,attachment_filename=name)
		except Exception as e:
			context["title"]="Error while downloading file, Please try again"
			return render(request, "mytitle.html",context)

	#For breaking the current path into all possible paths for direct menu-browsing
	if os.path.exists(initPath):
		temp_path = finalDirPath
		while 1:
			temp_path,dir_name = os.path.split(temp_path)
			if dir_name!='':
				path_list.append(dir_name)
			else:
				if temp_path!='':
					path_list.append(temp_path)
				path_list.append('Home')
				break
		path_list.reverse()
		split_path = path_list[:]
		for i,j in enumerate(path_list):
			if i>1:
				split_path[i] = os.path.join(split_path[i-1],split_path[i])
	else:
		path_list.append("Home")
		split_path = path_list[:]

#	if settings.DEBUG:
#		print split_path
#		print path_list

	try:
		if dirList != []:
			indexDict["directory"] = dirList
		if fileList != []:
			indexDict["file"] = fileList
		indexDict["initial"] = finalDirPath
		indexDict["pathlist"] = zip(path_list,split_path)
		indexDict["download"] = "Download this folder"
		print indexDict["pathlist"]
		# print fileList[finalDirPath]
	except KeyError as e:
		return HttpResponseNotFound("<h1>Key Error, Index Not Found.</h1>")
	except NameError as e:
		return HttpResponseNotFound("<h1>Error: Index Again</h1>")
	return render(request,"newbuttonform.html",indexDict)

def zip_file(dirpath, zipname):
	import os
	import zipfile
	from cStringIO import StringIO #much faster than the python StringIO

	s = StringIO()
	with zipfile.ZipFile(s, mode="w", compression=zipfile.ZIP_DEFLATED, allowZip64=True) as zf:
		#abs_src = os.path.abspath(dirpath)
		#print "abs_src: %s" % abs_src
		for dirname, subdirs, files in os.walk(dirpath):
			for filename in files:
				absname = os.path.abspath(os.path.join(dirname, filename))
				arcname = absname[len(dirpath) + 1:]
				#print "absname:%s arcname:%s" % (absname,arcname)
				#print 'zipping %s as %s' % (os.path.join(dirname, filename),arcname)
				zf.write(absname, arcname)
	#zf.close()
	resp = HttpResponse(s.getvalue(),content_type="application/x-zip-compressed")
	resp['Content-Disposition'] = 'attachment; filename=%s' % zipname
	resp['Content-Length'] = s.tell()
	return resp
	#return sendfile(request,s.getvalue(),attachment=True,attachment_filename=zipname)

def zip_file_optimised(dirpath, zipname):
	import mimetypes
	import os
	import tempfile
	import zipfile

	temp = tempfile.TemporaryFile()
	with zipfile.ZipFile(temp, mode="w", compression=zipfile.ZIP_DEFLATED, allowZip64=True) as zf:
		#abs_src = os.path.abspath(dirpath)
		#print "abs_src: %s" % abs_src
		for dirname, subdirs, files in os.walk(dirpath):
			for filename in files:
				absname = os.path.abspath(os.path.join(dirname, filename))
				arcname = absname[len(dirpath) + 1:]
				#print "absname:%s arcname:%s" % (absname,arcname)
				#print 'zipping %s as %s' % (os.path.join(dirname, filename),arcname)
				zf.write(absname, arcname)
	#zf.close()
	wrapper = FileWrapper(temp)
	resp = StreamingHttpResponse(wrapper,content_type="application/x-zip-compressed")
	resp['Content-Disposition'] = 'attachment; filename=%s' % zipname
	resp['Content-Length'] = temp.tell()
	temp.seek(0)
	return resp

def levenshtein(index, query, preferredCost):	
	import os

	from levenshtein import levSearch

	tfile = []
	tdir = []
	for path, dir, file in index:
		if dir!=[]:
			for i in dir:
				cost = levSearch(query, i, preferredCost)
				if cost<preferredCost and cost>=0:
					tdir.append((i,path))
		if file!=[]:
			for i in file:
				cost = levSearch(query, i, preferredCost)
				if cost<preferredCost and cost>=0:
					tfile.append((i,path))
	return tdir,tfile

def lev(query):
	import cPickle as pickle
	import os, time
	
	preferredCost = 5
	x = time.time()
	list = []
	drivePath = ['E:\\']
	for i in drivePath:
		for (path,dir,files) in os.walk(i):
			if(dir!=[] or files!=[]):
				list.append((path, dir, files))

	y = time.time()
	print y-x

	output = open("list", "wb")
	pickle.dump(list, output, pickle.HIGHEST_PROTOCOL) #-1 = pickle.HIGHEST_PROTOCOL
	output.close()
	
	input = open("list", "rb")
	obj = pickle.load(input)
	input.close()
	
	dirList, fileList = levenshtein(obj, query, preferredCost)
	z = time.time()
	print z-y
	return dirList, fileList

@register.filter
def get_item(dictionary, key):
    return dictionary.get(key)

@register.filter
def addstr(arg1, arg2):
	import os
	#print os.path.join(arg1,arg2)
	return os.path.join(arg1,arg2)