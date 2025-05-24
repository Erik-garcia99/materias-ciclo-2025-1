#en este los datos no estan normalizados, no cambiar casi mucho mas que nada en donde empieza el algoritmo de optimizacion

#------------------------------
import numpy as np
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import RobustScaler
from sklearn.metrics import r2_score
from sklearn.metrics import mean_squared_error
import matplotlib.pyplot as plt

#-----------------------------------
#challenge04_syntheticdataset22.txt

def model(A, THETA):
    """
    Multivariate Linear Regression Model, Yh = A*THETA
    The matrix A is sometimes called the design matrix.
    """
    #hipotesis
    return A.dot(THETA) 

#-----------------------------------
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

#de que grad seria el polinomio 
# Power Vector
def powerVector(Tau,V):
    if V.size == 0 or Tau == 0:
        return 1
    else:
        M = np.array([])
        Z = V[:-1]
        W = V[-1]
        for k in range(Tau+1):
            value = powerVector(Tau-k,Z)*(W**k)
            M = np.append(M,value)
        return M


#-----------------------------------
# Polynomial parameter number
def polyParamsNumber(n,tau):
    s = 0
    for l in range(tau+1):
        val = np.math.factorial(l+n-1)/np.math.factorial(n-1)
        val = val/np.math.factorial(l)
        s = s + val
    return int(s)


#-----------------------------------
"""funcion de costo. lamba se usa si se va a regularizar  """
def loss(Y_true, Y_pred, THETA, lambda_param):
    """ Mean Squared Error """
    E = Y_true - Y_pred
    SSE = np.square(np.linalg.norm(E, 'fro')) #funcion de osot 
    Reg = (lambda_param/(2*E.shape[0]))*np.square(np.linalg.norm(THETA[1:,:], 'fro')) #regularizacion, porque 1 en adenta la norma, eso seria si nos interesa los parametros 
    MSE = SSE/(2*E.shape[0]) + Reg
    return MSE


#-----------------------------------
def gradient(A,E,THETA,lambda_param):
    """ MSE Gradient """
    SSEGrad = -1.0*(A.T).dot(E)
    MSEGrad = SSEGrad/E.shape[0]
    MSEGrad[1:,:] = MSEGrad[1:,:] + (lambda_param/E.shape[0])*THETA[1:,:]
    return MSEGrad



#-----------------------------------

def gdx_optimization(
    X,
    Y,
    tau=1,
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
    """
    GDX Optimizer: Maneja cualquier tamaño de lote (incluyendo residuos).
    """
    n = X.shape[1]
    m = Y.shape[1]
    rho = polyParamsNumber(n, tau)
    THETA = np.random.randn(rho, m)
    delta_THETA = np.zeros((rho, m))
    lr = learning_rate
    q = X.shape[0]
    previous_loss = np.inf

    for epoch in range(maxEpochs + 1):

        THETA_prev = THETA.copy()
        current_loss = np.inf

        # Mezclar datos si no es full-batch
        if batch_size < q:
            indices = np.random.permutation(q)
            X = X[indices, :]
            Y = Y[indices, :]

        # Calcular número de lotes y residuo
        n_batches = q // batch_size
        residual = q % batch_size

        # Añadir lote residual si existe
        total_batches = n_batches + 1 if residual != 0 else n_batches

        for batch_idx in range(total_batches):
            # Calcular índices del lote actual
            start = batch_idx * batch_size
            end = start + batch_size

            # Ajustar fin para el último lote (residual)
            if batch_idx == total_batches - 1 and residual != 0:
                end = start + residual

            # Extraer mini-lote
            X_batch = X[start:end, :]
            Y_batch = Y[start:end, :]

            # Calcular gradiente
            A_batch = designMatrix(tau, X_batch)
            Y_pred = model(A_batch, THETA)
            E_batch = Y_batch - Y_pred
            Grad = gradient(A_batch, E_batch, THETA, lambda_param)

            # Actualizar parámetros con momentum
            delta_THETA = momentum * delta_THETA - (1 - momentum) * lr * Grad
            THETA += delta_THETA

        # Calcular pérdida global
        A = designMatrix(tau, X)
        Yh = model(A, THETA)
        current_loss = loss(Y, Yh, THETA, lambda_param)

        # Adaptar tasa de aprendizaje
        if current_loss > previous_loss * max_perf_inc:
            THETA = THETA_prev  # Rollback
            lr *= lr_dec
        elif current_loss < previous_loss:
            lr *= lr_inc

        # Monitoreo y condición de parada
        if epoch % show == 0:
            print(f"Epoch {epoch}: Loss={current_loss:.3e}, lr={lr:.2e}")
        if abs(previous_loss - current_loss) < stopping_threshold:
            break
        previous_loss = current_loss

    return THETA


#-----------------------------------
#usando por ahora sin normalizar challenge04_syntheticdataset22.txt
# Load dataset
data = np.loadtxt('challenge01_syntheticdataset22.txt',delimiter=',')
inputs  = data[:,0:2]
targets = data[:,2:4]

#-----------------------------------
# Train and Test Split Data
inputs_train, inputs_test, targets_train, targets_test = train_test_split(inputs, targets, random_state = 1, test_size = 0.4)

#-----------------------------------
# Train and Test Data
# Train Data
xTrain = inputs_train
tTrain = targets_train
# Test Data
xTest = inputs_test
tTest = targets_test

#-----------------------------------

# Find the optimal parameters m and b with RMSprop
tau = 1
lambda_param = 0.0

# THETA = rmsprop_optimization(
#     xTrain,
#     tTrain,
#     tau,
#     lambda_param,
#     maxEpochs=1000,
#     show=100,
#     batch_size=16,
#     learning_rate=0.01,
#     stopping_threshold=1e-6,
#     beta=0.9,
#     epsilon=1e-8,
# )

THETHA = gdx_optimization(


    xTrain,
    tTrain,
    tau,
    lambda_param,
    maxEpochs=1000,
    show=100,
    batch_size=16,
    learning_rate=0.01,
    stopping_threshold=1e-6,
    beta=0.9,
    epsilon=1e-8,
)


#-----------------------------------

# Make predictions
# Train data
A_train = designMatrix(tau,xTrain)
outputTrain = model(A_train,THETA)
# Test data
A_test = designMatrix(tau,xTest)
outputTest = model(A_test,THETA)




#-----------------------------------

# R2 for raw train data
R2_train = r2_score(tTrain.reshape(-1, 1),outputTrain.reshape(-1, 1))
print(R2_train)


#-----------------------------------

# MSE for raw train data, #varianza 
MSE_train = mean_squared_error(tTrain.reshape(-1, 1),outputTrain.reshape(-1, 1))
print(MSE_train)


#-----------------------------------

# R2 for raw test data, datos crudos para los datos de prueba 
R2_test = r2_score(tTest.reshape(-1, 1),outputTest.reshape(-1, 1))
print(R2_test)
#-----------------------------------


# MSE for raw test data, prueba no utilizados en el entrenamineto 
MSE_test = mean_squared_error(tTest.reshape(-1, 1),outputTest.reshape(-1, 1))
print(MSE_test)
#-----------------------------------


print(THETA)
#-----------------------------------



#-----------------------------------



#-----------------------------------



#-----------------------------------



#-----------------------------------




