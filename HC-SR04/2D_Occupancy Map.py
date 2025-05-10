import serial
import time
import numpy as np
import matplotlib.pyplot as plt

# — configure these to your setup —
PORT       = 'COM6'      # e.g. '/dev/ttyUSB0' or 'COM3'
BAUDRATE   = 115200
TIMEOUT    = 1          # seconds

# containers for data
static_map = {}          # angle → distance
dyn_hits   = []          # list of (angle,dist,timestamp)

def polar_to_xy(angle_deg, dist_cm):
    """Convert polar coordinates to x,y (in centimeters)."""
    theta = np.deg2rad(angle_deg)
    return dist_cm * np.cos(theta), dist_cm * np.sin(theta)

def read_serial_map(duration_s=10):
    """Read from serial for given duration and parse lines."""
    ser = serial.Serial(PORT, BAUDRATE, timeout=TIMEOUT)
    start = time.time()
    while time.time() - start < duration_s:
        line = ser.readline().decode('utf-8', errors='ignore').strip()
        if not line:
            continue
        parts = line.split(',')
        if len(parts) != 4:
            continue

        tag, ang_s, dist_s, ts_s = parts
        try:
            ang  = int(ang_s)
            dist = int(dist_s)
            ts   = int(ts_s)
        except ValueError:
            continue

        if tag == 'M':
            # static map entry
            static_map[ang] = dist
        elif tag == 'D':
            # dynamic obstacle hit
            dyn_hits.append((ang, dist, ts))

    ser.close()

def plot_results():
    # Plot static map
    xs, ys = zip(*(polar_to_xy(ang, d)
                   for ang, d in static_map.items()))
    plt.figure(figsize=(6,6))
    plt.scatter(xs, ys, s=5, c='red', label='static obstacles')

    # Plot dynamic obstacle trajectory (chronological)
    if dyn_hits:
        dyn_hits.sort(key=lambda x: x[2])  # sort by timestamp
        dx, dy = zip(*(polar_to_xy(ang, d)
                       for ang, d, _ in dyn_hits))
        plt.plot(dx, dy, '-o', markersize=4, label='dynamic path')

    plt.axis('equal')
    plt.title('Ultrasonic Scan Map')
    plt.xlabel('X (cm)')
    plt.ylabel('Y (cm)')
    plt.legend()
    plt.grid(True)
    plt.show()

if __name__ == '__main__':
    print(f"Reading serial data on {PORT} for 20 seconds…")
    read_serial_map(duration_s=20)
    print(f"Got {len(static_map)} static points and {len(dyn_hits)} dynamic hits.")
    plot_results()
