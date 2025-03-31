import numpy as np

# objective Extended Rosenbrock function
def objfcn(x):
    # Minima -> f=0 at (1,.....,1)
    n = len(x) # n par
    z = np.zeros((n,1))
    l2 = np.array(range(0,n,2)) # indice par
    l1 = np.array(range(1,n,2)) # indice impar
    z[l2]=10.0*(x[l1]-(x[l2])**2.0)
    z[l1]=1.0-x[l2]
    f = z.T @ z
    return f[0,0]




# Extended Rosenbrock gradient function
def objfcngrad(x):
    n = len(x) # n even
    Jz = np.zeros((n,n))
    z = np.zeros((n,1))
    l2 = np.array(range(0,n,2)) # indice par
    l1 = np.array(range(1,n,2)) # indice impar
    z[l2]=10.0*(x[l1]-(x[l2])**2.0)
    z[l1]=1.0-x[l2]

    for i in range(n//2):
        #alterado, por un warning, modificamos este para obtener el escalar al estar trabajdnco con un vector columa 2D, agregando el indice 0 podermos accedser a a la fila 2*i columna 0
        Jz[2*i,2*i]     = -20.0*x[2*i, 0]

        Jz[2*i,2*i+1]   = 10.0
        Jz[2*i+1,2*i]   = -1.0

    gX = 2.0*Jz.T @ z
    return gX


# def trainGDX(n,x_range,max_iter, lr_init=1e-3, momentum=0.9, lr_dec=0.7, lr_inc=1.05, max_perf_inc=1.04,tol=1e-8):


#     x= np.random.uniform(x_range[0],x_range[1],size=n)
#     # creacion de un arreglo uniforme va del rango a - b de la tupla definida como x_range

#     #tol establece un criterio de parada basado en la amgnitud del gradiente. 
#     epochs = []

#     #performace=[]

#     performace=objfcn(x)
#     gX=objfcngrad(x)
#     delta_x=np.zeros(n)

#     lr=lr_init


#     # para llevar el registro de los cambios de cada iteracion, se usa la funcion .copy() para hacer una copai indepediente del arreglo X para evitar que todos los elementos sean afectados 
#     trayectoria=[x.copy()]
    

#     for epochs in range(max_iter+1):

#         delta_x = momentum * delta_x - (1-momentum) * lr * gX

#         x_new= x + delta_x

#         new_perf=objfcn(x_new)

#         if new_perf / performace > max_perf_inc:

#             lr *= lr_dec
#             delta_x=lr* gX # gX gradite

#         elif new_perf < performace:
#             lr*=lr_inc

#         x= x_new
#         performace= new_perf

#         trayectoria.append(x.copy())
        
#         if np.linalg.norm(gX) < tol:
#             break

#     return x, performace, epochs, trayectoria

def trainGDX(n,x_range,max_iter,show, scale,  lr_init=1e-3, momentum=0.9, lr_dec=0.7, lr_inc=1.05, max_perf_inc=1.04,tol=1e-8):


    x = np.random.uniform(x_range[0], x_range[1], size=(n, 1))
    
    # creacion de un arreglo uniforme va del rango a - b de la tupla definida como x_range

    #tol establece un criterio de parada basado en la amgnitud del gradiente. 
    epoch = []

    performace=[]

    performace=objfcn(x)
    gX=objfcngrad(x)
    delta_x=np.zeros((n,1))

    stop_reason = "maximas iteraciones alcanzadas"

    lr=lr_init


    # para llevar el registro de los cambios de cada iteracion, se usa la funcion .copy() para hacer una copai indepediente del arreglo X para evitar que todos los elementos sean afectados 
    trayectoria=[x.copy()]
    grad_norm = np.linalg.norm(gX)

    for epoch in range(max_iter+1):


        #mostrar el progrsos 

        if show > 0 and (epoch % show == 0 or epoch== 0):
            print(f"Iter {epoch}: f(x) = {performace:.4e} |∇f| = {grad_norm:.4e} lr = {lr:.2e}")

        if grad_norm < tol:
            stop_reason=f"norma gradicete < {tol:.1e}"
            break


        delta_x = momentum * delta_x - (1-momentum) * lr * gX

        x_new= x + delta_x

        new_perf=objfcn(x_new)

        if new_perf / performace > max_perf_inc:

            lr *= lr_dec
            delta_x=lr* gX # gX gradite

        elif new_perf < performace:
            lr*=lr_inc

        x= x_new
        performace= new_perf
        gX= objfcngrad(x)

        trayectoria.append(x.copy())
        
        if np.linalg.norm(gX) < tol:
            break

    return x, performace, epoch, trayectoria



np.random.seed(1)
n=10
x=10*(np.random.randn(n,1))

x_opt, perf, epochs, trajectory= trainGDX(
n,
x_range=(-10,10),
max_iter=100000,
show=1000, 
scale=10
)

print(f"n=10 preformace final ={perf:.4e}")

print(x_opt)

# print(trajectory)



