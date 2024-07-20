BIN_NAME="example"
C_FLAGS="-Wall -Wextra -pedantic -g"

execute() {
    echo "$1"
    eval "$1"
}

execute "cc -o ${BIN_NAME} ${C_FLAGS} main.c cterm.c -I./"
