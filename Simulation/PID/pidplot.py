import pandas as pd
import matplotlib.pyplot as plt

# Read the CSV file
df = pd.read_csv('error_data.csv')  # Replace with your CSV file name

# Assuming the CSV has columns 'time' and 'error'
# If your CSV structure is different, adjust the column names accordingly
time_data = df['time']
error_data = df['error']

# Create the plot
plt.figure(figsize=(12, 6))
plt.plot(time_data, error_data)
plt.title('PID Error for Line-Following Robot')
plt.xlabel('Time')
plt.ylabel('Error')
plt.grid(True)

# Add some basic statistics to the plot
avg_error = error_data.mean()
max_error = error_data.max()
min_error = error_data.min()

plt.axhline(y=avg_error, color='r', linestyle='--', label=f'Average Error: {avg_error:.2f}')
plt.text(time_data.max(), max_error, f'Max: {max_error:.2f}', verticalalignment='bottom')
plt.text(time_data.max(), min_error, f'Min: {min_error:.2f}', verticalalignment='top')

plt.legend()

# Show the plot
plt.show()

# If you want to save the plot as an image file
# plt.savefig('pid_error_plot.png')