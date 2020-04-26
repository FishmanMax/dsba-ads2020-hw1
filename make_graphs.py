import pandas as pd
import matplotlib.pyplot as plt

results = pd.read_csv('./data/output.csv')
results.plot(x='size')
plt.show()