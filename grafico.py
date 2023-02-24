import matplotlib.pyplot as plt


messages = [9937800, 9935192, 9765817, 9946864, 9730338, 9730338, 9921224, 9984003, 9717065, 9983117, 9989318, 9966705, 9955792, 9991228, 9860093, 9853482, 9949508, 9927659, 9983136, 9875764]
tests = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20]

fig, ax = plt.subplots()
bars = ax.bar(tests, messages, color='green')
ax.bar(tests, [10000000 - x for x in messages], bottom=messages, color='red')
ax.set_xlabel('Test')
ax.set_ylabel('Messages')
ax.set_title('Messages Successfully Sent per Test')
ax.set_xticks(tests)
ax.set_xticklabels(tests)

plt.show()

import numpy as np

average_successes = np.mean(messages)
print("Average successes:", average_successes)