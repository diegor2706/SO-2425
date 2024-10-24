# Nombre del compilador
CC = gcc

# Flags del compilador
CFLAGS = -Wall -g

# Archivo fuente
SRCS = p1.c

# Archivos objeto generados
OBJS = $(SRCS:.c=.o)

# Nombre del ejecutable
EXEC = p1

# Regla por defecto: compila y ejecuta
all: $(EXEC)
	./$(EXEC)

# Compilar el ejecutable a partir de los objetos
$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $(EXEC) $(OBJS)

# Regla para generar los archivos objeto a partir de los archivos fuente
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Limpiar archivos objeto y el ejecutable
clean:
	rm -f $(OBJS) $(EXEC)
