#/usr/bin/python


import matplotlib.pyplot as plt
import pickle 


file1 = open('op_encrypt_results.pickle','r')
encrypt_results = pickle.load(file1)

file2 = open('op_client_results.pickle','r')
client_results = pickle.load(file2)

file3 = open('op_server_results.pickle','r')
server_results = pickle.load(file3)

[dimension,time_elapsed] = zip(*encrypt_results)
[dimension_,time_elapsed_] = zip(*client_results)
[dimension__,time_elapsed__] = zip(*server_results)

plt.xlabel('Dimension of Plaintext')
plt.ylabel('Time comsumed/s')
#plt.title('Performance Comparison of Encryption')
plt.title('Performance of Optimized Encryption')
plt.plot(list(dimension),list(time_elapsed),marker='o',linestyle='--',color='r',label="Encrypt")
#plt.plot(list(dimension_),list(time_elapsed_),marker='^',linestyle='-.',color='blue',label="Client")
plt.plot(list(dimension__),list(time_elapsed__),marker='+',linestyle='-.',color='blue',label="Server")
plt.axis([0,1000,0,6])
# add a legend, to let the viewer know which curve is which
# To do this, we should adding label arguments in plot method
# then call legend() to draw the legend box.
plt.legend(loc=9) 
#plt.show()
plt.savefig('Performance_of_Server_Transformation.png')


