#----------------------------------------------------------------
import numpy as np
import scipy.io as sp
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import RobustScaler
from sklearn.metrics import r2_score
from sklearn.metrics import mean_squared_error
import matplotlib.pyplot as plt
import math 



#----------------------------------------------------------------
def model(A, THETA):
    """
    Multivariate Linear Regression Model, Yh = A*THETA
    The matrix A is sometimes called the design matrix.
    """
    return A.dot(THETA)


#----------------------------------------------------------------

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


#----------------------------------------------------------------

#Polynomial parameter number
def polyParamsNumber(n, tau):
    return int(math.comb(n + tau, tau))


#----------------------------------------------------------------


def loss(Y_true, Y_pred, THETA, lambda_param):
    """ Mean Squared Error """
    E = Y_true - Y_pred
    SSE = np.square(np.linalg.norm(E, 'fro'))
    Reg = (lambda_param/(2*E.shape[0]))*np.square(np.linalg.norm(THETA[1:,:], 'fro'))
    MSE = SSE/(2*E.shape[0]) + Reg
    return MSE



#----------------------------------------------------------------
def gradient(A,E,THETA,lambda_param):
    """ MSE Gradient """
    SSEGrad = -1.0*(A.T).dot(E)
    MSEGrad = SSEGrad/E.shape[0]
    MSEGrad[1:,:] = MSEGrad[1:,:] + (lambda_param/E.shape[0])*THETA[1:,:]
    return MSEGrad





#----------------------------------------------------------------
# AdamD Optimizer
def adamd_optimization(
    X,
    Y,
    tau,
    lambda_param=0.0,
    maxEpochs=100,
    show=10,
    batch_size=16,
    learning_rate=0.001,
    beta1=0.9,
    beta2=0.999,
    epsilon=1e-8,
    stopping_threshold=1e-6,
):
    """Optimizador AdamD según el algoritmo proporcionado"""
    n = X.shape[1]
    m = Y.shape[1]
    rho = polyParamsNumber(n, tau)
    THETA = np.random.randn(rho, m) * 0.01
    q = X.shape[0]
    
    # Inicializar momentos
    m_t = np.zeros_like(THETA)
    v_t = np.zeros_like(THETA)
    
    t = 0
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
            t += 1  # Incrementar timestep
            
            start = batch_idx * batch_size
            end = start + batch_size
            if batch_idx == total_batches - 1 and residual != 0:
                end = start + residual

            X_batch = X[start:end, :]
            Y_batch = Y[start:end, :]
            A_batch = designMatrix(tau, X_batch)
            Y_pred = model(A_batch, THETA)
            E_batch = Y_batch - Y_pred
            g_t = gradient(A_batch, E_batch, THETA, lambda_param)
            
            # Actualizar momentos
            m_t = beta1 * m_t + (1 - beta1) * g_t
            v_t = beta2 * v_t + (1 - beta2) * (g_t ** 2)
            
            # Calcular learning rate para este timestep
            alpha_t = learning_rate * np.sqrt(1 - beta2 ** t)
            
            # Actualizar parámetros
            THETA -= alpha_t * m_t / (np.sqrt(v_t) + epsilon)

        A = designMatrix(tau, X)
        Yh = model(A, THETA)
        current_loss = loss(Y, Yh, THETA, lambda_param)

        if epoch % show == 0:
            print(f"Epoch {epoch}: Loss={current_loss:.3e}, lr={alpha_t:.2e}")

        if abs(previous_loss - current_loss) < stopping_threshold:
            break
            
        previous_loss = current_loss

    return THETA





#----------------------------------------------------------------
# Load dataset
mat = sp.loadmat('engine_dataset.mat.mat')
inputs  = mat['engineInputs'].T
targets = mat['engineTargets'].T






#----------------------------------------------------------------
# Train and Test Split Data
inputs_train, inputs_test, targets_train, targets_test = train_test_split(inputs, targets, random_state = 1, test_size = 0.4)




#----------------------------------------------------------------

# Train and Test Data
xTrain = inputs_train
tTrain = targets_train
xTest = inputs_test
tTest = targets_test



#----------------------------------------------------------------


#mini lote
# Find the optimal parameters with AdamD
tau = 1
lambda_param = 0.1

THETA = adamd_optimization(
    xTrain,
    tTrain,
    tau=tau,
    lambda_param=lambda_param,
    maxEpochs=10000,
    show=500,
    batch_size=64,
    learning_rate=0.001,
    beta1=0.9,
    beta2=0.999,
    epsilon=1e-8,
    stopping_threshold=1e-6,
)



#----------------------------------------------------------------

# Make predictions
# Train data
A_train = designMatrix(tau,xTrain)
outputTrain = model(A_train,THETA)
# Test data
A_test = designMatrix(tau,xTest)
outputTest = model(A_test,THETA)



#----------------------------------------------------------------

# Metrics
# Train data
R2_train = r2_score(tTrain.reshape(-1, 1), outputTrain.reshape(-1, 1))
print(R2_train)



#----------------------------------------------------------------
MSE_train = mean_squared_error(tTrain.reshape(-1, 1), outputTrain.reshape(-1, 1))
print(MSE_train)

#----------------------------------------------------------------

# Test data
R2_test = r2_score(tTest.reshape(-1, 1), outputTest.reshape(-1, 1))
print(R2_test)


#----------------------------------------------------------------


MSE_test = mean_squared_error(tTest.reshape(-1, 1), outputTest.reshape(-1, 1))
print(MSE_test)

#----------------------------------------------------------------
THETA




#----------------------------------------------------------------