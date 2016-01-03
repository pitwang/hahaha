#/usr/bin/python


import matplotlib.pyplot as plt
import pickle 

file = open('optimized_500_results.pickle','r')
new_results = pickle.load(file)

file1 = open('vhe_results.pickle','r')
original_results = pickle.load(file1)
[dimension,time_elapsed] = zip(*new_results)
[dimension_,time_elapsed_] = zip(*original_results)

plt.xlabel('Dimension of Plaintext')
plt.ylabel('Time comsumed/s')
#plt.title('Performance Comparison of Encryption')
plt.title('Performance of Optimized Encryption')
plt.plot(list(dimension),list(time_elapsed),marker='o',linestyle='--',color='r',label="Optimized")
#plt.plot(list(dimension_),list(time_elapsed_),marker='^',linestyle='-.',color='blue',label="Original")
plt.axis([0,4000,0,100])
# add a legend, to let the viewer know which curve is which
# To do this, we should adding label arguments in plot method
# then call legend() to draw the legend box.
plt.legend(loc=9) 
#plt.show()
plt.savefig('Performance_of_Optimized_Encryption(500).png')


