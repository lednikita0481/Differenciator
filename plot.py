import numpy as np
import matplotlib.pyplot as plt
x = np.linspace(-20.000000, 20.000000, 200)
y = (x**(101)-(101*(x))+(100))/(x**(2)-(2*(x))+(1))
plt.figure(figsize=(8,6), dpi=100)
plt.grid(True, linestyle="--")
plt.axis([-20.000000, 20.000000, np.min(y), np.max(y)])
plt.plot(x, y, "-m",linewidth=0.5)
y = 1419029908 * x + 44593981
plt.plot(x, y, "-g",linewidth=0.4)
plt.savefig('plot.png')