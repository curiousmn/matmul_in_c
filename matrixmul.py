import numpy as np

fil = "/tmp/matmul"
N = 1024 

if __name__ == "__main__":
    # 
    # numpy arrays to create float type NxN arrays
    A = np.random.rand(N, N).astype(np.float32)
    B = np.random.rand(N, N).astype(np.float32)
    
    C = A @ B
    print(A,'\n\n', B)
    with open(fil, 'wb') as f:
        f.write(A.data) 
        f.write(B.data) 
        f.write(C.data) 

        f.close()

