## Install Homebrew
# /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

## install packages
# brew install glfw glew assimp glm

## compile
g++ -std=c++17 -L/opt/homebrew/lib -I/opt/homebrew/include -lglfw -lglew -lglm -lassimp -framework OpenGL -o main *.cpp