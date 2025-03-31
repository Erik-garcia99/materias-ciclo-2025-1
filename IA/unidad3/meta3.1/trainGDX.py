import numpy as np

#deepseek

def objfcn(x):
    n = len(x)

    z=np.zeros(n)
    l2=np.array(range(0,n,2)) #indices pares
    l1=np.array(range(1,n,2)) #indices impares

    x_par= np.clip(x[l2],-1e3,1e3) #evitar overflow en x[l2]^2

    z[l2]= 10.0*(x[l1] -x_par**2.0)
    z[l1]=1.0-x_par

    return np.sum(z**2)


def objfcngrad(x):

    n=len(x)

    grad=np.zeros(n)

    x_clipped= np.clip(x, -1e3, 1e3) #clip valores de x

    for i in range(n//2):
        idx_par=2*i
        idx_impar=2*i + 1

        x_par= x_clipped[idx_par]
        x_impar=x_clipped[idx_impar]

        grand_term= x_impar -x_par**2

        grad[idx_par]= -20 * x_par * grand_term - 2 * (1-x_par)

        grad[idx_impar] = 10 * grand_term

        # grad[idx_par]= -20 * x[idx_par] * (x[idx_impar]- x[idx_par]**2) - 2 *(1-x[idx_par])

        #derivada respecto a x_impar 

        # grad[idx_impar]= 10 *(x[idx_impar] - x[idx_par]**2)

    return grad

#criterios de pagos

#lr bajar puede ser 1x10^-5 
def trainGDX(objfcn, objgradfunc, n, lr_init=0.001, momentum=0.9, lr_dec=0.7, lr_inc=1.05, max_perf_inc=1.04, max_iter=10000, tol=1e-8,x_range=(-10,10)):

    x=np.random.uniform(x_range[0], x_range[1], size=n)

    lr=lr_init #tasa de aprendizaje 

    delta_x=np.zeros(n)

    prev_loss= objfcn(x)
    trayectoria=[x.copy()]

    for t in range(max_iter):
        grad = objfcngrad(x)
        grad= np.clip(grad, -1e4, 1e4) # clip grandes extremos 

        delta_x= momentum* delta_x - (1- momentum)* lr * grad # actualiza el momentun 

        x_new= x+ delta_x

        x_new = np.clip(x_new, -1e4, 1e4) # clip de paremetros para evitar overflow

        new_loss = objfcn(x_new) # actualizamos los parametros (teta-t -> teta-t-1 + delta(teta-t))

        if new_loss / prev_loss > max_perf_inc:
            lr *=lr_dec
            delta_x=lr*grad

        elif new_loss < prev_loss:
            lr *=lr_inc

        x=x_new
        prev_loss= new_loss
        trayectoria.append(x.copy())

        if np.linalg.norm(grad) < tol:
            break

    return x, prev_loss, trayectoria



# Configuración de parámetros
np.random.seed(0)  # Para reproducibilidad

# Caso n=10 (rango [-10, 10])
x_opt_10, loss_10, _ = trainGDX(objfcn, objfcngrad, n=10)
print(f"n=10: Loss final = {loss_10:.4e}")

# Caso n=100 (rango [-10, 10])
x_opt_100, loss_100, _ = trainGDX(objfcn, objfcngrad, n=100)
print(f"n=100: Loss final = {loss_100:.4e}")

# Caso n=1000 (rango [-10, 10])
x_opt_100, loss_100, _ = trainGDX(objfcn, objfcngrad, n=1000)
print(f"n=100: Loss final = {loss_100:.4e}")

# Para n=1000 (rango [-100, 100])
x_opt_1000, loss_1000, _ = trainGDX(
    objfcn, 
    objfcngrad, 
    n=1000, 
    lr_init=0.0001,  # Tasa de aprendizaje más pequeña
    x_range=(-100, 100),
    max_iter=20000    # Más iteraciones
)
print(f"n=1000: Loss final = {loss_1000:.4e}")










