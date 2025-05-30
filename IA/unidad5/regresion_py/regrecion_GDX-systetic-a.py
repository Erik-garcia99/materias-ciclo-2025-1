



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
#------------------------------------------------------------------------------

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


def designMatrix(tau, X):
    """Matriz de diseño con términos polinómicos"""
    q, n = X.shape
    A_list = []
    for p in range(q):
        row = X[p, :]
        poly_terms = generate_polynomial_terms(row, tau)
        A_list.append(poly_terms)
    return np.vstack(A_list)





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
def generate_polynomial_terms(V, degree):
    """Genera términos polinómicos multivariados hasta el grado especificado"""
    n_features = len(V)
    terms = [1.0]  # Término constante
    
    for deg in range(1, degree + 1):
        for indices in combinations_with_replacement(range(n_features), deg):
            term = 1.0
            for idx in indices:
                term *= V[idx]
            terms.append(term)
    
    return np.array(terms)

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
    return sum(math.comb(n + d - 1, d) for d in range(tau + 1))




#------------------------------------------------------------------------------


def loss(Y_true, Y_pred, THETA, lambda_param):
    """ Mean Squared Error """
    E = Y_true - Y_pred
    SSE = np.square(np.linalg.norm(E, 'fro'))
    Reg = (lambda_param/(2*E.shape[0]))*np.square(np.linalg.norm(THETA[1:,:], 'fro'))
    MSE = SSE/(2*E.shape[0]) + Reg
    return MSE


#------------------------------------------------------------------------------

def gradient(A,E,THETA,lambda_param):
    """ MSE Gradient """
    SSEGrad = -1.0*(A.T).dot(E)
    MSEGrad = SSEGrad/E.shape[0]
    #modificacion en la condicion
    if THETA.shape[0] > 1:  # Evitar error si no hay parámetros para regularizar
        MSEGrad[1:,:] += (lambda_param / A.shape[0]) * THETA[1:,:]
    return MSEGrad



#------------------------------------------------------------------------------
#GDX

def gdx_optimization(X, Y, tau, lambda_param=0.0, maxEpochs=100, show=10, 
                     batch_size=64, learning_rate=0.01, momentum=0.9, 
                     lr_dec=0.5, lr_inc=1.05, max_perf_inc=1.04, 
                     stopping_threshold=1e-6):
    """Optimizador GDX con dimensiones corregidas"""
    # 1. Determinar dimensiones correctamente
    n_features = X.shape[1]
    n_outputs = Y.shape[1]
    
    # 2. Calcular número de parámetros polinómicos
    rho = polyParamsNumber(n_features, tau)
    
    # 3. Inicialización de parámetros
    THETA = np.random.randn(rho, n_outputs) * 0.01
    delta_THETA = np.zeros((rho, n_outputs))
    lr = learning_rate
    n_samples = X.shape[0]
    previous_loss = np.inf

    # 4. Bucle de entrenamiento
    for epoch in range(maxEpochs + 1):
        THETA_prev = THETA.copy()
        
        # Mezclar datos en cada época
        indices = np.random.permutation(n_samples)
        X_shuffled = X[indices]
        Y_shuffled = Y[indices]
        
        # Procesamiento por lotes
        for start in range(0, n_samples, batch_size):
            end = min(start + batch_size, n_samples)
            X_batch = X_shuffled[start:end]
            Y_batch = Y_shuffled[start:end]
            
            # 5. Generar matriz de diseño para el lote actual
            A_batch = designMatrix(tau, X_batch)
            
            # 6. Predicción y cálculo de error
            Y_pred = model(A_batch, THETA)
            E_batch = Y_batch - Y_pred
            
            # 7. Cálculo y ajuste del gradiente
            Grad = gradient(A_batch, E_batch, THETA, lambda_param)
            Grad = np.clip(Grad, -1e3, 1e3)  # Prevenir desbordamientos
            
            # 8. Actualización de parámetros con momento
            delta_THETA = momentum * delta_THETA - (1 - momentum) * lr * Grad
            THETA += delta_THETA
        
        # 9. Cálculo de pérdida completa después de la época
        A_full = designMatrix(tau, X)
        Y_pred_full = model(A_full, THETA)
        current_loss = loss(Y, Y_pred_full, THETA, lambda_param)
        
        # 10. Ajuste dinámico de tasa de aprendizaje
        if current_loss > previous_loss * max_perf_inc:
            THETA = THETA_prev
            lr *= lr_dec
        elif current_loss < previous_loss:
            lr *= lr_inc
        
        # 11. Monitoreo del progreso
        if epoch % show == 0:
            print(f"Epoch {epoch}: Loss={current_loss:.3e}, lr={lr:.2e}")
        
        # 12. Criterio de parada temprana
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

# Train and Test Data
# Train Data
xTrain = inputs_train
tTrain = targets_train
# Test Data
xTest = inputs_test
tTest = targets_test




#------------------------------------------------------------------------------




#mini batch
# Find the optimal parameters m and b with RMSprop
tau = 1
lambda_param = 0.1

THETA = gdx_optimization(
    xTrain,
    tTrain,
    tau=tau,
    lambda_param=lambda_param,
    maxEpochs=10000,
    show=500,
    batch_size=256,
    learning_rate=1e-8,
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
A_train = designMatrix(tau,xTrain)
outputTrain = model(A_train,THETA)
# Test data
A_test = designMatrix(tau,xTest)
outputTest = model(A_test,THETA)




#------------------------------------------------------------------------------


# R2 for raw train data
R2_train = r2_score(tTrain.reshape(-1, 1),outputTrain.reshape(-1, 1))
print(R2_train)


#------------------------------------------------------------------------------

# MSE for raw train data
MSE_train = mean_squared_error(tTrain.reshape(-1, 1),outputTrain.reshape(-1, 1))
print(MSE_train)


#------------------------------------------------------------------------------ 


# R2 for raw test data
R2_test = r2_score(tTest.reshape(-1, 1),outputTest.reshape(-1, 1))
print(R2_test)




#------------------------------------------------------------------------------ 




# MSE for raw test data
MSE_test = mean_squared_error(tTest.reshape(-1, 1),outputTest.reshape(-1, 1))
print(MSE_test)

#------------------------------------------------------------------------------ 


THETA






#------------------------------------------------------------------------------ 







#------------------------------------------------------------------------------ 






#------------------------------------------------------------------------------ 