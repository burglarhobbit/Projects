import os
import zipfile

def zip(src, dst):
	zf = zipfile.ZipFile("%s.zip" % (dst), "w", zipfile.ZIP_DEFLATED)
	abs_src = os.path.abspath(src)
	print "abs_src: %s" % abs_src
	for dirname, subdirs, files in os.walk(src):
		for filename in files:
			absname = os.path.abspath(os.path.join(dirname, filename))
			arcname = absname[len(abs_src) + 1:]
			print "absname:%s arcname:%s" % (absname,arcname)
			print 'zipping %s as %s' % (os.path.join(dirname, filename),arcname)
			zf.write(absname, arcname)
	zf.close()

zip("E:/Books", "aangi")