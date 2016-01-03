#/usr/bin/python


import matplotlib.pyplot as plt
import pickle 

file = open('vhe_results.pickle','r')
results = pickle.load(file)

[dimension,time_elapsed] = zip(*results)

plt.xlabel('Dimension of Plaintext')
plt.ylabel('Time comsumed/s')
plt.title('Performance after Optimization')
plt.plot(list(dimension),list(time_elapsed),marker='o',linestyle='--')
plt.axis([0,1000,0,110])
plt.show()
