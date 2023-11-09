# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/10/25 17:31:16 by juandrie          #+#    #+#              #
#    Updated: 2023/11/09 15:54:59 by juandrie         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Nom du fichier exécutable
TARGET = philo

# Compilateur
CC = gcc

# Flags de compilation (optimisation, warnings)
CFLAGS = -Wall -Wextra -Werror -g3
# LDFLAGS=-fsanitize=thread
#-fsanitize=thread
# Lien vers les bibliothèques nécessaires (ex. pthread)
# LDFLAGS = -lpthread

# Liste de tous les fichiers source (.c) 
SRC = main.c \
		routine.c\
		set.c\
		time.c\

# Liste des fichiers objets (.o) générés à partir des fichiers source
OBJ = $(SRC:.c=.o)

# Fichiers d'en-tête
HEADERS = philo.h

# Règle par défaut (lorsque vous tapez simplement 'make')
all: $(TARGET)

# Règle pour générer l'exécutable
$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

# Règle pour générer les fichiers objets
%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

# Règle pour nettoyer le répertoire (supprime les fichiers objets et l'exécutable)
clean:
	rm -f $(OBJ) $(TARGET)

# Règle pour supprimer aussi l'exécutable
fclean: clean
	rm -f $(TARGET)

# Règle pour recompiler tout
re: fclean all

# Marquer les règles comme n'ayant pas de fichiers de sortie
.PHONY: all clean fclean re
