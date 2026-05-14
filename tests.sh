#!/bin/sh

# Script de test pour Minishell
# A lancer dans le répertoire du projet

echo "--- 1. COMMANDES SIMPLES (EXTERNES) ---"
echo "ls"
echo "pwd"
echo "ls -l"
echo "ls -l -a"
echo ""

echo "--- 2. BUILTINS ---"
echo "# Test de pwd"
echo "pwd"
echo ""
echo "# Test de echo"
echo "echo hello world"
echo 'echo "hello   world"'
echo "echo -n hello world"
echo ""
echo "# Test de l'environnement"
echo "env"
echo "export VAR=test"
echo "env"
echo "unset VAR"
echo "env"
echo ""
echo "# Test de cd"
echo "pwd"
echo "cd .."
echo "pwd"
echo "cd -"
echo "pwd"
echo ""

echo "--- 3. PIPES SIMPLES ---"
echo "ls | wc -l"
echo "ls | grep main"
echo "cat Makefile | grep Src"
echo ""

echo "--- 4. PIPES AVEC BUILTINS ---"
echo "env | grep PATH"
echo 'echo "hello world" | wc -c'
echo ""

echo "--- 5. GESTION DES ERREURS ---"
echo "commandequin'existepas"
echo "cd /dossier/inexistant"
echo ""

echo "--- 6. COMMANDES AVEC CHEMIN ABSOLU ---"
echo "/bin/ls"
echo "/bin/echo hello"
echo ""

echo "--- FIN DES TESTS ---"
echo "Pour le test de 'exit', tapez 'exit' manuellement dans le shell."
