import numpy as np
import matplotlib.pyplot as plt
x = np.linspace(-10.000000, 10.000000, 200)
y = np.sin(x**(2.000))+(np.cos(x))
plt.figure(figsize=(8,6), dpi=100)
plt.grid(True, linestyle="--")
plt.axis([-10.000000, 10.000000, np.min(y), np.max(y)])
plt.plot(x, y, "-m",linewidth=0.5)
plt.savefig('plot.png')