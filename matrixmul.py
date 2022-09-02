#!/usr/bin/python3
import numpy as np
import time
import sys

fil = "/tmp/matmul"
N = int(sys.argv[1])
print(N)
if __name__ == "__main__":
    #
    # numpy arrays to create float type NxN arrays
    A = np.random.rand(N, N).astype(np.float32)
    B = np.random.rand(N, N).astype(np.float32)
    flops = 2*N*N*N
    et = time.monotonic()
    C = A @ B
    st = time.monotonic()
    s = st - et
    print(f"GFlops {flops/s * 1e-9}")
    #print(f"\nFirst matrix:\n{A}\n\nSecond matrix:\n{B}\n\nDotProduct \n{C}")
    #print(C)
    with open(fil, 'wb') as f:
        f.write(A.data)
        f.write(B.data)
        f.write(C.data)

        f.close()

