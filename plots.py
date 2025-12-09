import pandas as pd
import matplotlib.pyplot as plt
from decimal import Decimal, getcontext
from shapely import affinity
from shapely.geometry import Polygon
from shapely.ops import unary_union
from matplotlib.patches import Rectangle
import numpy as np
import os

# Precyzja
getcontext().prec = 25
scale_factor = Decimal('1e3')

class ChristmasTree:
    def __init__(self, center_x='0', center_y='0', angle='0'):
        self.center_x = Decimal(center_x)
        self.center_y = Decimal(center_y)
        self.angle = Decimal(angle)

        trunk_w = Decimal('0.15')
        trunk_h = Decimal('0.2')
        base_w = Decimal('0.7')
        mid_w = Decimal('0.4')
        top_w = Decimal('0.25')
        tip_y = Decimal('0.8')
        tier_1_y = Decimal('0.5')
        tier_2_y = Decimal('0.25')
        base_y = Decimal('0.0')
        trunk_bottom_y = -trunk_h

        initial_polygon = Polygon([
            (Decimal('0.0') * scale_factor, tip_y * scale_factor),
            (top_w / Decimal('2') * scale_factor, tier_1_y * scale_factor),
            (top_w / Decimal('4') * scale_factor, tier_1_y * scale_factor),
            (mid_w / Decimal('2') * scale_factor, tier_2_y * scale_factor),
            (mid_w / Decimal('4') * scale_factor, tier_2_y * scale_factor),
            (base_w / Decimal('2') * scale_factor, base_y * scale_factor),
            (trunk_w / Decimal('2') * scale_factor, base_y * scale_factor),
            (trunk_w / Decimal('2') * scale_factor, trunk_bottom_y * scale_factor),
            (-(trunk_w / Decimal('2')) * scale_factor, trunk_bottom_y * scale_factor),
            (-(trunk_w / Decimal('2')) * scale_factor, base_y * scale_factor),
            (-(base_w / Decimal('2')) * scale_factor, base_y * scale_factor),
            (-(mid_w / Decimal('4')) * scale_factor, tier_2_y * scale_factor),
            (-(mid_w / Decimal('2')) * scale_factor, tier_2_y * scale_factor),
            (-(top_w / Decimal('4')) * scale_factor, tier_1_y * scale_factor),
            (-(top_w / Decimal('2')) * scale_factor, tier_1_y * scale_factor),
        ])
        
        rotated = affinity.rotate(initial_polygon, float(self.angle), origin=(0, 0))
        self.polygon = affinity.translate(rotated,
                                        xoff=float(self.center_x * scale_factor),
                                        yoff=float(self.center_y * scale_factor))

def calculate_side_length(placed_trees):
    if not placed_trees:
        return Decimal('0')
    
    all_polygons = [t.polygon for t in placed_trees]
    bounds = unary_union(all_polygons).bounds
    
    minx = Decimal(bounds[0]) / scale_factor
    miny = Decimal(bounds[1]) / scale_factor
    maxx = Decimal(bounds[2]) / scale_factor
    maxy = Decimal(bounds[3]) / scale_factor
    
    width = maxx - minx
    height = maxy - miny
    side_length = max(width, height)
    
    return side_length


def plot_results(side_length, placed_trees, num_trees):
    _, ax = plt.subplots(figsize=(8, 8))
    colors = plt.cm.viridis(np.linspace(0, 1, num_trees))
    
    all_polygons = [t.polygon for t in placed_trees]
    bounds = unary_union(all_polygons).bounds
    #konwersja na float
    side_length = float(side_length)
    
    # rysowanie choinek
    for i, tree in enumerate(placed_trees):
        x_scaled, y_scaled = tree.polygon.exterior.xy
        x = [float(Decimal(val) / scale_factor) for val in x_scaled]
        y = [float(Decimal(val) / scale_factor) for val in y_scaled]
        ax.plot(x, y, color=colors[i], linewidth=2)
        ax.fill(x, y, alpha=0.6, color=colors[i])
    
    
    minx = float(Decimal(bounds[0]) / scale_factor)
    miny = float(Decimal(bounds[1]) / scale_factor)
    maxx = float(Decimal(bounds[2]) / scale_factor)
    maxy = float(Decimal(bounds[3]) / scale_factor)
    
    width = maxx - minx
    height = maxy - miny
    
    square_x = minx if width >= height else minx - (side_length - width) / 2
    square_y = miny if height >= width else miny - (side_length - height) / 2
    
    bounding_square = Rectangle(
        (square_x, square_y),
        side_length,
        side_length,
        fill=False,
        edgecolor='red',
        linewidth=3,
        linestyle='--'
    )
    ax.add_patch(bounding_square)
    
    padding = 0.3
    ax.set_xlim(square_x - padding, square_x + side_length + padding)
    ax.set_ylim(square_y - padding, square_y + side_length + padding)
    ax.set_aspect('equal', adjustable='box')
    ax.axis('off')
    ax.set_title(f'{num_trees} Trees | Side: {side_length:.6f}', 
                fontsize=14, fontweight='bold')
    plt.tight_layout()
    plt.show()


def plot_cpp_results(csv_filename):

    
    if not os.path.exists(csv_filename):
        print(f"Plik {csv_filename} nie istnieje!")
        return
    
    df = pd.read_csv(csv_filename)
    print(f"Wczytano {len(df)} choinek z {csv_filename}")
    

    placed_trees = []
    for idx, row in df.iterrows():
        tree = ChristmasTree(
            center_x=str(row['center_x']),
            center_y=str(row['center_y']),
            angle=str(row['angle'])
        )
        placed_trees.append(tree)
    
    # liczene  długości boku
    side_length = calculate_side_length(placed_trees)
    

    plot_results(side_length, placed_trees, len(placed_trees))
    
    print(f"Side length: {side_length:.12f}")


if __name__ == '__main__':

    csv_file = 'results.csv'  
    
    plot_cpp_results(csv_file)
