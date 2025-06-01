#------------------------------------------------------------------------------------



# Basic Imports
import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
from sklearn.model_selection import train_test_split #serpara los datos 
from sklearn.metrics import confusion_matrix, classification_report #estos los regulatado las claisigfacion y la matriz de confusion
from sklearn.preprocessing import StandardScaler #normalizar los datos 

#------------------------------------------------------------------------------------


def to_classlabel(z):
    return z.argmax(axis = 1) #etiqeuta en la columna



#------------------------------------------------------------------------------------

#y susecion de numeros, 
def one_hot_encode(y):
    n_class = np.unique(y).shape[0] #cunatas clases tiene Y 
    y_encode = np.zeros((y.shape[0], n_class)) #inicialzia la tabla, rengloes(instancias ) columnas numero de clases
    for idx, val in enumerate(y):
        y_encode[idx, val] = 1.0
    return y_encode




#------------------------------------------------------------------------------------

# Define Accuracy
def accuracy(y_true, y_pred):
    acc = np.sum(y_true == y_pred) / len(y_true)
    return acc #presicion 




#------------------------------------------------------------------------------------



class Softmax_Regression_GDX():
    
    def __init__(self, lambda_param=0.01):
        self.theta = None
        self.lambda_param = lambda_param  # Parámetro de regularización L2
        
    
    def _softmax(self, Z):
        # Estabilidad numérica: restar el máximo para evitar overflow
        shiftZ = Z - np.max(Z, axis=1, keepdims=True)
        expZ = np.exp(shiftZ)
        return expZ / np.sum(expZ, axis=1, keepdims=True)



    #fiunciond e cosot binaria 
    def _loss(self, Y, h):
        """Función de pérdida de entropía cruzada categórica con regularización L2"""
        epsilon = 1e-10  # Para evitar log(0)
        m = Y.shape[0]
        # Pérdida de entropía cruzada
        loss = -np.sum(Y * np.log(np.clip(h, epsilon, 1 - epsilon))) / m
        # Término de regularización L2 (excluir sesgo)
        reg_term = (self.lambda_param / (2 * m)) * np.sum(self.theta[1:, :]**2)
        return loss + reg_term

    def fit(self, A, y, learning_rate=0.01, momentum=0.9, 
            lr_dec=0.5, lr_inc=1.05, max_perf_inc=1.04,
            epochs=100, batch_size=32, show_step=10, 
            stopping_threshold=1e-6, verbose=False):
        
        # Convertir y a one-hot encoding
        Y = one_hot_encode(y)
        n_classes = Y.shape[1]
        n_features = A.shape[1]
        
        # Inicializar parámetros
        self.theta = np.random.randn(n_features, n_classes) * 0.01
        delta_theta = np.zeros_like(self.theta)
        n_obs = A.shape[0]
        lr = learning_rate
        previous_loss = np.inf
        epoch_loss = []
        
        for e in range(epochs + 1):
            THETA_prev = self.theta.copy()
            loss_e = 0
            
            # Barajar datos
            indices = np.random.permutation(n_obs)
            A_shuffled = A[indices]
            Y_shuffled = Y[indices]
            
            # Mini-lotes
            n_batches = n_obs // batch_size
            residual = n_obs % batch_size
            total_batches = n_batches + (1 if residual != 0 else 0)
            
            for batch_idx in range(total_batches):
                start = batch_idx * batch_size
                end = start + batch_size
                if batch_idx == n_batches and residual != 0:
                    end = start + residual
                
                A_batch = A_shuffled[start:end]
                Y_batch = Y_shuffled[start:end]

                #modificacion para lotes : 

                # if fit_params['batch_size'] == len(y_train):  # Solo para batch completo
                #     dropout_mask = np.random.binomial(1, 0.7, size=A_batch.shape)
                # A_batch = A_batch * dropout_mask
                
                # Calcular softmax y pérdida
                Z_batch = A_batch @ self.theta
                S_batch = self._softmax(Z_batch)
                loss_batch = self._loss(Y_batch, S_batch)
                loss_e += loss_batch
                
                # Calcular gradiente con regularización L2
                grad = (1 / len(A_batch)) * A_batch.T @ (S_batch - Y_batch)
                # Aplicar regularización solo a los pesos (no al sesgo)
                grad[1:, :] += (self.lambda_param / len(A_batch)) * self.theta[1:, :]
                
                # Actualización GDX con momento
                delta_theta = momentum * delta_theta - (1 - momentum) * lr * grad
                self.theta += delta_theta
            
            # Ajuste adaptativo de learning rate
            avg_loss = loss_e / total_batches
            if avg_loss > previous_loss * max_perf_inc:
                self.theta = THETA_prev
                lr *= lr_dec
            elif avg_loss < previous_loss:
                lr *= lr_inc
            
            epoch_loss.append(avg_loss)
            
            # Parada temprana
            if abs(previous_loss - avg_loss) < stopping_threshold:
                if verbose:
                    print(f"Early stopping at epoch {e}")
                break
            
            previous_loss = avg_loss
            
            if verbose and e % show_step == 0:
                print(f'Epoch: {e}, Loss: {avg_loss:.6f}, LR: {lr:.6f}')
        
        return epoch_loss
                
    # def predict(self, A):
    #     Z = A @ self.theta
    #     S = self._softmax(Z)
    #     return np.argmax(S, axis=1)

    def predict_proba(self, A):
        Z = A @ self.theta
        return self._softmax(Z)




#------------------------------------------------------------------------------------


class softMax_regresor_GDX_Multiclass():
    def __init__(self, lambda_param=0.01, num_classes=6):
        self.models = []
        self.losses = []
        self.lambda_param = lambda_param
        self.num_classes = num_classes
        
    def fit(self, A, y, **fit_params):
        self.models = []
        self.losses = []
        
        for i in range(self.num_classes):
            y_binary = np.where(y == i, 1, 0)
            
            positive_indices = np.where(y_binary == 1)[0]
            negative_indices = np.where(y_binary == 0)[0]
            
            n_pos = len(positive_indices)
            
            if n_pos > 0:
                neg_selected = np.random.choice(negative_indices, size=n_pos, replace=False)
                balanced_indices = np.concatenate([positive_indices, neg_selected])
                np.random.shuffle(balanced_indices)
                
                A_balanced = A[balanced_indices]
                y_balanced = y_binary[balanced_indices]
            else:
                print(f"Advertencia: Clase {i} tiene 0 muestras positivas")
                A_balanced = A
                y_balanced = y_binary
                
            model = Softmax_Regression_GDX(lambda_param=self.lambda_param)
            epoch_losses = model.fit(A_balanced, y_balanced, **fit_params)
            self.models.append(model)
            self.losses.append(epoch_losses)
    
    # def predict_proba(self, A):
    #     probas = []
    #     for model in self.models:
    #         # Usar función sigmoide directamente
    #         proba = 1 / (1 + np.exp(-np.dot(A, model.theta)))
    #         probas.append(proba)
    #     return np.array(probas).T

    def predict_proba(self, A):
        probas = []
        for model in self.models:
            proba_matrix = model.predict_proba(A)  # Usa el nuevo método
            
            # Manejar casos binarios (2 clases) o de una sola clase
            if proba_matrix.shape[1] == 2:
                proba_class_i = proba_matrix[:, 1]  # Clase positiva
            else:
                proba_class_i = proba_matrix[:, 0]  # Única clase disponible
            probas.append(proba_class_i)
        return np.column_stack(probas)
    
    def predict(self, A):
        probas = self.predict_proba(A)
        return np.argmax(probas, axis=1)
#------------------------------------------------------------------------------------



# # Read the data
# data = np.loadtxt('admisiones_dataset.txt',delimiter=',')
# inputs = data[:,0:2]
# idx = 2-data[:,2] #restamos el 1 para establecer el categorico, adminitivos - 1 no admitivos - 0 
# targets = np.array(idx, dtype=int)     # codificacion categorica
# # targets = one_hot_encode(labels)      # one hot encode to classlabel

try:
    df = pd.read_csv('dermatology.dat', header=None, na_values='?', delimiter=r'\s+')
    
    inputs = df.iloc[:, 0:34]
    targets = df.iloc[:, 34]
    
    age_mean = inputs[33].mean()
    inputs[33] = inputs[33].fillna(age_mean)
    
    inputs = inputs.astype(float)
    targets = targets.astype(int) - 1  # Convertir clases [1-6] a [0-5]

except Exception as e:
    print(f"Error al leer el archivo: {e}")
    exit()



#------------------------------------------------------------------------------------


# Split the data
# x_train,x_test,y_train,y_test = train_test_split(inputs,targets,test_size=0.40,random_state=1234) # test_size genreta entrenamiento y prueba 

# División de datos
x_train, x_test, y_train, y_test = train_test_split(
    inputs, targets, test_size=0.3, random_state=42
)




#------------------------------------------------------------------------------------
# Normalización
scaler = StandardScaler()
x_train = scaler.fit_transform(x_train)
x_test = scaler.transform(x_test)


#------------------------------------------------------------------------------------

#matrices de disenio, 
A_train = np.c_[np.ones(len(x_train)), x_train]
A_test  = np.c_[np.ones(len(x_test)), x_test]


#------------------------------------------------------------------------------------

# Parámetros para el constructor

#minilot4es
# Hiperparámetros GDX (SOLO para fit())
# lambda_param = 0.1  # Regularización L2
# fit_params = {
#     'learning_rate': 0.01,
#     'momentum': 0.95,
#     'lr_dec': 0.5,
#     'lr_inc': 1.05,
#     'max_perf_inc': 1.04,
#     'epochs': 1000,
#     'batch_size': 64,
#     'show_step': 100,
#     'stopping_threshold': 1e-6,
#     'verbose': True
# }


#online
lambda_param = 0.1  # Regularización L2
fit_params = {
    'learning_rate': 0.01,
    'momentum': 0.95,
    'lr_dec': 0.5,
    'lr_inc': 1.05,
    'max_perf_inc': 1.04,
    'epochs': 1000,
    'batch_size': 1,
    'show_step': 100,
    'stopping_threshold': 1e-6,
    'verbose': True
}


#lote
# lambda_param = 0.1  # Regularización L2
# fit_params = {
#     'learning_rate': 0.1,
#     'momentum': 0.95,
#     'lr_dec': 0.5,
#     'lr_inc': 1.05,
#     'max_perf_inc': 1.04,
#     'epochs': 300,
#     'batch_size': len(y_train),
#     'show_step': 50,
#     'stopping_threshold': 1e-4,
#     'verbose': True
# }
#-----------------------------------------------------------------------------------


# # Build and fit best LR model
# alpha = 0.01 #lr
# maxEpochs = 5000
# batch = 10 #minilotes
# show = 500 #view

# # Build model
# log_model = Logistic_Regression()
# # Fit Model
# theta, batch_loss, epoch_loss = log_model.fit(A_train, y_train, learning_rate=alpha, 
#                                 epochs=maxEpochs, batch_size=batch, show_step = show, verbose=True)


# Entrenamiento del modelo
model = softMax_regresor_GDX_Multiclass(lambda_param=lambda_param, num_classes=6)
model.fit(A_train, y_train, **fit_params)
#------------------------------------------------------------------------------------



# Predicciones y evaluación
# Predicciones
train_pred = model.predict(A_train)
test_pred = model.predict(A_test)


#------------------------------------------------------------------------------------


# # Evaluación
# print(f"Modo de aprendizaje: Minilotes (tamaño {fit_params['batch_size']})")
# print(f"Regularización L2: lambda={lambda_param}")
# print(f"Épocas completadas: {len(epoch_losses)}/{fit_params['epochs']}")

#------------------------------------------------------------------------------------

# # Calculate accuracy
# train_acc = accuracy(y_train, train_pred)
# print(f'Accuracy on training set: {train_acc}')
#resultados finales
# Resultados en entrenamiento
print("\nRendimiento en entrenamiento (multiclase - AdamD):")
print(f'Accuracy: {accuracy(y_train, train_pred):.4f}')
print("\nMatriz de confusión:")
print(confusion_matrix(y_train, train_pred))
print("\nReporte de clasificación:")
print(classification_report(y_train, train_pred))

#------------------------------------------------------------------------------------




# Calculate metrics - son de entrenamiento 
# cm_train = confusion_matrix(y_train, train_pred)
# train_report = classification_report(y_train, train_pred)

# print("Performance on training set:\n")
# print(f'Confusion Matrix:\n {cm_train}\n')
# print(f'Classification Report:\n {train_report}')

# Resultados en prueba
print("\nRendimiento en prueba (multiclase - AdamD):")
print(f'Accuracy: {accuracy(y_test, test_pred):.4f}')
print("\nMatriz de confusión:")
print(confusion_matrix(y_test, test_pred))
print("\nReporte de clasificación:")
print(classification_report(y_test, test_pred))


#------------------------------------------------------------------------------------

# Gráfica de pérdida
# Gráfica de pérdida
plt.figure(figsize=(12, 6))
plt.subplot(1, 2, 1)
plt.plot(epoch_loss, 'b-', linewidth=2)
plt.title('evolución de la Perdida por epoca')
plt.xlabel('epoca')
plt.ylabel('perdida (entropia eruzada binaria)')
plt.grid(True)

plt.tight_layout()
plt.savefig('loss_evolution.png', dpi=300)
plt.show()

