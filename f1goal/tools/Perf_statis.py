#-*- encoding = utf-8 -*-
import sys


def fetch_data(filelist):
	mods = { }

	for file_name in open(filelist):
		for line in open(file_name.strip('\n')):
			segs = line.strip("\n").split("|")
			if len(segs) < 5:
				continue
			mod_name = segs[2]
			if mod_name not in mods:
				mods[mod_name] = []
			tb = segs[3].split(".")
			te = segs[4].split(".")
			sec = int(te[0]) - int(tb[0])
			usec = int(te[1]) - int(tb[1])
			if usec < 0:
				usec += 1000000
				sec -= 1
			mods[mod_name].append((sec, usec))
	return mods

def statis(mods_data):
	statis_data = {}
	aver_usecs = 0
	num = 0

	for mod in mods_data:
		# aver, min, max, times
		sec = 0
		usec = 0
		min_usecs = 0
		max_usecs = 0
		statis_data[mod] = [0,0.0,0,0]
		num = len(mods_data[mod])
		for i in range(0, num):
			_sec = mods_data[mod][i][0]
			_usec = mods_data[mod][i][1]
			__usecs = _sec*1000000 + _usec
			if min_usecs > __usecs:
				min_usecs = __usecs
			if max_usecs < __usecs:
				max_usecs = __usecs
			sec += _sec
			usec += _usec
			if usec >= 1000000:
				sec += usec/1000000
				usec = usec/1000000
		statis_data[mod][0] = num
		statis_data[mod][1] = (sec*1000000 + usec)*1.0/num
		statis_data[mod][2] = max_usecs
		statis_data[mod][3] = min_usecs
	return statis_data

def show(statis_data):
	
	print "%s" %("="*(90+8*4))
	print "%32s\t%10s\t%16s\t%16s\t%16s" %("mod_name", "count", "aver/usecs", "max/usecs", "min/usecs")
	for mod in statis_data:
		print "%32s\t%10d\t%16f\t%16d\t%16d" %(mod, statis_data[mod][0], statis_data[mod][1], statis_data[mod][2], statis_data[mod][3])

if __name__ == "__main__":
	if len(sys.argv) > 1:
		mods_data =	fetch_data(sys.argv[1] + "/file_list")
		statis_data = statis(mods_data)
		show(statis_data)
