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

plt.figure(figsize=(10, 6))
sns.lineplot(data=df, x='Bodies', y='Update Time (ms)', hue='Implementation', marker='o')
plt.xscale('log')
plt.yscale('log')
plt.title('Update Time vs Number of Bodies (Log-Log)')
plt.grid(True, which="both", ls="--", alpha=0.5)
plt.savefig('update_time_scaling.png')
plt.close()

serial_data = df_mean[df_mean['Implementation'] == 'SERIAL'][['Bodies', 'Update Time (ms)']].set_index('Bodies')
df_speedup = df_mean.copy()

def calculate_speedup(row):
    try:
        return serial_data.loc[row['Bodies'], 'Update Time (ms)'] / row['Update Time (ms)']
    except KeyError:
        return None

df_speedup['Speedup'] = df_speedup.apply(calculate_speedup, axis=1)

plt.figure(figsize=(10, 6))
sns.barplot(data=df_speedup[df_speedup['Bodies'].isin([1000, 10000])], x='Bodies', y='Speedup', hue='Implementation')
plt.title('Update Time Speedup relative to SERIAL')
plt.ylabel('Speedup Factor (Higher is Better)')
plt.grid(axis='y', ls='--', alpha=0.7)
plt.savefig('speedup_comparison.png')
plt.close()

df_10k = df_mean[df_mean['Bodies'] == 10000].set_index('Implementation')
df_10k[['Update Time (ms)', 'Render Time (ms)']].plot(kind='bar', stacked=True, figsize=(8, 6))
plt.title('Frame Time Breakdown (N=10000)')
plt.ylabel('Time (ms)')
plt.xticks(rotation=0)
plt.savefig('frame_time_breakdown.png')
plt.close()
