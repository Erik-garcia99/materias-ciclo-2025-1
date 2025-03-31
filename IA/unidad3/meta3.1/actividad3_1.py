import numpy as np

def objfcn(x):
    n = len(x)
    z = np.zeros(n)
    l2 = np.arange(0, n, 2)
    l1 = np.arange(1, n, 2)
    x_l2 = np.clip(x[l2], -1e3, 1e3)  # Limitar valores de x[l2] para evitar overflow
    z[l2] = 10.0 * (x[l1] - x_l2**2)
    z[l1] = 1.0 - x_l2  # Usar x_l2 recortado
    return np.dot(z, z)

def objfcngrad(x):
    n = len(x)
    Jz = np.zeros((n, n))
    z = np.zeros(n)
    l2 = np.arange(0, n, 2)
    l1 = np.arange(1, n, 2)
    x_l2 = np.clip(x[l2], -1e3, 1e3)  # Limitar x[l2]
    z[l2] = 10.0 * (x[l1] - x_l2**2)
    z[l1] = 1.0 - x_l2

    for i in range(n // 2):
        Jz[2*i, 2*i] = -20.0 * x_l2[i]  # Usar x_l2 recortado
        Jz[2*i, 2*i + 1] = 10.0
        Jz[2*i + 1, 2*i] = -1.0

    gX = 2.0 * np.dot(Jz.T, z)
    return np.clip(gX, -1e3, 1e3)  # Recortar gradiente

def trainGDX(objfcn, objfcngrad, x0, lr=0.0001, momentum=0.9, lr_dec=0.7, 
             lr_inc=1.05, max_per_inc=1.04, max_iter=10000, tol=1e-6):
    x = x0.copy().astype(np.float64)
    grad = objfcngrad(x)
    delta_x = -lr * grad
    prev_f = objfcn(x)
    epsilon = 1e-8  # Evitar división por cero

    for t in range(max_iter):
        delta_x = momentum * delta_x - (1 - momentum) * lr * grad
        x_new = x + delta_x
        f_new = objfcn(x_new)

        # Manejar casos extremos
        if prev_f < epsilon:
            prev_f = epsilon
        ratio = f_new / prev_f if f_new < 1e100 else np.inf  # Evitar overflow

        if ratio > max_per_inc:
            lr = max(lr * lr_dec, 1e-12)  # Límite inferior muy pequeño
            delta_x = -lr * grad
        else:
            if f_new < prev_f:
                lr = min(lr * lr_inc, 1e-3)  # Límite superior conservador
            grad = objfcngrad(x_new)

        x = x_new
        prev_f = f_new

        if np.isnan(f_new) or np.linalg.norm(grad) < tol:
            break

    return x, prev_f

# Configuración para n=100 (inicialización cerca del mínimo)
n100 = 100
x0_n100 = np.random.uniform(0.5, 1.5, n100)  # Punto inicial cerca de (1, 1, ..., 1)
x_opt_n100, f_opt_n100 = trainGDX(objfcn, objfcngrad, x0_n100, lr=0.00001, momentum=0.9, max_iter=20000)



print(f"n=10: f_opt = {f_opt_n10:.6f}, error = {np.linalg.norm(x_opt_n10 - np.ones(n10)):.6f}")
print(f"n=100: f_opt = {f_opt_n100:.6f}, error = {np.linalg.norm(x_opt_n100 - np.ones(n100)):.6f}")