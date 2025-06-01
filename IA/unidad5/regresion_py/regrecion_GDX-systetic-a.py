



#este documento contiene datos no normalizados con el formato .mat por lo que es un poco diferente. 
#este docuemnto utiliza el engine_dataset.mat
#------------------------------------------------------------------------------
import numpy as np
import scipy.io as sp
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import RobustScaler
from sklearn.metrics import r2_score
from sklearn.metrics import mean_squared_error
import matplotlib.pyplot as plt
import math 
#agregacion
from itertools import combinations_with_replacement
from sklearn.preprocessing import RobustScaler
from sklearn.preprocessing import PolynomialFeatures
#---------------------------------------------------
# ---------------------------

def model(A, THETA):
    """
    Multivariate Linear Regression Model, Yh = A*THETA
    The matrix A is sometimes called the design matrix.
    """
    return A.dot(THETA)

#------------------------------------------------------------------------------
# Design Matrix Modificacion
"""
def designMatrix(Tau,X):
    q,_ = X.shape
    for p in range(q):
        M = powerVector(Tau,X[p,:])
        if p == 0:
            A = M
            continue
        A = np.vstack((A,M))
    return A"""

#funcion modificada


# def designMatrix(tau, X):
#     """Matriz de diseño con términos polinómicos"""
#     q, n = X.shape
#     A_list = []
#     for p in range(q):
#         row = X[p, :]
#         poly_terms = generate_polynomial_terms(row, tau)
#         A_list.append(poly_terms)
#     return np.vstack(A_list)

def designMatrix(tau, X):
    n_samples, n_features = X.shape
    n_terms = polyParamsNumber(n_features, tau)
    A = np.empty((n_samples, n_terms))
    
    for i in range(n_samples):
        terms = generate_polynomial_terms(X[i], tau)
        terms = np.nan_to_num(terms, nan=0.0, posinf=1e10, neginf=-1e10)
        A[i] = terms
    
    return A



# Power Vector M
# def powerVector(Tau, V):
#     if V.size == 0 or Tau == 0:
#         return np.array([1.0])
#     Z = V[:-1]
#     W = V[-1]
#     terms = []
#     for k in range(Tau + 1):
#         sub_terms = powerVector(Tau - k, Z)
#         for term in sub_terms:
#             terms.append(term * (W ** k))
#     return np.array(terms)

#este tennemos con el engine y funciona 
"""
def powerVector(Tau, V):
    if V.size == 0 or Tau == 0:
        return np.array([[1.0]])  # Asegurar 2D
    Z = V[:-1]
    W = V[-1]
    terms = []
    for k in range(Tau + 1):
        sub_terms = powerVector(Tau - k, Z)
        for term in sub_terms.flatten():  # Manejar sub_terms 2D
            terms.append(term * (W ** k))
    return np.array([terms])  # Devolver como 2D (1, n_terms)
"""

# #modificacion - agregacio 
# def powerVector(Tau, V):
#     """Genera términos polinomiales multivariados de grado <= Tau de forma iterativa."""
#     n = V.size
#     terms = []
#     # Generar todas las combinaciones de exponentes para las n variables
#     exponents = itertools.product(range(Tau + 1), repeat=n)
#     for exp in exponents:
#         if sum(exp) <= Tau:  # Solo términos hasta el grado Tau
#             term = 1.0
#             for i in range(n):
#                 term *= V[i] ** exp[i]
#             terms.append(term)
#     return np.array([terms])  # Devolver como array 2D (1, n_terms)


#Generar términos polinómicos de manera iterativa -> esta sustituyendo a powerVector
# def generate_polynomial_terms(V, degree):
#     """Genera términos polinómicos multivariados hasta el grado especificado"""
#     n_features = len(V)
#     terms = [1.0]  # Término constante
    
#     for deg in range(1, degree + 1):
#         for indices in combinations_with_replacement(range(n_features), deg):
#             term = 1.0
#             for idx in indices:
#                 term *= V[idx]
#             terms.append(term)
    
#     return np.array(terms)



def generate_polynomial_terms(V, degree):
    """Genera términos polinómicos multivariados hasta el grado especificado"""
    n_features = len(V)
    terms = [1.0]  # Término constante
    
    # Generar términos de grado 1
    if degree >= 1:
        terms.extend(V)
    
    # Generar términos de grado 2 y superiores
    if degree >= 2:
        # Términos cuadráticos puros
        terms.extend(V[i]*V[i] for i in range(n_features))
        
        # Términos cruzados
        for i in range(n_features):
            for j in range(i+1, n_features):
                terms.append(V[i] * V[j])
    
    return np.array(terms)

#-------------------------------------------------------------------------------

def initialize_theta(rho, n_outputs):
    # Inicialización Xavier/Glorot
    stdv = 1. / np.sqrt(rho)
    return np.random.uniform(-stdv, stdv, size=(rho, n_outputs))


#------------------------------------------------------------------------------
#Polynomial parameter number
# def polyParamsNumber(n,tau):
#     s = 0
#     for l in range(tau+1):
#         #val = np.math.factorial(l+n-1)/np.math.factorial(n-1)
#         val = math.factorial(l + n - 1) / (math.factorial(n - 1) * math.factorial(l))
#         val = val/np.math.factorial(l)
#         s = s + val
#     return int(s)


# def polyParamsNumber(n,tau):
#     s = 0
#     for l in range(tau+1):
#         val = math.factorial(l + n - 1) / (math.factorial(n - 1) * math.factorial(l))
#         val = val / math.factorial(l)  # Usar math en lugar de np.math
#         s += val
#     return int(s)

def polyParamsNumber(n, tau):
    """Calcula eficientemente el número de parámetros"""
    # Fórmula cerrada para grados bajos
    if tau == 1:
        return n + 1
    elif tau == 2:
        return (n * (n + 1)) // 2 + n + 1
    else:
        # Método general para grados más altos (aunque no se recomienda para tau > 2)
        return sum(math.comb(n + d - 1, d) for d in range(tau + 1))



#------------------------------------------------------------------------------


# def loss(Y_true, Y_pred, THETA, lambda_param):
#     """ Mean Squared Error """
#     E = Y_true - Y_pred
#     SSE = np.square(np.linalg.norm(E, 'fro'))
#     Reg = (lambda_param/(2*E.shape[0]))*np.square(np.linalg.norm(THETA[1:,:], 'fro'))
#     MSE = SSE/(2*E.shape[0]) + Reg
#     return MSE

def loss(Y_true, Y_pred, THETA, lambda_param):
    E = Y_true - Y_pred
    SSE = np.square(np.linalg.norm(E, 'fro'))
    n = E.shape[0]
    if n == 0:  # Evitar división por cero
        n = 1
    Reg = (lambda_param / (2 * n)) * np.square(np.linalg.norm(THETA[1:, :], 'fro'))
    MSE = SSE / (2 * n) + Reg
    return MSE

#------------------------------------------------------------------------------

# def gradient(A,E,THETA,lambda_param):
#     """ MSE Gradient """
#     SSEGrad = -1.0*(A.T).dot(E)
#     MSEGrad = SSEGrad/E.shape[0]
#     #modificacion en la condicion
#     if THETA.shape[0] > 1:  # Evitar error si no hay parámetros para regularizar
#         MSEGrad[1:,:] += (lambda_param / A.shape[0]) * THETA[1:,:]
#     return MSEGrad

def gradient(A, E, THETA, lambda_param):
    n_samples = A.shape[0]
    if n_samples == 0:  # Evitar división por cero
        n_samples = 1
    
    SSEGrad = -1.0 * (A.T).dot(E)
    MSEGrad = SSEGrad / n_samples
    
    if THETA.shape[0] > 1:
        # Solo regularizar si hay suficientes parámetros
        MSEGrad[1:, :] += (lambda_param / n_samples) * THETA[1:, :]
    
    # Clip más estricto para evitar desbordamientos
    return np.clip(MSEGrad, -1e2, 1e2)

#------------------------------------------------------------------------------
#GDX

# def gdx_optimization(X, Y, tau, lambda_param=0.0, maxEpochs=100, show=10, 
#                      batch_size=64, learning_rate=0.01, momentum=0.9, 
#                      lr_dec=0.5, lr_inc=1.05, max_perf_inc=1.04, 
#                      stopping_threshold=1e-6):
    
#     A_full = designMatrix(tau, X)
#     rho = A_full.shape[1]
#     n_features = X.shape[1]
#     n_outputs = Y.shape[1]
    
   
    
#     THETA = np.random.randn(rho, n_outputs) * 0.01
#     delta_THETA = np.zeros((rho, n_outputs))
#     lr = learning_rate
#     n_samples = X.shape[0]
#     previous_loss = np.inf

   
#     for epoch in range(maxEpochs + 1):
#         THETA_prev = THETA.copy()
        
        
#         indices = np.random.permutation(n_samples)
#         # X_shuffled = X[indices]
#         # Y_shuffled = Y[indices]
        
#         # Procesamiento por lotes
#         for start in range(0, n_samples, batch_size):
#             end = min(start + batch_size, n_samples)
#             # X_batch = X_shuffled[start:end]
#             # Y_batch = Y_shuffled[start:end]
            
#             # 5. Generar matriz de diseño para el lote actual
#             # A_batch = designMatrix(tau, X_batch)

#             A_batch = A_full[batch_idx]
#             Y_batch = Y[batch_idx]
            
#             # 6. Predicción y cálculo de error
#             Y_pred = model(A_batch, THETA)
#             E_batch = Y_batch - Y_pred
            
#             # 7. Cálculo y ajuste del gradiente
#             Grad = gradient(A_batch, E_batch, THETA, lambda_param)
#             Grad = np.clip(Grad, -1e3, 1e3)  # Prevenir desbordamientos
            
#             # 8. Actualización de parámetros con momento
#             delta_THETA = momentum * delta_THETA - (1 - momentum) * lr * Grad
#             THETA += delta_THETA
        
#         # 9. Cálculo de pérdida completa después de la época
#         A_full = designMatrix(tau, X)
#         Y_pred_full = model(A_full, THETA)
#         current_loss = loss(Y, Y_pred_full, THETA, lambda_param)
        
#         # 10. Ajuste dinámico de tasa de aprendizaje
#         if current_loss > previous_loss * max_perf_inc:
#             THETA = THETA_prev
#             lr *= lr_dec
#         elif current_loss < previous_loss:
#             lr *= lr_inc
        
#         # 11. Monitoreo del progreso
#         if epoch % show == 0:
#             print(f"Epoch {epoch}: Loss={current_loss:.3e}, lr={lr:.2e}")
        
#         # 12. Criterio de parada temprana
#         if abs(previous_loss - current_loss) < stopping_threshold:
#             print(f"Early stopping at epoch {epoch}")
#             break
            
#         previous_loss = current_loss

#     return THETA

def gdx_optimization(X, Y, tau, lambda_param=0.0, maxEpochs=100, show=10, 
                     batch_size=64, learning_rate=0.01, momentum=0.9, 
                     lr_dec=0.5, lr_inc=1.05, max_perf_inc=1.04, 
                     stopping_threshold=1e-6):
 

    n_samples, n_features = X.shape
    n_outputs = Y.shape[1]
    rho = polyParamsNumber(n_features, tau)
    

    print(f"precalculando matriz de disenio (tau={tau}, terminos={rho})...")
    A_full = designMatrix(tau, X)
    print("matriz de disenio precalculada")
    

    THETA = THETA = initialize_theta(rho, n_outputs)
    delta_THETA = np.zeros((rho, n_outputs))
    lr = learning_rate
    previous_loss = np.inf
    n_batches = (n_samples + batch_size - 1) // batch_size  
    
   
    for epoch in range(maxEpochs + 1):
        THETA_prev = THETA.copy()
        
        # Mezclar índices
        indices = np.random.permutation(n_samples)
        
        
        for batch_idx in range(n_batches):
            start = batch_idx * batch_size
            end = min((batch_idx + 1) * batch_size, n_samples)
            idx = indices[start:end]
            
            
            A_batch = A_full[idx]
            Y_batch = Y[idx]
            
          
            Y_pred = model(A_batch, THETA)
            E_batch = Y_batch - Y_pred
            
          
            Grad = gradient(A_batch, E_batch, THETA, lambda_param)
            Grad = np.clip(Grad, -1e3, 1e3)
            
           
            delta_THETA = momentum * delta_THETA - (1 - momentum) * lr * Grad
            THETA += delta_THETA
        
       
        Y_pred_full = model(A_full, THETA)
        current_loss = loss(Y, Y_pred_full, THETA, lambda_param)
        
       
        if current_loss > previous_loss * max_perf_inc:
            THETA = THETA_prev
            lr *= lr_dec
            print(f"Reduciendo lr a {lr:.2e}")
        elif current_loss < previous_loss:
            lr *= lr_inc
    
        if epoch % show == 0 or epoch == maxEpochs:
            print(f"Epoch {epoch}: Loss={current_loss:.3e}, lr={lr:.2e}")
        
        
        if abs(previous_loss - current_loss) < stopping_threshold:
            print(f"Early stopping at epoch {epoch}")
            break
            
        previous_loss = current_loss

    return THETA


#------------------------------------------------------------------------------


# Load dataset
# mat = sp.loadmat('engine_dataset.mat')
# print(mat.keys())
# # inputs  = mat['engineInputs'].T
# # targets = mat['engineTargets'].T


#////////////////////////////////////////////


mat = sp.loadmat('challenge04_syntheticdataset22.mat')
# print(mat.keys())
inputs  = mat['inputs'].T
targets = mat['targets'].T




#------------------------------------------------------------------------------

# Train and Test Split Data
inputs_train, inputs_test, targets_train, targets_test = train_test_split(inputs, targets, random_state = 1, test_size = 0.4)



#------------------------------------------------------------------------------
#normlaizar datos

scaler_x = RobustScaler()
scaler_t = RobustScaler()
xTrain = scaler_x.fit_transform(inputs_train)
tTrain = scaler_t.fit_transform(targets_train)
xTest = scaler_x.transform(inputs_test)
tTest = scaler_t.transform(targets_test)

#------------------------------------------------------------------------------

# # Train and Test Data
# # Train Data
# xTrain = inputs_train
# tTrain = targets_train
# # Test Data
# xTest = inputs_test
# tTest = targets_test




#------------------------------------------------------------------------------




#mini batch
# Find the optimal parameters m and b with RMSprop
tau = 1
lambda_param = 0.01

THETA = gdx_optimization(
    xTrain,
    tTrain,
    tau=tau,
    lambda_param=lambda_param,
    maxEpochs=10,
    show=1,
    batch_size=256,
    learning_rate=1e-4,
    momentum=0.9,          
    lr_dec=0.5,           
    lr_inc=1.05,
    max_perf_inc=1.04,
    stopping_threshold=1e-6,
)
#------------------------------------------------------------------------------

#ONLINE (1 a 1)
"""
tau = 2
lambda_param = 0.001
THETA = gdx_optimization(
    xTrain,
    tTrain,
    tau=tau,
    lambda_param=lambda_param,
    maxEpochs=100000,
    show=1000,
    batch_size=1,
    learning_rate=1e-6,
    momentum=0.9,          
    lr_dec=0.5,           
    lr_inc=1.05,
    max_perf_inc=1.04,
    stopping_threshold=1e-6,
)"""

#-----------------------------------------------------------------------

# #lote
# tau = 1
# lambda_param = 0.001
# THETA = gdx_optimization(
#     xTrain,
#     tTrain,
#     tau=tau,
#     lambda_param=lambda_param,
#     maxEpochs=10000,
#     show=500,
#     batch_size=xTrain.shape[0],
#     learning_rate=1e-4,
#     momentum=0.9,          
#     lr_dec=0.5,           
#     lr_inc=1.05,
#     max_perf_inc=1.04,
#     stopping_threshold=1e-6,
# )





#-----------------------------------------------------------------------
# Make predictions
# Train data
A_train = designMatrix(tau, xTrain)
A_test = designMatrix(tau, xTest)

# Predecir
outputTrain_scaled = model(A_train, THETA)
outputTest_scaled = model(A_test, THETA)


#------------------------------------------------------------------------------
#desnormalizar

outputTrain = scaler_t.inverse_transform(outputTrain_scaled)
outputTest = scaler_t.inverse_transform(outputTest_scaled)
tTrain_orig = scaler_t.inverse_transform(tTrain)
tTest_orig = scaler_t.inverse_transform(tTest)

#------------------------------------------------------------------------------


# R2 for raw train data
# R2_train = r2_score(tTrain_orig.reshape(-1, 1),outputTrain.reshape(-1, 1))
R2_train = r2_score(tTrain_orig, outputTrain)
print(R2_train)


#------------------------------------------------------------------------------

# MSE for raw train data
# MSE_train = mean_squared_error(tTrain_orig.reshape(-1, 1),outputTrain.reshape(-1, 1))
MSE_train = mean_squared_error(tTrain_orig, outputTrain)
print(MSE_train)


#------------------------------------------------------------------------------ 

# R2 for raw test data
# R2_test = r2_score(tTest_orig.reshape(-1, 1),outputTest.reshape(-1, 1))
R2_test = r2_score(tTest_orig, outputTest)
print(R2_test)




#------------------------------------------------------------------------------ 





# MSE for raw test data
# MSE_test = mean_squared_error(tTest_orig.reshape(-1, 1),outputTest.reshape(-1, 1))
MSE_test = mean_squared_error(tTest_orig, outputTest)
print(MSE_test)

#------------------------------------------------------------------------------ 


THETA






#------------------------------------------------------------------------------ 







#------------------------------------------------------------------------------ 






#------------------------------------------------------------------------------ 