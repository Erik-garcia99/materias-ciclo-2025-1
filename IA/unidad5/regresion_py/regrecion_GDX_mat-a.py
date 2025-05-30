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

#------------------------------------------------------------------------------

def model(A, THETA):
    """
    Multivariate Linear Regression Model, Yh = A*THETA
    The matrix A is sometimes called the design matrix.
    """
    return A.dot(THETA)

#------------------------------------------------------------------------------
# Design Matrix
def designMatrix(Tau,X):
    q,_ = X.shape
    for p in range(q):
        M = powerVector(Tau,X[p,:])
        if p == 0:
            A = M
            continue
        A = np.vstack((A,M))
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
    return int(math.comb(n + tau, tau))




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
    MSEGrad[1:,:] = MSEGrad[1:,:] + (lambda_param/E.shape[0])*THETA[1:,:]
    return MSEGrad



#------------------------------------------------------------------------------
#GDX
def gdx_optimization(
    X,
    Y,
    tau,
    lambda_param=0.0,
    maxEpochs=100,
    show=10,
    batch_size=16,
    learning_rate=0.01,
    momentum=0.9,
    lr_dec=0.5,
    lr_inc=1.05,
    max_perf_inc=1.04,
    stopping_threshold=1e-6,
):
    """Optimizador GDX con manejo de tamaño de lote variable"""
    n = X.shape[1]
    m = Y.shape[1]
    rho = polyParamsNumber(n, tau)
    #rho me esta dando porbelmas 
    THETA = np.random.randn(rho, m) *0.01 #reduce la escala de la inicalizaciones de los pesos
    delta_THETA = np.zeros((rho, m))
    lr = learning_rate
    q = X.shape[0]
    previous_loss = np.inf

    for epoch in range(maxEpochs + 1):
        THETA_prev = THETA.copy()
        current_loss = np.inf

        if batch_size < q:
            indices = np.random.permutation(q)
            X = X[indices, :]
            Y = Y[indices, :]

        n_batches = q // batch_size
        residual = q % batch_size
        total_batches = n_batches + 1 if residual != 0 else n_batches

        for batch_idx in range(total_batches):
            start = batch_idx * batch_size
            end = start + batch_size
            if batch_idx == total_batches - 1 and residual != 0:
                end = start + residual

            X_batch = X[start:end, :]
            Y_batch = Y[start:end, :]
            # A_batch = designMatrix(tau, X_batch)
            # Y_pred = model(A_batch, THETA)
            # E_batch = Y_batch - Y_pred

            A_batch = designMatrix(tau, X_batch)  # (batch_size, rho)
            Y_pred = model(A_batch, THETA)  # (batch_size, m)
            E_batch = Y_batch - Y_pred  # (batch_size, m)
            Grad = gradient(A_batch, E_batch, THETA, lambda_param)
            #evitar desbordamienteos 
            Grad = np.clip(Grad, -1e3, 1e3)
            delta_THETA = momentum * delta_THETA - (1 - momentum) * lr * Grad
            THETA += delta_THETA

        A = designMatrix(tau, X)
        Yh = model(A, THETA)
        current_loss = loss(Y, Yh, THETA, lambda_param)

        if current_loss > previous_loss * max_perf_inc:
            THETA = THETA_prev
            lr *= lr_dec
        elif current_loss < previous_loss:
            lr *= lr_inc

        if epoch % show == 0:
            print(f"Epoch {epoch}: Loss={current_loss:.3e}, lr={lr:.2e}")
        if abs(previous_loss - current_loss) < stopping_threshold:
            break
        previous_loss = current_loss

    return THETA


#------------------------------------------------------------------------------

# Load dataset
# mat = sp.loadmat('engine_dataset.mat')
# print(mat.keys())
# # inputs  = mat['engineInputs'].T
# # targets = mat['engineTargets'].T

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