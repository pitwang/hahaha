#/usr/bin/python
# coding=utf-8

import subprocess
import pickle

def stat(dimension,op):
	time_elapsed = 0.0
	count = 1
	for i in range(count):
		process = subprocess.Popen('./project '+str(dimension)+' '+op, \
		shell= True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)

		out, err = process.communicate()
		time_elapsed +=  float(out)
	return time_elapsed/count


if __name__=='__main__':	
	op_list = ['encrypt','client','server']
	for op in op_list:
		results = [(200*i,stat(200*i,op)) for i in range(1,6)]
		file = open('op_'+op+'_results.pickle','w')
		pickle.dump(results,file)

