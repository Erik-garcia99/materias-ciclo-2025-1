import numpy as np
import os

# objective Extended Rosenbrock function
'''calcula el valor de la funcion objetivo, la funcion tiene un minumo en el punto (1,1,....,1)
n representa el nuermo de dimensiones del problema 
'''
def objfcn(x):
    # Minima -> f=0 at (1,.....,1)
    n = len(x) # n par
    z = np.zeros((n,1))
    l2 = np.array(range(0,n,2)) # indice par
    l1 = np.array(range(1,n,2)) # indice impar
    z[l2] = 10.0 * (x[l1] - (x[l2])**2.0)
    z[l1] = 1.0 - x[l2]
    f = z.T @ z
    return f[0,0]

# Extended Rosenbrock gradient function
def objfcngrad(x):
    n = len(x) # n even
    Jz = np.zeros((n,n))
    z = np.zeros((n,1))
    l2 = np.array(range(0,n,2)) # indice par
    l1 = np.array(range(1,n,2)) # indice impar
    z[l2] = 10.0 * (x[l1] - (x[l2])**2.0)
    z[l1] = 1.0 - x[l2]

    for i in range(n//2):
        Jz[2*i, 2*i]     = -20.0 * x[2*i, 0]
        Jz[2*i, 2*i+1]   = 10.0
        Jz[2*i+1, 2*i]   = -1.0

    gX = 2.0 * Jz.T @ z
    return gX

def objfcnjac(x):
    # Extended Rosenbrock Jacobian Function
    n = len(x) # n even
    Jz = np.zeros((n,n))
    z = np.zeros((n,1))
    l2 = np.array(range(0,n,2)) # indice par
    l1 = np.array(range(1,n,2)) # indice impar
    z[l2]=10.0*(x[l1]-(x[l2])**2.0)
    z[l1]=1.0-x[l2]
    for i in range(n//2):
        Jz[2*i,2*i] = -20.0*x[2*i]
        Jz[2*i,2*i+1] = 10.0
        Jz[2*i+1,2*i] = -1.0
    gX = 2.0*Jz.T @ z
    normgX = np.linalg.norm(gX)
    return z, Jz, normgX


################################################################################
#funciones para la meta 3.2

#funcion de lectura OK
#funcion de la hipotesis OK
#funcoin del costo OK
#funcion del gradiente

#lecutra esta OK
#usamos la que no los serpara
def lectura(URL):
    X_data = []
    Y_data = []
    with open(URL, 'r') as archivo:
        for linea in archivo:
            linea_limpia = linea.strip()
            if not linea_limpia:
                continue
            # Separar por espacios y dividir en predictores (primeros 2) y respuestas (últimos 2)
            valores = list(map(float, linea_limpia.split(',')))  # Usar ',' como separador
            if len(valores) != 4:
                continue  # Cada línea debe tener 4 números: x1, x2, y1, y2
            X_data.append(valores[:2])  # Primeros dos: predictores
            Y_data.append(valores[2:])  # Últimos dos: respuestas
    
    # Construir matriz de diseño A = [X | 1]
    X_matrix = np.array(X_data)
    unos = np.ones((X_matrix.shape[0], 1))
    A = np.hstack((X_matrix, unos))
    Y = np.array(Y_data)
    return A, Y




#hipotesis esta OK
def hipotesis(A, THETA):
    return A @ THETA


def funcion_costo(Y, Y_hat, A):

    E= Y-Y_hat #calcula el error que seria XI matriz de erroes distribuida normalemnte 



    E_vector_F= E.flatten(order='F') #matriz vectorizada en forma de fila
    E_vector_C=E.flatten(order='F').reshape(-1, 1) #matriz vectroizada en manera de columna
    
    #SEE = E_vector_F @ E_vector_C #multiplicacion para calcular el costo

    SEE =np.sum(E**2)
    g=Y.shape[0] #entradas
    m=Y.shape[1] #salidas

    MSE = SEE/(g*m)
    RMSE = np.sqrt(MSE)     
    return E, SEE, MSE, RMSE


def fcnGrad(A,E):
    return -2*A.T @ E



# Función para calcular R^2 (agrégala junto a las otras funciones)
# def calcular_r2(Y_real, Y_pred):
#     """
#     Calcula el coeficiente de determinación R².
    
#     Args:
#         Y_real (np.ndarray): Valores reales (matriz n x m)
#         Y_pred (np.ndarray): Valores predichos (matriz n x m)
        
#     Returns:
#         float: Valor de r2 entre 0 y 1
#     """
#     # Aplanar las matrices a vectores
#     y_real = Y_real.flatten()
#     y_pred = Y_pred.flatten()
    
#     # Calcular suma de cuadrados
#     ss_res = np.sum((y_real - y_pred)**2)
#     ss_tot = np.sum((y_real - np.mean(y_real))**2)
    
#     # Evitar división por cero
#     if ss_tot == 0:
#         return 1.0  # Todos los valores son iguales
    
#     r2 = 1 - (ss_res / ss_tot)
#     return r2

def calcular_r2(Y_real, Y_pred):
    """Coeficiente de determinación R²"""
    ss_res = np.sum((Y_real - Y_pred)**2)
    ss_tot = np.sum((Y_real - np.mean(Y_real, axis=0))**2)
    return 1 - (ss_res / ss_tot)
##############################################################################################

def trainGDX(A,Y, x_range, max_iter, show, lr_init=1e-3, momentum=0.9, lr_dec=0.7, lr_inc=1.05, max_perf_inc=1.04, tol=1e-8):


    #dimension del problema 
    q,g = A.shape

    m=Y.shape[1]
    n_params=g*m #total de parametro a optimizar

    #inician los parametros 
    theta_flat= np.random.uniform(x_range[0],x_range[1],(n_params,1)) #estos los mandare en forma de tupla

    #estado incial
    theta=theta_flat.reshape(g,m)
    Y_hat = hipotesis(A,theta)
    E,SEE,_,_=funcion_costo(Y,Y_hat,A)

    performace = fcnGrad(A,E) #valor iniciar de la funcion objetivo  grad_matrix
    grad_flat=performace.reshape(-1,1)

    delta_x = np.zeros_like(theta_flat)
    lr = lr_init
    trayectoria = [theta_flat.copy()] # guarda los esstados de x en cada iteracion, utiliza la funcion .copy(), para no alterar los demas valores anteriores a la iteracion actual 
    grad_norm = np.linalg.norm(grad_flat) #norma del gradiente inical, se calcula la norma euclidiana del vector x actual durante la optimización

    for epoch in range(max_iter+1):

        #condicion de parada. 
        if grad_norm < tol:
            print(f"convergencia alcanzada en {epoch} iteraciones")
            break


        if show > 0 and (epoch % show == 0 or epoch == 0):
            print(f"Iter {epoch}: costo ={SEE:.4e} |Grad|= {grad_norm:.4e} lr ={lr:.2e}")


        # Actualizar delta_x usando momentum
        delta_x = momentum * delta_x - (1 - momentum) * lr * grad_flat

        # Guardar el estado actual antes de actualizar
        theta_prev = theta_flat.copy()
        SEE_prev = SEE
        delta_x_prev = delta_x.copy()
        
        theta_flat+=delta_x # DDDDDDDD:<<<<<<<<<

        # Actualizar x
        theta = theta_flat.reshape(g,m)
        Y_hat=hipotesis(A,theta)
        E,SEE,_,_ = funcion_costo(Y,Y_hat,A)
        performace = fcnGrad(A,E)
        grad_flat = performace.reshape(-1,1)
        grad_norm = np.linalg.norm(grad_flat)

        # control adaptativo de tasa de aprendizaje
        if SEE / SEE_prev > max_perf_inc:
            # Revertir los cambios
            theta_flat = theta_prev
            SEE=SEE_prev
            delta_x = delta_x_prev
            # Reducir la tasa de aprendizaje
            lr *= lr_dec
            # Recalcular el gradiente
            theta=theta_flat.reshape(g,m)
            Y_hat = hipotesis(A,theta)
            E,_,_,_=funcion_costo(Y,Y_hat,A)
            performace=fcnGrad(A,E)
            grad_flat=performace.reshape(-1,1)
            grad_norm=np.linalg.norm(grad_flat)
        elif SEE < SEE_prev:
            # Si el rendimiento ha mejorado, aumentar la tasa de aprendizaje
            lr *= lr_inc

        trayectoria.append(theta_flat.copy())

        #preprar salidad
        theta_opt = theta_flat.reshape(g,m)

    return theta_opt, SEE, epoch, trayectoria, performace




def main():
    #PC gris
    #URL= "C:/Users/erikG/OneDrive/Documents/ciclo2025-1/IA/undiad3/meta3_2/data/challenge00_syntheticdataset22.txt"
    
    #pc negra
    #URL ="C:/Users/erikG/Documents/ciclo2025-1/materias-ciclo-2025-1/IA/undiad3/meta3_2/caso_estudio-GDX/challenge00_syntheticdataset22.txt"

    # URL="data/challenge00_syntheticdataset22.txt"
    URL="data/challenge01_syntheticdataset22.txt"
 


    A,Y = lectura(URL)

    ##############################################
    #ayuda
    #A y Y son matrices
    #A es X las entradas y Y las salidas estunadas no son las hipoteticas 


    # print(A) #matriz de diseño 
    # print("\n")
    # print(Y) #matriz de salidas estimadas
    # print("\n")

    paramIniciales=(-0.1,0.1)
    ###################################################

    theta_opt,costo, iteraciones, trayectoria, performance= trainGDX(A,Y,paramIniciales,1000000,100)

    print("\n fuera de la funcion \n")
    print("performance: \n", performance)
    print("\nPARAMETROS OPTIMOS:\n", theta_opt)
    print("\ncosto: \n",costo)

    print("\n")

    Y_hat= hipotesis(A,theta_opt)
    _,_,_,RMSE = funcion_costo(Y,Y_hat,A)
    r2=calcular_r2(Y,Y_hat)


    print(f"funcionde presicion : {r2:.4f}")

main()

