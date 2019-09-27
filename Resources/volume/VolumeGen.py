import math

vol_size = 128
vol_data = [0 for i in range(vol_size * vol_size * vol_size)]
vol_center = vol_size // 2

def clamp(n,lo,hi):
    if n < lo:
        return lo
    if n > hi:
        return hi
    return n

for z in range(vol_size):
    for y in range(vol_size):
        for x in range(vol_size):
            idx = x + vol_size * (y + z * vol_size)
            dx, dy, dz = x-vol_center,y-vol_center*2,z-vol_center
            d = math.floor(math.sqrt(dx**2 + dy**2 + dz**2))
            vol_data[idx] = clamp(255-int(d),0,255)

f = open('Generated.raw', 'wb')
f.write(bytearray(vol_data))
f.close()