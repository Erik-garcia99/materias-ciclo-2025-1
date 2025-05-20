# import numpy as np
# from numpy.linalg import solve
# import os

# def load_data(filename):
#     """Carga datos usando rutas relativas desde la ubicación del script."""
#     script_dir = os.path.dirname(__file__)  # Ruta del script actual
#     file_path = os.path.join(script_dir, filename)
#     data = np.genfromtxt(file_path, delimiter=',')
#     X = data[:, :2]
#     Y = data[:, 2:]
#     return X, Y

# def compute_jacobian(A, num_outputs):
#     """Calcula la matriz Jacobiana para el modelo de regresión multivariada."""
#     num_samples = A.shape[0]
#     num_params = A.shape[1]
#     J = np.zeros((num_samples * num_outputs, num_params * num_outputs))
#     for p in range(num_samples):
#         for j in range(num_outputs):
#             row = p * num_outputs + j
#             col_start = j * num_params
#             J[row, col_start:col_start + num_params] = -A[p, :]
#     return J

# def trainLM(X, Y, lr=0.001, lr_dec=0.1, lr_inc=10.0, lr_max=1e10, max_iter=100, tol=1e-6):
#     """Implementa el algoritmo de Levenberg-Marquardt para regresión multivariada."""
#     # Construir matriz de diseño A = [X | 1]
#     A = np.hstack((X, np.ones((X.shape[0], 1))))
#     num_outputs = Y.shape[1]
#     num_params = A.shape[1]
    
#     # Inicializar parámetros
#     Theta = np.zeros((num_params, num_outputs))
    
#     # Precalcular Jacobiano y J^T J (constante en regresión lineal)
#     J = compute_jacobian(A, num_outputs)
#     JtJ = J.T @ J
    
#     gamma = lr
#     for t in range(max_iter):
#         # Calcular error actual
#         E = Y - A @ Theta
#         SSE = np.sum(E ** 2)
#         if SSE < tol:
#             break
        
#         current_gamma = gamma
#         while current_gamma <= lr_max:
#             e = E.flatten('F')  # Vector de errores aplanado
#             Jte = J.T @ e
#             delta_theta = solve(JtJ + current_gamma * np.eye(JtJ.shape[0]), -Jte)
#             Theta_proposed = Theta + delta_theta.reshape((num_params, num_outputs), order='F')
            
#             # Calcular nuevo error
#             E_new = Y - A @ Theta_proposed
#             SSE_new = np.sum(E_new ** 2)
            
#             if SSE_new < SSE:
#                 Theta = Theta_proposed
#                 gamma = current_gamma * lr_dec
#                 break
#             else:
#                 current_gamma *= lr_inc
#         else:
#             print("Gamma superó el límite máximo.")
#             break
#     return Theta

# # Cargar datos y entrenar modelo para ambos datasets
# datasets = {
#     'challenge00': 'challenge00_syntheticdataset22.txt',  # Sin "synthetic"
#     'challenge01': 'challenge01_syntheticdataset22.txt'
# }

# for name, path in datasets.items():
#     print(f"\n--- Entrenando modelo para {name} ---")
#     X, Y = load_data(path)
#     Theta = trainLM(X, Y)
#     print(f"Parámetros óptimos Θ:\n{Theta}")



import numpy as np
from numpy.linalg import solve
import os

def load_data(filename):
    """Carga y normaliza los datos correctamente."""
    script_dir = os.path.dirname(__file__)
    file_path = os.path.join(script_dir, filename)
    data = np.genfromtxt(file_path, delimiter=',')
    X = data[:, :2]
    Y = data[:, 2:]
    
    # Normalización Z-score con epsilon para evitar divisiones por cero
    X_mean, X_std = np.mean(X, axis=0), np.std(X, axis=0) + 1e-8
    Y_mean, Y_std = np.mean(Y, axis=0), np.std(Y, axis=0) + 1e-8
    X = (X - X_mean) / X_std
    Y = (Y - Y_mean) / Y_std
    
    return X, Y, (X_mean, X_std, Y_mean, Y_std)


def compute_jacobian(A, num_outputs):
    """Jacob corianaregida para múltiples salidas."""
    num_samples, num_params = A.shape
    J = np.zeros((num_samples * num_outputs, num_params * num_outputs))
    
    for j in range(num_outputs):
        J[j::num_outputs, j*num_params:(j+1)*num_params] = -A
    
    return J


def compute_r_squared(Y_true, Y_pred):
    """Calcula el coeficiente de determinación R² para múltiples salidas."""
    # Suma de errores al cuadrado (SSE)
    SSE = np.sum((Y_true - Y_pred) ** 2)
    
    # Suma total de cuadrados (SST)
    Y_mean = np.mean(Y_true, axis=0)  # Media por columna (cada variable de respuesta)
    SST = np.sum((Y_true - Y_mean) ** 2)
    
    # Evitar división por cero
    R2 = 1 - (SSE / SST) if SST != 0 else 0.0
    return R2


def trainLM(X, Y, Y_mean, Y_std, lr=1e-4, lr_dec=0.1, lr_inc=10.0, lr_max=1e5, max_iter=100000, tol=1e-6):
    """Algoritmo optimizado con correcciones críticas."""
    A = np.hstack((X, np.ones((X.shape[0], 1))))
    num_outputs = Y.shape[1]
    num_params = A.shape[1]
    
    # Inicialización Xavier para mejor convergencia
    Theta = np.random.randn(num_params, num_outputs) * np.sqrt(2/(num_params + num_outputs))
    
    J = compute_jacobian(A, num_outputs)
    JtJ = J.T @ J + 1e-4 * np.eye(J.shape[1])  # Regularización
    
    best_SSE = np.inf
    gamma = lr
    
    for t in range(max_iter):
        E = Y - A @ Theta
        SSE = np.sum(E ** 2)
        
        if SSE < tol:
            break
            
        if SSE < best_SSE:
            best_SSE = SSE
        else:
            print("Early stopping: Sin mejora")
            break
            
        current_gamma = gamma
        while current_gamma <= lr_max:
            e = E.flatten('F')
            delta_theta = solve(
                JtJ + current_gamma * np.diag(np.diag(JtJ)),
                -J.T @ e
            ).reshape((num_params, num_outputs), order='F')
            
            Theta_proposed = Theta + delta_theta
            E_new = Y - A @ Theta_proposed
            SSE_new = np.sum(E_new ** 2)
            
            if SSE_new < SSE:
                Theta = Theta_proposed
                gamma = max(current_gamma * lr_dec, 1e-10)
                break
            else:
                current_gamma *= lr_inc
        else:
            print("Advertencia: Gamma máximo alcanzado")
            break
    
    # Denormalización para cálculo correcto de R²
    Y_pred = (A @ Theta) * Y_std + Y_mean
    Y_true = Y * Y_std + Y_mean
    SSE = np.sum((Y_true - Y_pred) ** 2)
    SST = np.sum((Y_true - np.mean(Y_true, axis=0)) ** 2)
    R2 = 1 - (SSE / (SST + 1e-8))
    
    print(f"R² optimizado: {R2:.4f}")
    return Theta


# Uso
datasets = {
    'challenge00': 'challenge00_syntheticdataset22.txt',
    'challenge01': 'challenge01_syntheticdataset22.txt'
}

for name, path in datasets.items():
    print(f"\n--- Entrenando {name} ---")
    X, Y, stats = load_data(path)
    X_mean, X_std, Y_mean, Y_std = stats
    Theta = trainLM(X, Y, Y_mean, Y_std)
    print(f"Parámetros finales:\n{Theta}")