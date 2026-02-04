import pandas as pd
import matplotlib.pyplot as plt

# Wczytanie danych z pliku CSV
df = pd.read_csv('results/sa_log_g50_T200.00_A0.980_maxI540_NS520260130_031128.csv', sep=';')

# # Utworzenie figury z wykresami
# fig, ((ax1, ax2), (ax3, ax4)) = plt.subplots(2, 2, figsize=(14, 10))
# fig.suptitle('Analiza algorytmu symulowanego wyżarzania', fontsize=16, fontweight='bold')
# Wykres 1: Current fitness i Best fitness w czasie
plt.figure(figsize=(10,6))
plt.plot(df['iteration'], df['current_fitness'], label='Current Fitness', 
         color='blue', alpha=0.6, linewidth=1.5)
plt.plot(df['iteration'], df['best_fitness'], label='Best Fitness', 
         color='red', linewidth=2)
plt.xlabel('Iteracja')
plt.ylabel('Fitness')
# plt.title('Przebieg wartości fitness')
plt.legend()
plt.grid(True, alpha=0.3)

# # Wykres 2: Temperatura w czasie
# ax2.plot(df['iteration'], df['temperature'], color='orange', linewidth=2)
# ax2.set_xlabel('Iteracja')
# ax2.set_ylabel('Temperatura')
# ax2.set_title('Schładzanie temperatury')
# ax2.grid(True, alpha=0.3)

# # Wykres 3: Current fitness vs temperatura (scatter)
# scatter = ax3.scatter(df['temperature'], df['current_fitness'], 
#                       c=df['iteration'], cmap='viridis', alpha=0.6, s=30)
# ax3.set_xlabel('Temperatura')
# ax3.set_ylabel('Current Fitness')
# ax3.set_title('Current Fitness vs Temperatura')
# ax3.grid(True, alpha=0.3)
# plt.colorbar(scatter, ax=ax3, label='Iteracja')

# # Wykres 4: Poprawa best fitness w czasie (skumulowana)
# ax4.plot(df['iteration'], df['best_fitness'], color='green', linewidth=2)
# ax4.fill_between(df['iteration'], df['best_fitness'], 
#                   df['best_fitness'].max(), alpha=0.3, color='green')
# ax4.set_xlabel('Iteracja')
# ax4.set_ylabel('Best Fitness')
# ax4.set_title('Najlepsza wartość fitness')
# ax4.grid(True, alpha=0.3)

plt.tight_layout()
plt.savefig('sa_plots/analiza_sa_g50_new4.png', dpi=300, bbox_inches='tight')
plt.show()

# Dodatkowy wykres - wszystko na jednym (z dwoma osiami Y)
# fig2, ax5 = plt.subplots(figsize=(12, 6))

# color1 = 'tab:blue'
# ax5.set_xlabel('Iteracja')
# ax5.set_ylabel('Fitness', color=color1)
# ax5.plot(df['iteration'], df['current_fitness'], label='Current Fitness', 
#          color='blue', alpha=0.5, linewidth=1.5)
# ax5.plot(df['iteration'], df['best_fitness'], label='Best Fitness', 
#          color='darkblue', linewidth=2)
# ax5.tick_params(axis='y', labelcolor=color1)
# ax5.legend(loc='upper left')
# ax5.grid(True, alpha=0.3)

# ax6 = ax5.twinx()
# color2 = 'tab:red'
# ax6.set_ylabel('Temperatura', color=color2)
# ax6.plot(df['iteration'], df['temperature'], label='Temperatura', 
#          color='red', linewidth=2, linestyle='--')
# ax6.tick_params(axis='y', labelcolor=color2)
# ax6.legend(loc='upper right')

# plt.title('Kompleksowa analiza algorytmu', fontsize=14, fontweight='bold')
# plt.tight_layout()
# plt.savefig('analiza_sa_combined.png', dpi=300, bbox_inches='tight')
# plt.show()

# Statystyki
print("\n=== STATYSTYKI ===")
print(f"Najlepsza wartość fitness: {df['best_fitness'].min():.6f}")
print(f"Najgorsza wartość current fitness: {df['current_fitness'].max():.6f}")
print(f"Średnia current fitness: {df['current_fitness'].mean():.6f}")
print(f"Temperatura początkowa: {df['temperature'].iloc[0]:.2f}")
print(f"Temperatura końcowa: {df['temperature'].iloc[-1]:.2f}")
print(f"Liczba iteracji: {len(df)}")