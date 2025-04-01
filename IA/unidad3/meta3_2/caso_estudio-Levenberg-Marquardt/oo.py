import numpy as np

def lectura(URL):
    X_data = []
    Y_data = []
    with open(URL, 'r') as archivo:
        for linea in archivo:
            valores = list(map(float, linea.strip().split()))
            if len(valores) == 4:
                X_data.append(valores[:2])
                Y_data.append(valores[2:])
    X_matrix = np.array(X_data)
    unos = np.ones((X_matrix.shape[0], 1))
    A = np.hstack((X_matrix, unos))
    Y = np.array(Y_data)
    return A, Y

def calcular_jacobiana(A, m):
    q, g = A.shape
    J = np.zeros((q*m, g*m))
    for j in range(m):
        fila_inicio = j*q
        col_inicio = j*g
        J[fila_inicio:fila_inicio+q, col_inicio:col_inicio+g] = -A
    return J

def objfcn_reg(theta_vec, A, Y):
    g = A.shape[1]
    m = Y.shape[1]
    Theta = theta_vec.reshape((g, m), order='F')
    Y_hat = A @ Theta
    E = Y - Y_hat
    return np.sum(E**2)

def objfcnjac_reg(theta_vec, A, Y):
    g = A.shape[1]
    m = Y.shape[1]
    Theta = theta_vec.reshape((g, m), order='F')
    Y_hat = A @ Theta
    E = Y - Y_hat
    residuals = E.flatten(order='F')
    J = calcular_jacobiana(A, m)
    gX = 2.0 * J.T @ residuals
    normgX = np.linalg.norm(gX)
    return residuals, J, normgX

def trainLM(x0, A, Y, max_iter=1000, lr_init=1e-3, lr_dec=0.1, lr_inc=10.0, gamma_max=1e10, tol=1e-6, verbose=True, show=100):
    x = x0.copy()
    performance = []
    gamma = lr_init
    for epoch in range(max_iter):
        try:
            residuals, J, norm_grad = objfcnjac_reg(x, A, Y)
            current_performance = objfcn_reg(x, A, Y)
            performance.append(current_performance)
            
            if verbose and (epoch % show == 0):
                print(f"Iter {epoch:5d}: Cost = {current_performance:.4e} | ||grad|| = {norm_grad:.4e} | gamma = {gamma:.4e}")
            
            if norm_grad < tol:
                if verbose:
                    print(f"Converged at iteration {epoch}")
                break
            
            A_lm = J.T @ J + gamma * np.eye(len(x))
            b = -J.T @ residuals
            delta = np.linalg.solve(A_lm, b)
            
            x_new = x + delta
            new_performance = objfcn_reg(x_new, A, Y)
            
            if new_performance < current_performance:
                x = x_new
                gamma *= lr_dec
            else:
                gamma *= lr_inc
            
            if gamma > gamma_max:
                if verbose:
                    print("Gamma reached maximum value")
                break
        except np.linalg.LinAlgError:
            gamma *= lr_inc
    return x, performance

def calcular_r2(Y_real, Y_pred):
    ss_res = np.sum((Y_real - Y_pred)**2)
    ss_tot = np.sum((Y_real - np.mean(Y_real, axis=0))**2)
    return 1 - (ss_res / ss_tot)

# Ejemplo de uso
if __name__ == "__main__":
    # Leer datos
    URL = 'ruta/al/archivo.txt'  # Cambiar a la ruta correcta
    A, Y = lectura(URL)
    g = A.shape[1]
    m = Y.shape[1]
    
    # Inicializar parámetros
    Theta_initial = np.zeros((g, m))
    x0 = Theta_initial.flatten(order='F')
    
    # Entrenar modelo
    theta_opt, performance = trainLM(x0, A, Y, max_iter=1000, show=100, tol=1e-6)
    
    # Resultados
    Theta_opt = theta_opt.reshape((g, m), order='F')
    Y_hat = A @ Theta_opt
    r2 = calcular_r2(Y, Y_hat)
    print("\nCoeficientes óptimos:")
    print(Theta_opt)
    print(f"\nR²: {r2:.4f}")