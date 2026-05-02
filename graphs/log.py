import re
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

data = []
header_pattern = re.compile(r"---\s+(SERIAL|PTHREADS|CUDA)\s+:\s+(\d+)\s+bodies.*?\(Run (\d+)\)\s+---")
frame_time_pattern = re.compile(r"frame_time:\s+([\d.]+)\s+ms")
update_time_pattern = re.compile(r"update_time:\s+([\d.]+)\s+ms")
render_time_pattern = re.compile(r"render_time:\s+([\d.]+)\s+ms")

def parse_file(filename):
    with open(filename, 'r') as f:
        content = f.read()
    blocks = re.split(r'--- (?=[A-Z]+)', '\n' + content)
    for block in blocks:
        if not block.strip(): continue
        block = '--- ' + block
        h_match = header_pattern.search(block)
        if not h_match: continue
        impl, bodies, run = h_match.groups()
        ft_match = frame_time_pattern.search(block)
        ut_match = update_time_pattern.search(block)
        rt_match = render_time_pattern.search(block)
        if ft_match and ut_match and rt_match:
            data.append({
                'Implementation': impl,
                'Bodies': int(bodies),
                'Run': int(run),
                'Frame Time (ms)': float(ft_match.group(1)),
                'Update Time (ms)': float(ut_match.group(1)),
                'Render Time (ms)': float(rt_match.group(1))
            })

parse_file('benchmark_results.txt')
parse_file('benchmark_results100k.txt')

df = pd.DataFrame(data)
df_mean = df.groupby(['Implementation', 'Bodies']).mean(numeric_only=True).reset_index()

# 1. Log-Scale Bar Chart (Update Time)
plt.figure(figsize=(12, 6))
sns.barplot(data=df_mean, x='Bodies', y='Update Time (ms)', hue='Implementation')
plt.yscale('log')
plt.title('Average Update Time vs Number of Bodies (Log Scale Y-Axis)')
plt.ylabel('Update Time (ms) - Log Scale')
plt.grid(axis='y', ls='--', alpha=0.7, which='both')
plt.savefig('log_update_time_bar.png')
plt.close()

# 2. Log-Scale Bar Chart (Frame Time)
plt.figure(figsize=(12, 6))
sns.barplot(data=df_mean, x='Bodies', y='Frame Time (ms)', hue='Implementation')
plt.yscale('log')
plt.title('Average Total Frame Time vs Number of Bodies (Log Scale Y-Axis)')
plt.ylabel('Frame Time (ms) - Log Scale')
plt.grid(axis='y', ls='--', alpha=0.7, which='both')
plt.savefig('log_frame_time_bar.png')
plt.close()

# 3. Log-Log Line Chart for Frame Time
plt.figure(figsize=(10, 6))
sns.lineplot(data=df, x='Bodies', y='Frame Time (ms)', hue='Implementation', marker='o')
plt.xscale('log')
plt.yscale('log')
plt.title('Total Frame Time vs Number of Bodies (Log-Log Scale)')
plt.ylabel('Frame Time (ms)')
plt.grid(True, which="both", ls="--", alpha=0.5)
plt.savefig('log_frame_time_line.png')
plt.close()
