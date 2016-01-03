#/usr/bin/python
# coding=utf-8

import subprocess
import pickle

def stat(arg):
	time_elapsed = 0.0
	count = 1
	for i in range(count):
		process = subprocess.Popen('./project '+str(arg), \
		shell= True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

		out, err = process.communicate()
		time_elapsed +=  float(out)
	return time_elapsed/count


if __name__=='__main__':
	results = [(500*i,stat(500*i)) for i in range(1,8)]
	file = open('optimized_500_results.pickle','w')
	pickle.dump(results,file)

